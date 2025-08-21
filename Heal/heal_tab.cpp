#include "heal_tab.h"

#include <filesystem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QJsonArray>
#include <QLabel>
#include <QMessageBox>
#include <QSaveFile>
#include <QRadioButton>

#include "../Functions/memory_functions.h"

HealTab::HealTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Heal");

    healList_listWidget = new QListWidget(this);
    connect(healList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        delete healList_listWidget->takeItem(healList_listWidget->row(item));});

    profile_listWidget = new QListWidget(this);
    profile_lineEdit = new QLineEdit(this);

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    healList();
    profileList();

}

void HealTab::healList() {
    auto groupbox = new QGroupBox("Healing", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);

    auto clearHealList_button = new QPushButton("Clear List", this);
    connect(clearHealList_button, &QPushButton::clicked, this, &HealTab::clearHealList);

    auto add_button = new QPushButton("Add Healing", this);

    auto option_comboBox = new QComboBox(this);
    option_comboBox->addItems({"Say", "Use"});

    auto heal_lineEdit = new QLineEdit(this);
    heal_lineEdit->setPlaceholderText("Exura or Item ID");

    auto condition_comboBox = new QComboBox(this);
    condition_comboBox->addItems({"HP%", "MP%"});

    auto below_lineEdit = new QLineEdit(this);
    below_lineEdit->setPlaceholderText("100");
    below_lineEdit->setValidator(new QIntValidator(1,100));

    auto above_lineEdit = new QLineEdit(this);
    above_lineEdit->setPlaceholderText("10");
    above_lineEdit->setValidator(new QIntValidator(0,99));

    auto minMp_lineEdit = new QLineEdit(this);
    minMp_lineEdit->setPlaceholderText("25");
    minMp_lineEdit->setValidator(new QIntValidator(0,99999));

    auto layout1 = new QHBoxLayout(groupbox);
    layout1->addWidget(new QLabel("Below", this));
    layout1->addWidget(below_lineEdit);
    layout1->addWidget(new QLabel("-", this));
    layout1->addWidget(above_lineEdit);

    auto layout2 = new QHBoxLayout(groupbox);
    layout2->addWidget(new QLabel("Min MP", this));
    layout2->addWidget(minMp_lineEdit);


    auto layout_left = new QVBoxLayout();
    layout_left->addWidget(healList_listWidget);
    layout_left->addWidget(clearHealList_button);

    auto layout_right = new QVBoxLayout();
    layout_right->addWidget(add_button);
    layout_right->addWidget(option_comboBox);
    layout_right->addWidget(heal_lineEdit);
    layout_right->addWidget(condition_comboBox);
    layout_right->addLayout(layout1);
    layout_right->addLayout(layout2);


    groupbox_layout->addLayout(layout_left);
    groupbox_layout->addLayout(layout_right);

    connect(add_button, &QPushButton::clicked, this, [this, option_comboBox, heal_lineEdit, below_lineEdit, above_lineEdit, minMp_lineEdit, condition_comboBox]() {

    addHeal(option_comboBox->currentText(), heal_lineEdit->text(), below_lineEdit->text().toInt(),
        above_lineEdit->text().toInt(), minMp_lineEdit->text().toInt(), condition_comboBox->currentText());
        heal_lineEdit->clear();
        below_lineEdit->clear();
        above_lineEdit->clear();
        minMp_lineEdit->clear();
        });

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}



void HealTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");
    connect(save_button, &QPushButton::clicked, this, &HealTab::saveProfile);
    connect(load_button, &QPushButton::clicked, this, &HealTab::loadProfile);

    QDir dir("Save/Healing");
    QStringList filters("*.json");
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& file : files) {
        profile_listWidget->addItem(file.baseName());
    }

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();
    layout1->addWidget(new QLabel("Name", this));
    layout1->addWidget(profile_lineEdit);
    layout2->addWidget(save_button);
    layout2->addWidget(load_button);

    groupbox_layout->addWidget(profile_listWidget);
    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 1, 0);

}


void HealTab::saveProfile() const {
    const QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Save", "Give profile name.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Healing")).filePath(name + ".json");

    QJsonArray healArray;
    for (int i = 0; i < healList_listWidget->count(); ++i) {
        const auto* item = healList_listWidget->item(i);
        const QVariantMap map = item->data(Qt::UserRole).toMap();
        healArray.append(QJsonObject::fromVariantMap(map));
    }

    QJsonObject root;
    root["version"] = 1;
    root["healing"] = healArray;

    QSaveFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(nullptr, "Save", "Can't open file:\n" + file.errorString());
        return;
    }
    const QByteArray json = QJsonDocument(root).toJson(QJsonDocument::Indented);
    if (file.write(json) == -1) {
        QMessageBox::critical(nullptr, "Save", "Save Error:\n" + file.errorString());
        return;
    }
    if (!file.commit()) {
        QMessageBox::critical(nullptr, "Save", "Commit error:\n" + file.errorString());
        return;
    }

    if (profile_listWidget->findItems(name, Qt::MatchExactly).isEmpty())
        profile_listWidget->addItem(name);

    QMessageBox::information(nullptr, "Save", "Profile saved: " + name);
}

void HealTab::loadProfile() const {
    QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty() && profile_listWidget->currentItem())
        name = profile_listWidget->currentItem()->text();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Load", "Choose or write profile name.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Healing")).filePath(name + ".json");
    QFile file(fullPath);
    if (!file.exists()) {
        QMessageBox::warning(nullptr, "Load", "File do not exist:\n" + fullPath);
        return;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Load", "Can not open file:\n" + file.errorString());
        return;
    }

    QJsonParseError perr{};
    const QJsonObject root =  QJsonDocument::fromJson(file.readAll(), &perr).object();
    const QJsonArray arr = root.value("healing").toArray();

    healList_listWidget->clear();

    profile_lineEdit->setText(name);
    const auto matches = profile_listWidget->findItems(name, Qt::MatchExactly);
    if (!matches.isEmpty())
        profile_listWidget->setCurrentItem(matches.first());

    QMessageBox::information(nullptr, "Load", "Profile loaded: " + name);
}

void HealTab::clearHealList() const {
    healList_listWidget->clear();
}

void HealTab::addHeal(const QString& option, const QString& heal, int hpBelow, int hpAbove, int minMp, const QString& contidion) const {
    auto* item = new QListWidgetItem(option + " " + heal);
    QVariantMap data;
    data["option"] = option;
    data["heal"] = heal;
    data["hpBelow"] = hpBelow;
    data["hpAbove"] = hpAbove;
    data["minMp"] = minMp;
    data["contidion"] = contidion;

    item->setData(Qt::UserRole, data);
    healList_listWidget->addItem(item);
}

void HealTab::setHealEnabled(bool on) {
    if (on) {
        if (healThread) return;
        QList<QVariantMap> healing;
        healing.reserve(healList_listWidget->count());
        for (int i = 0; i < healList_listWidget->count(); ++i) {
            auto* item = healList_listWidget->item(i);
            healing.append(item->data(Qt::UserRole).toMap());
        }

        healThread = new HealThread(healing);
        healThread->start();
    } else {
        if (!healThread) return;
        healThread->stop();
        healThread->wait();
        delete healThread;
        healThread = nullptr;
    }
}
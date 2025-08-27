#include "spell_tab.h"
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

#include "../Functions/Game.h"

SpellTab::SpellTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Spell");

    spellList_listWidget = new QListWidget(this);
    connect(spellList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        delete spellList_listWidget->takeItem(spellList_listWidget->row(item));});

    profile_listWidget = new QListWidget(this);
    profile_lineEdit = new QLineEdit(this);

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    spellList();
    profileList();

}

void SpellTab::spellList() {
    auto groupbox = new QGroupBox("Spells", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto option_comboBox = new QComboBox();
    option_comboBox->addItem("Say");
    option_comboBox->addItem("Use on target");
    option_comboBox->addItem("Use on yourself");
    option_comboBox->setMinimumWidth(40);

    auto target_lineEdit = new QLineEdit();
    target_lineEdit->setPlaceholderText("Dragon, Cyclops | * - All targets");
    target_lineEdit->setMinimumWidth(180);

    auto count_lineEdit = new QLineEdit();
    count_lineEdit->setPlaceholderText("2");
    count_lineEdit->setValidator(new QIntValidator(1, 15));
    count_lineEdit->setMinimumWidth(40);

    auto spell_lineEdit = new QLineEdit();
    spell_lineEdit->setPlaceholderText("Spell name | ItemID");
    spell_lineEdit->setMinimumWidth(200);

    auto minHp_lineEdit = new QLineEdit();
    minHp_lineEdit->setPlaceholderText("50");
    minHp_lineEdit->setValidator(new QIntValidator(0, 100));

    auto targetHpFrom_lineEdit = new QLineEdit();
    targetHpFrom_lineEdit->setPlaceholderText("100");
    targetHpFrom_lineEdit->setValidator(new QIntValidator(100, 1));

    auto targetHpTo_lineEdit = new QLineEdit();
    targetHpTo_lineEdit->setPlaceholderText("0");
    targetHpTo_lineEdit->setValidator(new QIntValidator(0, 99));

    auto minMp_lineEdit = new QLineEdit();
    minMp_lineEdit->setPlaceholderText("300");
    minMp_lineEdit->setValidator(new QIntValidator(0, 9999));

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    layout1->addWidget(option_comboBox);
    layout1->addWidget(spell_lineEdit);
    layout1->addWidget(new QLabel("Min HP%:"));
    layout1->addWidget(minHp_lineEdit);
    layout1->addWidget(new QLabel("Min MP:"));
    layout1->addWidget(minMp_lineEdit);

    layout2->addWidget(new QLabel("Count:"));
    layout2->addWidget(count_lineEdit);
    layout2->addWidget(new QLabel("Name:"));
    layout2->addWidget(target_lineEdit);
    layout2->addWidget(new QLabel("HP% range", this));
    layout2->addWidget(targetHpFrom_lineEdit);
    layout2->addWidget(new QLabel("to", this));
    layout2->addWidget(targetHpTo_lineEdit);

    groupbox_layout->addWidget(spellList_listWidget);
    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);



    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}



void SpellTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");
    connect(save_button, &QPushButton::clicked, this, &SpellTab::saveProfile);
    connect(load_button, &QPushButton::clicked, this, &SpellTab::loadProfile);

    QDir dir("Save/Spells");
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


void SpellTab::saveProfile() const {
    const QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Save", "Give profile name.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Spells")).filePath(name + ".json");

    QJsonArray healArray;
    for (int i = 0; i < spellList_listWidget->count(); ++i) {
        const auto* item = spellList_listWidget->item(i);
        const QVariantMap map = item->data(Qt::UserRole).toMap();
        healArray.append(QJsonObject::fromVariantMap(map));
    }

    QJsonObject root;
    root["version"] = 1;
    root["spells"] = healArray;

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

void SpellTab::loadProfile() const {
    QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty() && profile_listWidget->currentItem())
        name = profile_listWidget->currentItem()->text();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Load", "Choose or write profile name.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Spells")).filePath(name + ".json");
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
    const QJsonArray arr = root.value("spells").toArray();

    spellList_listWidget->clear();

    profile_lineEdit->setText(name);
    const auto matches = profile_listWidget->findItems(name, Qt::MatchExactly);
    if (!matches.isEmpty())
        profile_listWidget->setCurrentItem(matches.first());

    QMessageBox::information(nullptr, "Load", "Profile loaded: " + name);
}

void SpellTab::setSpellEnabled(bool on) {
    if (on) {
        if (spellThread) return;
        QList<QVariantMap> spells;
        spells.reserve(spellList_listWidget->count());
        for (int i = 0; i < spellList_listWidget->count(); ++i) {
            auto* item = spellList_listWidget->item(i);
            spells.append(item->data(Qt::UserRole).toMap());
        }

        spellThread = new SpellThread(spells);
        spellThread->start();
    } else {
        if (!spellThread) return;
        spellThread->stop();
        spellThread->wait();
        delete spellThread;
        spellThread = nullptr;
    }
}
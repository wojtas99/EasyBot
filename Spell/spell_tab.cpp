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

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    spellList();
    profileList();

}

void SpellTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto profileName = new QLineEdit();

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    QDir dir("Save/Spells");
    QStringList filters("*.json");
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& file : files) {
        profile_listWidget->addItem(file.baseName());
    }

    layout1->addWidget(new QLabel("Name", this));
    layout1->addWidget(profileName);

    connect(save_button, &QPushButton::clicked, [this, profileName](){
        saveProfile(profileName->text());
        profileName->clear();
    });

    connect(load_button, &QPushButton::clicked, [this, profileName](){
        spellList_listWidget->clear();
        if (!profileName->text().isEmpty()) { loadProfile(profileName->text());}
        else if (!profile_listWidget->currentItem()->text().isEmpty()) {loadProfile(profile_listWidget->currentItem()->text());}
        profileName->clear();
    });

    layout2->addWidget(save_button);
    layout2->addWidget(load_button);

    groupbox_layout->addWidget(profile_listWidget);
    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);
    groupbox->setFixedSize(QSize(200, 160));
    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 1, 0, 1, 1);
}

void SpellTab::spellList() {
    auto groupbox = new QGroupBox("Spells", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto add_button = new QPushButton("Add Spell", this);

    auto option_comboBox = new QComboBox();
    option_comboBox->addItem("Say");
    option_comboBox->addItem("Use on target");
    option_comboBox->addItem("Use on yourself");
    option_comboBox->setMinimumWidth(40);

    auto target_lineEdit = new QLineEdit();
    target_lineEdit->setPlaceholderText("Dragon | * - All targets");
    target_lineEdit->setMinimumWidth(180);

    auto count_lineEdit = new QLineEdit();
    count_lineEdit->setPlaceholderText("2");
    count_lineEdit->setValidator(new QIntValidator(1, 15));

    auto spell_lineEdit = new QLineEdit();
    spell_lineEdit->setPlaceholderText("Spell name | ItemID");
    spell_lineEdit->setMinimumWidth(100);

    auto minHp_lineEdit = new QLineEdit();
    minHp_lineEdit->setPlaceholderText("50");
    minHp_lineEdit->setValidator(new QIntValidator(0, 100));

    auto targetHpFrom_lineEdit = new QLineEdit();
    targetHpFrom_lineEdit->setPlaceholderText("100");
    targetHpFrom_lineEdit->setValidator(new QIntValidator(0, 100));

    auto targetHpTo_lineEdit = new QLineEdit();
    targetHpTo_lineEdit->setPlaceholderText("0");
    targetHpTo_lineEdit->setValidator(new QIntValidator(0, 99));

    auto minMp_lineEdit = new QLineEdit();
    minMp_lineEdit->setPlaceholderText("300");
    minMp_lineEdit->setValidator(new QIntValidator(0, 9999));

    auto dist_lineEdit = new QLineEdit();
    dist_lineEdit->setPlaceholderText("3");
    dist_lineEdit->setValidator(new QIntValidator(1, 7));

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    layout1->addWidget(option_comboBox);
    layout1->addWidget(new QLabel("Name:"));
    layout1->addWidget(target_lineEdit);
    layout1->addWidget(spell_lineEdit);

    layout2->addWidget(new QLabel("Min HP%:"));
    layout2->addWidget(minHp_lineEdit);
    layout2->addWidget(new QLabel("Min MP:"));
    layout2->addWidget(minMp_lineEdit);

    layout2->addWidget(new QLabel("HP% range", this));
    layout2->addWidget(targetHpFrom_lineEdit);
    layout2->addWidget(new QLabel("to", this));
    layout2->addWidget(targetHpTo_lineEdit);
    layout2->addWidget(new QLabel("Count:"));
    layout2->addWidget(count_lineEdit);
    layout2->addWidget(new QLabel("Dist:"));
    layout2->addWidget(dist_lineEdit);

    groupbox_layout->addWidget(spellList_listWidget);
    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);
    groupbox_layout->addWidget(add_button);
    connect(add_button, &QPushButton::clicked, this, [this, option_comboBox, spell_lineEdit, minHp_lineEdit, minMp_lineEdit, count_lineEdit, target_lineEdit,
        targetHpFrom_lineEdit, targetHpTo_lineEdit, dist_lineEdit]() {

    addSpell(option_comboBox->currentText(), spell_lineEdit->text(), minHp_lineEdit->text().toInt(),
    minMp_lineEdit->text().toInt(), count_lineEdit->text().toInt(), target_lineEdit->text(),
    targetHpFrom_lineEdit->text().toInt(), targetHpTo_lineEdit->text().toInt(), dist_lineEdit->text().toInt());
        spell_lineEdit->clear();
        minHp_lineEdit->clear();
        minMp_lineEdit->clear();
        count_lineEdit->clear();
        target_lineEdit->clear();
        targetHpFrom_lineEdit->clear();
        targetHpTo_lineEdit->clear();
        dist_lineEdit->clear();
    });


    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}

void SpellTab::addSpell(const QString& option, const QString& spell, int minHp, int minMp, int count, const QString& targetName, int hpFrom, int hpTo, int dist) const {
    QString name = option + " " + spell + " -> " + targetName;
    auto* item = new QListWidgetItem(option + " " + spell + " -> " + targetName);
    QVariantMap data;
    data["option"] = option.toStdString().c_str();
    data["spell"] = spell.toStdString().c_str();
    data["minHp"] = minHp;
    data["minMp"] = minMp;
    data["count"] = count;
    data["targetName"] = targetName.toStdString().c_str();
    data["hpFrom"] = hpFrom;
    data["hpTo"] = hpTo;
    data["dist"] = dist;

    item->setData(Qt::UserRole, data);
    spellList_listWidget->addItem(item);
}

// Start Profile Functions

void SpellTab::loadProfile(const QString& profileName) {
    QList<QVariantMap> m_loaded = loadProfileSignal("Spells", profileName);
    for (auto item: m_loaded) {
        auto option = item.value("option").toString();
        auto spell = item.value("spell").toString();
        auto targetName = item.value("targetName").toString();
        auto* data = new QListWidgetItem(option + " " + spell + " -> " + targetName);
        data->setData(Qt::UserRole, item);
        spellList_listWidget->addItem(data);
    }
}

void SpellTab::saveProfile(const QString& profileName) {
    QList<QVariantMap> targets;
    for (int i = 0; i < spellList_listWidget->count(); ++i) {
        QListWidgetItem* item = spellList_listWidget->item(i);
        QVariantMap data = item->data(Qt::UserRole).toMap();
        targets.append(data);
    }
    if (saveProfileSignal("Spells", profileName, targets)) {
        profile_listWidget->addItem(profileName);
    }
}

// End Profile Functions

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
        QMetaObject::invokeMethod(spellThread, "stop", Qt::QueuedConnection);
        connect(spellThread, &QThread::finished, this, [this]{
            spellThread->deleteLater();
            spellThread = nullptr;
        });
    }
}

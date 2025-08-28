#include "loot_tab.h"

#include <filesystem>
#include <iostream>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QJsonArray>
#include <QMessageBox>
#include <QSaveFile>

LootTab::LootTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Loot");

    lootList_listWidget = new QListWidget(this);
    connect(lootList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
    delete lootList_listWidget->takeItem(lootList_listWidget->row(item));});

    profile_listWidget = new QListWidget(this);

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    lootList();
    profileList();

}

void LootTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto profileName = new QLineEdit();

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    QDir dir("Save/Items");
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
        lootList_listWidget->clear();
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

void LootTab::lootList() {
    auto groupbox = new QGroupBox("Items", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);


    auto clearItemList_button = new QPushButton("Clear List", this);
    connect(clearItemList_button, &QPushButton::clicked, this, &LootTab::clearList);

    auto item_id_lineEdit = new QLineEdit(this);
    item_id_lineEdit->setPlaceholderText("Item ID - 2148");
    auto container_name_lineEdit = new QLineEdit(this);
    container_name_lineEdit->setPlaceholderText("Container Name - Yellow Backpack");
    auto item_name_lineEdit = new QLineEdit(this);
    item_name_lineEdit->setPlaceholderText("Item Name - Gold Coin");

    last_looked_item_id = new QLabel("Last Item ID: 0000", this);

    auto add_button = new QPushButton("Add", this);



    connect(add_button, &QPushButton::clicked, this, [this, item_id_lineEdit, container_name_lineEdit, item_name_lineEdit]() {
        addItem(item_id_lineEdit->text().toInt(), container_name_lineEdit->text(), item_name_lineEdit->text());
        item_id_lineEdit->clear();
        container_name_lineEdit->clear();
        item_name_lineEdit->clear();
    });


    auto layout_left = new QVBoxLayout();
    layout_left->addWidget(lootList_listWidget);
    layout_left->addWidget(clearItemList_button);

    auto layout_right = new QVBoxLayout();
    layout_right->addWidget(add_button);
    layout_right->addWidget(item_id_lineEdit);
    layout_right->addWidget(container_name_lineEdit);
    layout_right->addWidget(item_name_lineEdit);
    layout_right->addWidget(last_looked_item_id);

    groupbox_layout->addLayout(layout_left);
    groupbox_layout->addLayout(layout_right);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}

void LootTab::addItem(int item_id, const QString& container_name, const QString& item_name) const
{
    auto* item = new QListWidgetItem(item_name + " -> " + container_name);
    QVariantMap data;
    data["name"] = item_name;
    data["id"] = item_id;
    data["container"] = container_name;

    item->setData(Qt::UserRole, data);
    lootList_listWidget->addItem(item);
}

// Start Profile Functions

void LootTab::loadProfile(const QString& profileName) {
    QList<QVariantMap> m_loaded = loadProfileSignal("Items", profileName);
    for (auto item: m_loaded) {
        auto name = item.value("name").toString();
        auto container = item.value("container").toString();
        auto* data = new QListWidgetItem(name + " -> " + container);
        data->setData(Qt::UserRole, item);
        lootList_listWidget->addItem(data);
    }
}

void LootTab::saveProfile(const QString& profileName) {
    QList<QVariantMap> targets;
    for (int i = 0; i < lootList_listWidget->count(); ++i) {
        QListWidgetItem* item = lootList_listWidget->item(i);
        QVariantMap data = item->data(Qt::UserRole).toMap();
        targets.append(data);
    }
    if (saveProfileSignal("Items", profileName, targets)) {
        profile_listWidget->addItem(profileName);
    }
}

void LootTab::clearList() const {
    lootList_listWidget->clear();
}

// End Profile Functions

void LootTab::setLootEnabled(bool on) {
    if (on) {
        if (lootThread) return;
        QList<QVariantMap> items;
        for (int i = 0; i < lootList_listWidget->count(); ++i) {
            QListWidgetItem* item = lootList_listWidget->item(i);
            QVariantMap data = item->data(Qt::UserRole).toMap();
            items.append(data);
        }
        lootThread = new LootThread(items);
        std::cout << "Loot Thread created" << std::endl;
        lootThread->start();
    } else {
        if (!lootThread) return;
        lootThread->stop();
        lootThread->wait();
        delete lootThread;
        lootThread = nullptr;
    }
}

void LootTab::updateLastLookedItem(int id) const {
    if (last_looked_item_id)
        last_looked_item_id->setText(QString("Last Item ID: %1").arg(id));
}

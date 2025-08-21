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
    profile_lineEdit = new QLineEdit(this);

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    lootList();
    profileList();

}


void LootTab::lootList() {
    auto groupbox = new QGroupBox("Items", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);


    auto clearItemList_button = new QPushButton("Clear List", this);
    connect(clearItemList_button, &QPushButton::clicked, this, &LootTab::clearItemList);

    auto item_id_lineEdit = new QLineEdit(this);
    item_id_lineEdit->setPlaceholderText("Item ID - 2148");
    auto container_name_lineEdit = new QLineEdit(this);
    container_name_lineEdit->setPlaceholderText("Container Name - Yellow Backpack");
    auto item_name_lineEdit = new QLineEdit(this);
    item_name_lineEdit->setPlaceholderText("Item Name - Gold Coin");

    auto open_next_bp_checkBox = new QCheckBox("Open Next BP");

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
    layout_right->addWidget(open_next_bp_checkBox);
    layout_right->addWidget(last_looked_item_id);

    groupbox_layout->addLayout(layout_left);
    groupbox_layout->addLayout(layout_right);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}



void LootTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");
    connect(save_button, &QPushButton::clicked, this, &LootTab::saveProfile);
    connect(load_button, &QPushButton::clicked, this, &LootTab::loadProfile);

    QDir dir("Save/Items");
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


void LootTab::saveProfile() const {
    const QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Save", "Give profile name.");
        return;
    }
    if (lootList_listWidget->count() == 0) {
        QMessageBox::information(nullptr, "Save", "There is no items to save.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Waypoints")).filePath(name + ".json");

    QJsonArray itmArray;
    for (int i = 0; i < lootList_listWidget->count(); ++i) {
        const auto* item = lootList_listWidget->item(i);
        const QVariantMap map = item->data(Qt::UserRole).toMap();
        itmArray.append(QJsonObject::fromVariantMap(map));
    }

    QJsonObject root;
    root["version"] = 1;
    root["items"] = itmArray;

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

void LootTab::loadProfile() const {
    QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty() && profile_listWidget->currentItem())
        name = profile_listWidget->currentItem()->text();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Load", "Choose or write profile name.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Items")).filePath(name + ".json");
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
    const QJsonArray arr = root.value("waypoints").toArray();
    if (arr.isEmpty()) {
        QMessageBox::information(nullptr, "Load", "No waypoints in file.");
        lootList_listWidget->clear();
        return;
    }

    lootList_listWidget->clear();

    profile_lineEdit->setText(name);
    const auto matches = profile_listWidget->findItems(name, Qt::MatchExactly);
    if (!matches.isEmpty())
        profile_listWidget->setCurrentItem(matches.first());

    QMessageBox::information(nullptr, "Load", "Profile loaded: " + name);
}

void LootTab::clearItemList() const {
    lootList_listWidget->clear();
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
void LootTab::startLootThread(int container_index) {
    if (lootThread) {
        lootThread->stop();
        lootThread->wait();
        delete lootThread;
        lootThread = nullptr;
    }
    QList<QVariantMap> items;
    for (int i = 0; i < lootList_listWidget->count(); ++i) {
        QListWidgetItem* item = lootList_listWidget->item(i);
        QVariantMap data = item->data(Qt::UserRole).toMap();
        items.append(data);
    }
    lootThread = new LootThread(items, container_index);
    lootThread->start();
}

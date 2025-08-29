#include "walker_tab.h"

#include <filesystem>
#include <iostream>
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
#include <QRadioButton>

#include "../Functions/Game.h"

WalkerTab::WalkerTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Walker");
    waypointList_listWidget = new QListWidget(this);
    connect(waypointList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
    delete waypointList_listWidget->takeItem(waypointList_listWidget->row(item));});
    connect(waypointList_listWidget, &QListWidget::itemSelectionChanged, this, [this]() {
    auto* item = waypointList_listWidget->currentItem();
    const QVariantMap map = item->data(Qt::UserRole).toMap();
    const QString action  = map.value("action").toString();

    if (map.value("option").toString() == "Action" || map.value("option").toString() == "Use") {
        action_textEdit->setPlainText(action);
    } else {
        action_textEdit->clear();
    }});
    profile_listWidget = new QListWidget(this);
    action_textEdit = new QTextEdit(this);
    action_textEdit->setEnabled(false);
    auto* layout = new QGridLayout(this);
    setLayout(layout);
    waypointList();
    profileList();
}

void WalkerTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto profileName = new QLineEdit();

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    QDir dir("Save/Waypoints");
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
        waypointList_listWidget->clear();
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

void WalkerTab::waypointList() {
    auto groupbox = new QGroupBox("Waypoints", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);


    auto add_button = new QPushButton("Add Waypoint", this);
    connect(add_button, &QPushButton::clicked, this, &WalkerTab::addWaypoint);

    auto clearList_button = new QPushButton("Clear List", this);
    connect(clearList_button, &QPushButton::clicked, this, &WalkerTab::clearList);


    // --- Directions ---
    QStringList directions = {"NW", "N", "NE", "W", "C", "E", "SW", "S", "SE"};
    auto directionsGroup = new QGroupBox("Directions", this);
    auto directionsLayout = new QGridLayout();
    directions_buttonGroup = new QButtonGroup(this);
    directions_buttonGroup->setExclusive(true);

    int idx = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            auto* rb = new QRadioButton(directions[idx], this);
            directionsLayout->addWidget(rb, row, col);
            directions_buttonGroup->addButton(rb);
            if (directions[idx++] == "C") rb->setChecked(true);
        }
    }
    directionsGroup->setLayout(directionsLayout);

    // --- Options (Stand..Use) ---
    QStringList options = {"Stand", "Node", "Lure", "Label", "Action", "Use"};
    auto optionsGroup = new QGroupBox("Options", this);
    auto optionsLayout = new QGridLayout();
    options_buttonGroup = new QButtonGroup(this);
    options_buttonGroup->setExclusive(true);

    idx = 0;
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            auto* rb = new QRadioButton(options[idx], this);
            optionsLayout->addWidget(rb, row, col);
            options_buttonGroup->addButton(rb);
            if (options[idx++] == "Stand") rb->setChecked(true);
        }
    }
    connect(options_buttonGroup,static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>(&QButtonGroup::buttonToggled),this,[this](QAbstractButton* btn, bool checked) {
            if (!checked) return;
            const QString t = btn->text();
            const bool allow = (t == "Label" || t == "Action" || t == "Use");
            action_textEdit->setEnabled(allow);
        if (t == "Label") {
            action_textEdit->setPlaceholderText("Start");
        } else if (t == "Action") {
            action_textEdit->setPlaceholderText("if (api.cap() < 400) { api.label(\"Bank\") } else { api.label(\"Start\")}\n""api.sleep(1000);");
        } else if (t == "Use") {
            action_textEdit->setPlaceholderText("0 - Use \n""2148 - Use ItemID");
        } else {
            action_textEdit->setPlaceholderText("");
            action_textEdit->setText("");
        }
        });


    optionsGroup->setLayout(optionsLayout);


    auto layout_left = new QVBoxLayout();
    layout_left->addWidget(waypointList_listWidget);
    layout_left->addWidget(clearList_button);

    auto layout_right = new QVBoxLayout();
    layout_right->addWidget(add_button);
    layout_right->addWidget(directionsGroup);
    layout_right->addWidget(optionsGroup);
    layout_right->addWidget(action_textEdit);


    groupbox_layout->addLayout(layout_left);
    groupbox_layout->addLayout(layout_right);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}

void WalkerTab::addWaypoint() const {
    auto* dirBtn = directions_buttonGroup ? directions_buttonGroup->checkedButton() : nullptr;
    auto* optBtn = options_buttonGroup ? options_buttonGroup->checkedButton() : nullptr;

    const QString direction = dirBtn->text();
    const QString option    = optBtn->text();

    uint32_t x = Game::map_view->LocalPlayer->x;
    uint32_t y = Game::map_view->LocalPlayer->y;
    uint16_t z = Game::map_view->LocalPlayer->z;
    QString itemText;
    if (option == "Label") {
        itemText = action_textEdit->toPlainText().trimmed();
    } else {
        itemText = QString("%1 %2 %3 %4 %5").arg(option).arg(x).arg(y).arg(z).arg(direction);
    }

    auto* item = new QListWidgetItem(itemText);
    static const QMap<QString, QPoint> dirOffsets = {
        {"N",  { 0, -1}},
        {"S",  { 0,  1}},
        {"E",  { 1,  0}},
        {"W",  {-1,  0}},
        {"NW", {-1, -1}},
        {"NE", { 1, -1}},
        {"SW", {-1,  1}},
        {"SE", { 1,  1}}
    };
    if (dirOffsets.contains(direction)) {
        x += dirOffsets[direction].x();
        y += dirOffsets[direction].y();
    }

    QVariantMap data;
    data["x"] = x;
    data["y"] = y;
    data["z"] = z;
    data["direction"] = direction;
    data["option"] = option;
    data["label"] = "";
    data["action"] = "";
    if (option == "Label") {data["label"] = action_textEdit->toPlainText();}
    if (option == "Action") {data["action"] = action_textEdit->toPlainText();}
    if (option == "Use") {data["action"] = QString("api.useWith(%1, %2, %3, %4);").arg(action_textEdit->toPlainText()).arg(x).arg(y).arg(z);}
    action_textEdit->clear();

    item->setData(Qt::UserRole, data);
    waypointList_listWidget->addItem(item);
}

// Start Profile Functions

void WalkerTab::loadProfile(const QString& profileName) {
    QList<QVariantMap> m_loaded = loadProfileSignal("Waypoints", profileName);
    for (auto item: m_loaded) {
        QString itemText;
        auto option = item.value("option").toString();
        auto label = item.value("label").toString();
        auto x = item.value("x").toInt();
        auto y = item.value("y").toInt();
        auto z = item.value("z").toInt();
        auto direction = item.value("direction").toString();
        if (option == "Label") {
            itemText = label;
        } else {
            itemText = QString("%1 %2 %3 %4 %5").arg(option).arg(x).arg(y).arg(z).arg(direction);
        }
        auto* data = new QListWidgetItem(itemText);
        data->setData(Qt::UserRole, item);
        waypointList_listWidget->addItem(data);
    }
}

void WalkerTab::saveProfile(const QString& profileName) {
    QList<QVariantMap> targets;
    for (int i = 0; i < waypointList_listWidget->count(); ++i) {
        QListWidgetItem* item = waypointList_listWidget->item(i);
        QVariantMap data = item->data(Qt::UserRole).toMap();
        targets.append(data);
    }
    if (saveProfileSignal("Waypoints", profileName, targets)) {
        profile_listWidget->addItem(profileName);
    }
}

void WalkerTab::clearList() const {
    waypointList_listWidget->clear();
}

// End Profile Functions

void WalkerTab::onWalkerIndexUpdate(int idx) const
{
    if (!waypointList_listWidget) return;
    if (idx >= 0 && idx < waypointList_listWidget->count()) {
        waypointList_listWidget->setCurrentRow(idx);
        waypointList_listWidget->scrollToItem(
            waypointList_listWidget->item(idx),
            QAbstractItemView::PositionAtCenter);
    }
}

void WalkerTab::setWalkerEnabled(bool on) {
    if (on) {
        if (walkerThread) return;
        QList<QVariantMap> waypoints;
        waypoints.reserve(waypointList_listWidget->count());
        for (int i = 0; i < waypointList_listWidget->count(); ++i) {
            auto* item = waypointList_listWidget->item(i);
            waypoints.append(item->data(Qt::UserRole).toMap());
        }
        walkerThread = new WalkerThread(waypoints);
        connect(walkerThread, &WalkerThread::indexUpdate,this,&WalkerTab::onWalkerIndexUpdate,Qt::QueuedConnection);
        walkerThread->start();
    } else {
        if (!walkerThread) return;
        QMetaObject::invokeMethod(walkerThread, "stop", Qt::QueuedConnection);
        connect(walkerThread, &QThread::finished, this, [this]{
            walkerThread->deleteLater();
            walkerThread = nullptr;
        });
    }
}

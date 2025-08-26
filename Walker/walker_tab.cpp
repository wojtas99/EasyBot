#include "walker_tab.h"

#include <filesystem>
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
#include <QRadioButton>
#include <QSpinBox>

#include "../Functions/Game.h"

WalkerTab::WalkerTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Walker");

    waypointList_listWidget = new QListWidget(this);
    connect(waypointList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
    delete waypointList_listWidget->takeItem(waypointList_listWidget->row(item));});

    profile_listWidget = new QListWidget(this);
    profile_lineEdit = new QLineEdit(this);

    action_textEdit = new QTextEdit(this);
    action_textEdit->setEnabled(false);
    auto* layout = new QGridLayout(this);
    setLayout(layout);
    waypointList();
    profileList();

}

void WalkerTab::waypointList() {
    auto groupbox = new QGroupBox("Waypoints", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);


    auto add_button = new QPushButton("Add Waypoint", this);
    connect(add_button, &QPushButton::clicked, this, &WalkerTab::addWaypoint);

    auto clearWaypointList_button = new QPushButton("Clear List", this);
    connect(clearWaypointList_button, &QPushButton::clicked, this, &WalkerTab::clearWaypointList);


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
    QStringList options = {"Stand", "Node", "Lure", "Shovel", "Rope", "Pick", "Label", "Action", "Use"};
    auto optionsGroup = new QGroupBox("Options", this);
    auto optionsLayout = new QGridLayout();
    options_buttonGroup = new QButtonGroup(this);
    options_buttonGroup->setExclusive(true);

    idx = 0;
    for (int row = 0; row < 3; ++row) {
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
            const bool allow = (t == "Label" || t == "Action");
            action_textEdit->setEnabled(allow);
        });


    optionsGroup->setLayout(optionsLayout);


    auto layout_left = new QVBoxLayout();
    layout_left->addWidget(waypointList_listWidget);
    layout_left->addWidget(clearWaypointList_button);

    auto layout_right = new QVBoxLayout();
    layout_right->addWidget(add_button);
    layout_right->addWidget(directionsGroup);
    layout_right->addWidget(optionsGroup);
    layout_right->addWidget(action_textEdit);


    groupbox_layout->addLayout(layout_left);
    groupbox_layout->addLayout(layout_right);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}



void WalkerTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");
    connect(save_button, &QPushButton::clicked, this, &WalkerTab::saveProfile);
    connect(load_button, &QPushButton::clicked, this, &WalkerTab::loadProfile);

    QDir dir("Save/Waypoints");
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


void WalkerTab::saveProfile() const {
    const QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Save", "Give profile name.");
        return;
    }
    if (waypointList_listWidget->count() == 0) {
        QMessageBox::information(nullptr, "Save", "There is no waypoints to save.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Waypoints")).filePath(name + ".json");

    QJsonArray wptArray;
    for (int i = 0; i < waypointList_listWidget->count(); ++i) {
        const auto* item = waypointList_listWidget->item(i);
        const QVariantMap map = item->data(Qt::UserRole).toMap();
        wptArray.append(QJsonObject::fromVariantMap(map));
    }

    QJsonObject root;
    root["version"] = 1;
    root["waypoints"] = wptArray;

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

void WalkerTab::loadProfile() const {
    QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty() && profile_listWidget->currentItem())
        name = profile_listWidget->currentItem()->text();
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, "Load", "Choose or write profile name.");
        return;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/Waypoints")).filePath(name + ".json");
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
        waypointList_listWidget->clear();
        return;
    }

    waypointList_listWidget->clear();
    for (const QJsonValue& v : arr) {
        const QJsonObject obj = v.toObject();
        const QString option    = obj.value("option").toString();
        const QString label    = obj.value("label").toString();
        const QString direction = obj.value("direction").toString();
        const uint32_t x = obj.value("x").toInt();
        const uint32_t y = obj.value("y").toInt();
        const uint16_t z = static_cast<uint16_t>(obj.value("z").toInt());

        QString itemText;
        if (option == "Label") {
            itemText = label;
        } else {
            itemText = QString("%1 %2 %3 %4 %5").arg(option).arg(x).arg(y).arg(z).arg(direction);
        }

        auto* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, obj.toVariantMap());
        waypointList_listWidget->addItem(item);
    }

    profile_lineEdit->setText(name);
    const auto matches = profile_listWidget->findItems(name, Qt::MatchExactly);
    if (!matches.isEmpty())
        profile_listWidget->setCurrentItem(matches.first());

    QMessageBox::information(nullptr, "Load", "Profile loaded: " + name);
}

void WalkerTab::clearWaypointList() const {
    waypointList_listWidget->clear();
}

void WalkerTab::addWaypoint() const
{
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
    action_textEdit->clear();

    item->setData(Qt::UserRole, data);
    waypointList_listWidget->addItem(item);
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
        walkerThread->stop();
        walkerThread->wait();
        delete walkerThread;
        walkerThread = nullptr;
    }
}

void WalkerTab::onWalkerIndexUpdate(int idx) {
    if (!waypointList_listWidget) return;
    if (idx >= 0 && idx < waypointList_listWidget->count()) {
        waypointList_listWidget->setCurrentRow(idx);
        waypointList_listWidget->scrollToItem(
            waypointList_listWidget->item(idx),
            QAbstractItemView::PositionAtCenter);
    }
}
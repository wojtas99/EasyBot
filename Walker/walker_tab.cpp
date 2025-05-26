#include "walker_tab.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <__msvc_ostream.hpp>

#include "../Functions/memory_functions.h"

WalkerTab::WalkerTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Walker");
    setFixedSize(350, 400);

    status_label = new QLabel("", this);
    status_label->setStyleSheet("color: red; font-weight: bold;");
    status_label->setAlignment(Qt::AlignCenter);

    waypointList_listWidget = new QListWidget(this);
    profile_listWidget = new QListWidget(this);
    profile_lineEdit = new QLineEdit(this);
    action_textEdit = new QTextEdit(this);
    record_checkBox = new QCheckBox("Auto Recording", this);
    start_checkBox = new QCheckBox("Start Walker", this);
    option_comboBox = new QComboBox(this);
    QStringList directions = {
        "Center", "North", "South", "East", "West",
        "North-East", "North-West", "South-East", "South-West", "Lure"
    };
    option_comboBox->addItems(directions);

    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    profileList();
    waypointList();
    start_walker();

    layout->addWidget(status_label, 3, 0, 1, 2);
}

void WalkerTab::profileList() {
    QGroupBox* groupbox = new QGroupBox("Save && Load", this);
    QVBoxLayout* groupbox_layout = new QVBoxLayout(groupbox);

    QPushButton* save_button = new QPushButton("Save");
    QPushButton* load_button = new QPushButton("Load");
    connect(save_button, &QPushButton::clicked, this, &WalkerTab::saveProfile);
    connect(load_button, &QPushButton::clicked, this, &WalkerTab::loadProfile);

    QDir dir("Save/Waypoints");
    QStringList filters("*.json");
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& file : files) {
        profile_listWidget->addItem(file.baseName());
    }

    QHBoxLayout* layout1 = new QHBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout1->addWidget(new QLabel("Name:", this));
    layout1->addWidget(profile_lineEdit);
    layout2->addWidget(save_button);
    layout2->addWidget(load_button);

    groupbox_layout->addWidget(profile_listWidget);
    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);

    static_cast<QGridLayout*>(layout())->addWidget(groupbox, 2, 0);
}

void WalkerTab::waypointList() {
    QGroupBox* groupbox = new QGroupBox("Waypoints", this);
    QHBoxLayout* groupbox_layout = new QHBoxLayout(groupbox);

    QPushButton* stand_waypoint_button = new QPushButton("Stand", this);
    QPushButton* rope_waypoint_button = new QPushButton("Rope", this);
    QPushButton* shovel_waypoint_button = new QPushButton("Shovel", this);
    QPushButton* ladder_waypoint_button = new QPushButton("Ladder", this);
    QPushButton* action_waypoint_button = new QPushButton("Action", this);
    QPushButton* label_waypoint_button = new QPushButton("Label", this);
    QPushButton* clearWaypointList_button = new QPushButton("Clear List", this);

    connect(clearWaypointList_button, &QPushButton::clicked, this, &WalkerTab::clearWaypointList);
    connect(stand_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint(0); });
    connect(rope_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint(1); });
    connect(shovel_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint(2); });
    connect(ladder_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint(3); });
    connect(action_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint(4); });
    connect(label_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint(5); });

    QVBoxLayout* groupbox2_layout = new QVBoxLayout();
    QVBoxLayout* layout1 = new QVBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();
    QHBoxLayout* layout3 = new QHBoxLayout();
    QHBoxLayout* layout4 = new QHBoxLayout();

    layout1->addWidget(waypointList_listWidget);
    layout1->addWidget(clearWaypointList_button);

    layout2->addWidget(option_comboBox);

    layout3->addWidget(stand_waypoint_button);
    layout3->addWidget(action_waypoint_button);
    layout3->addWidget(label_waypoint_button);

    layout4->addWidget(rope_waypoint_button);
    layout4->addWidget(shovel_waypoint_button);
    layout4->addWidget(ladder_waypoint_button);

    groupbox2_layout->addLayout(layout2);
    groupbox2_layout->addLayout(layout3);
    groupbox2_layout->addLayout(layout4);
    groupbox2_layout->addWidget(action_textEdit);

    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(groupbox2_layout);

    static_cast<QGridLayout*>(layout())->addWidget(groupbox, 0, 0, 1, 2);
}

void WalkerTab::start_walker() {
    QGroupBox* groupbox = new QGroupBox("Start", this);
    QVBoxLayout* groupbox_layout = new QVBoxLayout(groupbox);

    connect(start_checkBox, &QCheckBox::stateChanged, this, &WalkerTab::startWalkerThread);
    connect(record_checkBox, &QCheckBox::stateChanged, this, &WalkerTab::startRecordThread);

    QHBoxLayout* layout1 = new QHBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout1->addWidget(start_checkBox);
    layout2->addWidget(record_checkBox);

    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);

    static_cast<QGridLayout*>(layout())->addWidget(groupbox, 2, 1);
}

void WalkerTab::saveProfile() {
    // Implementacja zapisu profilu do JSON (QJsonDocument)
}

void WalkerTab::loadProfile() {
    // Implementacja ładowania profilu z JSON
}

void WalkerTab::addWaypoint(int index) {
    //MemoryFunctions::moveTo(32021, 31920, 5);
    //MemoryFunctions::openContainer();
    MemoryFunctions::say();
    auto wpt = MemoryFunctions::read_my_wpt();
    int x = wpt[0];
    int y = wpt[1];
    int z = wpt[2];

    QString itemText = QString("Waypoint: %1 %2 %3").arg(x).arg(y).arg(z);
    QListWidgetItem* item = new QListWidgetItem(itemText);
    QVariantMap data;
    data["x"] = x;
    data["y"] = y;
    data["z"] = z;
    item->setData(Qt::UserRole, data);
    waypointList_listWidget->addItem(item);
    status_label->setStyleSheet("color: green; font-weight: bold;");
    status_label->setText("Waypoint added!");
}


void WalkerTab::clearWaypointList() {
    waypointList_listWidget->clear();
    status_label->setText("");
}

void WalkerTab::startWalkerThread(int state) {
    if (start_checkBox->isChecked() == true) {
        if (!walkerThread) {
            QList<QVariantMap> waypoints;
            for (int i = 0; i < waypointList_listWidget->count(); ++i) {
                QListWidgetItem* item = waypointList_listWidget->item(i);
                QVariantMap data = item->data(Qt::UserRole).toMap();
                waypoints.append(data);
            }
            walkerThread = new WalkerThread(waypoints);
            connect(walkerThread, &WalkerThread::indexUpdate, this, [this](int wpt_index) {
                waypointList_listWidget->setCurrentRow(wpt_index);
            });
            std::cout<<"Thread created"<< std::endl;
            walkerThread->start();
        }
    } else {
        if (walkerThread) {
            walkerThread->stop();
            walkerThread->wait();
            delete walkerThread;
            walkerThread = nullptr;
        }
    }
}

void WalkerTab::startRecordThread(int state) {
    // Obsługa wątku record
}

#include "walker_tab.h"

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
#include <QSaveFile>

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

    option_comboBox = new QComboBox(this);
    QStringList options = {
        "Center", "North", "South", "East", "West",
        "North-East", "North-West", "South-East", "South-West", "Lure"
    };
    option_comboBox->addItems(options);

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    waypointList();
    profileList();

    layout->addWidget(status_label, 3, 0, 1, 2);
}

void WalkerTab::waypointList() {
    auto groupbox = new QGroupBox("Waypoints", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);

    auto start_checkBox = new QCheckBox("Start Walker", this);

    auto stand_waypoint_button = new QPushButton("Stand", this);
    auto rope_waypoint_button = new QPushButton("Rope", this);
    auto shovel_waypoint_button = new QPushButton("Shovel", this);
    auto ladder_waypoint_button = new QPushButton("Ladder", this);
    auto action_waypoint_button = new QPushButton("Action", this);
    auto label_waypoint_button = new QPushButton("Label", this);
    auto clearWaypointList_button = new QPushButton("Clear List", this);

    connect(clearWaypointList_button, &QPushButton::clicked, this, &WalkerTab::clearWaypointList);
    connect(stand_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint("Stand"); });
    connect(rope_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint("Rope"); });
    connect(shovel_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint("Shovel"); });
    connect(ladder_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint("Ladder"); });
    connect(action_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint("Action"); });
    connect(label_waypoint_button, &QPushButton::clicked, [this]() { addWaypoint("Label"); });

    connect(waypointList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        delete waypointList_listWidget->takeItem(waypointList_listWidget->row(item));
    });

    connect(start_checkBox, &QCheckBox::stateChanged, this, &WalkerTab::startWalkerThread);

    auto groupbox2_layout = new QVBoxLayout();
    auto layout1 = new QVBoxLayout();
    auto layout2 = new QHBoxLayout();
    auto layout3 = new QHBoxLayout();
    auto layout4 = new QHBoxLayout();

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
    groupbox2_layout->addWidget(start_checkBox);
    groupbox2_layout->addStretch();

    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(groupbox2_layout);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 0, 0, 1, 2);
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

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 1, 0, 1, 1);
    dynamic_cast<QGridLayout*>(layout())->setColumnStretch(0, 1);
    dynamic_cast<QGridLayout*>(layout())->setColumnStretch(1, 1);

}

void WalkerTab::saveProfile() const {
    const QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty()) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText("Set profile name!");
        return;
    }
    QJsonArray wptArray;
    for (int i = 0; i < waypointList_listWidget->count(); ++i) {
        auto *item = waypointList_listWidget->item(i);
        QJsonObject obj =
            QJsonObject::fromVariantMap(item->data(Qt::UserRole).toMap());
        obj["wpt"] = item->text().split(' ')[0];
        wptArray.append(obj);
    }

    QJsonDocument doc(QJsonObject{{"waypoints", wptArray}});
    const QString fullPath =
        QString("%1/Save/Waypoints/%2.json").arg(QDir::currentPath(), name);
    QSaveFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText(QString("Open failed: %1").arg(file.errorString()));
        return;
    }
    if (file.write(doc.toJson(QJsonDocument::Indented)) == -1) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText(QString("Write failed: %1").arg(file.errorString()));
        return;
    }
    if (!file.commit()) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText(QString("Commit failed: %1").arg(file.errorString()));
        return;
    }
    if (profile_listWidget->findItems(name, Qt::MatchExactly).isEmpty())
        profile_listWidget->addItem(name);
    status_label->setStyleSheet("color:green;font-weight:bold;");
    status_label->setText(QString("Profile %1 saved.").arg(name));
    profile_lineEdit->clear();
}

void WalkerTab::loadProfile() const {
    QString name = profile_lineEdit->text().trimmed();
    if (name.isEmpty() && profile_listWidget->currentItem())
        name = profile_listWidget->currentItem()->text();

    if (name.isEmpty()) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText("No profile selected!");
        return;
    }
    const QString fullPath =
        QString("%1/Save/Waypoints/%2.json").arg(QDir::currentPath(), name);

    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly)) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText(QString("Open failed: %1").arg(file.errorString()));
        return;
    }

    QJsonParseError perr;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &perr);
    if (doc.isNull()) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText(QString("Parse error: %1 (offset %2)")
                              .arg(perr.errorString()).arg(perr.offset));
        return;
    }
    const QJsonArray arr = doc["waypoints"].toArray();
    if (arr.isEmpty()) {
        status_label->setStyleSheet("color:red;font-weight:bold;");
        status_label->setText("No waypoints found in file!");
        return;
    }
    waypointList_listWidget->clear();

    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        QString wpt    = obj["wpt"].toString();
        uint32_t x     = obj["x"].toInt();
        uint32_t y     = obj["y"].toInt();
        uint16_t z     = static_cast<uint16_t>(obj["z"].toInt());
        QString option = obj["option"].toString();

        QString itemText =
            QString("%1 %2 %3 %4 %5").arg(wpt).arg(x).arg(y).arg(z).arg(option);

        auto *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, obj.toVariantMap()); // stash the full map
        waypointList_listWidget->addItem(item);
    }

    status_label->setStyleSheet("color:green;font-weight:bold;");
    status_label->setText(QString("Profile %1 loaded.").arg(name));

    profile_lineEdit->setText(name);
    auto matches = profile_listWidget->findItems(name, Qt::MatchExactly);
    if (!matches.isEmpty())
        profile_listWidget->setCurrentItem(matches.first());
}



void WalkerTab::addWaypoint(const QString& index) const {
    uint32_t x = MemoryFunctions::map_view->LocalPlayer->x;
    uint32_t y = MemoryFunctions::map_view->LocalPlayer->y;
    uint16_t z = MemoryFunctions::map_view->LocalPlayer->z;

    if (option_comboBox->currentText() == "North") {
        y = y-1;
    } else if (option_comboBox->currentText() == "South") {

        y = y+1;
    } else if (option_comboBox->currentText() == "East") {
        x = x+1;
    } else if (option_comboBox->currentText() == "West") {
        x = x-1;
    }

    QString itemText = QString("%1 %2 %3 %4 %5").arg(index).arg(x).arg(y).arg(z).arg(option_comboBox->currentText());
    auto* item = new QListWidgetItem(itemText);
    QVariantMap data;
    data["x"] = x;
    data["y"] = y;
    data["z"] = z;
    data["option"] = option_comboBox->currentText();
    data["wpt"] = index;
    item->setData(Qt::UserRole, data);

    waypointList_listWidget->addItem(item);
    option_comboBox->setCurrentIndex(0);
    status_label->setStyleSheet("color: green; font-weight: bold;");
    status_label->setText(QString("Wpt %1 %2 %3 added!").arg(x).arg(y).arg(z));
}

void WalkerTab::clearWaypointList() const {
    waypointList_listWidget->clear();
    status_label->setText("");
}

void WalkerTab::startWalkerThread(int state) {
    if (state) {
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

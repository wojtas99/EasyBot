#include "script_tab.h"

#include <filesystem>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QSaveFile>
#include <QRadioButton>

#include "../Functions/Game.h"

ScriptTab::ScriptTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Script");

    scriptList_listWidget = new QListWidget(this);

    profile_listWidget = new QListWidget(this);

    auto* layout = new QGridLayout(this);
    setLayout(layout);
    scriptList();
    profileList();

}

void ScriptTab::profileList() {
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto profileName = new QLineEdit();

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    QDir dir("Save/Scripts");
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
        scriptList_listWidget->clear();
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

void ScriptTab::scriptList() {
    auto groupbox = new QGroupBox("Scripts", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);

    auto del_button = new QPushButton("Delete Script");
    auto add_button = new QPushButton("Add Script");

    auto sleep_lineEdit = new QLineEdit();
    sleep_lineEdit->setValidator(new QIntValidator(0, INT_MAX));
    sleep_lineEdit->setPlaceholderText("1000 - Time in ms");

    auto script_textEdit = new QTextEdit();
    script_textEdit->setPlaceholderText("api.say(\"exura\")");

    auto clearList_button = new QPushButton("Clear List", this);
    connect(clearList_button, &QPushButton::clicked, this, &ScriptTab::clearList);


    auto left_layout = new QVBoxLayout();
    auto right_layout = new QVBoxLayout();

    left_layout->addWidget(scriptList_listWidget);
    left_layout->addWidget(del_button);
    left_layout->addWidget(clearList_button);

    right_layout->addWidget(sleep_lineEdit);
    right_layout->addWidget(script_textEdit);
    right_layout->addWidget(add_button);

    groupbox_layout->addLayout(left_layout);
    groupbox_layout->addLayout(right_layout);

    groupbox->setLayout(groupbox_layout);

    connect(add_button, &QPushButton::clicked, [this, script_textEdit, sleep_lineEdit](){
        if (!script_textEdit->toPlainText().isEmpty() && !sleep_lineEdit->text().isEmpty()) {
            addScript(script_textEdit->toPlainText().trimmed(), sleep_lineEdit->text().toInt());
             script_textEdit->clear();
             sleep_lineEdit->clear();
        }
    });

    connect(scriptList_listWidget, &QListWidget::itemChanged, this, &ScriptTab::onItemChanged);
    connect(del_button, &QPushButton::clicked, this, &ScriptTab::deleteSelected);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}

void ScriptTab::addScript(const QString& scriptData, int sleep_time) const {
    auto* item = new QListWidgetItem(scriptData);
    item->setFlags(item->flags()
                   | Qt::ItemIsUserCheckable
                   | Qt::ItemIsSelectable
                   | Qt::ItemIsEnabled);
    item->setCheckState(Qt::CheckState::Unchecked);
    QVariantMap data;
    data["script"] = scriptData;
    data["loop"] = sleep_time;
    item->setData(Qt::UserRole, data);
    scriptList_listWidget->addItem(item);
}

// Start Profile Functions

void ScriptTab::loadProfile(const QString& profileName) {
    QList<QVariantMap> m_loaded = loadProfileSignal("Scripts", profileName);
    for (auto item: m_loaded) {
        auto script = item.value("script").toString();
        auto loop = item.value("loop").toString();
        auto* data = new QListWidgetItem(script);
        data->setFlags(data->flags()
               | Qt::ItemIsUserCheckable
               | Qt::ItemIsSelectable
               | Qt::ItemIsEnabled);
        data->setCheckState(Qt::CheckState::Unchecked);
        data->setData(Qt::UserRole, item);
        scriptList_listWidget->addItem(data);
    }
}

void ScriptTab::saveProfile(const QString& profileName) {
    QList<QVariantMap> targets;
    for (int i = 0; i < scriptList_listWidget->count(); ++i) {
        QListWidgetItem* item = scriptList_listWidget->item(i);
        QVariantMap data = item->data(Qt::UserRole).toMap();
        targets.append(data);
    }
    if (saveProfileSignal("Scripts", profileName, targets)) {
        profile_listWidget->addItem(profileName);
    }
}

void ScriptTab::clearList() {
    stopAllThreads();
    scriptList_listWidget->clear();
}

void ScriptTab::deleteSelected() {
    QListWidgetItem* item = scriptList_listWidget->currentItem();
    if (item) {
        stopAndRemoveThread(item);
        delete scriptList_listWidget->takeItem(scriptList_listWidget->row(item));
    }
}

// End Profile Functions

void ScriptTab::setScriptsEnabled(bool on) const {
    scriptList_listWidget->clear();
}

void ScriptTab::onItemChanged(QListWidgetItem* item) {
    if (item->checkState()) {
        if (!m_scriptThreads.contains(item)) {
            const QVariantMap data = item->data(Qt::UserRole).toMap();
            auto* th = new ScriptThread(data, this);
            m_scriptThreads.insert(item, th);
            th->start();
        }
    } else {
        stopAndRemoveThread(item);
    }
}

void ScriptTab::stopAndRemoveThread(QListWidgetItem* item) {
    auto it = m_scriptThreads.find(item);
    if (it == m_scriptThreads.end()) return;

    ScriptThread* th = it.value();
    m_scriptThreads.erase(it);
    connect(th, &QThread::finished, this, [th]{
        th->deleteLater();
    }, Qt::UniqueConnection);
    QMetaObject::invokeMethod(th, "stop", Qt::QueuedConnection);
}

void ScriptTab::stopAllThreads() {
    const auto keys = m_scriptThreads.keys();
    for (QListWidgetItem* item : keys) {
        stopAndRemoveThread(item);
    }
}

ScriptTab::~ScriptTab() {
    stopAllThreads();
}

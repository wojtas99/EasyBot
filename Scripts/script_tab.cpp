#include "script_tab.h"

#include <filesystem>
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

    auto add_button = new QPushButton("Add Script");
    auto script_textEdit = new QTextEdit();

    auto clearList_button = new QPushButton("Clear List", this);
    connect(clearList_button, &QPushButton::clicked, this, &ScriptTab::clearList);


    auto left_layout = new QVBoxLayout();
    auto right_layout = new QVBoxLayout();

    left_layout->addWidget(scriptList_listWidget);
    left_layout->addWidget(clearList_button);

    right_layout->addWidget(script_textEdit);
    right_layout->addWidget(add_button);

    groupbox_layout->addLayout(left_layout);
    groupbox_layout->addLayout(right_layout);

    groupbox->setLayout(groupbox_layout);

    connect(add_button, &QPushButton::clicked, this, [=]{
        const QString code = script_textEdit->toPlainText().trimmed();
        if (code.isEmpty()) return;

        QString name = code.section('\n', 0, 0).trimmed();
        if (name.isEmpty())
            name = QString("Script %1").arg(scriptList_listWidget->count() + 1);

        auto* item = new QListWidgetItem(name, scriptList_listWidget);
        item->setFlags(item->flags()
                       | Qt::ItemIsUserCheckable
                       | Qt::ItemIsSelectable
                       | Qt::ItemIsEnabled
                       | Qt::ItemIsEditable);
        item->setCheckState(Qt::Checked); // domyślnie włączony

        QVariantMap data;
        data["name"] = name;
        data["code"] = code;
        data["enabled"] = true;
        item->setData(Qt::UserRole, data);

        script_textEdit->clear();
    });

    // NOWE: aktualizuj UserRole, gdy user odznaczy/zaznaczy albo zmieni nazwę
    connect(scriptList_listWidget, &QListWidget::itemChanged,
            this, [=](QListWidgetItem* item){
        QVariantMap m = item->data(Qt::UserRole).toMap();
        m["enabled"] = (item->checkState() == Qt::Checked);
        m["name"] = item->text();
        item->setData(Qt::UserRole, m);
    });

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox);
}


// Start Profile Functions

void ScriptTab::loadProfile(const QString& profileName) {
    QList<QVariantMap> m_loaded = loadProfileSignal("Scripts", profileName);
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

void ScriptTab::clearList() const {
    scriptList_listWidget->clear();
}

// End Profile Functions

void ScriptTab::setScriptsEnabled(bool on) {
}

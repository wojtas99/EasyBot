#include "status_tab.h"

#include <iostream>
#include <QDir>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QSaveFile>
#include <__msvc_ostream.hpp>


StatusTab::StatusTab(QWidget* parent) : QWidget(parent) {
    auto* g = new QGroupBox("Features", this);
    auto* v = new QVBoxLayout(g);

    walker_checkBox = new QCheckBox("Enable Walking", g);
    walker_checkBox->setChecked(false);
    v->addWidget(walker_checkBox);
    target_checkBox = new QCheckBox("Enable Targeting", g);
    target_checkBox->setChecked(false);
    v->addWidget(target_checkBox);
    loot_checkBox = new QCheckBox("Enable Looting", g);
    loot_checkBox->setChecked(false);
    v->addWidget(loot_checkBox);
    healing_checkBox = new QCheckBox("Enable Healing", g);
    healing_checkBox->setChecked(false);
    v->addWidget(healing_checkBox);
    spells_checkBox = new QCheckBox("Enable Spells", g);
    spells_checkBox->setChecked(false);
    v->addWidget(spells_checkBox);
    script_checkBox= new QCheckBox("Enable Scripts", g);
    script_checkBox->setChecked(false);
    v->addWidget(script_checkBox);
    v->addStretch();

    auto* root = new QVBoxLayout(this);
    root->addWidget(g);
    root->addStretch();

    connect(walker_checkBox, &QCheckBox::toggled, this, &StatusTab::walkerToggled);
    connect(target_checkBox, &QCheckBox::toggled, this, &StatusTab::targetToggled);
    connect(loot_checkBox, &QCheckBox::toggled, this, &StatusTab::lootToggled);
    connect(healing_checkBox, &QCheckBox::toggled, this, &StatusTab::healingToggled);
    connect(spells_checkBox, &QCheckBox::toggled, this, &StatusTab::spellsToggled);
    connect(script_checkBox, &QCheckBox::toggled, this, &StatusTab::scriptsToggled);
}

bool StatusTab::saveProfile(const QString& tab, const QString& profileName, const QList<QVariantMap>& m_items) {
    if (profileName.isEmpty()) {
        QMessageBox::warning(nullptr, "Save", "Give profile name.");
        return false;
    }
    if (m_items.isEmpty()) {
        QMessageBox::information(nullptr, "Save", "There is no data to save.");
        return false;
    }
    const QString fullPath = QDir(QDir::current().filePath("Save/" + tab)).filePath(profileName + ".json");
    QJsonArray dataArray;
    for (const auto& item : m_items) {
        dataArray.append(QJsonObject::fromVariantMap(item));
    }
    QJsonObject root;
    root[tab] = dataArray;

    QSaveFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(nullptr, "Save", "Can't open file:\n" + file.errorString());
        return false;
    }
    const QByteArray json = QJsonDocument(root).toJson(QJsonDocument::Indented);
    if (file.write(json) == -1) {
        QMessageBox::critical(nullptr, "Save", "Save Error:\n" + file.errorString());
        return false;
    }
    if (!file.commit()) {
        QMessageBox::critical(nullptr, "Save", "Commit error:\n" + file.errorString());
        return false;
    }

    QMessageBox::information(nullptr, "Save", "Profile saved: " + profileName);
    return true;
}

QList<QVariantMap> StatusTab::loadProfile(const QString& tab, const QString& profileName)
{
    QList<QVariantMap> m_loaded;
    if (profileName.isEmpty()) {
        QMessageBox::warning(nullptr, "Load", "Choose or write profile name.");
        return m_loaded;
    }

    const QString fullPath = QDir(QDir::current().filePath("Save/" + tab)).filePath(profileName + ".json");

    QFile file(fullPath);
    if (!file.exists()) {
        QMessageBox::warning(nullptr, "Load", "File does not exist:\n" + fullPath);
        return m_loaded;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Load", "Cannot open file:\n" + file.errorString());
        return m_loaded;
    }

    QJsonParseError perr{};
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &perr);
    if (perr.error != QJsonParseError::NoError) {
        QMessageBox::critical(nullptr, "Load", "JSON parse error:\n" + perr.errorString());
        return m_loaded;
    }

    const QJsonObject root = doc.object();
    const QJsonArray arr = root.value(tab).toArray();
    if (arr.isEmpty()) {
        QMessageBox::information(nullptr, "Load", "No data in file.");
        return m_loaded;
    }

    m_loaded.reserve(arr.size());
    for (const QJsonValue& v : arr) {
        if (!v.isObject())
            continue;
        m_loaded.append(v.toObject().toVariantMap());
    }

    QMessageBox::information(nullptr, "Load", "Profile loaded: " + profileName);
    return m_loaded;
}





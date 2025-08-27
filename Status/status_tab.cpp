#include "status_tab.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>


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
    v->addStretch();

    auto* root = new QVBoxLayout(this);
    root->addWidget(g);
    root->addStretch();

    connect(walker_checkBox, &QCheckBox::toggled, this, &StatusTab::walkerToggled);
    connect(target_checkBox, &QCheckBox::toggled, this, &StatusTab::targetToggled);
    connect(loot_checkBox, &QCheckBox::toggled, this, &StatusTab::lootToggled);
    connect(healing_checkBox, &QCheckBox::toggled, this, &StatusTab::healingToggled);
    connect(spells_checkBox, &QCheckBox::toggled, this, &StatusTab::spellsToggled);
}


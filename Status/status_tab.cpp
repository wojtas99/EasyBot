#include "status_tab.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>


StatusTab::StatusTab(QWidget* parent) : QWidget(parent) {
    auto* g = new QGroupBox("Features", this);
    auto* v = new QVBoxLayout(g);

    walker_checkBox = new QCheckBox("Enable walker", g);
    walker_checkBox->setChecked(false);
    v->addWidget(walker_checkBox);
    v->addStretch();

    auto* root = new QVBoxLayout(this);
    root->addWidget(g);
    root->addStretch();

    connect(walker_checkBox, &QCheckBox::toggled, this, &StatusTab::walkerToggled);
}


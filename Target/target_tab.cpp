#include "target_tab.h"
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

TargetTab::TargetTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Walker");
    setFixedSize(350, 400);

    status_label = new QLabel("", this);
    status_label->setStyleSheet("color: red; font-weight: bold;");
    status_label->setAlignment(Qt::AlignCenter);

    targetList_listWidget = new QListWidget(this);

    targetName_textEdit = new QLineEdit(this);

    start_checkBox = new QCheckBox("Start Targeting", this);

    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    targetList();
    startTarget();

    layout->addWidget(status_label, 3, 0, 1, 2);
}

void TargetTab::targetList() {
    QGroupBox* groupbox = new QGroupBox("Targets", this);
    QHBoxLayout* groupbox_layout = new QHBoxLayout(groupbox);

    QPushButton* add_button = new QPushButton("Add", this);

    connect(add_button, &QPushButton::clicked, this, &TargetTab::addTarget);

    QVBoxLayout* groupbox2_layout = new QVBoxLayout();
    QVBoxLayout* layout1 = new QVBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();

    layout1->addWidget(targetList_listWidget);

    layout2->addWidget(targetName_textEdit);
    layout2->addWidget(add_button);

    groupbox2_layout->addLayout(layout2);

    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(groupbox2_layout);

    static_cast<QGridLayout*>(layout())->addWidget(groupbox, 0, 0, 1, 2);
}

void TargetTab::startTarget() {
    QGroupBox* groupbox = new QGroupBox("Start", this);
    QVBoxLayout* groupbox_layout = new QVBoxLayout(groupbox);

    connect(start_checkBox, &QCheckBox::stateChanged, this, &TargetTab::startTargetThread);

    QHBoxLayout* layout1 = new QHBoxLayout();
    layout1->addWidget(start_checkBox);

    groupbox_layout->addLayout(layout1);

    static_cast<QGridLayout*>(layout())->addWidget(groupbox, 2, 1);
}

void TargetTab::addTarget(int index) {
    QString itemText = QString(targetName_textEdit->text());
    QListWidgetItem* item = new QListWidgetItem(itemText);
    QVariantMap data;
    data["name"] = itemText;
    item->setData(Qt::UserRole, data);
    targetList_listWidget->addItem(item);
    status_label->setStyleSheet("color: green; font-weight: bold;");
    status_label->setText("Target added!");
}


void TargetTab::startTargetThread(int state) {
    if (start_checkBox->isChecked() == true) {
        if (!targetThread) {
            QList<QVariantMap> targets;
            for (int i = 0; i < targetList_listWidget->count(); ++i) {
                QListWidgetItem* item = targetList_listWidget->item(i);
                QVariantMap data = item->data(Qt::UserRole).toMap();
                targets.append(data);
            }
            targetThread = new TargetThread(targets);
            std::cout<<"Target Thread created"<< std::endl;
            targetThread->start();
        }
    } else {
        if (targetThread) {
            targetThread->stop();
            targetThread->wait();
            delete targetThread;
            targetThread = nullptr;
        }
    }
}


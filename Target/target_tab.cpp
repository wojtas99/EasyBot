#include "target_tab.h"
#include <iostream>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include <QLineEdit>
#include <QSlider>

#include "../Functions/memory_functions.h"

TargetTab::TargetTab(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Targeting");
    setFixedSize(350, 400);

    status_label = new QLabel("", this);
    status_label->setStyleSheet("color: red; font-weight: bold;");
    status_label->setAlignment(Qt::AlignCenter);

    targetList_listWidget = new QListWidget(this);
    targetList_listWidget->setMinimumWidth(150);
    targetList_listWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    targetName_lineEdit = new QLineEdit(this);
    hpFrom_lineEdit = new QLineEdit(this);
    hpTo_lineEdit = new QLineEdit(this);
    distanceSlider = new QSlider(Qt::Horizontal, this);
    distanceSlider->setMinimum(1);
    distanceSlider->setMaximum(7);
    distanceSlider->setSingleStep(1);

    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    targetList();

    layout->addWidget(status_label, 3, 0, 1, 2);
}

void TargetTab::targetList() {
    QGroupBox* groupbox = new QGroupBox("Targets", this);
    QHBoxLayout* groupbox_layout = new QHBoxLayout(groupbox);

    QPushButton* add_button = new QPushButton("Add", this);
    auto start_checkBox = new QCheckBox("Start Targeting", this);

    connect(add_button, &QPushButton::clicked, this, [this]() {
        int hpFrom = hpFrom_lineEdit->text().toInt();
        int hpTo = hpTo_lineEdit->text().toInt();
        int distance = distanceSlider->value();
        addTarget(targetName_lineEdit->text(), hpFrom, hpTo, distance);
    });

    connect(start_checkBox, &QCheckBox::stateChanged, this, &TargetTab::startTargetThread);

    QVBoxLayout* groupbox2_layout = new QVBoxLayout();

    QHBoxLayout* layout1 = new QHBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();
    QHBoxLayout* layout3 = new QHBoxLayout();

    QLabel* valueLabel = new QLabel("Distance: 1", this);
    connect(distanceSlider, &QSlider::valueChanged, this, [valueLabel](int value){
        valueLabel->setText(QString("Distance: %1").arg(value));
    });

    layout1->addWidget(new QLabel("Name:", this));
    layout1->addWidget(targetName_lineEdit);
    layout1->addWidget(add_button);

    layout2->addWidget(new QLabel("HP From", this));
    layout2->addWidget(hpFrom_lineEdit);
    layout2->addWidget(new QLabel("-", this));
    layout2->addWidget(hpTo_lineEdit);
    layout2->addWidget(new QLabel("%", this));

    layout3->addWidget(valueLabel);
    layout3->addWidget(distanceSlider);

    groupbox2_layout->addLayout(layout1);
    groupbox2_layout->addLayout(layout2);
    groupbox2_layout->addLayout(layout3);
    groupbox2_layout->addWidget(start_checkBox);
    groupbox2_layout->addStretch();

    groupbox_layout->addWidget(targetList_listWidget, 1); // Stretch
    groupbox_layout->addLayout(groupbox2_layout, 0);

    static_cast<QGridLayout*>(layout())->addWidget(groupbox, 0, 0, 1, 2);
}

void TargetTab::addTarget(const QString& name, int hpFrom, int hpTo, int distance) {
    QListWidgetItem* item = new QListWidgetItem(name);
    QVariantMap data;
    data["name"] = name;
    data["hpFrom"] = hpFrom;
    data["hpTo"] = hpTo;
    data["distance"] = distance;
    item->setData(Qt::UserRole, data);
    targetList_listWidget->addItem(item);

    status_label->setStyleSheet("color: green; font-weight: bold;");
    status_label->setText("Target added!");
}

void TargetTab::startTargetThread(int state) {
    if (state) {
        if (!targetThread) {
            QList<QVariantMap> targets;
            for (int i = 0; i < targetList_listWidget->count(); ++i) {
                QListWidgetItem* item = targetList_listWidget->item(i);
                QVariantMap data = item->data(Qt::UserRole).toMap();
                targets.append(data);
            }
            targetThread = new TargetThread(targets);
            std::cout << "Target Thread created" << std::endl;
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

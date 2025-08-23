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

    targetList_listWidget = new QListWidget(this);

    profile_listWidget = new QListWidget(this);

    auto layout = new QGridLayout(this);
    setLayout(layout);

    targetList();
    profileList();
}

void TargetTab::targetList() {
    auto groupbox = new QGroupBox("Targets", this);
    groupbox->setFixedHeight(200);
    auto groupbox_layout = new QHBoxLayout(groupbox);

    auto* add_button = new QPushButton("Add", this);
    auto start_checkBox = new QCheckBox("Start Targeting", this);

    auto targetName_lineEdit = new QLineEdit(this);
    targetName_lineEdit->setPlaceholderText("Orc | * - all monsters");

    auto hpFrom_lineEdit = new QLineEdit(this);
    hpFrom_lineEdit->setPlaceholderText("100");
    hpFrom_lineEdit->setValidator(new QIntValidator(1, 100, this));
    hpFrom_lineEdit->setMaxLength(3);
    auto hpTo_lineEdit = new QLineEdit(this);
    hpTo_lineEdit->setPlaceholderText("0");
    hpTo_lineEdit->setValidator(new QIntValidator(0, 99, this));
    hpTo_lineEdit->setMaxLength(2);

    auto distance_slider = new QSlider(Qt::Horizontal, this);
    distance_slider->setMinimum(1);
    distance_slider->setMaximum(7);
    distance_slider->setSingleStep(1);

    auto monsterAttacks_comboBox = new QComboBox(this);
    monsterAttacks_comboBox->addItem("Don't avoid");
    monsterAttacks_comboBox->addItem("Stay Diagonal");
    monsterAttacks_comboBox->addItem("Stay Frontal");


    auto desiredStance_comboBox = new QComboBox(this);
    desiredStance_comboBox->addItem("No Action");
    desiredStance_comboBox->addItem("Chase");
    desiredStance_comboBox->addItem("Stay Away 2SQM");
    desiredStance_comboBox->addItem("Stay Away 3SQM");
    desiredStance_comboBox->addItem("Stay Away 4SQM");

    auto count_comboBox = new QComboBox(this);
    count_comboBox->addItem("Any");
    count_comboBox->addItem("2");
    count_comboBox->addItem("3");
    count_comboBox->addItem("4");
    count_comboBox->addItem("5");


    connect(add_button, &QPushButton::clicked, this, [this, hpFrom_lineEdit, hpTo_lineEdit, distance_slider, desiredStance_comboBox, targetName_lineEdit, monsterAttacks_comboBox, count_comboBox]() {
        int hpFrom = hpFrom_lineEdit->text().toInt();
        int hpTo = hpTo_lineEdit->text().toInt();
        int distance = distance_slider->value();
        int count = count_comboBox->currentIndex();

        QString desiredStance = desiredStance_comboBox->currentText();
        QString monsterAttacks = monsterAttacks_comboBox->currentText();

        addTarget(targetName_lineEdit->text(), hpFrom, hpTo, distance, desiredStance, monsterAttacks, count);
        hpFrom_lineEdit->clear();
        hpTo_lineEdit->clear();
        targetName_lineEdit->clear();
        distance_slider->setValue(1);
        desiredStance_comboBox->setCurrentIndex(0);
        monsterAttacks_comboBox->setCurrentIndex(0);
        count_comboBox->setCurrentIndex(0);
    });

    connect(targetList_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        delete targetList_listWidget->takeItem(targetList_listWidget->row(item));
    });


    connect(start_checkBox, &QCheckBox::stateChanged, this, &TargetTab::startTargetThread);

    auto groupbox2_layout = new QVBoxLayout();

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();
    auto layout3 = new QHBoxLayout();
    auto layout4 = new QHBoxLayout();
    auto layout5 = new QHBoxLayout();
    auto layout6 = new QHBoxLayout();

    auto valueLabel = new QLabel("Distance 1", this);
    connect(distance_slider, &QSlider::valueChanged, this, [valueLabel](int value){
        valueLabel->setText(QString("Distance %1").arg(value));
    });

    layout1->addWidget(new QLabel("Name", this));
    layout1->addWidget(targetName_lineEdit);
    layout1->addWidget(add_button);

    layout2->addWidget(new QLabel("HP% range", this));
    layout2->addWidget(hpFrom_lineEdit);
    layout2->addWidget(new QLabel("to", this));
    layout2->addWidget(hpTo_lineEdit);

    layout3->addWidget(valueLabel);
    layout3->addWidget(distance_slider);

    layout4->addWidget(new QLabel("Count", this));
    layout4->addWidget(count_comboBox);

    layout5->addWidget(new QLabel("Desired Stance", this));
    layout5->addWidget(desiredStance_comboBox);

    layout6->addWidget(new QLabel("Monster Attacks", this));
    layout6->addWidget(monsterAttacks_comboBox);



    groupbox2_layout->addLayout(layout1);
    groupbox2_layout->addLayout(layout2);
    groupbox2_layout->addLayout(layout3);
    groupbox2_layout->addLayout(layout4);
    groupbox2_layout->addLayout(layout5);
    groupbox2_layout->addLayout(layout6);
    groupbox2_layout->addWidget(start_checkBox);
    groupbox2_layout->addStretch();

    groupbox_layout->addWidget(targetList_listWidget);
    groupbox_layout->addLayout(groupbox2_layout);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 0, 0, 1, 2);
}

void TargetTab::profileList()
{
    auto groupbox = new QGroupBox("Save && Load", this);
    auto groupbox_layout = new QVBoxLayout(groupbox);

    auto save_button = new QPushButton("Save");
    auto load_button = new QPushButton("Load");

    auto layout1 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();

    layout1->addWidget(new QLabel("Name", this));

    layout2->addWidget(save_button);
    layout2->addWidget(load_button);

    groupbox_layout->addWidget(profile_listWidget);
    groupbox_layout->addLayout(layout1);
    groupbox_layout->addLayout(layout2);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 1, 0, 1, 1);
    dynamic_cast<QGridLayout*>(layout())->setColumnStretch(0, 1);
    dynamic_cast<QGridLayout*>(layout())->setColumnStretch(1, 1);
}

void TargetTab::addTarget(const QString& name, int hpFrom, int hpTo, int distance, const QString& desiredStance, const QString& monsterAttacks, int count) const {
    auto* item = new QListWidgetItem(name);
    QVariantMap data;
    data["name"] = name;
    data["hpFrom"] = hpFrom;
    data["hpTo"] = hpTo;
    data["distance"] = distance;
    data["desiredStance"] = desiredStance;
    data["monsterAttacks"] = monsterAttacks;
    data["count"] = count + 1;

    item->setData(Qt::UserRole, data);
    targetList_listWidget->addItem(item);
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
            connect(targetThread, &TargetThread::requestLoot,this,&TargetTab::requestLoot);
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

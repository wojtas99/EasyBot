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
    targetName_lineEdit->setPlaceholderText("Orc | * - all monsters");

    hpFrom_lineEdit = new QLineEdit(this);
    hpFrom_lineEdit->setPlaceholderText("100");
    hpFrom_lineEdit->setValidator(new QIntValidator(1, 100, this));
    hpFrom_lineEdit->setMaxLength(3);
    hpTo_lineEdit = new QLineEdit(this);
    hpTo_lineEdit->setPlaceholderText("0");
    hpTo_lineEdit->setValidator(new QIntValidator(0, 99, this));
    hpTo_lineEdit->setMaxLength(2);

    distance_slider = new QSlider(Qt::Horizontal, this);
    distance_slider->setMinimum(1);
    distance_slider->setMaximum(7);
    distance_slider->setSingleStep(1);

    action_comboBox = new QComboBox(this);
    action_comboBox->addItem("No Action");
    action_comboBox->addItem("Chase");
    action_comboBox->addItem("Stay Away");

    auto layout = new QGridLayout(this);
    setLayout(layout);

    targetList();

    layout->addWidget(status_label, 3, 0, 1, 2);
}

void TargetTab::targetList() {
    auto groupbox = new QGroupBox("Targets", this);
    auto groupbox_layout = new QHBoxLayout(groupbox);

    auto* add_button = new QPushButton("Add", this);
    auto start_checkBox = new QCheckBox("Start Targeting", this);

    connect(add_button, &QPushButton::clicked, this, [this]() {
        if (targetName_lineEdit->text().isEmpty()) {
            status_label->setStyleSheet("color: red; font-weight: bold;");
            status_label->setText("Enter a target name!");
            return;
        }

        for (int i = 0; i < targetList_listWidget->count(); ++i) {
            if (targetList_listWidget->item(i)->text() == targetName_lineEdit->text()) {
                status_label->setStyleSheet("color: red; font-weight: bold;");
                status_label->setText("Target already exists!");
                return;
            }
        }

        QString hpFromText = hpFrom_lineEdit->text().isEmpty() ? hpFrom_lineEdit->placeholderText() : hpFrom_lineEdit->text();
        QString hpToText = hpTo_lineEdit->text().isEmpty() ? hpTo_lineEdit->placeholderText() : hpTo_lineEdit->text();

        int hpFrom = hpFromText.toInt();
        int hpTo = hpToText.toInt();
        int distance = distance_slider->value();

        QString action = action_comboBox->currentText();

        addTarget(targetName_lineEdit->text(), hpFrom, hpTo, distance, action);
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

    layout4->addWidget(new QLabel("Action", this));
    layout4->addWidget(action_comboBox);



    groupbox2_layout->addLayout(layout1);
    groupbox2_layout->addLayout(layout2);
    groupbox2_layout->addLayout(layout3);
    groupbox2_layout->addLayout(layout4);
    groupbox2_layout->addWidget(start_checkBox);
    groupbox2_layout->addStretch();

    groupbox_layout->addWidget(targetList_listWidget, 1); // Stretch
    groupbox_layout->addLayout(groupbox2_layout, 0);

    dynamic_cast<QGridLayout*>(layout())->addWidget(groupbox, 0, 0, 1, 2);
}

void TargetTab::addTarget(const QString& name, int hpFrom, int hpTo, int distance, const QString& action) const {
    auto* item = new QListWidgetItem(name);
    QVariantMap data;
    data["name"] = name;
    data["hpFrom"] = hpFrom;
    data["hpTo"] = hpTo;
    data["distance"] = distance;
    data["action"] = action;

    item->setData(Qt::UserRole, data);
    targetList_listWidget->addItem(item);

    distance_slider->setValue(1);
    hpFrom_lineEdit->clear();
    hpTo_lineEdit->clear();
    targetName_lineEdit->clear();
    action_comboBox->setCurrentIndex(0);


    status_label->setStyleSheet("color: green; font-weight: bold;");
    status_label->setText(name + " added!");


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

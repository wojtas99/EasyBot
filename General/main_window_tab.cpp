#include "main_window_tab.h"
#include <QApplication>
#include <QGridLayout>

MainWindowTab::MainWindowTab(QWidget *parent) : QWidget(parent) {
    setFixedSize(400, 100);
    setWindowTitle("EasyBot");

    QGridLayout *layout = new QGridLayout(this);

    walkerTab_button = new QPushButton("Walker", this);
    targetLootTab_button = new QPushButton("Targeting", this);
    healingTab_button = new QPushButton("Healing && Attack", this);
    settingsTab_button = new QPushButton("Settings", this);
    smartHotkeysTab_button = new QPushButton("Smart Hotkeys", this);
    trainingTab_button = new QPushButton("Training", this);

    layout->addWidget(walkerTab_button, 0, 0);
    layout->addWidget(targetLootTab_button, 1, 0);
    layout->addWidget(healingTab_button, 0, 1);
    layout->addWidget(settingsTab_button, 1, 1);
    layout->addWidget(smartHotkeysTab_button, 0, 2);
    layout->addWidget(trainingTab_button, 1, 2);

    setLayout(layout);

    connect(walkerTab_button, &QPushButton::clicked, this, &MainWindowTab::walker);
    connect(targetLootTab_button, &QPushButton::clicked, this, &MainWindowTab::targetLoot);
    connect(healingTab_button, &QPushButton::clicked, this, &MainWindowTab::healing);
    connect(settingsTab_button, &QPushButton::clicked, this, &MainWindowTab::settings);
    connect(smartHotkeysTab_button, &QPushButton::clicked, this, &MainWindowTab::smartHotkeys);
    connect(trainingTab_button, &QPushButton::clicked, this, &MainWindowTab::training);
}
void MainWindowTab::walker() {
    if (!walker_tab) {
        walker_tab = new WalkerTab();
    }
    walker_tab->show();
}
void MainWindowTab::targetLoot() {
    if (!target_tab) {
        target_tab = new TargetTab();
    }
    target_tab->show();
}
void MainWindowTab::healing() { /* Tu otwórz nowe okno lub dialog dla Healing */ }
void MainWindowTab::settings() { /* Tu otwórz nowe okno lub dialog dla Settings */ }
void MainWindowTab::smartHotkeys() { /* Tu otwórz nowe okno lub dialog dla SmartHotkeys */ }
void MainWindowTab::training() { /* Tu otwórz nowe okno lub dialog dla Training */ }

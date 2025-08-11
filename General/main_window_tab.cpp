#include "main_window_tab.h"
#include <QApplication>
#include <QGridLayout>
#include <QTabWidget>

MainWindowTab::MainWindowTab(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("EasyBot");
    setFixedSize(500, 500);

    QGridLayout *layout = new QGridLayout(this);

    main_tabWidget = new QTabWidget(this);
    status_tabWidget = new StatusTab();
    walker_tabWidget = new WalkerTab();
    target_tabWidget = new TargetTab();
    loot_tabWidget = new LootTab();
    heal_tabWidget = new HealTab();
    main_tabWidget->addTab(status_tabWidget, "Status");
    main_tabWidget->addTab(walker_tabWidget, "Walker");
    main_tabWidget->addTab(target_tabWidget, "Target");
    main_tabWidget->addTab(loot_tabWidget, "Loot");
    main_tabWidget->addTab(heal_tabWidget, "Heal");

    connect(status_tabWidget, &StatusTab::walkerToggled,walker_tabWidget,  &WalkerTab::setWalkerEnabled);
    connect(status_tabWidget, &StatusTab::healingToggled,heal_tabWidget,  &HealTab::setHealEnabled);
    connect(target_tabWidget, &TargetTab::requestLoot,loot_tabWidget,   &LootTab::startLootThread,Qt::QueuedConnection);

    setCentralWidget(main_tabWidget);
}

MainWindowTab::~MainWindowTab() {}
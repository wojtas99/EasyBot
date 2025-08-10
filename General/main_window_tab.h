#pragma once
#include <QMainWindow>
#include <QPushButton>
#include "../Status/status_tab.h"
#include "../Walker/walker_tab.h"
#include "../Target/target_tab.h"
#include "../Loot/loot_tab.h"


class MainWindowTab : public QMainWindow {
    Q_OBJECT
public:
    MainWindowTab(QWidget *parent = nullptr);
    ~MainWindowTab() override;


private:

    QTabWidget *main_tabWidget;
    StatusTab *status_tabWidget;
    WalkerTab *walker_tabWidget;
    TargetTab *target_tabWidget;
    LootTab *loot_tabWidget;
};





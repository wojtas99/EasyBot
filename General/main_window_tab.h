#pragma once
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include "../Status/status_tab.h"
#include "../Walker/walker_tab.h"
#include "../Target/target_tab.h"
#include "../Loot/loot_tab.h"
#include "../Heal/heal_tab.h"
#include "../Spell/spell_tab.h"
#include "../Scripts/script_tab.h"
#include "../safe_queue.h"
#include "../Functions/Game.h"
#include "../include/MinHook.h"



class MainWindowTab : public QMainWindow {
    Q_OBJECT
public:
    MainWindowTab(QWidget *parent = nullptr);
    ~MainWindowTab() override;
    LootTab *loot_tabWidget;

private:
    QTabWidget *main_tabWidget;
    StatusTab *status_tabWidget;
    WalkerTab *walker_tabWidget;
    TargetTab *target_tabWidget;
    HealTab *heal_tabWidget;
    SpellTab *spell_tabWidget;
    ScriptTab *script_tabWidget;

    bool m_hookInitialized = false;
    bool m_itemHookInitialized = false;
};






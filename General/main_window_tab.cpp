#include "main_window_tab.h"

#include <iostream>
#include <QApplication>
#include <QGridLayout>
#include <QTabWidget>
SafeQueue Game::actionQueue;
static MainWindowTab* s_mainWindow = nullptr;

typedef void(__stdcall* tGameMainLoop)();
tGameMainLoop originalGameMainLoop = nullptr;

void __stdcall hookedGameMainLoop() {
    originalGameMainLoop();
    Game::actionQueue.execute_all();
}


typedef void(__fastcall* tLookFunc)(__int64 a1, void (__fastcall ****a2)(__int64, __int64));
tLookFunc originalLookFunc = nullptr;

void __fastcall hookedLookFunc(__int64 a1, void (__fastcall ****a2)(__int64, __int64))
{
    uint64_t result = *reinterpret_cast<uint64_t*>(a2);
    Item* item = reinterpret_cast<Item*>(result);
    int id = item->id;
    QMetaObject::invokeMethod(s_mainWindow->loot_tabWidget, [w = s_mainWindow->loot_tabWidget, id](){
    w->updateLastLookedItem(id);}, Qt::QueuedConnection);
    //Game::talkChannel(std::to_string(item->id).c_str());
    originalLookFunc(a1, a2);
}

void setupLookHook(uint64_t itemFuncAddress) {
    if (MH_CreateHook(reinterpret_cast<void*>(itemFuncAddress),
                      &hookedLookFunc,
                      reinterpret_cast<void**>(&originalLookFunc)) != MH_OK) {
        return;
                      }
    if (MH_EnableHook(reinterpret_cast<void*>(itemFuncAddress)) != MH_OK) {
        return;
    }
    std::cout << "[HOOK] LookFunc Sucessfully\n";
}

void setupMainLoopHook(uint64_t gameLoopAddress) {
    if (MH_Initialize() != MH_OK)
        return;
    if (MH_CreateHook(reinterpret_cast<void*>(gameLoopAddress), &hookedGameMainLoop,
                      reinterpret_cast<void**>(&originalGameMainLoop)) != MH_OK)
        return;
    if (MH_EnableHook(reinterpret_cast<void*>(gameLoopAddress)) != MH_OK)
        return;
    std::cout << "[HOOK] MainFunc Sucessfully\n";
}
MainWindowTab::MainWindowTab(QWidget *parent) : QMainWindow(parent) {
    s_mainWindow = this;
    if (!m_hookInitialized) {
        setupMainLoopHook(Game::main_func_address);
        setupLookHook(Game::look_func_address);
        m_hookInitialized = true;
    }

    setWindowTitle("EasyBot");
    setFixedSize(500, 500);

    main_tabWidget = new QTabWidget(this);
    status_tabWidget = new StatusTab();
    walker_tabWidget = new WalkerTab();
    target_tabWidget = new TargetTab();
    loot_tabWidget = new LootTab();
    heal_tabWidget = new HealTab();
    spell_tabWidget = new SpellTab();
    script_tabWidget = new ScriptTab();
    main_tabWidget->addTab(status_tabWidget, "Status");
    main_tabWidget->addTab(walker_tabWidget, "Walker");
    main_tabWidget->addTab(target_tabWidget, "Target");
    main_tabWidget->addTab(loot_tabWidget, "Loot");
    main_tabWidget->addTab(heal_tabWidget, "Heal");
    main_tabWidget->addTab(spell_tabWidget, "Spell");
    main_tabWidget->addTab(script_tabWidget, "Scripts");

    connect(status_tabWidget, &StatusTab::walkerToggled,walker_tabWidget,  &WalkerTab::setWalkerEnabled);
    connect(status_tabWidget, &StatusTab::targetToggled,target_tabWidget,  &TargetTab::setTargetEnabled);
    connect(status_tabWidget, &StatusTab::lootToggled,loot_tabWidget,  &LootTab::setLootEnabled);
    connect(status_tabWidget, &StatusTab::healingToggled,heal_tabWidget,  &HealTab::setHealEnabled);
    connect(status_tabWidget, &StatusTab::spellsToggled,spell_tabWidget,  &SpellTab::setSpellEnabled);
    connect(status_tabWidget, &StatusTab::scriptsToggled,script_tabWidget,  &ScriptTab::setScriptsEnabled);


    connect(target_tabWidget, &TargetTab::saveProfileSignal,status_tabWidget,  &StatusTab::saveProfile);
    connect(walker_tabWidget, &WalkerTab::saveProfileSignal,status_tabWidget,  &StatusTab::saveProfile);
    connect(loot_tabWidget, &LootTab::saveProfileSignal,status_tabWidget,  &StatusTab::saveProfile);
    connect(heal_tabWidget, &HealTab::saveProfileSignal,status_tabWidget,  &StatusTab::saveProfile);
    connect(spell_tabWidget, &SpellTab::saveProfileSignal,status_tabWidget,  &StatusTab::saveProfile);
    connect(script_tabWidget, &ScriptTab::saveProfileSignal,status_tabWidget,  &StatusTab::saveProfile);

    connect(target_tabWidget, &TargetTab::loadProfileSignal,status_tabWidget,  &StatusTab::loadProfile);
    connect(walker_tabWidget, &WalkerTab::loadProfileSignal,status_tabWidget,  &StatusTab::loadProfile);
    connect(loot_tabWidget, &LootTab::loadProfileSignal,status_tabWidget,  &StatusTab::loadProfile);
    connect(heal_tabWidget, &HealTab::loadProfileSignal,status_tabWidget,  &StatusTab::loadProfile);
    connect(spell_tabWidget, &SpellTab::loadProfileSignal,status_tabWidget,  &StatusTab::loadProfile);
    connect(script_tabWidget, &ScriptTab::loadProfileSignal,status_tabWidget,  &StatusTab::loadProfile);

    setCentralWidget(main_tabWidget);
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

MainWindowTab::~MainWindowTab() {}




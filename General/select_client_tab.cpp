#include "select_client_tab.h"
#include <QGridLayout>
#include "../Functions/Game.h"
#include "../include/MinHook.h"
#include <iostream>
#include <QMessageBox>

SelectClientTab::SelectClientTab(QWidget *parent) : QWidget(parent) {
    setFixedSize(100, 400);
    setWindowTitle("EasyBot");

    QGridLayout *layout = new QGridLayout(this);

    medivia_button = new QPushButton("Medivia", this);

    layout->addWidget(medivia_button, 0, 0);

    setLayout(layout);

    connect(medivia_button, &QPushButton::clicked, this, &SelectClientTab::load_medivia);
}

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
    Game::talkChannel(std::to_string(item->id).c_str());
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
SafeQueue Game::actionQueue;

void SelectClientTab::load_medivia() {
    Game mf(Game::LoadOption::Medivia);
    this->close();
    main_window_tab = new MainWindowTab();
    main_window_tab->show();
    if (!m_hookInitialized) {
        setupMainLoopHook(Game::main_func_address);
        setupLookHook(Game::look_func_address);
        m_hookInitialized = true;
    }
}



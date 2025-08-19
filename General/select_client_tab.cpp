#include "select_client_tab.h"
#include <QGridLayout>
#include "../Functions/memory_functions.h"
#include "../include/MinHook.h"
#include <iostream>
#include <QMessageBox>

SelectClientTab::SelectClientTab(QWidget *parent) : QWidget(parent) {
    setFixedSize(100, 400);
    setWindowTitle("EasyBot");

    QGridLayout *layout = new QGridLayout(this);

    altaron_button = new QPushButton("Altaron", this);
    medivia_button = new QPushButton("Medivia", this);

    layout->addWidget(altaron_button, 0, 0);
    layout->addWidget(medivia_button, 1, 0);

    setLayout(layout);

    connect(altaron_button, &QPushButton::clicked, this, &SelectClientTab::load_altaron);
    connect(medivia_button, &QPushButton::clicked, this, &SelectClientTab::load_medivia);
}

typedef void(__stdcall* tGameMainLoop)();
tGameMainLoop originalGameMainLoop = nullptr;

void __stdcall hookedGameMainLoop() {
    originalGameMainLoop();
    MemoryFunctions::actionQueue.execute_all();
}

typedef __int64(__fastcall* tOpenFunc)(__int64 a1,  uint64_t *a2, uint64_t *a3);
tOpenFunc originalOpenFunc = nullptr;

void __fastcall hookedOpenFunc(__int64 a1,  uint64_t *a2, uint64_t *a3)
{

    originalOpenFunc(a1, a2, a3);

}

void setupOpenHook(uint64_t openFuncAddress) {
    if (MH_CreateHook(reinterpret_cast<void*>(openFuncAddress),
                      &hookedOpenFunc,
                      reinterpret_cast<void**>(&originalOpenFunc)) != MH_OK) {
        std::cout << "[HOOK] Error (OpenFunc)\n";
        return;
                      }

    if (MH_EnableHook(reinterpret_cast<void*>(openFuncAddress)) != MH_OK) {
        std::cout << "[HOOK] Error (OpenFunc)\n";
        return;
    }

    std::cout << "[HOOK] OpenFunc\n";
}



void setupMainLoopHook(uint64_t gameLoopAddress) {
    if (MH_Initialize() != MH_OK)
        return;
    if (MH_CreateHook(reinterpret_cast<void*>(gameLoopAddress), &hookedGameMainLoop,
                      reinterpret_cast<void**>(&originalGameMainLoop)) != MH_OK)
        return;
    if (MH_EnableHook(reinterpret_cast<void*>(gameLoopAddress)) != MH_OK)
        return;
    std::cout << "Main Loop hooked successfully!\n";
}
SafeQueue MemoryFunctions::actionQueue;

void SelectClientTab::load_altaron() {
    MemoryFunctions mf(MemoryFunctions::LoadOption::Altaron);
    this->close();
    main_window_tab = new MainWindowTab();
    main_window_tab->show();
}

void SelectClientTab::load_medivia() {
    MemoryFunctions mf(MemoryFunctions::LoadOption::Medivia);
    this->close();
    main_window_tab = new MainWindowTab();
    main_window_tab->show();
    if (!m_hookInitialized) {
        setupMainLoopHook(reinterpret_cast<uint64_t>(MemoryFunctions::main_func_address));
        m_hookInitialized = true;
    }
}



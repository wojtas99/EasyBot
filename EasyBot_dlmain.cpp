#include <windows.h>
#include <iostream>
#include <QApplication>

#include "Functions/memory_functions.h"
#include "General/select_client_tab.h"
#include "Structs/medivia_struct.h"


HMODULE myhModule;

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(myhModule, 0);
}

DWORD WINAPI TibiaBot(HMODULE hModule) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    uintptr_t moduleBase = (uintptr_t)GetModuleHandleW(NULL);
    int argc = 0;
    QApplication app(argc, nullptr);
    SelectClientTab w;
    w.show();
    QString darkStyle =
    "QWidget { background-color: #232629; color: #f0f0f0; }"
    "QPushButton { background-color: #393939; color: #f0f0f0; border: 1px solid #444; border-radius: 4px; padding: 5px; }"
    "QPushButton:hover { background-color: #484848; }"
    "QLineEdit, QTextEdit, QComboBox, QListWidget { background-color: #2a2a2a; color: #f0f0f0; border: 1px solid #444; }"
    "QGroupBox { border: 1px solid #444; margin-top: 6px; }"
    "QGroupBox::title { subcontrol-origin: margin; left: 7px; padding: 0 3px 0 3px; }"
    "QCheckBox { color: #f0f0f0; }"
    "QLabel { color: #f0f0f0; }";
    app.setStyleSheet(darkStyle);

    app.exec();




    std::cout << "Main module base address: 0x" << std::hex << moduleBase << std::endl;

    std::cout << "Hello From DLL\n";
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 1) {
            break;
        }
        if (GetAsyncKeyState(VK_SPACE) & 1) {
            std::cout << "Calling MapMovement..." << std::endl;
        }
        Sleep(10);
    }
    fclose(f);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            myhModule = hModule;
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TibiaBot, NULL, 0, NULL);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

#include <windows.h>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
namespace fs = std::filesystem;

// Funkcja: Kopiuj plik z nadpisaniem
bool copy_files(const fs::path& from, const fs::path& to) {
    try {
        fs::copy_file(from, to, fs::copy_options::overwrite_existing);
        return true;
    } catch (...) {
        std::cerr << "Nie można skopiować pliku: " << from << std::endl;
        return false;
    }
}

// Funkcja: Kopiuj cały folder (np. platforms)
bool copy_dir(const fs::path& from, const fs::path& to) {
    try {
        if (fs::exists(to)) fs::remove_all(to);
        fs::create_directory(to);
        for (const auto& entry : fs::directory_iterator(from)) {
            if (entry.is_regular_file()) {
                fs::copy_file(entry.path(), to / entry.path().filename(), fs::copy_options::overwrite_existing);
            }
        }
        return true;
    } catch (...) {
        std::cerr << "Nie można skopiować folderu: " << from << std::endl;
        return false;
    }
}

// Funkcja: Usuwanie plików po zamknięciu gry
void cleanup(const fs::path& gameDir, const std::vector<std::string>& dlls, const fs::path& platforms) {
    for (const auto& dll : dlls) {
        try { fs::remove(gameDir / dll); } catch (...) {}
    }
    try { fs::remove(gameDir / "EasyBot.dll"); } catch (...) {}
    try { fs::remove_all(platforms); } catch (...) {}
}

// Funkcja: Inject DLL do procesu
bool InjectDLL(HANDLE hProcess, const std::string& dllPath) {
    void* pRemoteBuf = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteBuf) return false;
    WriteProcessMemory(hProcess, pRemoteBuf, dllPath.c_str(), dllPath.size() + 1, NULL);
    FARPROC pLoadLibraryA = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)pLoadLibraryA, pRemoteBuf, 0, NULL);
    if (!hThread) return false;
    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hThread);
    return true;
}

// Funkcja loadera: przyjmuje ścieżkę do DLL i EXE
bool inject(const std::string& dllPath, const std::string& exePath, QWidget* parent = nullptr) {
    // Ustal katalogi plików
    fs::path botDir = fs::path(dllPath).parent_path();
    fs::path gameDir = fs::path(exePath).parent_path();
    std::string dllName = fs::path(dllPath).filename().string();
    std::string gameExe = fs::path(exePath).filename().string();
    std::string platformsDir = "platforms";

    // Pliki Qt do skopiowania (dodaj inne, jeśli trzeba)
    std::vector<std::string> qtDlls = {
        "Qt5Core.dll", "Qt5Gui.dll", "Qt5Widgets.dll", "Qt5Qml.dll", "Qt5Svg.dll", "Qt5Network.dll"
    };

    // ---- KOPIOWANIE ----
    for (const auto& dll : qtDlls)
        copy_files(botDir / dll, gameDir / dll);
    copy_dir(botDir / platformsDir, gameDir / platformsDir);
    copy_files(botDir / dllName, gameDir / dllName);

    // ---- START GRY ----
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL created = CreateProcessA(
        (gameDir / gameExe).string().c_str(),
        NULL, NULL, NULL, FALSE,
        CREATE_SUSPENDED,
        NULL, gameDir.string().c_str(), &si, &pi
    );
    if (!created) {
        cleanup(gameDir, qtDlls, gameDir / platformsDir);
        return false;
    }

    // ---- INJECT DLL ----
    if (!InjectDLL(pi.hProcess, (gameDir / dllName).string())) {
        TerminateProcess(pi.hProcess, 0);
        cleanup(gameDir, qtDlls, gameDir / platformsDir);
        return false;
    }

    // ---- START GRY ----
    ResumeThread(pi.hThread);

    // ---- CZEKAJ NA KONIEC GRY ----
    WaitForSingleObject(pi.hProcess, INFINITE);

    // ---- SPRZĄTANIE ----
    cleanup(gameDir, qtDlls, gameDir / platformsDir);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}
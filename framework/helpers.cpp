// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Helper functions

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "helpers.h"

// Log file location
extern const LPCWSTR LOG_FILE;

// Helper function to get the module base address
uintptr_t GetBaseAddress() {
    return (uintptr_t) GetModuleHandleW(L"GameAssembly.dll");
}

// Helper function to append text to a file
void LogWrite(std::string text) {
    HANDLE hfile = CreateFileW(LOG_FILE, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE)
        MessageBoxW(0, L"Could not open log file", 0, 0);

    DWORD written;
    WriteFile(hfile, text.c_str(), (DWORD) text.length(), &written, NULL);
    WriteFile(hfile, "\r\n", 2, &written, NULL);
    CloseHandle(hfile);
}

// Helper function to open a new console window and redirect stdout there
void NewConsole() {
    AllocConsole();
    freopen_s((FILE**) stdout, "CONOUT$", "w", stdout);
}

int randi(int lo, int hi)
{
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}
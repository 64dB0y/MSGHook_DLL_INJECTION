#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

#define DLL_PATH "C:\\DLL_INJECTION\\msghook_local.dll"

char* FindProcessName(DWORD pid) {
    HANDLE hProcSnap;
    PROCESSENTRY32 pe32;
    static char procName[MAX_PATH];

    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcSnap) return NULL;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcSnap, &pe32)) {
        CloseHandle(hProcSnap);
        return NULL;
    }

    while (Process32Next(hProcSnap, &pe32)) {
        if (pe32.th32ProcessID == pid) {
            WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, procName, sizeof(procName), NULL, NULL);
            break;
        }
    }

    CloseHandle(hProcSnap);

    return procName;
}

int main() {
    HMODULE hModule = LoadLibraryA("C:\\DLL_INJECTION\\msghook_local.dll");
    if (hModule == NULL) {
        printf("Cannot find the DLL: %s\n", DLL_PATH);
        return 1;
    }

    // Hook 설치 함수의 주소를 가져옵니다.
    HHOOK(*InstallHook)(DWORD) = (HHOOK(*)(DWORD))GetProcAddress(hModule, "InstallHook");
    if (InstallHook == NULL) {
        printf("Cannot find the InstallHook function\n");
        return 1;
    }

    // Hook 제거 함수의 주소를 가져옵니다.
    BOOL(*UninstallHook)(HHOOK) = (BOOL(*)(HHOOK))GetProcAddress(hModule, "UninstallHook");
    if (UninstallHook == NULL) {
        printf("Cannot find the UninstallHook function\n");
        return 1;
    }

    // Target process의 PID를 입력 받습니다.
    DWORD targetPID;
    printf("Enter the target process PID: ");
    scanf_s("%lu", &targetPID);

    // Target process의 이름을 얻습니다.
    char* procName = FindProcessName(targetPID);
    if (procName == NULL) {
        printf("Cannot find the target process with PID: %lu\n", targetPID);
        return 1;
    }

    printf("PID = %lu is for the process '%s'. The hook is being installed...\n", targetPID, procName);

    // Hook을 설치합니다.
    HHOOK hook = InstallHook(targetPID);
    if (hook == NULL) {
        printf("Cannot install the hook\n");
        return 1;
    }

    // 'q' or 'Q'를 누르면 Hook을 제거합니다.
    char ch;
    do {
        ch = getchar();
    } while (ch != 'q' && ch != 'Q');

    // Hook을 제거합니다.
    BOOL unhooked = UninstallHook(hook);
    if (!unhooked) {
        printf("Cannot uninstall the hook\n");
        return 1;
    }

    printf("The hook is uninstalled.\n");
    return 0;
}
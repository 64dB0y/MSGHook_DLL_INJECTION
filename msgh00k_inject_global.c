#include <stdio.h>
#include <windows.h>

#define DLL_PATH "C:\\DLL_INJECTION\\msghook_global.dll"

int main() {
    HMODULE hModule = LoadLibraryA(DLL_PATH);
    if (hModule == NULL) {
        printf("Cannot find the DLL: %s\n", DLL_PATH);
        return 1;
    }

    // Hook 설치 함수의 주소를 가져옵니다.
    HHOOK(*InstallHook)(void) = (HHOOK(*)(void))GetProcAddress(hModule, "InstallHook");
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

    printf("The hook is being installed...\n");

    // Hook을 설치합니다.
    HHOOK hook = InstallHook();
    if (hook == NULL) {
        printf("Cannot install the hook\n");
        return 1;
    }

    printf("Press 'q' to uninstall the hook.\n");

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

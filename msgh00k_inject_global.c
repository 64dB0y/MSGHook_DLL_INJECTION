#include <stdio.h>
#include <windows.h>

#define DLL_PATH "C:\\DLL_INJECTION\\msghook_global.dll"

int main() {
    HMODULE hModule = LoadLibraryA(DLL_PATH);
    if (hModule == NULL) {
        printf("Cannot find the DLL: %s\n", DLL_PATH);
        return 1;
    }

    // Hook ��ġ �Լ��� �ּҸ� �����ɴϴ�.
    HHOOK(*InstallHook)(void) = (HHOOK(*)(void))GetProcAddress(hModule, "InstallHook");
    if (InstallHook == NULL) {
        printf("Cannot find the InstallHook function\n");
        return 1;
    }

    // Hook ���� �Լ��� �ּҸ� �����ɴϴ�.
    BOOL(*UninstallHook)(HHOOK) = (BOOL(*)(HHOOK))GetProcAddress(hModule, "UninstallHook");
    if (UninstallHook == NULL) {
        printf("Cannot find the UninstallHook function\n");
        return 1;
    }

    printf("The hook is being installed...\n");

    // Hook�� ��ġ�մϴ�.
    HHOOK hook = InstallHook();
    if (hook == NULL) {
        printf("Cannot install the hook\n");
        return 1;
    }

    printf("Press 'q' to uninstall the hook.\n");

    // 'q' or 'Q'�� ������ Hook�� �����մϴ�.
    char ch;
    do {
        ch = getchar();
    } while (ch != 'q' && ch != 'Q');

    // Hook�� �����մϴ�.
    BOOL unhooked = UninstallHook(hook);
    if (!unhooked) {
        printf("Cannot uninstall the hook\n");
        return 1;
    }

    printf("The hook is uninstalled.\n");
    return 0;
}

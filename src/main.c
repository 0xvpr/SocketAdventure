#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <psapi.h>

//#define DEBUG

#define MANA_ACTIVE   (1 << 0)
#define MANA_INACTIVE (1 << 1)

static int mana = 0;
static HANDLE hThread = NULL;
static uintptr_t mana_addr = 0;

void ToggleInfiniteMana(int mana);

DWORD WINAPI MainThread(LPVOID lpReserved)
{
#ifdef DEBUG
    FILE* fp;
    AllocConsole();
    freopen_s(&fp, "CONOUT$", "w", stdout);
#endif // DEBUG

    SOCKADDR_IN server_addr       = { 0 };
    SOCKADDR_IN client_addr       = { 0 };
    WSADATA     wsaData           = { 0 };
    CHAR        send_buffer[8]    = { 0 };
    SOCKET      server            =     0;
    SOCKET      client            =     0;
    WORD        wVersionRequested =     0;
    BOOL        rv                =     0;

    wVersionRequested = MAKEWORD(2, 2);
    if ((rv = WSAStartup(wVersionRequested, &wsaData)))
    {
        printf("Startup failed: %d\n", rv);
        goto mainloop;
    }

    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Create Server Socket
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Invalid Socket\n");
        goto mainloop;
    }

    // Bind to Socket
    if ((rv = bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr))) == SOCKET_ERROR)
    {
        printf("Failed to bind: %d\n", rv);
        goto mainloop;
    }

    // Listen on Socket
    if ((rv = listen(server, 1)) == SOCKET_ERROR)
    {
        printf("Failed to listen\n");
        goto mainloop;
    }

    int client_addr_len = sizeof(client_addr);
    if (!(client = accept(server, (struct sockaddr *)&client_addr, &client_addr_len)))
    {
        printf("Failed to connect to client\n");
        goto mainloop;
    }

mainloop:
    while (!GetAsyncKeyState(VK_HOME))
    {
        if ((GetAsyncKeyState(VK_NUMPAD1) & 1) > 0)
        {
            mana = !mana;
            ToggleInfiniteMana(mana);

            if (client)
            {
                *(unsigned int *)send_buffer = (mana ? MANA_ACTIVE : MANA_INACTIVE);
                send(client, send_buffer, sizeof(unsigned int), 0);
                fprintf(stdout, "sent: %u\n", *(unsigned int *)send_buffer);
            }
        }
    }
    // Reset Hacks
    ToggleInfiniteMana(0);

    // Kill Connections
    send(client, "EOF", 3, 0);
    closesocket(client);
    closesocket(server);
    WSACleanup();

#ifdef DEBUG
    fclose(fp);
    FreeConsole();
#endif // DEBUG
    FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    (void)lpReserved;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls((HMODULE)hInstance);
            hThread = CreateThread(NULL, 0, MainThread, hInstance, 0, NULL);
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            CloseHandle(hThread);
            break;
        }
        default:
            break;
    }

    return 0;
}

static inline void Patch(void* dst, void* src, size_t size)
{
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_WRITECOPY, &oldprotect);
    for (size_t i = 0; i < size; ++i)
    {
        ((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
    }
    VirtualProtect(dst, size, oldprotect, NULL);
}

void ToggleInfiniteMana(int active)
{
    static uint8_t mana_original[2] = {
        0x2B, 0xC2 // sub eax, edx
    };

    static uint8_t mana_patch[2] = {
        0x90, // nop
        0x90  // nop
    };

    if (active)
    {
        mana_addr = mana_addr ? mana_addr : (uintptr_t)GetModuleHandle(TEXT("GameLogic.dll")) + 0x525C5;
        mana_addr ? Patch((void *)mana_addr, mana_patch, sizeof(mana_patch)) : (void)NULL;
    }
    else
    {
        mana_addr = mana_addr ? mana_addr : (uintptr_t)GetModuleHandle(TEXT("GameLogic.dll")) + 0x525C5;
        mana_addr ? Patch((void *)mana_addr, mana_original, sizeof(mana_original)) : (void)NULL;
    }
}

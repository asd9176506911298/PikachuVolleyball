// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include <Windows.h>
#include <iomanip>
#include <iostream>

bool bEnd = false, bJump = false, bNet = false, bRemote = false, bHappy = false;

std::string isOpen(bool status)
{
    return status ? "ON" : "OFF";
}

void printOnOff()
{
    system("cls");
    std::cout << std::setw(28) << std::left << "[NUMPAD1] 球落地不結束" << isOpen(bEnd) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD2] 無限跳" << isOpen(bJump) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD3] 穿過網子" << isOpen(bNet) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD4] 遠距離打球" << isOpen(bRemote) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD5] 皮卡丘說你好" << isOpen(bHappy) << std::endl;
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);


    printOnOff();
    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bEnd = !bEnd;
            printOnOff();
            if (bEnd)
                mem::Patch((BYTE*)0x403C8B, (BYTE*)"\xEB\x76", 2);
            else
                mem::Patch((BYTE*)0x403C8B, (BYTE*)"\x75\x76", 2);
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bJump = !bJump;
            printOnOff();
            if (bJump)
                mem::Nop((BYTE*)0x4020A1, 2);
            else
                mem::Patch((BYTE*)0x4020A1, (BYTE*)"\x75\x4A", 2);
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bNet = !bNet;
            printOnOff();
            if (bNet)
                mem::Nop((BYTE*)0x402086, 6);
            else
                mem::Patch((BYTE*)0x402086, (BYTE*)"\x89\x86\xA8\x00\x00\x00", 6);
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            bRemote = !bRemote;
            printOnOff();
            if (bRemote)
                mem::Nop((BYTE*)0x403090, 2);
            else
                mem::Patch((BYTE*)0x403090, (BYTE*)"\x7F\x08", 2);
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            bHappy = !bHappy;
            printOnOff();
            if (bHappy)
                mem::Patch((BYTE*)0x40279B, (BYTE*)"\xB9\x05\x00\x00\x00\x90", 6);
            else
                mem::Patch((BYTE*)0x40279B, (BYTE*)"\x8B\x8E\xC0\x00\x00\x00", 6);
        }


        Sleep(10);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HANDLE hThread;
        hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
        if (hThread)
            CloseHandle(hThread);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


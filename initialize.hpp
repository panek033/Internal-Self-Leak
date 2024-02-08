#pragma once
#include "include/helper.h"
#include "include/Console.hpp"
#include "include/Game.hpp"
#include "include/D3D11Window.hpp"
#include "include/Hooking.hpp"
using namespace husisudfr;

void ClientBGThread()
{
    while (g_Running) {
        //g_Menu->Loops();
        std::this_thread::sleep_for(0ms);
        std::this_thread::yield();
    }
}

DWORD WINAPI MainThread_Initialize()
{
    g_Console = std::make_unique<Console>();

    ///  ESTABLISH GAME DATA   
    g_GameData = std::make_unique<GameData>();
    g_GameVariables = std::make_unique<GameVariables>();

    ///  CREATE WINDOW AND ESTABLISH HOOKS
    g_D3D11Window = std::make_unique<D3D11Window>();
    g_Hooking = std::make_unique<Hooking>();
    g_Menu = std::make_unique<Menu>();
    g_Hooking->Hook();

    std::thread WCMUpdate(ClientBGThread);	//	Initialize Loops Thread
    ///  RENDER LOOP
    g_Running = TRUE;
    while (g_Running)
    {
        if (GetAsyncKeyState(VK_F4) & 1) g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
    }

    ///  EXIT
    WCMUpdate.join();						//	Exit Loops Thread
    FreeLibraryAndExitThread(g_hModule, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}
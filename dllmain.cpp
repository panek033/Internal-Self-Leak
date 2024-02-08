#pragma once
#include "pch.h"
#include "initialize.hpp"
#include "Cheat/includes.h"
#include "Cheat/hooking.h"

extern DWORD WINAPI MainThread_Initialize();
BOOL APIENTRY DllMain(HMODULE Mdl, DWORD  Reason, LPVOID lReserv)
{
    using namespace husisudfr;
    g_hModule = Mdl;

    if (Reason == DLL_PROCESS_ATTACH) {       
        
        DisableThreadLibraryCalls(Mdl);
        husint::mytekgej();
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread_Initialize, g_hModule, NULL, NULL);
    }

    

    return TRUE;
}
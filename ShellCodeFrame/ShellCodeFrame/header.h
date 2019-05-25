#pragma once
#include "api.h"
#include <windows.h>
#include <winternl.h>


void ShellCodeStart();
void ShellCodeEntry();
void ShellCodeEnd();
void CreateShellCode();


void Initfunctions(Pfunctions pfn);
HMODULE GetProcAddressWithHash(DWORD dwModuleFunctionHash);
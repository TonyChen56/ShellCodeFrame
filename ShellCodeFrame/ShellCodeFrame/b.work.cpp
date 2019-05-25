#include "api.h"
#include "header.h"
#include "hash.h"

//************************************************************
// 函数名称: GetProcAddressWithHash
// 函数说明: 通过哈希获取函数地址
// 作	 者: GuiShou
// 时	 间: 2019/5/25
// 参	 数: dwModuleFunctionHash 函数的哈希值
// 返 回 值: HMODULE 函数地址
//************************************************************
HMODULE GetProcAddressWithHash(DWORD dwModuleFunctionHash)
{
	PPEB PebAddress;
	PMY_PEB_LDR_DATA pLdr;
	PMY_LDR_DATA_TABLE_ENTRY pDataTableEntry;
	PVOID pModuleBase;
	PIMAGE_NT_HEADERS pNTHeader;
	DWORD dwExportDirRVA;
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	PLIST_ENTRY pNextModule;
	DWORD dwNumFunctions;
	USHORT usOrdinalTableIndex;
	PDWORD pdwFunctionNameBase;
	PCSTR pFunctionName;
	UNICODE_STRING BaseDllName;
	DWORD dwModuleHash;
	DWORD dwFunctionHash;
	PCSTR pTempChar;
	DWORD i;

#if defined(_WIN64)
	PebAddress = (PPEB)__readgsqword(0x60);
#elif defined(_M_ARM)
	PebAddress = (PPEB)((ULONG_PTR)_MoveFromCoprocessor(15, 0, 13, 0, 2) + 0);
	__emit(0x00006B1B);
#else
	PebAddress = (PPEB)__readfsdword(0x30);
#endif

	pLdr = (PMY_PEB_LDR_DATA)PebAddress->Ldr;
	pNextModule = pLdr->InLoadOrderModuleList.Flink;
	pDataTableEntry = (PMY_LDR_DATA_TABLE_ENTRY)pNextModule;

	while (pDataTableEntry->DllBase != NULL)
	{
		dwModuleHash = 0;
		pModuleBase = pDataTableEntry->DllBase;
		BaseDllName = pDataTableEntry->BaseDllName;
		pNTHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)pModuleBase + ((PIMAGE_DOS_HEADER)pModuleBase)->e_lfanew);
		dwExportDirRVA = pNTHeader->OptionalHeader.DataDirectory[0].VirtualAddress;

		//获取下一个模块地址
		pDataTableEntry = (PMY_LDR_DATA_TABLE_ENTRY)pDataTableEntry->InLoadOrderLinks.Flink;

		// 如果当前模块不导出任何函数，则转到下一个模块 加载模块入口
		if (dwExportDirRVA == 0)
		{
			continue;
		}

		//计算模块哈希值
		for (i = 0; i < BaseDllName.MaximumLength; i++)
		{
			pTempChar = ((PCSTR)BaseDllName.Buffer + i);

			dwModuleHash = ROTR32(dwModuleHash, 13);

			if (*pTempChar >= 0x61)
			{
				dwModuleHash += *pTempChar - 0x20;
			}
			else
			{
				dwModuleHash += *pTempChar;
			}
		}

		pExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)pModuleBase + dwExportDirRVA);

		dwNumFunctions = pExportDir->NumberOfNames;
		pdwFunctionNameBase = (PDWORD)((PCHAR)pModuleBase + pExportDir->AddressOfNames);

		for (i = 0; i < dwNumFunctions; i++)
		{
			dwFunctionHash = 0;
			pFunctionName = (PCSTR)(*pdwFunctionNameBase + (ULONG_PTR)pModuleBase);
			pdwFunctionNameBase++;

			pTempChar = pFunctionName;

			do
			{
				dwFunctionHash = ROTR32(dwFunctionHash, 13);
				dwFunctionHash += *pTempChar;
				pTempChar++;
			} while (*(pTempChar - 1) != 0);

			dwFunctionHash += dwModuleHash;

			if (dwFunctionHash == dwModuleFunctionHash)
			{
				usOrdinalTableIndex = *(PUSHORT)(((ULONG_PTR)pModuleBase + pExportDir->AddressOfNameOrdinals) + (2 * i));
				return (HMODULE)((ULONG_PTR)pModuleBase + *(PDWORD)(((ULONG_PTR)pModuleBase + pExportDir->AddressOfFunctions) + (4 * usOrdinalTableIndex)));
			}
		}
	}

	return NULL;
}



//************************************************************
// 函数名称: Initfunctions
// 函数说明: 动态获取所有的函数地址
// 作	 者: GuiShou
// 时	 间: 2019/5/19
// 参	 数: Pfunctions pfn  保存有所有API函数地址的结构体
// 返 回 值: void
//************************************************************
void Initfunctions(Pfunctions pfn)
{
	//获取LoadLibraryA函数地址
	pfn->fnLoadLibraryA = (pfnLoadLibraryA)GetProcAddressWithHash(HASH_LoadLibraryA);
	//将user32.dll加载到当前进程中
	char szUser32[] = { 'u', 's', 'e', 'r', '3', '2', '.', 'd', 'l', 'l', 0 };
	pfn->fnLoadLibraryA(szUser32);
	//获取MessageBoxA函数地址
	pfn->fnMessageBoxA = (pfnMessageBoxA)GetProcAddressWithHash(HASH_MessageBoxA);
}

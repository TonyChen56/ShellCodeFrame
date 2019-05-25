#pragma once
#include <windows.h>
#include <winternl.h>
//计算哈希值
#define ROTR32(value, shift)	(((DWORD) value >> (BYTE) shift) | ((DWORD) value << (32 - (BYTE) shift)))

//重新定义PEB结构。winternl.h中的结构定义是不完整的。
typedef struct _MY_PEB_LDR_DATA {
	ULONG Length;
	BOOL Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} MY_PEB_LDR_DATA, *PMY_PEB_LDR_DATA;

typedef struct _MY_LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} MY_LDR_DATA_TABLE_ENTRY, *PMY_LDR_DATA_TABLE_ENTRY;


//定义函数指针
typedef HMODULE(WINAPI *pfnLoadLibraryA)(LPCSTR lpLibFileName);
typedef int (WINAPI *pfnMessageBoxA)(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);

//函数指针结构体
typedef struct _FUNCTIONS
{
	pfnLoadLibraryA fnLoadLibraryA;
	pfnMessageBoxA fnMessageBoxA;
}Functions,*Pfunctions;



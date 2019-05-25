#include "header.h"
#pragma comment(linker,"/entry:EntryMain")

//************************************************************
// 函数名称: EntryMain
// 函数说明: 入口函数
// 作	 者: GuiShou
// 时	 间: 2019/5/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void EntryMain()
{
	CreateShellCode(); 
}


//************************************************************
// 函数名称: CreateShellCode
// 函数说明: 将ShellCode写到文件
// 作	 者: GuiShou
// 时	 间: 2019/5/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void CreateShellCode()
{
	HANDLE hFile = CreateFileA("ShellCode.bin", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "CreateFileA Error", "Error", MB_ERR_INVALID_CHARS);
		return;
	}
	DWORD dwSize = (DWORD)ShellCodeEnd - (DWORD)ShellCodeStart;
	DWORD dwWrite = 0;;
	WriteFile(hFile, ShellCodeStart, dwSize, &dwWrite,NULL);
	CloseHandle(hFile);
}
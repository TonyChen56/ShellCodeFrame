#include "header.h"
#include "api.h"

__declspec(naked)void ShellCodeStart()
{
	__asm
	{
		jmp ShellCodeEntry;
	}
}



//************************************************************
// 函数名称: ShellCodeEntry
// 函数说明: 真正的ShellCode代码
// 作	 者: GuiShou
// 时	 间: 2019/5/19
// 参	 数: void
// 返 回 值: void
//************************************************************
void ShellCodeEntry()
{
	Functions fn;
	//动态获取所有需要的函数指针
	Initfunctions(&fn);
	//调用MessageBox
	char szText[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'u', 'i', 'S', 'h', 'o', 'u', 0 };
	char szCaption[] = { 'T', 'i', 'p', 0 };
	fn.fnMessageBoxA(NULL, szText, szCaption, MB_OK);
}
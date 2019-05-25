#include <stdio.h>
#include <windows.h>

int main(int argc,char* argv[])
{ 
	//打开要执行的ShellCode文件
	HANDLE hFile = CreateFileA("ShellCode.bin", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		printf("CreateFile Error");
		return -1;
	}

	DWORD dwSize = 0;
	//获取ShellCode的总大小
	dwSize = GetFileSize(hFile, NULL);

	//申请一块可读可写可执行的内存
	LPVOID lpAddress = VirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpAddress == NULL)
	{
		printf("VirtualAlloc Error");
		CloseHandle(hFile);
		return -1;
	}

	//将文件读取到申请的内存中
	DWORD dwRead = 0;
	ReadFile(hFile, lpAddress, dwSize, &dwRead, 0);

	//执行ShellCode
	__asm
	{
		call lpAddress;
	}
	return 0;
}
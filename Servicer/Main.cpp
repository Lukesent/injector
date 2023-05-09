#include "injector.h"

HWND                g_hWindow = nullptr;

void WINAPI everything(HMODULE hModule)
{
	Injector inject;
	Beep(500, 350);

	while (!((g_hWindow = FindWindowA(XorStr("Valve001"), nullptr))))
		Sleep(2500);
	
	while (!GetAsyncKeyState(VK_INSERT))
		Sleep(1000);

	if (inject.find_target_process(XorStr("csgo.exe")))
		if (inject.inject(XorStr("C:\\Program Files\\Malwarebytes\\Anti-Malware\\mbam.dll")))
			Beep(300, 150);

	Sleep(1500);
	FreeLibraryAndExitThread(hModule, 0);
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(everything), hModule, NULL, nullptr);
	}

	if (dwReason == DLL_PROCESS_DETACH)
	{
		return TRUE;
	}

	return TRUE;
}

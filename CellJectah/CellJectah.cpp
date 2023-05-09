#include <Windows.h>
#include <wincon.h>
#include <random>
#include <iostream>
#include <TlHelp32.h>
#include "XorStr.hpp"
#include <ctime>

std::string GenerateRandomString(int length)
{
	static const char alphanum[] =
		"0123456789"
		"!@#$%^&*"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string str;

	for (auto i = 0; i < length; i++)
	{
		str += alphanum[std::_Random_device() % sizeof(alphanum)];
	}

	return str;
}

void GenerateRandomWindowTitle(int stringLength)
{

	SetConsoleTitle(GenerateRandomString(stringLength).c_str());

}

DWORD GetProcessIdFromProcessName(const char* szProcessName)
{
	HANDLE th32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 procEntry;

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	while (Process32Next(th32, &procEntry))
	{
		if (_stricmp(szProcessName, procEntry.szExeFile) == 0)
		{
			CloseHandle(th32);
			return procEntry.th32ProcessID;
		}
	}

	CloseHandle(th32);

	return false();
}

// Load a DLL into a remote process
// Returns true on success, false on failure
bool LoadRemoteDLL(DWORD processId, LPCSTR dllPath)
{
    // Open the target process with the necessary permissions
    HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, processId);
    if (processHandle == NULL)
    {
        // Failed to open the process
        return false;
    }

    // Allocate memory in the target process to store the DLL path
    LPVOID remotePath = VirtualAllocEx(processHandle, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (remotePath == NULL)
    {
        // Failed to allocate memory in the process
        CloseHandle(processHandle);
        return false;
    }

    // Write the DLL path to the allocated memory
    if (!WriteProcessMemory(processHandle, remotePath, dllPath, strlen(dllPath) + 1, NULL))
    {
        // Failed to write the DLL path to the process
        VirtualFreeEx(processHandle, remotePath, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return false;
    }

    // Load the DLL into the process using a remote thread
    LPTHREAD_START_ROUTINE loadLibraryAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE loadThread = CreateRemoteThread(processHandle, NULL, NULL, loadLibraryAddr, remotePath, NULL, NULL);
    if (loadThread == NULL)
    {
        // Failed to create the remote thread
        VirtualFreeEx(processHandle, remotePath, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return false;
    }

    // Wait for the thread to finish and retrieve the handle to the loaded module
    HMODULE loadedModule = NULL;
    if (WaitForSingleObject(loadThread, INFINITE) == WAIT_OBJECT_0)
    {
        GetExitCodeThread(loadThread, (LPDWORD)&loadedModule);
    }

    // Clean up the allocated memory and handles
    VirtualFreeEx(processHandle, remotePath, 0, MEM_RELEASE);
    CloseHandle(loadThread);
    CloseHandle(processHandle);

    return loadedModule != NULL;
}


void RenameFunctions(char** argv)
{
	GenerateRandomWindowTitle(20);
	std::string x = GenerateRandomString(11);
	x += XorStr(".exe");
	rename(argv[0], x.c_str());
}


int main(int argc, char* argv[])
{
	// First Process that puts child dll into process that finds csgo and injects
	RenameFunctions(argv);

	DWORD pID = GetProcessIdFromProcessName(XorStr("mbamtray.exe"));

	if(!pID || pID == 0)
	{
		std::cout << XorStr("Invalid or Not Open!");
		Sleep(1000);
		return 0;
	}

	std::cout << XorStr("Malware Deep-Sea plugin! PID: ") << pID << std::endl;

	if(!CellJectMe(pID))
	std::cout << XorStr("Failed to add Deep-Sea plugin") << std::endl;

	return 0;
}


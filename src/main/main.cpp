#include "pch.hpp"
#include "main.hpp"

#define ERROR_MSGBOX_CAPTION "\"Fuck off EA App\"'s fatal error"
#define ERROR_MSGBOX_CAPTION_L L"\"Fuck off EA App\"'s fatal error"

CModule OriginExe;
CModule OriginClientServiceExe;
CModule OriginClient;

CMemory OriginExeAdr;
CMemory OriginClientServiceExeAdr;
CMemory OriginClientAdr;

CModule Qt5Core;

const fs::path GetExePath()
{
	WCHAR result[MAX_PATH];
	DWORD length = GetModuleFileNameW(NULL, result, MAX_PATH);
	if (!length)
	{
		MessageBoxA(nullptr, "Failed calling GetModuleFileNameW.\nThe program cannot continue and has to exit.", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
		std::exit(1);
	}
	fs::path path{ result };
	path._Remove_filename_and_separator();
	return path;
}

const std::string GetExeName()
{
	WCHAR result[MAX_PATH];
	DWORD length = GetModuleFileNameW(NULL, result, MAX_PATH);
	if (!length)
	{
		MessageBoxA(nullptr, "Failed calling GetModuleFileNameW.\nThe program cannot continue and has to exit.", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
		std::exit(1);
	}
	fs::path path{ result };
	return path.filename().string();
}

void InternalSetup()
{
	MH_Initialize();
	MH_SetThreadFreezeMethod(MH_FREEZE_METHOD_FAST_UNDOCUMENTED);

	auto exe_path = GetExePath();
	auto exe_name = GetExeName();
	bool isOriginExe = exe_name == "Origin.exe";
	bool isOriginClientServiceExe = exe_name == "OriginClientService.exe";

	/*if (isOriginExe)
	{
		// preload the OriginClient.dll
		if (!LoadLibraryW((exe_path / L"OriginClient.dll").wstring().c_str()))
			MessageBoxA(nullptr, "Failed preloading OriginClient.dll", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}*/

	OriginExe = CModule("Origin.exe", uintptr_t(GetModuleHandleA(nullptr)));
	OriginClientServiceExe = CModule("OriginClientService.exe", uintptr_t(GetModuleHandleA(nullptr)));
	//OriginClient = CModule("OriginClient.dll");

	OriginExeAdr = CMemory(OriginExe.GetModuleBase());
	OriginClientServiceExeAdr = CMemory(OriginClientServiceExe.GetModuleBase());
	//OriginClientAdr = CMemory(OriginClient.GetModuleBase());

	if (isOriginExe)
	{
		// Main Origin.exe program
		DoOriginExePatches();
		//DoOriginClientDllPatches();
	}
	else if (isOriginClientServiceExe)
	{
		DoOriginClientServiceExePatches();
	}
	else
	{
		// WARN: loaded into unrecognized program!
	}

	MH_STATUS result;
	if ((result = MH_EnableHook(MH_ALL_HOOKS)) != MH_OK)
		MessageBoxA(nullptr, ("MH_EnableHook(MH_ALL_HOOKS) error: " + std::to_string(result)).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);
}

extern "C" void FuckOffEAAppSetup()
{
	InternalSetup();
}

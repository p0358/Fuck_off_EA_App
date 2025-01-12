#include "pch.hpp"
#include "main.hpp"

#define ERROR_MSGBOX_CAPTION "\"Fuck off EA App\"'s fatal error"
#define ERROR_MSGBOX_CAPTION_L L"\"Fuck off EA App\"'s fatal error"

CModule OriginExe;
CModule OriginClientServiceExe;
CModule OriginClient;
CModule EALinkExe;

CMemory OriginExeAdr;
CMemory OriginClientServiceExeAdr;
CMemory OriginClientAdr;
CMemory EALinkExeAdr;

CModule Qt5Core;
CModule Qt5Network;

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
	// Initialize MinHook
	MH_Initialize();
	MH_SetThreadFreezeMethod(MH_FREEZE_METHOD_FAST_UNDOCUMENTED);

	auto exe_path = GetExePath();
	auto exe_name = GetExeName();
	bool isOriginExe = exe_name == "Origin.exe";
	bool isOriginClientServiceExe = exe_name == "OriginClientService.exe";
	bool isOriginThinSetupInternalExe = exe_name == "OriginThinSetupInternal.exe";
	bool isEALinkExe = exe_name == "EALink.exe";

	OriginExe = CModule("Origin.exe", uintptr_t(GetModuleHandleA(nullptr)));
	OriginClientServiceExe = CModule("OriginClientService.exe", uintptr_t(GetModuleHandleA(nullptr)));
	EALinkExe = CModule("EALinkExe.exe", uintptr_t(GetModuleHandleA(nullptr)));

	OriginExeAdr = CMemory(OriginExe.GetModuleBase());
	OriginClientServiceExeAdr = CMemory(OriginClientServiceExe.GetModuleBase());
	EALinkExeAdr = CMemory(EALinkExe.GetModuleBase());

	if (isOriginExe)
	{
		// Main Origin.exe program
		DoOriginExePatches(); // this function will set up a hook to wait for OriginClient.dll to be loaded
	}
	else if (isOriginClientServiceExe)
	{
		// statically imported by OriginClientService.exe
		Qt5Core = CModule("Qt5Core.dll");

		DoOriginClientServiceExePatches();
	}
	else if (isOriginThinSetupInternalExe)
	{
		// statically imported by OriginThinSetupInternal.exe
		Qt5Core = CModule("Qt5Core.dll");

		DoOriginThinSetupInternalExePatches();
	}
	else if (isEALinkExe)
	{
		// statically imported by EALinkExe.exe
		Qt5Core = CModule("Qt5Core.dll");

		DoEALinkExePatches();
	}
	else
	{
		// WARN: loaded into unrecognized program!
	}

	// Enable all queued hooks
	MH_STATUS result;
	if ((result = MH_EnableHook(MH_ALL_HOOKS)) != MH_OK)
		MessageBoxA(nullptr, ("MH_EnableHook(MH_ALL_HOOKS) error: " + std::to_string(result)).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);
}

extern "C" void FuckOffEAAppSetup() // called from Loader project
{
	InternalSetup();
}

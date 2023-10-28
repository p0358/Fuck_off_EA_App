// Patches for Origin.exe

#include "pch.hpp"
#include "main.hpp"

decltype(&LoadLibraryW) LoadLibraryW_org;
HMODULE WINAPI LoadLibraryW_Hook(LPCWSTR lpLibFileName)
{
	auto ret = LoadLibraryW_org(lpLibFileName);

	static bool didDoOriginClientDllPatches = false;
	if (ret && !didDoOriginClientDllPatches && GetProcAddress(ret, "OriginApplicationStart"))
	{
		OriginClient = CModule("OriginClient.dll");
		OriginClientAdr = CMemory(OriginClient.GetModuleBase());

		// statically imported by OriginClient.dll
		Qt5Core = CModule("Qt5Core.dll");

		DoOriginClientDllPatches();

		MH_STATUS result;
		if ((result = MH_EnableHook(MH_ALL_HOOKS)) != MH_OK)
			MessageBoxA(nullptr, ("MH_EnableHook(MH_ALL_HOOKS) error: " + std::to_string(result)).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);

		didDoOriginClientDllPatches = true;
	}

	return ret;
}

// Updater::CheckForSameVersion
bool(__thiscall* updaterCheckForSameVersion_org)(void*, char, char*);
bool __fastcall updaterCheckForSameVersion_hook(void* thisptr /*ecx*/, void* /*edx*/, char a2, char* incomingVersion)
{
	OutputDebugStringA((std::string("[Origin.exe] [Updater::CheckForSameVersion] incomingVersion:") + incomingVersion).c_str());
	if (!std::string_view(incomingVersion).starts_with("10."))
		return true; // tell it that version numbers match
	return updaterCheckForSameVersion_org(thisptr, a2, incomingVersion);
}

void DoOriginExePatches()
{
	// We hook LoadLibraryW to monitor for when OriginClient.dll is actually loaded, as it's loaded dynamically.
	// It is worth noting that it's not loaded right away, instead Origin.exe itself first starts the updater, which
	// then starts Origin.exe again, to then finally actually start up, making this solution necessary.
	CreateHookNamed("kernel32", "LoadLibraryW", LoadLibraryW_Hook, reinterpret_cast<LPVOID*>(&LoadLibraryW_org));

	auto OriginExe = CModule("Origin.exe", uintptr_t(GetModuleHandleA(nullptr)));
	auto updaterCheckForSameVersion_adr = CMemory(OriginExe.GetModuleBase()).FindPattern("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC DC 04", CMemory::Direction::DOWN, 5 * 1024 * 1024);
	if (updaterCheckForSameVersion_adr)
	{
		CreateHook(updaterCheckForSameVersion_adr.GetPtr(), updaterCheckForSameVersion_hook, reinterpret_cast<LPVOID*>(&updaterCheckForSameVersion_org));
	}
	else
	{
		if (!strstr(GetCommandLineA(), "/noUpdate"))
			MessageBoxA(nullptr, "Failed resolving pattern of Updater::CheckForSameVersion, we may fail to block the broken Origin update.", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}
}

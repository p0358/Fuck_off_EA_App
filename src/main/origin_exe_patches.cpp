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
		Qt5Network = CModule("Qt5Network.dll");

		DoOriginClientDllPatches();

		MH_STATUS result;
		if ((result = MH_EnableHook(MH_ALL_HOOKS)) != MH_OK)
			MessageBoxA(nullptr, ("MH_EnableHook(MH_ALL_HOOKS) error: " + std::to_string(result)).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);

		didDoOriginClientDllPatches = true;
	}

	return ret;
}

// Updater::CheckForSameVersion
bool(__thiscall* updaterCheckForSameVersion_org)(void*, char, wchar_t*);
bool __fastcall updaterCheckForSameVersion_hook(void* thisptr /*ecx*/, void* /*edx*/, char a2, wchar_t* incomingVersion)
{
	OutputDebugStringW((std::wstring(L"[Origin.exe] [Updater::CheckForSameVersion] incomingVersion:") + incomingVersion).c_str());
	if (!std::wstring_view(incomingVersion).starts_with(L"10."))
		return true; // tell it that version numbers match
	return updaterCheckForSameVersion_org(thisptr, a2, incomingVersion);
}

// this[42] = downloadURL [0x100u]
// this[48] = updateRule [0x100u]
// Downloader::UpdateCheck::loadXML
bool(__thiscall* updaterLoadXML_org)(wchar_t**, int);
bool __fastcall updaterLoadXML_hook(wchar_t** thisptr /*ecx*/, void* /*edx*/, int a2)
{
	auto ret = updaterLoadXML_org(thisptr, a2);
	wchar_t* downloadURL = thisptr[42];
	wchar_t* updateRule = thisptr[48];
	if (ret && downloadURL && downloadURL[0] && updateRule && updateRule[0])
	{
		OutputDebugStringW((std::wstring(L"[Origin.exe] [Downloader::UpdateCheck::loadXML] downloadURL = ") + downloadURL).c_str());
		OutputDebugStringW((std::wstring(L"[Origin.exe] [Downloader::UpdateCheck::loadXML] updateRule = ") + updateRule).c_str());
		wcscpy_s(updateRule, 256, L"OPTIONAL"); // make the update optional

		// fix broken update URL for 10.5.129 (major EA moment)
		if (wcscpy(downloadURL, L"https://origin-a.akamaihd.net/Stage-Origin-Client-Download/origin/live/OriginUpdate_10_5_129_55742.zip") == 0)
			wcscpy_s(downloadURL, 256, L"https://origin-a.akamaihd.net/Origin-Client-Download/origin/live/OriginUpdate_10_5_129_55742.zip");
	}

	return ret;
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
	
	auto updaterLoadXML_adr = CMemory(OriginExe.GetModuleBase()).FindPattern("55 8B EC 6A ? 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC ? A1 ? ? ? ? 33 C5 89 45 ? 53 56 57 50 8D 45 ? 64 A3 ? ? ? ? 8B D9 C7 45", CMemory::Direction::DOWN, 5 * 1024 * 1024);
	if (updaterLoadXML_adr)
	{
		CreateHook(updaterLoadXML_adr.GetPtr(), updaterLoadXML_hook, reinterpret_cast<LPVOID*>(&updaterLoadXML_org));
	}
	else
	{
		if (!strstr(GetCommandLineA(), "/noUpdate"))
			MessageBoxA(nullptr, "Failed resolving pattern of Downloader::UpdateCheck::loadXML, we may fail to fix up Origin update.", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}
}

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
		DoOriginClientDllPatches();
		MH_STATUS result;
		if ((result = MH_EnableHook(MH_ALL_HOOKS)) != MH_OK)
			MessageBoxA(nullptr, ("MH_EnableHook(MH_ALL_HOOKS) error: " + std::to_string(result)).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);
		didDoOriginClientDllPatches = true;
	}

	return ret;
}

void DoOriginExePatches()
{
	CreateHookNamed("kernel32", "LoadLibraryW", LoadLibraryW_Hook, reinterpret_cast<LPVOID*>(&LoadLibraryW_org));
}

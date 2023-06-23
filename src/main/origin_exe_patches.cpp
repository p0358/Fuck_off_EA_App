// Patches for Origin.exe

#include "pch.hpp"
#include "main.hpp"

decltype(&LoadLibraryW) LoadLibraryW_org;
HMODULE WINAPI LoadLibraryW_Hook(LPCWSTR lpLibFileName)
{
	auto ret = LoadLibraryW_org(lpLibFileName);
	//MessageBoxW(0, lpLibFileName, L"hello1???????", 0);

	static bool didDoOriginClientDllPatches = false;
	if (ret && !didDoOriginClientDllPatches && GetProcAddress(ret, "OriginApplicationStart"))
	{
		////MessageBoxW(0, lpLibFileName, L"OriginApplicationStart???????", 0);
		OriginClient = CModule("OriginClient.dll");
		OriginClientAdr = CMemory(OriginClient.GetModuleBase());
		Qt5Core = CModule("Qt5Core.dll");
		DoOriginClientDllPatches();
		MH_STATUS result;
		if ((result = MH_EnableHook(MH_ALL_HOOKS)) != MH_OK)
			MessageBoxA(nullptr, ("MH_EnableHook(MH_ALL_HOOKS) error: " + std::to_string(result)).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);
		didDoOriginClientDllPatches = true;
	}
	//MessageBoxW(0, lpLibFileName, L"hello2???????", 0);

	if (ret && GetProcAddress(ret, "??0QSize@@QAE@XZ"))
	{
		//Qt5Core = CModule("Qt5Core.dll");
		//MessageBoxA(0, std::to_string(uintptr_t(Qt5Core.GetModuleBase())).c_str(), "Qt5Core???????", 0);
	}

	return ret;
}

void DoOriginExePatches()
{
	CreateHookNamed("kernel32", "LoadLibraryW", LoadLibraryW_Hook, reinterpret_cast<LPVOID*>(&LoadLibraryW_org));
}

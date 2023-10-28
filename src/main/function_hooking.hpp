#pragma once

inline void CreateHook_Error(const std::string& err)
{
	MessageBoxA(nullptr, err.c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
}

inline size_t GetModuleHandleFromAddress(void* adr)
{
	HMODULE hm = NULL;
	if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)adr, &hm) == 0)
	{
		return 0;
	}
	return reinterpret_cast<size_t>(hm);
}

inline std::string GetModuleNameFromHandle(HMODULE hm)
{
	char path[MAX_PATH];
	if (GetModuleFileNameA(hm, path, sizeof(path)) == 0)
	{
		return "<unknown>";
	}
	return std::string(path);
}

inline MH_STATUS WINAPI CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal)
{
	auto ret = MH_CreateHook(pTarget, pDetour, ppOriginal);
	if (ret != MH_OK) [[unlikely]]
	{
		auto baseAddress = GetModuleHandleFromAddress(pTarget);
		if (baseAddress)
		{
			CreateHook_Error("Error hooking function at \"" + GetModuleNameFromHandle((HMODULE)baseAddress) + "\" module's offset of " + std::to_string(baseAddress - size_t(pTarget)));
		}
		else
		{
			CreateHook_Error("Error hooking function at " + std::to_string(size_t(pTarget)));
		}
	}
	return ret;
}

inline MH_STATUS WINAPI CreateHook(uintptr_t pTarget, LPVOID pDetour, LPVOID* ppOriginal)
{
	return CreateHook((LPVOID)pTarget, pDetour, ppOriginal);
}

inline MH_STATUS WINAPI CreateHookNamed(const char* moduleName, const char* procName, LPVOID pDetour, LPVOID* ppOriginal)
{
	HMODULE hModule = GetModuleHandleA(moduleName);
	if (!moduleName)
		moduleName = "<exe>";
	if (!hModule) [[unlikely]]
	{
		auto err = GetLastError();
		CreateHook_Error("GetModuleHandle failed for " + std::string(moduleName) + " (Error = " + std::to_string(err) + ")");
		return MH_ERROR_MODULE_NOT_FOUND;
	}

	void* exportPtr = GetProcAddress(hModule, procName);
	if (!exportPtr) [[unlikely]]
	{
		auto err = GetLastError();
		CreateHook_Error("GetModuleHandle failed for " + std::string(procName) + " (ModuleName = " + std::string(moduleName) + ", Error = " + std::to_string(err) + ")");
		return MH_ERROR_FUNCTION_NOT_FOUND;
	}

	LPVOID pTarget = exportPtr;
	auto ret = MH_CreateHook(pTarget, pDetour, ppOriginal);
	if (ret != MH_OK) [[unlikely]]
	{
		CreateHook_Error("Error hooking function " + std::string(procName) + " (" + std::to_string((uintptr_t)exportPtr) + ") in " + std::string(moduleName));
	}
	return ret;
}

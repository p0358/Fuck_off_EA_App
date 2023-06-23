#pragma once

inline void CreateHook_Error(const std::string& err)
{
	MessageBoxA(0, err.c_str(), "Error in CreateHook", MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
}

inline MH_STATUS WINAPI CreateHook(DWORD64 baseAddress, unsigned int offset, LPVOID pDetour, LPVOID* ppOriginal)
{
	LPVOID pTarget = (LPVOID)(baseAddress + offset);
	auto ret = MH_CreateHook(pTarget, pDetour, ppOriginal);
	if (ret != MH_OK) [[unlikely]]
	{
		CreateHook_Error("Error hooking function at module's offset of %X" + std::to_string(offset));
	}
	return ret;
}

inline MH_STATUS WINAPI CreateHookNamed(const char* moduleName, const char* procName, LPVOID pDetour, LPVOID* ppOriginal)
{
	HMODULE hModule = GetModuleHandleA(moduleName);
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

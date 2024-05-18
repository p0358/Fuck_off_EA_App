#include "pch.hpp"

extern "C"
{
    extern void FuckOffEAAppSetup();
}

const std::wstring GetExePathWide()
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
	return path.wstring();
}

void LibraryLoadError(DWORD dwMessageId, const wchar_t* libName, const wchar_t* location)
{
	char text[4096];
	std::string message = std::system_category().message(dwMessageId);
	sprintf_s(text, "Failed to load the %ls at \"%ls\" (%lu):\n\n%hs", libName, location, dwMessageId, message.c_str());
	if (dwMessageId == 126 && std::filesystem::exists(location))
	{
		sprintf_s(
			text,
			"%s\n\nThe file at the specified location DOES exist, so this error indicates that one of its *dependencies* failed to be "
			"found.",
			text);
	}
	MessageBoxA(GetForegroundWindow(), text, ERROR_MSGBOX_CAPTION, MB_ICONERROR);
}

void LoadProxiedLibrary()
{
    wchar_t org_dll[MAX_PATH];
    GetSystemDirectoryW(org_dll, MAX_PATH);
    lstrcatW(org_dll, L"\\version.dll");

    HINSTANCE hL = LoadLibraryExW(org_dll, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!hL)
    {
        LibraryLoadError(GetLastError(), L"version.dll", org_dll);
        return;
    }
    
    extern void ProxySetup(HINSTANCE hL);
    ProxySetup(hL);
}

void Init()
{
    LoadProxiedLibrary(); // loads original `version.dll` to proxy the exported functions back to it
    FuckOffEAAppSetup(); // that function is located in the "Main" project
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	DisableThreadLibraryCalls(hInstDLL);

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
        Init();
		return TRUE;
	}

	return FALSE;
}

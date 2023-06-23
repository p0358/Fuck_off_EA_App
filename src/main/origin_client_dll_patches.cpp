// Patches for OriginClient.dll

#include "pch.hpp"
#include "main.hpp"

void DoOriginClientDllPatches()
{
	{
		// Spoof Origin version to '10.5.122.52971' if we're older
		//auto originClientVersion = OriginClient.GetExportedFunction("OriginClientVersion").RCast<const char*(*)()>()();
		//auto originClientVersionn = reinterpret_cast<const char*(*)()>(GetProcAddress(HMODULE(OriginClient.GetModuleBase()), "OriginClientVersion"));
		auto originClientVersionn = reinterpret_cast<const char*(*)()>(GetProcAddress(GetModuleHandleA("OriginClient.dll"), "OriginClientVersion"));
		auto originClientVersion = originClientVersionn();
		MessageBoxA(0, originClientVersion, "hello3", 0);
		std::string originClientVersionStr{ originClientVersion };
		std::regex versionRegex{ "^10\\.5\\.([0-9]+)\\.[0-9]+$" };
		std::smatch m;
		if (std::regex_search(originClientVersionStr, m, versionRegex) && m.size() == 2 && m[1].length())
		{
			try
			{
				auto v = std::stoi(m[1]);
				if (v < 122)
				{
					CMemory(originClientVersion).PatchString("10.5.122.52971");
				}
			}
			catch (const std::exception& e)
			{
				MessageBoxA(nullptr, ("Error checking Origin version: " + std::string(e.what()) + "\nVersion: " + originClientVersionStr).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);
			}
		}
	}
}

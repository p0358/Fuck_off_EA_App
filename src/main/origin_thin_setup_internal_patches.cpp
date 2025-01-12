// Patches for OriginClientService.exe

#include "pch.hpp"
#include "main.hpp"

namespace OriginThinSetupInternalExePatches
{
	// Origin::ThinSetup::ThinSetupController::onDownloadUpdateContent
	bool(__thiscall* onDownloadUpdateContent_org)(uint32_t**);
	bool __fastcall onDownloadUpdateContent_hook(uint32_t** thisptr)
	{
		static auto QString_destroy = GetExport<void(__thiscall*)(void*)>(Qt5Core, "??1QString@@QAE@XZ");
		static auto QString_toStdString = GetExport<void* (__thiscall*)(void*, std::string*)>(Qt5Core, "?toStdString@QString@@QBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
		static auto QString_QString_from_cchar = GetExport<void* (__thiscall*)(void*, const char*)>(Qt5Core, "??0QString@@QAE@PBD@Z");

		if (QString_destroy && QString_toStdString && QString_QString_from_cchar)
		{
			void* updateURL = thisptr[17] + 17;

			std::string qss;
			QString_toStdString(updateURL, &qss);
			//MessageBoxA(0, qss.c_str(), "updateURL", 0);

			if (qss == "https://origin-a.akamaihd.net/Stage-Origin-Client-Download/origin/live/OriginUpdate_10_5_129_55742.zip")
			{
				QString_destroy(updateURL);
				QString_QString_from_cchar(updateURL, "https://origin-a.akamaihd.net/Origin-Client-Download/origin/live/OriginUpdate_10_5_129_55742.zip");

				std::string qss;
				QString_toStdString(updateURL, &qss);
				//MessageBoxA(0, qss.c_str(), "updateURL overriden", 0);
			}
		}

		auto ret = onDownloadUpdateContent_org(thisptr);
		return ret;
	}
}

void DoOriginThinSetupInternalExePatches()
{
	using namespace OriginThinSetupInternalExePatches;
	
	auto OriginExe = CModule("OriginThinSetupInternal.exe", uintptr_t(GetModuleHandleA(nullptr)));

	auto onDownloadUpdateContent_adr = CMemory(OriginExe.GetModuleBase()).FindPattern("55 8B EC 6A ? 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC ? 56 A1 ? ? ? ? 33 C5 50 8D 45 ? 64 A3 ? ? ? ? 8B F1 8B 4E ? 8D 45 ? 50 E8 ? ? ? ? 68", CMemory::Direction::DOWN, 25 * 1024 * 1024);
	if (onDownloadUpdateContent_adr)
	{
		CreateHook(onDownloadUpdateContent_adr.GetPtr(), onDownloadUpdateContent_hook, reinterpret_cast<LPVOID*>(&onDownloadUpdateContent_org));
	}
	else
	{
		MessageBoxA(nullptr, "Failed resolving pattern of Origin::ThinSetup::ThinSetupController::onDownloadUpdateContent, we may fail to fix up Origin update.", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}
}

// Patches for EALink.exe

#include "pch.hpp"
#include "main.hpp"

namespace EALinkExePatches
{
	// Origin::Services::readSetting
	void* (__cdecl* readSetting_org)(void*, void*, int, void*);
	void* __cdecl readSetting_hook(void* out_qv, void* setting, int a3, void* a4)
	{
		static auto QVariant_QVariant_from_bool = GetExport<void* (__thiscall*)(void*, bool)>(Qt5Core, "??0QVariant@@QAE@_N@Z");
		static auto SETTING_MigrationDisabled = GetExport<void*>(EALinkExe, "?SETTING_MigrationDisabled@Services@Origin@@3VSetting@12@A"); // non-const symbol
		if (!SETTING_MigrationDisabled)
			SETTING_MigrationDisabled = GetExport<void*>(EALinkExe, "?SETTING_MigrationDisabled@Services@Origin@@3VSetting@12@B"); // const symbol
		if (!QVariant_QVariant_from_bool || !SETTING_MigrationDisabled) [[unlikely]]
			MessageBoxA(nullptr, ("Error in Origin::Services::readSetting: one of the functions could not have been resolved, we will crash\n"
				"\nQVariant_QVariant_from_bool: " + std::to_string(uintptr_t(QVariant_QVariant_from_bool))
				+ "\nSETTING_MigrationDisabled: " + std::to_string(uintptr_t(SETTING_MigrationDisabled))
				).c_str(),
				ERROR_MSGBOX_CAPTION, MB_ICONERROR);

		if (setting == SETTING_MigrationDisabled)
		{
			// override to true
			QVariant_QVariant_from_bool(out_qv, true); // caller will destruct this
			return out_qv;
		}

		return readSetting_org(out_qv, setting, a3, a4);
	}
}

void DoEALinkExePatches()
{
	using namespace EALinkExePatches;
	// checked in Origin::Services::OriginConfigService::init
	// seems to set the boolean on the object and also fire a delayed QTimer to `1downloadConfig()` if migration isn't disabled
	CreateHookNamed(nullptr, "?readSetting@Services@Origin@@YA?AVVariant@12@ABVSetting@12@V?$QSharedPointer@VAbstractSession@Session@Services@Origin@@@@@Z", readSetting_hook, reinterpret_cast<LPVOID*>(&readSetting_org));
}

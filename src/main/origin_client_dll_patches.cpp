// Patches for OriginClient.dll

#include "pch.hpp"
#include "main.hpp"

// Origin::Client::JsInterface::ClientSettingsProxy::findUserMigrationTimePeriod
void*(__stdcall *findUserMigrationTimePeriod_org)(void*);
void* __stdcall findUserMigrationTimePeriod_hook(void* a1)
{
	static auto QJsonObject_QJsonObject = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??0QJsonObject@@QAE@XZ");
	static auto QJsonObject_QJsonObject_destruct = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??1QJsonObject@@QAE@XZ");
	static auto QJsonObject_toVariantMap = GetExport<void*(__thiscall*)(void*, void*)>(Qt5Core, "?toVariantMap@QJsonObject@@QBE?AV?$QMap@VQString@@VQVariant@@@@XZ");
	if (!QJsonObject_QJsonObject || !QJsonObject_QJsonObject_destruct || !QJsonObject_toVariantMap) [[unlikely]]
		MessageBoxA(nullptr, ("Error in Origin::Client::JsInterface::ClientSettingsProxy::findUserMigrationTimePeriod: one of the functions could not have been resolved, we will crash\n"
			"\nQJsonObject_QJsonObjectl: " + std::format("{:#010x}", uintptr_t(QJsonObject_QJsonObject))
			+ "\nQJsonObject_QJsonObject_destruct: " + std::format("{:#010x}", uintptr_t(QJsonObject_QJsonObject_destruct))
			+ "\nQJsonObject_toVariantMap: " + std::format("{:#010x}", uintptr_t(QJsonObject_toVariantMap))
			).c_str(),
			ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	
	// just return an empty Qt JSON object
	char json[12] = { 0 };
	QJsonObject_QJsonObject(&json);
	QJsonObject_toVariantMap(&json, a1);
	QJsonObject_QJsonObject_destruct(&json);
	return a1;
}

struct migrationConfigDummyStruct
{
	bool bool0;
	bool bool1;
	char pad[12];
	int int16;
	int int20;
	int int24;
	int int28;
	int int32;
	int int36;
	int int40;
	int int44;
};

// Origin::Services::OriginConfigService::migrationConfig
migrationConfigDummyStruct* (__thiscall* migrationConfig_org)(void*, migrationConfigDummyStruct*);
migrationConfigDummyStruct* __fastcall migrationConfig_hook(void* thisptr /*ecx*/, void* /*edx*/, migrationConfigDummyStruct* cfg)
{
	migrationConfig_org(thisptr, cfg);
	cfg->bool0 = false;
	cfg->bool1 = false;
	return cfg;
}

// Origin::Services::readSetting
void*(__cdecl* readSetting_org)(void*, void*, int, void*);
void* __cdecl readSetting_hook(void* out_qv, void* setting, int a3, void* a4)
{
	static auto QVariant_QVariant_from_bool = GetExport<void*(__thiscall*)(void*, bool)>(Qt5Core, "??0QVariant@@QAE@_N@Z");
	static auto QVariant_QVariant_from_cchar = GetExport<void*(__thiscall*)(void*, const char*)>(Qt5Core, "??0QVariant@@QAE@PBD@Z");
	static auto QVariant_destroy = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??1QVariant@@QAE@XZ");

	static auto SETTING_MigrationDisabled = GetExport<void*>(OriginClient, "?SETTING_MigrationDisabled@Services@Origin@@3VSetting@12@A"); // non-const symbol
	if (!SETTING_MigrationDisabled)
		SETTING_MigrationDisabled = GetExport<void*>(OriginClient, "?SETTING_MigrationDisabled@Services@Origin@@3VSetting@12@B"); // const symbol
	static auto SETTING_REMEMBER_ME_PROD = GetExport<void*>(OriginClient, "?SETTING_REMEMBER_ME_PROD@Services@Origin@@3VSetting@12@A"); // non-const symbol
	if (!SETTING_REMEMBER_ME_PROD)
		SETTING_REMEMBER_ME_PROD = GetExport<void*>(OriginClient, "?SETTING_REMEMBER_ME_PROD@Services@Origin@@3VSetting@12@B"); // const symbol

	static auto Origin_Services_Variant_operator_QString = GetExport<void* (__thiscall*)(void*, void*)>(OriginClient, "??BVariant@Services@Origin@@QBE?AVQString@@XZ");
	static auto QString_destroy = GetExport<void(__thiscall*)(void*)>(Qt5Core, "??1QString@@QAE@XZ");
	static auto QString_toStdString = GetExport<void* (__thiscall*)(void*, std::string*)>(Qt5Core, "?toStdString@QString@@QBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");

	static bool didWarnAboutMissingAlready = false;
	if (!didWarnAboutMissingAlready && (!QVariant_QVariant_from_bool || !QVariant_QVariant_from_cchar || !QVariant_destroy
		|| !SETTING_MigrationDisabled || !SETTING_REMEMBER_ME_PROD || !Origin_Services_Variant_operator_QString || !QString_destroy
		|| !QString_toStdString)) [[unlikely]]
	{
		didWarnAboutMissingAlready = true;
		MessageBoxA(nullptr, ("Error in Origin::Services::readSetting: one of the exports could not have been resolved, we may crash\n"
			"\nQVariant_QVariant_from_bool: " + std::format("{:#010x}", uintptr_t(QVariant_QVariant_from_bool))
			+ "\nQVariant_QVariant_from_cchar: " + std::format("{:#010x}", uintptr_t(QVariant_QVariant_from_cchar))
			+ "\nQVariant_destroy: " + std::format("{:#010x}", uintptr_t(QVariant_destroy))
			+ "\nSETTING_MigrationDisabled: " + std::format("{:#010x}", uintptr_t(SETTING_MigrationDisabled))
			+ "\nSETTING_REMEMBER_ME_PROD: " + std::format("{:#010x}", uintptr_t(SETTING_REMEMBER_ME_PROD))
			+ "\nOrigin_Services_Variant_operator_QString: " + std::format("{:#010x}", uintptr_t(Origin_Services_Variant_operator_QString))
			+ "\nQString_destroy: " + std::format("{:#010x}", uintptr_t(QString_destroy))
			+ "\nQString_toStdString: " + std::format("{:#010x}", uintptr_t(QString_toStdString))
			).c_str(),
			ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}

	void* ret = readSetting_org(out_qv, setting, a3, a4);

	if (IsDebuggerPresent())
	{
		
		if (Origin_Services_Variant_operator_QString && QString_destroy && QString_toStdString)
		{
			char qs[4] = { 0 };
			Origin_Services_Variant_operator_QString(ret, qs);

			std::string qss;
			QString_toStdString(qs, &qss);

			std::string setting_name;
			QString_toStdString((void*)(uintptr_t(setting) + 8), &setting_name);
			OutputDebugStringA(("readSetting: " + setting_name + "=" + qss).c_str());

			/*if (setting_name == "RememberMeProd")
			{
				QVariant_destroy(ret);
				static auto QVariant_QVariant_from_cchar = GetExport<void* (__thiscall*)(void*, const char*)>(Qt5Core, "??0QVariant@@QAE@PBD@Z");
				QVariant_QVariant_from_cchar(out_qv, "TUU...||accounts.ea.com||2025-03-11T21:21:02Z||/connect");
				OutputDebugStringA("overriding!");
			}*/

			QString_destroy(qs);
		}
	}
	
	if (setting == SETTING_MigrationDisabled)
	{
		// override to true
		QVariant_destroy(ret);
		QVariant_QVariant_from_bool(out_qv, true); // caller will destruct this
		return out_qv;
	}
	
	if (setting == SETTING_REMEMBER_ME_PROD)
	{
		OutputDebugStringA("[Origin::Services::readSetting] SETTING_REMEMBER_ME_PROD");
		char qs[4] = { 0 };
		Origin_Services_Variant_operator_QString(ret, qs);

		std::string qss;
		QString_toStdString(qs, &qss);
		QString_destroy(qs);
		//OutputDebugStringA(("[Origin::Services::readSetting] qss: " + qss).c_str());

		std::vector<std::string> parts;
		for (const auto word : std::views::split(qss, "||"sv))
			parts.push_back(std::string{ word.begin(), word.end() });

		if (parts.size() >= 2)
		{
			//OutputDebugStringA(("[Origin::Services::readSetting] parts[1]: " + parts[1]).c_str());
			if (parts[1] == ".ea.com")
			{
				OutputDebugStringA("[Origin::Services::readSetting] Fixing domain name to accounts.ea.com for cookie remid");
				parts[1] = "accounts.ea.com";

				qss.clear();
				for (auto word : parts)
					qss += "||" + word;
				qss.erase(0, 2);

				QVariant_destroy(ret);
				QVariant_QVariant_from_cchar(out_qv, qss.c_str()); // caller will destruct this
				//OutputDebugStringA(("[Origin::Services::readSetting] final value: " + qss).c_str());
				return out_qv;
			}
		}
	}

	return ret;
}

// Origin::Engine::Content::LocalContent::treatUpdatesAsMandatory
bool(__thiscall* treatUpdatesAsMandatory_org)(void*);
bool __fastcall treatUpdatesAsMandatory_hook(void* thisptr /*ecx*/, void* /*edx*/)
{
	return false;
}

// Origin::Services::SelfUpdateService::checkForUpdate
void(__thiscall* checkForUpdate_org)(void*);
void __fastcall checkForUpdate_hook(void* thisptr /*ecx*/)
{
	// How about no?
	// If there's a real update, and the user didn't disable updating, then it will be
	// downloaded upon next Origin launch by thin setup.
}

void DoOriginClientDllPatches()
{
	{
		// Spoof Origin version to '10.5.122.52971' if we're older,
		// this works around the "Please use EA App to continue" message, which appears as a result of user-agent check in JavaScript.
		// The `OriginClientVersion` function returns a shared pointer to the version string that is also used while constructing said user agent.
		auto originClientVersion = GetExport<const char* (*)()>(OriginClient, "OriginClientVersion")();
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

	// The function hooked below is used by the primary EA App migration screen, to determine whether said screen should be displayed
	CreateHookNamed("OriginClient", "?findUserMigrationTimePeriod@ClientSettingsProxy@JsInterface@Client@Origin@@QAE?AV?$QMap@VQString@@VQVariant@@@@XZ", findUserMigrationTimePeriod_hook, reinterpret_cast<LPVOID*>(&findUserMigrationTimePeriod_org));
	
	// This is for native parts of Origin that check migration config, apart from the JavaScript UI and its notice
	CreateHookNamed("OriginClient", "?migrationConfig@OriginConfigService@Services@Origin@@QBE?AUMigrationConfigT@server@@XZ", migrationConfig_hook, reinterpret_cast<LPVOID*>(&migrationConfig_org));
	CreateHookNamed("OriginClient", "?readSetting@Services@Origin@@YA?AVVariant@12@ABVSetting@12@V?$QSharedPointer@VAbstractSession@Session@Services@Origin@@@@@Z", readSetting_hook, reinterpret_cast<LPVOID*>(&readSetting_org));
	
	// Bonus patch: allow to launch games without updating by treating all updates as non-mandatory
	CreateHookNamed("OriginClient", "?treatUpdatesAsMandatory@LocalContent@Content@Engine@Origin@@QBE_NXZ", treatUpdatesAsMandatory_hook, reinterpret_cast<LPVOID*>(&treatUpdatesAsMandatory_org));
	
	// This checks for update to nag the user in the UI and auto-download the update zip, we don't ever need that
	CreateHookNamed("OriginClient", "?checkForUpdate@SelfUpdateService@Services@Origin@@QAEXXZ", checkForUpdate_hook, reinterpret_cast<LPVOID*>(&checkForUpdate_org));
}

// Patches for OriginClient.dll

#include "pch.hpp"
#include "main.hpp"

// Origin::Client::JsInterface::ClientSettingsProxy::findUserMigrationTimePeriod
void*(__stdcall *findUserMigrationTimePeriod_org)(void*);
void* __stdcall findUserMigrationTimePeriod_hook(void* a1)
{
	// just return an empty Qt JSON object
	static auto QJsonObject_QJsonObject = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??0QJsonObject@@QAE@XZ");
	static auto QJsonObject_QJsonObject_destruct = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??1QJsonObject@@QAE@XZ");
	static auto QJsonObject_toVariantMap = GetExport<void*(__thiscall*)(void*, void*)>(Qt5Core, "?toVariantMap@QJsonObject@@QBE?AV?$QMap@VQString@@VQVariant@@@@XZ");
	if (!QJsonObject_QJsonObject || !QJsonObject_QJsonObject_destruct || !QJsonObject_toVariantMap)
		MessageBoxA(nullptr, "Error in findUserMigrationTimePeriod: one of the functions could not have been resolved, we will crash", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	char json[12] = { 0 };
	QJsonObject_QJsonObject(&json);
	QJsonObject_toVariantMap(&json, a1);
	QJsonObject_QJsonObject_destruct(&json);
	return a1;
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
}

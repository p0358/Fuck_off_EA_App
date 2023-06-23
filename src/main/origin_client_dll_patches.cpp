// Patches for OriginClient.dll

#include "pch.hpp"
#include "main.hpp"

#if 0
// Origin::Client::JsInterface::ClientSettingsProxy::qt_static_metacall
void(*qt_static_metacall_org)(void*, int, int, int);
void __cdecl qt_static_metacall_hook(void* thisptr, int a2, int a3, int a4)
{
	if (!a2 && a3 == 8) // TODO: is this number guaranteed to stay this way?
	{
		// call to findUserMigrationTimePeriod
		MessageBoxA(0, "call to findUserMigrationTimePeriod", "", 0);
		return;
	}
	qt_static_metacall_org(thisptr, a2, a3, a4);
}
#endif
// Origin::Client::JsInterface::ClientSettingsProxy::findUserMigrationTimePeriod
void*(__stdcall *findUserMigrationTimePeriod_org)(void*);
void* __stdcall findUserMigrationTimePeriod_hook(void* a1)
{
	// just return an empty Qt JSON object
	char json[12] = { 0 };
	static auto QJsonObject_QJsonObject = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??0QJsonObject@@QAE@XZ");
	static auto QJsonObject_QJsonObject_destruct = GetExport<void*(__thiscall*)(void*)>(Qt5Core, "??1QJsonObject@@QAE@XZ");
	static auto QJsonObject_toVariantMap = GetExport<void*(__thiscall*)(void*, void*)>(Qt5Core, "?toVariantMap@QJsonObject@@QBE?AV?$QMap@VQString@@VQVariant@@@@XZ");
	MessageBoxA(0, std::to_string(uintptr_t(QJsonObject_QJsonObject)).c_str(), "QJsonObject_QJsonObject", 0);
	MessageBoxA(0, std::to_string(uintptr_t(QJsonObject_QJsonObject_destruct)).c_str(), "QJsonObject_QJsonObject_destruct", 0);
	MessageBoxA(0, std::to_string(uintptr_t(QJsonObject_toVariantMap)).c_str(), "QJsonObject_toVariantMap", 0);
	QJsonObject_QJsonObject(&json);
	QJsonObject_toVariantMap(&json, a1);
	QJsonObject_QJsonObject_destruct(&json);
	return a1;
}

void DoOriginClientDllPatches()
{
	{
		// Spoof Origin version to '10.5.122.52971' if we're older
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
					//MessageBoxA(0, "ORIGIN VERSION PATCHED", "PATCHED", 0);
				}
			}
			catch (const std::exception& e)
			{
				MessageBoxA(nullptr, ("Error checking Origin version: " + std::string(e.what()) + "\nVersion: " + originClientVersionStr).c_str(), ERROR_MSGBOX_CAPTION, MB_ICONERROR);
			}
		}
	}

	//CreateHookNamed("OriginClient", "?qt_static_metacall@ClientSettingsProxy@JsInterface@Client@Origin@@CAXPAVQObject@@W4Call@QMetaObject@@HPAPAX@Z", qt_static_metacall_hook, reinterpret_cast<LPVOID*>(&qt_static_metacall_org));
	//CMemory(GetExport<uintptr_t>(OriginClient, "?findUserMigrationTimePeriod@ClientSettingsProxy@JsInterface@Client@Origin@@QAE?AV?$QMap@VQString@@VQVariant@@@@XZ"))
	//	.Patch({ 0x8B, 0x45, 0x08, 0xC3 });
	CreateHookNamed("OriginClient", "?findUserMigrationTimePeriod@ClientSettingsProxy@JsInterface@Client@Origin@@QAE?AV?$QMap@VQString@@VQVariant@@@@XZ", findUserMigrationTimePeriod_hook, reinterpret_cast<LPVOID*>(&findUserMigrationTimePeriod_org));
}

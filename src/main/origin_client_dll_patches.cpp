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
			"\nQJsonObject_QJsonObjectl: " + std::to_string(uintptr_t(QJsonObject_QJsonObject))
			+ "\nQJsonObject_QJsonObject_destruct: " + std::to_string(uintptr_t(QJsonObject_QJsonObject_destruct))
			+ "\nQJsonObject_toVariantMap: " + std::to_string(uintptr_t(QJsonObject_toVariantMap))
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
	static auto SETTING_MigrationDisabled = GetExport<void*>(OriginClient, "?SETTING_MigrationDisabled@Services@Origin@@3VSetting@12@A"); // non-const symbol
	if (!SETTING_MigrationDisabled)
		SETTING_MigrationDisabled = GetExport<void*>(OriginClient, "?SETTING_MigrationDisabled@Services@Origin@@3VSetting@12@B"); // const symbol

	static bool didWarnAboutMissingAlready = false;
	if (!didWarnAboutMissingAlready && (!QVariant_QVariant_from_bool || !SETTING_MigrationDisabled)) [[unlikely]]
	{
		didWarnAboutMissingAlready = true;
		MessageBoxA(nullptr, ("Error in Origin::Services::readSetting: one of the exports could not have been resolved, we may crash\n"
			"\nQVariant_QVariant_from_bool: " + std::to_string(uintptr_t(QVariant_QVariant_from_bool))
			+ "\nSETTING_MigrationDisabled: " + std::to_string(uintptr_t(SETTING_MigrationDisabled))
			).c_str(),
			ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}
	
	if (setting == SETTING_MigrationDisabled)
	{
		// override to true
		QVariant_QVariant_from_bool(out_qv, true); // caller will destruct this
		return out_qv;
	}

	return readSetting_org(out_qv, setting, a3, a4);
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

#if 0
// NOTE: This hook is completely broken, but also not needed...
// QString::compare
bool(__thiscall* qstring_compare_org)(void*, void*, int);
bool __fastcall qstring_compare_hook(void* thisptr /*ecx*/, void* /*edx*/, void* comparee, int a3)
{
	if (a3 == 1)
	{
		static auto QString_startsWith = GetExport<void* (__thiscall*)(void*, void*, int)>(Qt5Core, "?startsWith@QString@@QBE_NABV1@W4CaseSensitivity@Qt@@@Z");
		static auto QString_QString = GetExport<void* (__thiscall*)(void*, const char*)>(Qt5Core, "??0QString@@QAE@PBD@Z");
		static int ten = 0;
		static int twelve = 0;
		if (ten == 0) QString_QString(&twelve, "10.5.");
		if (twelve == 0) QString_QString(&twelve, "12.");
		if (QString_startsWith(comparee, &twelve, 0) && QString_startsWith(thisptr, &ten, 0))
			return 0;
	}
	return qstring_compare_org(thisptr, comparee, a3);
}
#endif

#if 0
// Origin::Client::LoginViewController::init
void(__thiscall* loginViewController_init_org)(void*, DWORD*, /* Origin::Client::AuthenticationJsHelper* */ void*, int);
void __fastcall loginViewController_init_hook(void* thisptr /*ecx*/, void* /*edx*/, DWORD* a2, void* a3, int a4)
{
	loginViewController_init_org(thisptr, a2, a3, a4);
	////*(bool*)((uintptr_t)thisptr + 166) = true; // make it think we've loaded all cookies already
	//*(bool*)((uintptr_t)thisptr + 167) = true; // make it think Trusted Clock is already initialized
	loginViewController_init_org(thisptr, a2, a3, a4);

	// this does NOT seem to work here!
	// Origin::Client::LoginViewController::killSplashScreenAndShowLoginWindow
	//static auto killSplashScreenAndShowLoginWindow = GetExport<void(__thiscall*)(void*)>(OriginClient, "?killSplashScreenAndShowLoginWindow@LoginViewController@Client@Origin@@AAEXXZ");
	//killSplashScreenAndShowLoginWindow(thisptr);
}

// Origin::Services::TrustedClock::isInitialized
bool(__thiscall* trustedClock_isInitialized_org)(void*);
bool __fastcall trustedClock_isInitialized_hook(void* thisptr /*ecx*/)
{
	return true;
}

// Origin::Client::LoginViewController::loadLoginPage
void(__thiscall* loginViewController_loadLoginPage_org)(void*, int);
void __fastcall loginViewController_loadLoginPage_hook(void* thisptr /*ecx*/, void* /*edx*/, int a2)
{
	loginViewController_loadLoginPage_org(thisptr, a2);

	static auto killSplashScreenAndShowLoginWindow = GetExport<void(__thiscall*)(void*)>(OriginClient, "?killSplashScreenAndShowLoginWindow@LoginViewController@Client@Origin@@AAEXXZ");
	
	static bool didWarnAboutMissingAlready = false;
	if (!didWarnAboutMissingAlready && (!killSplashScreenAndShowLoginWindow)) [[unlikely]]
	{
		didWarnAboutMissingAlready = true;
		MessageBoxA(nullptr, "Error in Origin::Client::LoginViewController::loadLoginPage: killSplashScreenAndShowLoginWindow was not found, your Origin client will probably be stuck on splash screen\n"
			"\nRight-click Origin icon in your tray and click Open Origin as a workaround.",
			ERROR_MSGBOX_CAPTION, MB_ICONERROR);
	}

	// well it seems someone forgot to call this func in a reasonable place like here
	if (killSplashScreenAndShowLoginWindow)
		killSplashScreenAndShowLoginWindow(thisptr);
}
#endif

// QWebEngineCookieStore::setCookie
void(__thiscall* QWebEngineCookieStore_setCookie_org)(void*, /* QNetworkCookie */ void*, /* QUrl */ void*);
void __fastcall QWebEngineCookieStore_setCookie_hook(void* thisptr /*ecx*/, void* /*edx*/, void* cookie, void* url)
{
	static auto QString_destroy = GetExport<void(__thiscall*)(void*)>(Qt5Core, "??1QString@@QAE@XZ");
	static auto QString_QString_from_cchar = GetExport<void* (__thiscall*)(void*, const char*)>(Qt5Core, "??0QString@@QAE@PBD@Z");
	static auto QString_toStdString = GetExport<void* (__thiscall*)(void*, std::string*)>(Qt5Core, "?toStdString@QString@@QBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
	static auto QByteArray_toStdString = GetExport<void* (__thiscall*)(void*, std::string*)>(Qt5Core, "?toStdString@QByteArray@@QBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
	static auto QByteArray_destroy = GetExport<void* (__thiscall*)(void*)>(Qt5Core, "??1QByteArray@@QAE@XZ");
	static auto QNetworkCookie_setDomain = GetExport<void(__thiscall*)(void*, void*)>(Qt5Network, "?setDomain@QNetworkCookie@@QAEXABVQString@@@Z");
	static auto QNetworkCookie_domain = GetExport<void(__thiscall*)(void*, void*)>(Qt5Network, "?domain@QNetworkCookie@@QBE?AVQString@@XZ");
	static auto QNetworkCookie_name = GetExport<void(__thiscall*)(void*, void*)>(Qt5Network, "?name@QNetworkCookie@@QBE?AVQByteArray@@XZ");

	OutputDebugStringA("[QWebEngineCookieStore::setCookie] Howdy");

	if (QString_destroy && QString_QString_from_cchar && QString_toStdString && QByteArray_toStdString && QByteArray_destroy && QNetworkCookie_setDomain && QNetworkCookie_domain)
	{
		char qs_cookie_domain[4] = { 0 };
		char qba_cookie_name[4] = { 0 };
		std::string cookie_domain;
		std::string cookie_name;

		QNetworkCookie_domain(cookie, qs_cookie_domain);
		QString_toStdString(qs_cookie_domain, &cookie_domain);
		
		QNetworkCookie_name(cookie, qba_cookie_name);
		QByteArray_toStdString(qba_cookie_name, &cookie_name);

		OutputDebugStringA(("[QWebEngineCookieStore::setCookie] name=" + cookie_name + " domain=" + cookie_domain).c_str());
		//__debugbreak();

		if (cookie_name == "remid" && cookie_domain == "")
		{
			OutputDebugStringA("[QWebEngineCookieStore::setCookie] Fixing domain name to accounts.ea.com for cookie remid");
			char qs_cookie_domain_new[4] = { 0 };
			QString_QString_from_cchar(qs_cookie_domain_new, "accounts.ea.com");
			QNetworkCookie_setDomain(cookie, qs_cookie_domain_new);
			QString_destroy(qs_cookie_domain_new);
		}

		QString_destroy(qs_cookie_domain);
		QByteArray_destroy(qba_cookie_name);
	}
	else
	{
		static bool didWarnAboutMissingAlready = false;
		if (!didWarnAboutMissingAlready) [[unlikely]]
		{
			didWarnAboutMissingAlready = true;
			MessageBoxA(nullptr, "Error in QWebEngineCookieStore::setCookie: one of the exports could not have been resolved!", ERROR_MSGBOX_CAPTION, MB_ICONERROR);
		}
	}

	QWebEngineCookieStore_setCookie_org(thisptr, cookie, url);
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
	//CreateHookNamed("Qt5Core", "?compare@QString@@QBEHABV1@W4CaseSensitivity@Qt@@@Z", qstring_compare_hook, reinterpret_cast<LPVOID*>(&qstring_compare_org));

	// Patches around broken login screen due to missing cookie (hopefully it's a temporary thing?)
	///CreateHookNamed("OriginClient", "?init@LoginViewController@Client@Origin@@QAEXABW4StartupState@@W4IconType@OriginBanner@UIToolkit@3@ABVQString@@@Z", loginViewController_init_hook, reinterpret_cast<LPVOID*>(&loginViewController_init_org));
	//CreateHookNamed("OriginClient", "?isInitialized@TrustedClock@Services@Origin@@QBE_NXZ", trustedClock_isInitialized_hook, reinterpret_cast<LPVOID*>(&trustedClock_isInitialized_org));
	///CreateHookNamed("OriginClient", "?loadLoginPage@LoginViewController@Client@Origin@@IAEX_N@Z", loginViewController_loadLoginPage_hook, reinterpret_cast<LPVOID*>(&loginViewController_loadLoginPage_org));

	// 
	//CreateHookNamed("Qt5WebEngineCore", "?setCookie@QWebEngineCookieStore@@QAEXABVQNetworkCookie@@ABVQUrl@@@Z", QWebEngineCookieStore_setCookie_hook, reinterpret_cast<LPVOID*>(&QWebEngineCookieStore_setCookie_org));
}

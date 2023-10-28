#pragma once

#define ERROR_MSGBOX_CAPTION "\"Fuck off EA App\"'s fatal error"
#define ERROR_MSGBOX_CAPTION_L L"\"Fuck off EA App\"'s fatal error"

#include "function_hooking.hpp"

extern CModule OriginExe;
extern CModule OriginClientServiceExe;
extern CModule OriginClient;
extern CModule EALinkExe;

extern CMemory OriginExeAdr;
extern CMemory OriginClientServiceExeAdr;
extern CMemory OriginClientAdr;
extern CMemory EALinkExeAdr;

extern CModule Qt5Core;

extern void DoOriginExePatches();
extern void DoOriginClientServiceExePatches();
extern void DoOriginClientDllPatches();
extern void DoEALinkExePatches();

template<typename T>
inline T GetExport(const CModule& module, const char* exportName)
{
	return reinterpret_cast<T>(GetProcAddress(HMODULE(module.GetModuleBase()), exportName));
}


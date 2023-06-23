#pragma once

#include "function_hooking.hpp"

extern CModule OriginExe;
extern CModule OriginClientServiceExe;
extern CModule OriginClient;

extern CMemory OriginExeAdr;
extern CMemory OriginClientServiceExeAdr;
extern CMemory OriginClientAdr;

extern void DoOriginExePatches();
extern void DoOriginClientServiceExePatches();
extern void DoOriginClientDllPatches();

#define ERROR_MSGBOX_CAPTION "\"Fuck off EA App\"'s fatal error"
#define ERROR_MSGBOX_CAPTION_L L"\"Fuck off EA App\"'s fatal error"


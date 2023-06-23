#include "pch.hpp"

extern "C"
{
    FARPROC p_GetFileVersionInfoA = 0;
    FARPROC p_GetFileVersionInfoExA = 0;
    FARPROC p_GetFileVersionInfoExW = 0;
    FARPROC p_GetFileVersionInfoW = 0;
    FARPROC p_GetFileVersionInfoSizeA = 0;
    FARPROC p_GetFileVersionInfoSizeExA = 0;
    FARPROC p_GetFileVersionInfoSizeExW = 0;
    FARPROC p_GetFileVersionInfoSizeW = 0;
    FARPROC p_VerFindFileA = 0;
    FARPROC p_VerFindFileW = 0;
    FARPROC p_VerInstallFileA = 0;
    FARPROC p_VerInstallFileW = 0;
    FARPROC p_VerQueryValueA = 0;
    FARPROC p_VerQueryValueW = 0;
}

extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoA() { __asm jmp[p_GetFileVersionInfoA] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoExA() { __asm jmp[p_GetFileVersionInfoExA] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoExW() { __asm jmp[p_GetFileVersionInfoExW] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoW() { __asm jmp[p_GetFileVersionInfoW] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoSizeA() { __asm jmp[p_GetFileVersionInfoSizeA] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoSizeExA() { __asm jmp[p_GetFileVersionInfoSizeExA] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoSizeExW() { __asm jmp[p_GetFileVersionInfoSizeExW] }
extern "C" __declspec(dllexport) void PROXY_GetFileVersionInfoSizeW() { __asm jmp[p_GetFileVersionInfoSizeW] }
extern "C" __declspec(dllexport) void PROXY_VerFindFileA() { __asm jmp[p_VerFindFileA] }
extern "C" __declspec(dllexport) void PROXY_VerFindFileW() { __asm jmp[p_VerFindFileW] }
extern "C" __declspec(dllexport) void PROXY_VerInstallFileA() { __asm jmp[p_VerInstallFileA] }
extern "C" __declspec(dllexport) void PROXY_VerInstallFileW() { __asm jmp[p_VerInstallFileW] }
extern "C" __declspec(dllexport) void PROXY_VerQueryValueA() { __asm jmp[p_VerQueryValueA] }
extern "C" __declspec(dllexport) void PROXY_VerQueryValueW() { __asm jmp[p_VerQueryValueW] }

void ProxySetup(HINSTANCE hL)
{
    p_GetFileVersionInfoA = GetProcAddress(hL, "GetFileVersionInfoA");
    p_GetFileVersionInfoExA = GetProcAddress(hL, "GetFileVersionInfoExA");
    p_GetFileVersionInfoExW = GetProcAddress(hL, "GetFileVersionInfoExW");
    p_GetFileVersionInfoW = GetProcAddress(hL, "GetFileVersionInfoW");
    p_GetFileVersionInfoSizeA = GetProcAddress(hL, "GetFileVersionInfoSizeA");
    p_GetFileVersionInfoSizeExA = GetProcAddress(hL, "GetFileVersionInfoSizeExA");
    p_GetFileVersionInfoSizeExW = GetProcAddress(hL, "GetFileVersionInfoSizeExW");
    p_GetFileVersionInfoSizeW = GetProcAddress(hL, "GetFileVersionInfoSizeW");
    p_VerFindFileA = GetProcAddress(hL, "VerFindFileA");
    p_VerFindFileW = GetProcAddress(hL, "VerFindFileW");
    p_VerInstallFileA = GetProcAddress(hL, "VerInstallFileA");
    p_VerInstallFileW = GetProcAddress(hL, "VerInstallFileW");
    p_VerQueryValueA = GetProcAddress(hL, "VerQueryValueA");
    p_VerQueryValueW = GetProcAddress(hL, "VerQueryValueW");
}

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <psapi.h>
#include <intrin.h>

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <regex>
#include <filesystem>
namespace fs = std::filesystem;

#include <MinHook.h>

#include <silver-bun/memaddr.h>
#include <silver-bun/module.h>
#include <silver-bun/utils.h>

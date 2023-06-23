os.chdir(_MAIN_SCRIPT_DIR)

project "Main"
kind "StaticLib"
language "C++"

targetname "fuck_off_ea_app"

pchheader "pch.hpp"
pchsource "./src/main/pch.cpp"
forceincludes {"pch.hpp"}

files {"./src/main/**.hpp", "./src/main/**.cpp", "./src/main/**.asm"}
includedirs {"./src/main"}

dependencies.imports()

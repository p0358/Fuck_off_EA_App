os.chdir(_MAIN_SCRIPT_DIR)

project "Loader"
kind "SharedLib"
language "C++"

targetname "version"

pchheader "pch.hpp"
pchsource "./src/loader/pch.cpp"
forceincludes {"pch.hpp"}

files {"./src/loader/**.hpp", "./src/loader/**.cpp", "./src/loader/**.asm", "./src/loader/**.def"}
includedirs {"./src/loader"}

links {"main"}

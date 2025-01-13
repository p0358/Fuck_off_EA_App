os.chdir(_MAIN_SCRIPT_DIR)

project "Loader"
kind "SharedLib"
language "C++"

targetname "version"

pchheader "pch.hpp"
pchsource "./src/loader/pch.cpp"
forceincludes {"pch.hpp"}

files {"./src/loader/**.hpp", "./src/loader/**.cpp", "./src/loader/**.asm", "./src/loader/**.def", "./src/loader/resource/**.*"}
includedirs {"./src/loader", "%{prj.location}/src"}

prebuildcommands {"pushd %{_MAIN_SCRIPT_DIR}", "premake\\premake5 generate-buildinfo", "popd"}

links {"main"}

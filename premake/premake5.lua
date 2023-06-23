-- Note: loaded scripts will change the current directory due to the code below:
-- https://github.com/premake/premake-core/blob/5b6289407d24fd48d5d8066119295a9c89947c4c/src/host/lua_auxlib.c#L153
-- for this reason we have to change directory manually back at the beginning on each script to avoid major pain!
os.chdir(_MAIN_SCRIPT_DIR)

require("utils")

dependencies = {
	basePath = path.join(_MAIN_SCRIPT_DIR, "thirdparty"),
	depsScriptsPath = path.join(_MAIN_SCRIPT_DIR, "premake/deps"),
}

deps = {} -- actual projects are here

function dependencies.load()
	local dir = path.join(dependencies.depsScriptsPath, "*.lua")
	local deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		local dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for name, proj in pairs(deps) do
		proj.import()
	end
end

function dependencies.projects()
	for name, proj in pairs(deps) do
		proj.project()
	end
end

dependencies.load()

workspace "Fuck_off_EA_App"
startproject "Fuck_off_EA_App"
location "./build"
objdir "%{wks.location}/obj"
targetdir "%{wks.location}/bin/%{cfg.platform}-%{cfg.buildcfg}"
libdirs {"%{wks.location}/bin/%{cfg.platform}-%{cfg.buildcfg}"}

solutionitems {
	"./README.md",
	"./.github/workflows/build.yml",
	["Premake"] = {
		"./premake/*.lua",
		["Deps"] = {
			"./premake/deps/**.lua"
		}
	},
}

configurations {"Release"}

language "C++"
cppdialect "C++20"

architecture "x86"
platforms "Win32"

systemversion "latest"
symbols "On"
staticruntime "On"
editandcontinue "Off"
--warnings "Extra"
characterset "ASCII"

flags {"MultiProcessorCompile", "No64BitChecks"}

defines {"_WINDOWS", "WIN32", "NOMINMAX"}

buildoptions {"/utf-8"}
linkoptions {}

filter "configurations:Release"
	optimize "Speed"
	buildoptions {}
	linkoptions {}
	defines {"NDEBUG"}
	flags {"LinkTimeOptimization"}
filter {}

filter "configurations:Debug"
	optimize "Debug"
	defines {"DEBUG", "_DEBUG"}
filter {}

includedirs {"./thirdparty"}
libdirs {"./thirdparty"}

require("loader")
require("main")

group "Dependencies"
dependencies.projects()

silverbun = {
	source = path.join(dependencies.basePath, "silver-bun"),
}

function silverbun.import()
    links {"silverbun"}
    --defines {"USE_PRECOMPILED_HEADERS"}

	silverbun.includes()
end

function silverbun.includes()
	includedirs {silverbun.source}
end

function silverbun.project()
    project "silverbun"
    language "C++"

    --silverbun.includes()
    includedirs {
		path.join(silverbun.source, "silver-bun"),
	}

    files {
        path.join(silverbun.source, "silver-bun/**.cpp"),
        path.join(silverbun.source, "silver-bun/**.h"),
    }

    warnings "Off"
    kind "StaticLib"
end

deps.silverbun = silverbun

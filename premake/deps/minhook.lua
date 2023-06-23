minhook = {
	source = path.join(dependencies.basePath, "minhook"),
}

function minhook.import()
    links {"minhook"}

	minhook.includes()
end

function minhook.includes()
	includedirs {
		path.join(minhook.source, "include"),
	}
end

function minhook.project()
	project "minhook"
    language "C"

    minhook.includes()

    includedirs {
        path.join(minhook.source, "src"),
    }

    files {
        path.join(minhook.source, "src/**.c"),
    }

    warnings "Off"
    kind "StaticLib"
end

deps.minhook = minhook

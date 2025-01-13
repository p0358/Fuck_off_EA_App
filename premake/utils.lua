os.chdir(_MAIN_SCRIPT_DIR)

gitVersioningCommand = "git describe --tags --dirty --always"
gitCurrentBranchCommand = "git symbolic-ref -q --short HEAD"

require("vstudio")
premake.api.register {
	name = "solutionitems",
	scope = "workspace",
	kind = "table",
}
function recursiveSolutionItemsProject(wks, tbl)
	for _, file in pairs(tbl) do
		if type(_) ~= "number" then

			local name = _
			local tbl = file
			premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "'..name..'", "'..name..'", "{' .. os.uuid("Solution Items:"..wks.name..":"..name) .. '}"')
			premake.push("ProjectSection(SolutionItems) = preProject")

			local children = false

			for _, file in pairs(file) do
				if type(_) == "number" and type(file) == "string" then
					for _, file in ipairs(os.matchfiles(file)) do
						file = path.rebase(file, ".", wks.location)
						premake.w(file.." = "..file)
					end
				end

				if type(_) ~= "number" then
					children = true
				end
			end

			premake.pop("EndProjectSection")
			premake.pop("EndProject")

			if children then
				recursiveSolutionItemsProject(wks, tbl)
			end
		end
	end
end
premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if wks.solutionitems and #wks.solutionitems > 0 then
		-- root level
		premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "Solution Items", "Solution Items", "{' .. os.uuid("Solution Items:"..wks.name) .. '}"')
		premake.push("ProjectSection(SolutionItems) = preProject")
		for _, file in pairs(wks.solutionitems) do
			if type(_) == "number" and type(file) == "string" then
				for _, file in ipairs(os.matchfiles(file)) do
					file = path.rebase(file, ".", wks.location)
					premake.w(file.." = "..file)
				end
			end
		end
		premake.pop("EndProjectSection")
		premake.pop("EndProject")
		-- subprojects
		recursiveSolutionItemsProject(wks, wks.solutionitems)
	end
	base(wks)
end)
function recursiveSolutionItemsNestedProjectLines(wks, parent, tbl)
    local parent_uuid
    if parent == "Solution Items" then
        parent_uuid = os.uuid("Solution Items:"..wks.name)
    else
        parent_uuid = os.uuid("Solution Items:"..wks.name..":"..parent)
    end

	for _, file in pairs(tbl) do
		if type(_) ~= "number" then
			local name = _
			local new_tbl = file
			project_uuid = os.uuid("Solution Items:"..wks.name..":"..name)
			premake.w("{%s} = {%s}", project_uuid, parent_uuid)
            --print("child:", name, "parent:", parent)
			recursiveSolutionItemsNestedProjectLines(wks, name, new_tbl)
		end
	end
end
premake.override(premake.vstudio.sln2005, "nestedProjects", function(base, wks)
	premake.push("GlobalSection(NestedProjects) = preSolution")
	-- base copy-paste START
	local tr = premake.workspace.grouptree(wks)
	if premake.tree.hasbranches(tr) then
		premake.tree.traverse(tr, {
			onnode = function(n)
				if n.parent.uuid then
					premake.w("{%s} = {%s}", (n.project or n).uuid, n.parent.uuid)
				end
			end
		})
	end
	-- base copy-paste END
	-- our START
	parent = "Solution Items"
	recursiveSolutionItemsNestedProjectLines(wks, parent, wks.solutionitems)
	-- our END
	premake.pop("EndGlobalSection")
end)

--premake.override(premake.config, "canLinkIncremental", function(base, cfg)
--    if cfg.kind ~= "StaticLib" and not cfg.flags.NoIncrementalLink and not _OPTIONS["ci-build"] then
--        -- TODO: this is still not gonna work due to /OPT:REF and /OPT:ICF and /LTCG, probably not viable
--        -- we should think if we want to just disable them for non-CI builds outright in the CI-enabled projects
--        -- or create a separate build configuration like Release-Fast or something
--        return true -- warning LNK4075: ignoring '/INCREMENTAL' due to '/LTCG' specification
--    end
--    return base(cfg)
--end)

-- Quote the given string input as a C string
function cstrquote(value)
	if value == nil then
		return "\"\""
	end
	result = value:gsub("\\", "\\\\")
	result = result:gsub("\"", "\\\"")
	result = result:gsub("\n", "\\n")
	result = result:gsub("\t", "\\t")
	result = result:gsub("\r", "\\r")
	result = result:gsub("\a", "\\a")
	result = result:gsub("\b", "\\b")
	result = "\"" .. result .. "\""
	return result
end

-- Converts tags in "vX.X.X" format and given revision number Y to an array of numbers {X,X,X,Y}.
-- In the case where the format does not work fall back to padding with zeroes and just ending with the revision number.
-- partscount can be either 3 or 4.
function vertonumarr(value, vernumber, partscount)
	vernum = {}
	for num in string.gmatch(value or "", "%d+") do
		if #vernum < 3 then
			table.insert(vernum, tonumber(num))
		end
	end
	while #vernum < 3 do
		table.insert(vernum, 0)
	end
	if #vernum < partscount then
		table.insert(vernum, tonumber(vernumber))
	end
	return vernum
end

function io.writefile(filename, content)
	-- do not write if contents haven't changed
	if content == io.readfile(filename) then
		return true
	end

	local file = io.open(filename, "w+b")
	if file then
		file:write(content)
		file:close()
		return true
	end
end

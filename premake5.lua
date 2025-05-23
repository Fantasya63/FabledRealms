 workspace "FabledRealms"
	architecture "x64"
	startproject "FabledRealms"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to sol dir
IncludeDir = {}
IncludeDir["GLFW"] = "FabledRealms/vendor/GLFW/include"
IncludeDir["Glad"] = "FabledRealms/vendor/Glad/include"
IncludeDir["glm"] = "FabledRealms/vendor/glm"
IncludeDir["stb"] = "FabledRealms/vendor/stb"
IncludeDir["irrKlang"] = "FabledRealms/vendor/irrKlang/include"
IncludeDir["FastNoiseLite"] = "FabledRealms/vendor/FastNoiseLite"

include "FabledRealms/vendor/GLFW"
include "FabledRealms/vendor/glad"

project "FabledRealms"
	location "FabledRealms"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	binOutputDir = "bin/" .. outputdir .. "/%{prj.name}";
	targetdir (binOutputDir)
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "frpch.h"
	pchsource "FabledRealms/src/frpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl",
		"%{prj.name}/vendor/FastNoiseLite/**.h",

	}

	defines
	{
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.FastNoiseLite}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.irrKlang}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"FabledRealms/vendor/irrKlang/lib/irrKlang.lib",
	}

	local assetsSourcePath = path.translate(path.join(os.getcwd(), "FabledRealms/Assets/"))
	local assetsDestinationPath = path.translate(path.join(os.getcwd(), binOutputDir .. "/Assets"))

	local irrKlangDLLPath = path.translate(path.join(os.getcwd(), "FabledRealms/vendor/irrKlang/lib/dlls/"))
	local irrKlangDLLDestPath = path.translate(path.join(os.getcwd(), binOutputDir))

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FR_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
		}
		
		postbuildcommands {
			"{COPY} \"" .. assetsSourcePath .. "\" \"" .. assetsDestinationPath .. "\"",
			"{COPY} \"" .. irrKlangDLLPath .. "\" \"" .. irrKlangDLLDestPath .. "\"",
		}
 

	filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Dist"
		defines "FR_DIST"
		runtime "Release"
		optimize "on"
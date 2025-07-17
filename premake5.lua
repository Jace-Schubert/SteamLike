workspace "SteamLike"
	architecture "x64"
    startproject "SteamLike"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SteamLike"
	location "SteamLike"
	kind "ConsoleApp"
	staticruntime "Off"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/sqlite/sqlite3.c",
        "%{prj.name}/vendor/sqlite/sqlite3.h"
	}

	includedirs
	{
        "%{prj.name}/vendor/sqlite"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		buildoptions
		{
			"/utf-8"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		buildoptions "/MD"
		optimize "On"
workspace "DuckEngine"
	architecture "x64"
    startproject "DuckEditor"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "DuckEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "DuckEngine/vendor/Glad/include"
IncludeDir["glm"] = "DuckEngine/vendor/glm"
IncludeDir["ImGui"] = "DuckEngine/vendor/imgui"
IncludeDir["assimp"] = "DuckEngine/vendor/assimp/include"
IncludeDir["stb_image"] = "DuckEngine/vendor/stb_image"

group "Dependance"
	include "DuckEngine/vendor/GLFW"
	include "DuckEngine/vendor/Glad"
	include "DuckEngine/vendor/assimp"
	include "DuckEngine/vendor/imgui"

group ""

project "DuckEngine"
	location "DuckEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "depch.h"
    pchsource "DuckEngine/src/depch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"assimp",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

project "DuckEditor"
	location "DuckEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"DuckEngine/src",
		"DuckEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"DuckEngine",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
workspace "ProjectSolution"
	architecture "x64"
    startproject "ProjectSolution"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["OpenGLEngine"] = "../../OpenGLEngine/src"

LibraryDir = {}
LibraryDir["OpenGLEngine"] = "../../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/OpenGLEgine.lib"

project "ProjectSolution"
	location "ProjectSolution"
	kind "StaticLib"
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
		"%{prj.name}/src",
		"%{IncludeDir.OpenGLEngine}"
	}

	links
	{
		"%{LibraryDir.OpenGLEngine}"
	}
	
	flags { "MultiProcessorCompile" }
	optimize "Speed"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
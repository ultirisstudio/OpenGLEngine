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
IncludeDir["glm"] = "../../OpenGLEngine/vendor/glm"
IncludeDir["assimp"] = "../../OpenGLEngine/vendor/assimp/include"

LibraryDir = {}
LibraryDir["OpenGLEngine"] = "../../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/OpenGLEngine"
LibraryDir["GLFW"] = "../../OpenGLEngine/vendor/GLFW/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/GLFW"
LibraryDir["Glad"] = "../../OpenGLEngine/vendor/Glad/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Glad"

project "ProjectSolution"
	location "ProjectSolution"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.OpenGLEngine}"
	}

	libdirs
	{
		"%{LibraryDir.OpenGLEngine}",
		"%{LibraryDir.GLFW}",
		"%{LibraryDir.Glad}"
	}
	
	links
	{
		"OpenGLEngine.lib",
		"GLFW.lib",
		"Glad.lib"
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
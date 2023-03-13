workspace "OpenGLEngine"
	architecture "x64"
    startproject "OpenGLEditor"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "OpenGLEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "OpenGLEngine/vendor/Glad/include"
IncludeDir["glm"] = "OpenGLEngine/vendor/glm"
IncludeDir["ImGui"] = "OpenGLEngine/vendor/imgui"
IncludeDir["assimp"] = "OpenGLEngine/vendor/assimp/include"
IncludeDir["stb_image"] = "OpenGLEngine/vendor/stb_image"
IncludeDir["ImGuizmo"] = "OpenGLEngine/vendor/ImGuizmo"

group "Dependance"
	include "OpenGLEngine/vendor/GLFW"
	include "OpenGLEngine/vendor/Glad"
	include "OpenGLEngine/vendor/assimp"
	include "OpenGLEngine/vendor/imgui"
	include "OpenGLEngine/vendor/ImGuizmo"

group ""

project "OpenGLEngine"
	location "OpenGLEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "depch.h"
    pchsource "OpenGLEngine/src/depch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"Glad",
		"assimp",
		"ImGui",
		"ImGuizmo",
		"opengl32.lib"
	}
	
	flags { "MultiProcessorCompile" }

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

project "OpenGLEditor"
	location "OpenGLEditor"
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
		"OpenGLEngine/src",
		"OpenGLEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"OpenGLEngine"
	}
	
	flags { "MultiProcessorCompile" }
	
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
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
IncludeDir["GLFW"] = "../../OpenGLEngine/vendor/GLFW/include"
IncludeDir["glm"] = "../../OpenGLEngine/vendor/glm"
IncludeDir["assimp"] = "../../OpenGLEngine/vendor/assimp/include"
IncludeDir["glm"] = "../../OpenGLEngine/vendor/glm"
IncludeDir["ImGui"] = "../../OpenGLEngine/vendor/imgui"
IncludeDir["assimp"] = "../../OpenGLEngine/vendor/assimp/include"
IncludeDir["stb_image"] = "../../OpenGLEngine/vendor/stb_image"
IncludeDir["yaml_cpp"] = "../../OpenGLEngine/vendor/yaml_cpp/include"
IncludeDir["ImGuizmo"] = "../../OpenGLEngine/vendor/ImGuizmo"

LibraryDir = {}
LibraryDir["OpenGLEngine"] = "../../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/OpenGLEngine"
LibraryDir["GLFW"] = "../../OpenGLEngine/vendor/GLFW/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/GLFW"
LibraryDir["Glad"] = "../../OpenGLEngine/vendor/Glad/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Glad"
LibraryDir["assimp"] = "../../OpenGLEngine/vendor/assimp/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/assimp"
LibraryDir["yaml_cpp"] = "../../OpenGLEngine/vendor/yaml_cpp/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/yaml-cpp"
LibraryDir["ImGui"] = "../../OpenGLEngine/vendor/imgui/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/ImGui"
LibraryDir["ImGuizmo"] = "../../OpenGLEngine/vendor/ImGuizmo/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/ImGuizmo"


project "ProjectSolution"
	location "ProjectSolution"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.OpenGLEngine}"
	}

	libdirs
	{
		"ProjectSolution/src",
		"../../OpenGLEngine/vendor",
		"%{LibraryDir.OpenGLEngine}",
		"%{LibraryDir.GLFW}",
		"%{LibraryDir.Glad}",
		"%{LibraryDir.assimp}",
		"%{LibraryDir.yaml_cpp}",
		"%{LibraryDir.ImGui}",
		"%{LibraryDir.ImGuizmo}",
	}
	
	links
	{
		"OpenGLEngine.lib",
		"GLFW.lib",
		"Glad.lib",
		"assimp.lib",
		"yaml-cpp.lib",
		"ImGui.lib",
		"ImGuizmo.lib"
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
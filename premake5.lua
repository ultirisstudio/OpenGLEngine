include "Dependencies.lua"

workspace "QuasarEngine"
	architecture "x86_64"
    startproject "QuasarEngine-Editor"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependance"
	include "vendor/GLFW"
	include "vendor/Glad"
	include "vendor/assimp"
	include "vendor/ImGui"
	include "vendor/ImGuizmo"
	include "vendor/yaml_cpp"
	include "vendor/tinyfiledialogs"
	include "vendor/rp3d"
	include "vendor/mbedtls"
	include "vendor/zlib"
	include "vendor/lua"

group ""

group "Core"
	include "QuasarEngine-Core"
group ""

group "Scripting"
	include "QuasarEngine-ScriptCore"
group ""

group "Editor"
	include "QuasarEngine-Editor"
group ""
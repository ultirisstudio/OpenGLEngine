include "Dependencies.lua"

workspace "OpenGLEngine"
	architecture "x86_64"
    startproject "OpenGLEditor"

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
	include "OpenGLEngine/vendor/GLFW"
	include "OpenGLEngine/vendor/Glad"
	include "OpenGLEngine/vendor/assimp"
	include "OpenGLEngine/vendor/imgui"
	include "OpenGLEngine/vendor/ImGuizmo"
	include "OpenGLEngine/vendor/yaml_cpp"
	include "OpenGLEngine/vendor/tinyfiledialogs"
	include "OpenGLEngine/vendor/rp3d"
	include "OpenGLEngine/vendor/mbedtls"

group ""

group "Core"
	include "OpenGLEngine"
	include "OpenGLEngine-ScriptCore"
group ""

group "Editor"
	include "OpenGLEditor"
group ""

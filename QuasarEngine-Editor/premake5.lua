project "QuasarEngine-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.QuasarEngineCore}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.tinyfiledialogs}",
		"%{IncludeDir.reactphysics3d}",
		"%{IncludeDir.mbedtls}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.reactphysics3d}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"QuasarEngine-Core",
		"GLFW",
		"tinyfiledialogs",
		"yaml-cpp",
		"mbedtls",
		"zlib",
		"ImGui",
		"ImGuizmo",
		"reactphysics3d",
		"assimp",
		"Glad"
	}
	
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

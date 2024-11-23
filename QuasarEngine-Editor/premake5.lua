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
		"%{IncludeDir.zlib}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.tinyfiledialogs}",
		"%{IncludeDir.reactphysics3d}",
		"%{IncludeDir.mbedtls}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"QuasarEngine-Core",
		"tinyfiledialogs",
		"yaml-cpp",
		"mbedtls",
		"zlib",
		"reactphysics3d",
		"Glad",
		"ImGui",
		"ImGuizmo",
		"GLFW",
		"assimp"
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

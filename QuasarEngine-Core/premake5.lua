project "QuasarEngine-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "qepch.h"
    pchsource "src/qepch.cpp"

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
		"../vendor/stb_image/stb_image.h",
		"../vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.reactphysics3d}",
		"%{IncludeDir.mbedtls}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.tinygltf}",
		"%{IncludeDir.VulkanSDK}"
	}
	
	libdirs
	{
		"%{LibraryDir.VulkanSDK}"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	links
	{
		"GLFW",
		"Glad",
		"assimp",
		"ImGui",
		"ImGuizmo",
		"reactphysics3d",
		"mbedtls",
		"opengl32.lib",
		
		"%{Library.Vulkan}" --,
		-- "%{Library.VulkanUtils}"
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
project "DuckShading"
    kind "StaticLib"
	language "C++"
	cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines {
		"SG_STATIC"
	}
	

    includedirs {
        "include"
    }

    files {
        "include/**.hpp",
        "src/**.cpp"
    }
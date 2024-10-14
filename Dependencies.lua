IncludeDir = {}
IncludeDir["QuasarEngineCore"] = "%{wks.location}/QuasarEngine-Core/src"
IncludeDir["GLFW"] = "%{wks.location}/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/vendor/glm"
IncludeDir["filewatch"] = "%{wks.location}/vendor/filewatch"
IncludeDir["ImGui"] = "%{wks.location}/vendor/ImGui"
IncludeDir["assimp"] = "%{wks.location}/vendor/assimp/include"
IncludeDir["stb_image"] = "%{wks.location}/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/vendor/yaml_cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/vendor/ImGuizmo"
IncludeDir["tinyfiledialogs"] = "%{wks.location}/vendor/tinyfiledialogs/include"
IncludeDir["mono"] = "%{wks.location}/vendor/mono/include"
IncludeDir["reactphysics3d"] = "%{wks.location}/vendor/rp3d/include"
IncludeDir["mbedtls"] = "%{wks.location}/vendor/mbedtls/include"
IncludeDir["zlib"] = "%{wks.location}/vendor/zlib/include"
IncludeDir["entt"] = "%{wks.location}/vendor/entt"
IncludeDir["lua"] = "%{wks.location}/vendor/lua/include"
IncludeDir["sol"] = "%{wks.location}/vendor/sol/include"

LibraryDir = {}
LibraryDir["Mono"] = "%{wks.location}/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.Mono}/mono-2.0-sgen.lib"

-- windows

Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"
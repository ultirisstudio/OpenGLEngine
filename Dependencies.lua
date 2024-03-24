IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/OpenGLEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/OpenGLEngine/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/OpenGLEngine/vendor/glm"
IncludeDir["filewatch"] = "%{wks.location}/OpenGLEngine/vendor/filewatch"
IncludeDir["ImGui"] = "%{wks.location}/OpenGLEngine/vendor/imgui"
IncludeDir["assimp"] = "%{wks.location}/OpenGLEngine/vendor/assimp/include"
IncludeDir["stb_image"] = "%{wks.location}/OpenGLEngine/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/OpenGLEngine/vendor/yaml_cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/OpenGLEngine/vendor/ImGuizmo"
IncludeDir["tinyfiledialogs"] = "%{wks.location}/OpenGLEngine/vendor/tinyfiledialogs/include"
IncludeDir["mono"] = "%{wks.location}/OpenGLEngine/vendor/mono/include"
IncludeDir["reactphysics3d"] = "%{wks.location}/OpenGLEngine/vendor/rp3d/include"
IncludeDir["mbedtls"] = "%{wks.location}/OpenGLEngine/vendor/mbedtls/include"
IncludeDir["zlib"] = "%{wks.location}/OpenGLEngine/vendor/zlib/include"

LibraryDir = {}
LibraryDir["Mono"] = "%{wks.location}/OpenGLEngine/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.Mono}/mono-2.0-sgen.lib"

-- windows

Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"
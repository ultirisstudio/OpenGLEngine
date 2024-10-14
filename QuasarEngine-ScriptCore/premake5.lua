project "QuasarEngine-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"
	csversion ("10.0")
	
	targetdir ("%{wks.location}/QuasarEngine-Editor/Assets/Scripts")
	objdir ("%{wks.location}/QuasarEngine-Editor/Assets/Scripts/Intermediates")
	
	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
workspace "CaveCrawler"
	architecture "x86"
	startproject "CaveCrawler"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CaveCrawler"
	location "CaveCrawler"
	language "c++"
	kind "ConsoleApp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"CaveCrawler/src"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
	postbuildcommands 
	{
		("{COPY} %{prj.location}/Assets %{cfg.targetdir}/Assets")
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		buildoptions "/MDd"	
		
		links 
		{ 
			"SDL2maind.lib"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		buildoptions "/MD"	
		
		links 
		{ 
			"SDL2main.lib"
		}
workspace "everything"
	configurations { "Debug", "Release" }
	platforms { "Win32", "Win64" }
	
	location "build"
	
	filter { "platforms:Win32" }
		system "Windows"
		architecture "x86"
	
	filter { "platforms:Win64" }
		system "Windows"
		architecture "x86_64"
		
	filter {}

project "game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"

	files {
		"source/**.h", 
		"source/**.c", 
		"source/**.cpp" 
	}
	
	includedirs {
		"extern/sokol",
		"source"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

   filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

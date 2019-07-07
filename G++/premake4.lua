solution "ImplicitTerrains"
	configurations { "Debug", "Release" }

	platforms { "x64" }
	
	includedirs { ".", "../Code/Include", "/usr/include/" }

	rootDir = path.getabsolute("../")
	
	configuration "Debug"
		targetdir "./Out/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		targetdir "./Out/Release"
		flags { "OptimizeSpeed" }

	configuration "linux"
		buildoptions { "-mtune=native -march=native" }
		buildoptions { "-std=c++14" }
		buildoptions { "-W -Wall -Wsign-compare -Wno-unused-parameter -Wno-unused-variable" }
		buildoptions { "-flto"}
		linkoptions { "-flto"}

	configuration { "linux", "debug" }
		buildoptions { "-g"}
		linkoptions { "-g"}

fileList = { rootDir .. "/Code/Source/*.cpp", rootDir .. "/Code/Include/*.h" }
project("ImplicitTerrains")
	language "C++"
	kind "ConsoleApp"
	targetdir "Out"
files ( fileList )
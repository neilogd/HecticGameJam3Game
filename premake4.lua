local action = _ACTION or ""

if psybrusSDK == nil then
	print "Missing PSYBRUS_SDK: Defaulting to ../Psybrus"
	psybrusSDK = "../Psybrus"
else
	print ("PSYBRUS_SDK: " .. psybrusSDK)
end

boostRoot = os.getenv("BOOST_ROOT")
if boostRoot == nil then
	print "Missing BOOST_ROOT: Defaulting to C:/Boost"
	boostRoot = "C:/Boost"
else
	print ("BOOST_ROOT: " .. boostRoot)
end

solution "HecticGameJam3Game"
	psybrusSDK = os.getenv("PSYBRUS_SDK") or "../Psybrus"
	boostRoot = os.getenv("BOOST_ROOT") or "C:/Boost"
	boostInclude = boostRoot .. "/include/boost-1_56"
	boostLib = boostRoot .. "/lib"
	location ( "Build/" .. action )
	configurations { "Debug", "Release", "Production", "ProductionSymbols" }

	configuration "vs*"
		defines { "STATICLIB", "_CRT_SECURE_NO_WARNINGS", "_STATIC_CPPLIB" }	

	configuration "Debug"
		targetdir ( "Build/" .. action .. "/bin/Debug" )
		defines { "PSY_USE_PROFILER=0", "STATICLIB", "WINDOWS", "_WIN32", "WIN32", "DEBUG", "PSY_DEBUG", "PSY_SERVER" }
		flags { "StaticRuntime", "EnableSSE", "EnableSSE2", "FloatFast", "NativeWChar", "Symbols" }

	configuration "Release"
		targetdir ( "Build/" .. action .. "/bin/Release" )
		defines { "PSY_USE_PROFILER=0", "STATICLIB", "WINDOWS", "_WIN32", "WIN32", "NDEBUG", "PSY_RELEASE", "PSY_SERVER" }
		flags { "StaticRuntime", "EnableSSE", "EnableSSE2", "FloatFast", "NativeWChar", "Symbols", "Optimize" }

	configuration "Production"
		targetdir ( "Build/" .. action .. "/bin/Production" )
		defines { "STATICLIB", "WINDOWS", "_WIN32", "WIN32", "NDEBUG", "PSY_PRODUCTION" }
		flags { "StaticRuntime", "EnableSSE", "EnableSSE2", "FloatFast", "NativeWChar", "NoFramePointer", "Optimize" }

	configuration "ProductionSymbols"
		targetdir ( "Build/" .. action .. "/bin/Production" )
		defines { "STATICLIB", "WINDOWS", "_WIN32", "WIN32", "NDEBUG", "PSY_PRODUCTION" }
		flags { "StaticRuntime", "EnableSSE", "EnableSSE2", "FloatFast", "NativeWChar", "NoFramePointer", "Optimize", "Symbols" }

	-- Build externals.
	dofile (psybrusSDK .. "/External/premake4.lua")

	-- Build engine.
	dofile (psybrusSDK .. "/Engine/premake4.lua")

	-- Build game source.
	dofile ("./Source/premake4.lua")


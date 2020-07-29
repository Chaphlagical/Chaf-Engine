workspace "CEngine"
	architecture "x64"
	startproject "App"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

GraphicsAPI="CHAF_OPENGL_API";

includeDir={}
includeDir["GLFW"]="../vendor/GLFW/include"
includeDir["glad"]="../vendor/glad/include"
includeDir["ImGui"]="../vendor/ImGui"
includeDir["glm"]="../vendor/glm"
includeDir["stb_image"]="../vendor/stb_image"
includeDir["entt"]="../vendor/entt/include"
includeDir["dirent"]="../vendor/dirent/include"

group "Dependencies"
	include "../vendor/GLFW/"
	include "../vendor/glad/"
	include "../vendor/ImGui/"
group ""

----------------------------------App----------------------------------------------------------

project "App"
	location "Build/App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../src/%{prj.name}/**.h",
		"../src/%{prj.name}/**.cpp",
	}

	vpaths
	{
		["src"]={"../src/App/**.h","../src/App/**.cpp"}
	}

	includedirs
	{
		"../src/",
		"../vendor/spdlog/include",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.entt}"
	}

	links
	{
		"Engine",
		"Editor",
		"Renderer",
		"Gui",
		"Scene"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CHAF_PLATFORM_WINDOWS",
		}

	--defines "IMGUI_API=__declspec(dllimport)"

	filter "configurations:Debug"
		defines "CHAF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CHAF_DIST"
		runtime "Release"
		optimize "on"

---------------------------------Engine--------------------------------------------------------

project "Engine"
	location "build/Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../src/%{prj.name}/**.h",
		"../src/%{prj.name}/**.cpp",
		"../vendor/glm/glm/**.hpp",
		"../vendor/glm/glm/**.inl",
		"../vendor/stb_image/*.h",
		"../vendor/stb_image/*.cpp",
		"../vendor/entt/include/*.h",
	}

	vpaths
	{
		["platform/opengl"]={"../src/Engine/Platform/OpenGL/*.h", "../src/Engine/Platform/OpenGL/*.cpp"},
		["src"]={"../src/Engine/*.cpp","../src/Engine/*.h"},
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		GraphicsAPI
	}

	includedirs
	{
		"../src/",
		"../src/PCH/",
		"../src/PCH/",
		"../vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CHAF_PLATFORM_WINDOWS",
			"CHAF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CHAF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CHAF_DIST"
		runtime "Release"
		optimize "on"

---------------------------------Editor--------------------------------------------------------

project "Editor"
	location "build/Editor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../src/%{prj.name}/**.h",
		"../src/%{prj.name}/**.cpp",
	}

	vpaths
	{
		["src"]={"../src/Editor/*.h","../src/Editor/*.cpp"},
		["FileDialog"]={"../src/Editor/FileDialog/*"}
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_WIN64"
	}

	vpaths
	{
	
	}

	includedirs
	{
		"../src/",
		"../src/PCH/",
		"../vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}",
		"%{includeDir.dirent}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
		"ImGui",
		"Engine",
		"Renderer",
		"Scene",
		"Gui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CHAF_PLATFORM_WINDOWS",
			"CHAF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CHAF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CHAF_DIST"
		runtime "Release"
		optimize "on"

---------------------------------Renderer--------------------------------------------------------

project "Renderer"
	location "build/Renderer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../src/%{prj.name}/**.h",
		"../src/%{prj.name}/**.cpp",
		"../src/%{prj.name}/**.cc",
	}

	vpaths
	{
		["platform/opengl"]={"../src/Renderer/Platform/OpenGL/*.h", "../src/Renderer/Platform/OpenGL/*.cpp"},
		["src"]={"../src/Renderer/*.h", "../src/Renderer/*.cpp"},	
		["tinyobjloader"]={"../src/Renderer/tinyobjloader/*.h", "../src/Renderer/tinyobjloader/*.cc"},
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		GraphicsAPI
	}

	includedirs
	{
		"../src/",
		"../src/PCH/",
		"../vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CHAF_PLATFORM_WINDOWS",
			"CHAF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CHAF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CHAF_DIST"
		runtime "Release"
		optimize "on"

---------------------------------Scene--------------------------------------------------------

project "Scene"
	location "build/Scene"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../src/%{prj.name}/**.h",
		"../src/%{prj.name}/**.cpp",
		"../src/%{prj.name}/**.cc",
	}

	vpaths
	{
		["src"]={"../src/Scene/*.h", "../src/Scene/*.cpp"},	
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"CHAF_OPENGL_API"
	}

	includedirs
	{
		"../src/",
		"../src/PCH/",
		"../vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CHAF_PLATFORM_WINDOWS",
			"CHAF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CHAF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CHAF_DIST"
		runtime "Release"
		optimize "on"

---------------------------------Gui--------------------------------------------------------

project "Gui"
	location "build/Gui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../src/%{prj.name}/**.h",
		"../src/%{prj.name}/**.cpp",
	}

	vpaths
	{
		["platform/opengl"]={"../src/Gui/Platform/OpenGL/*.h", "../src/Gui/Platform/OpenGL/*.cpp"},
		["src"]={"../src/Gui/*.h", "../src/Gui/*.cpp"},	
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		GraphicsAPI
	}

	includedirs
	{
		"../src/",
		"../src/PCH/",
		"../vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
		"Engine",
		"Renderer",
		"Scene",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CHAF_PLATFORM_WINDOWS",
			"CHAF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CHAF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CHAF_DIST"
		runtime "Release"
		optimize "on"
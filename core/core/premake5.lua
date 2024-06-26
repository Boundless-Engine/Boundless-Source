project "BCore"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      -- DEBUG
      "../../core/debug/logger/src",
      "../../core/debug/profiler/src",
      
      -- CORE
      "../../core/app/src",
      "../../core/events/src",
      "../../core/graphics/src",
      "../../core/gui/src",
      "../../core/input/src",
      "../../core/window/src",

      -- SHARED
      "../../core/system/src",
      --  DEPENDENCIES
      "../../dependencies/GLFW/include/",
      "../../dependencies/imgui/",
      "../../dependencies/json/single_include/nlohmann",
      -- GRAPHICS SDK
      "%{IncludeDir.VulkanSDK}"
   }

   links
   {
      -- CORE
      "BApp",
      "BEvent",
      "BGraphics",
      "BGui",
      "BWindow",
      -- DEBUG
      "BLogger",
      "BProfiler",
      -- SHARED
      "BSystem",
      -- DEPENDENCIES
      "GLFW",
      "ImGui",
      -- GRAPHICS SDK
      "%{Library.Vulkan}"
   }

	targetdir ("../../build/" .. outputdir .. "/%{prj.name}")
	objdir ("../../build/int/" .. outputdir .. "/%{prj.name}")
	
   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
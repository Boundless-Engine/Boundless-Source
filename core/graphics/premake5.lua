project "BGraphics"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      "../../core/debug/logger/src",
      "../../core/debug/profiler/src",
      "../../core/system/src/",
      "../../core/window/src",
      "../../core/events/src",
      "../../dependencies/GLFW/include",
      "../../dependencies/imgui/",
      "../../dependencies/glm/",

      "%{IncludeDir.VulkanSDK}",
   }
   
   links
   {
      "BLogger",
      "BProfiler",
      "BEvent",
      "BWindow",
      "ImGui",
      "BSystem",
      "GLFW",
      "ImGui",

      "%{Library.Vulkan}",
      "%{Library.shaderc_shared}"
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
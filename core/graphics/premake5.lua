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
      "../../core/system/src/",
      "../../core/window/src",
      "../../core/events/src",
      "../../dependencies/GLFW/include",
      "../../dependencies/imgui/",
      "%{IncludeDir.VulkanSDK}"
   }
   
   links
   {
      "BEvent",
      "BWindow",
      "ImGui",
      "BSystem",
      "GLFW",
      "ImGui",
      "%{Library.Vulkan}"
   }


   targetdir ("../../build/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../../build/bin/" .. outputdir .. "/%{prj.name}")

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
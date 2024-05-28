project "BGui"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      --  shared
      "../../core/system/src/",

      -- core
      "../../core/window/src",
      "../../core/graphics/src",
      "../../core/events/src",
      
      -- dependencies
      "../../dependencies/imgui/",
      "../../dependencies/GLFW/include/",
      "../../dependencies/json/single_include/nlohmann",
      
      -- graphics
      "%{IncludeDir.VulkanSDK}",
   }
   
   links
   {
      -- shared
      "BSystem",
      -- core
      "BEvent",
      "BWindow",
      "BGraphics",

      -- dependencies
      "ImGui",
      "GLFW",

      -- graphics
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
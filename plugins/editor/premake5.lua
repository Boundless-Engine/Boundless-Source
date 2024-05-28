project "Boundless-Editor"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
        "src",
         -- debug
        "../../core/debug/src",
         -- core
        "../../core/app/src",
        "../../core/events/src",
        "../../core/input/src",
        "../../core/system/src",
        "../../core/window/src",
        "../../core/gui/src",
        "../../core/graphics/src",

         -- graphics
         "../../graphics/vulkan/src",
         "../../core/core/src",
         -- dependencies
         "../../dependencies/imgui/",
         "../../dependencies/GLFW/include",
         
         "../../dependencies/json/single_include/nlohmann",

         -- graphics apis
         "%{IncludeDir.VulkanSDK}"
    }

   links
   {
      "BCore"
      -- -- core
      --   "BSystem",
      --   "BApp",
      --   "BWindow",
      --   "BEvent",
      --   "BInput",
      --   "BGui",
      --   "BGraphics",
      -- -- dependencies
      --   "GLFW",
      -- -- SDK 
      --   "%{Library.Vulkan}"
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
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
        -- SHARED
        "BSystem",
        -- DEPENDENCIES
        "GLFW",
        "ImGui",
        -- GRAPHICS SDK
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
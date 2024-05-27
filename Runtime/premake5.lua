project "Runtime"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"

   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      -- plugins
      "../plugins/editor/src",
      
      -- core 
      "../core/app/src",
      "../core/events/src",
      "../core/input/src",
      "../core/system/src",
      "../core/window/src",
      "../core/graphics/src",
      "../core/gui/src",
      "../core/core/src",
      -- dependencies
      "../dependencies/glfw/include/",
      "../dependencies/imgui/",

      "%{IncludeDir.VulkanSDK}",
   }

   links
   {
      -- core
      "BApp",
      "BEvent",
      "BInput",
      "BSystem",
      "BWindow",
      
      -- graphics
      "%{Library.Vulkan}",
      
      -- dependencies 
      "GLFW",
      "ImGui",

      -- core
      "BCore",
      -- "plugins"
      "Boundless-Editor"
   }

   targetdir ("../build/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../build/bin/" .. outputdir .. "/%{prj.name}")

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
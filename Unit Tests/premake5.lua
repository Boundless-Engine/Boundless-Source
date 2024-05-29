project "Unit Tests"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"

   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      "../core/app/src",
      "../core/events/src",
      "../core/input/src",
      "../core/system/src",
      "../core/window/src",
      "../devops/unit test framework/src",


   }

   links
   {
      "BUnitTestFramework",
      "BApp",
      "BEvent",
      "BInput",
      "BSystem",
      "BWindow",
      "BGraphics",
      "BGui",
   }

   targetdir ("../build/" .. outputdir .. "/%{prj.name}")
   objdir ("../build/int/" .. outputdir .. "/%{prj.name}")

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
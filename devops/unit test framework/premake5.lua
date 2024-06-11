project "BUnitTestFramework"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"

   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      "../core/system/src",
      "../core/window/src",
      "../../dependencies/imgui",
      
      "../../dependencies/json/single_include/nlohmann",
   }

   links
   {
      "ImGui",
      "BSystem",
      "BWindow",
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
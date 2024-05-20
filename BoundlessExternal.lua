-- WalnutExternal.lua

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["GLFW"] = "./dependencies/GLFW/include"
IncludeDir["GLM"] = "./dependencies/GLM/"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

group "Dependencies"
   include "dependencies/GLFW"
group ""

group "Graphics"
   include "graphics/vulkan"
   include "graphics/directx"
group ""

group "Debug"
   include "core/debug/logger"
   include "core/debug/profiler"
group ""

group "Core"
   include "core/window"
group ""
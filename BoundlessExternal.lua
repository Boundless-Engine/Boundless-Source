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
   include "dependencies/imgui"
group ""

group "DevOps"
include "devops/unit test framework"
group ""
group "Debug"
   include "core/debug/logger"
   include "core/debug/profiler"
group ""

group "Shared"
   include "core/system"
group ""

group "Plugins"
   include "plugins/editor"
group ""

group "Core"
   include "core/app"
   include "core/events"
   include "core/graphics"
   include "core/gui"
   include "core/input"
   include "core/window"
   include "core/core"
group ""
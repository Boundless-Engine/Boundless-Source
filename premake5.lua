-- premake5.lua
workspace "Boundless Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Unit Tests"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "BoundlessExternal.lua"
include "Unit Tests"
include "Runtime"
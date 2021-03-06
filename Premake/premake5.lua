--[[
/******************************************************************************/
/*!
 \file   premake5.lua
 \author Joshua Biggs
 \par    Course: GAM305
 \par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
 \brief
 */
/******************************************************************************/
]]
solution "SinEngine"
  configurations { "Debug", "Release" }
  location "../"
  linker    =   {"Zilch.lib", "fmod_vc.lib", "fmod.dll"}
  toolset "v120"
  project "SinEngine"
    targetname "SinEngine"
    location "."
    language "C++"
    objdir "../Solution/ObjectFiles"
    kind "WindowedApp"
    pchheader "Precompiled.h"
    pchsource "../Source/Build/Precompiled.cpp"

    -- source files
    files
    {
      "../Source/**.h",
      "../Source/**.cpp",
      "../Source/**.c",
      "../Source/**.tpp",
      "../Assets/Shaders/**.glsl",
      "../Assets/**.txt",
      "../Assets/**.data",
      "../Assets/**.z",
    }
    
    excludes
    {
    }
    
    
    includedirs 
    {
        "../ExternalDependencies/Include",
        "../ExternalDependencies/Include/WICTextureLoader",
        "../ExternalDependencies/Zilch",
        "../Source/**",
        "../ExternalDependencies/Include/FMod/include"
	}
    
    libdirs 
    {
        "../ExternalDependencies/Zilch",
        "../ExternalDependencies/Include/FMod/library"
    }


--[[ Debug Configuration ]]
    configuration "Debug"
      flags   { "Symbols" }
      defines { "_DEBUG"  , "_CRT_SECURE_NO_WARNINGS" }
      links {linker}
      targetdir "../Solution/"
      postbuildcommands
              {
                "copy ..\\ExternalDependencies\\Include\\FMod\\library\\fmod.dll ..\\Solution\\",
              }
        
--[[ Release Configuration ]]        
    configuration "Release"
      flags   { "Symbols" }
      defines { "NDEBUG"  , "_CRT_SECURE_NO_WARNINGS" }      
      optimize  "Full"
      links {linker}
      targetdir "../Solution/BinaryFiles/Release"
      postbuildcommands
              {   
                "copy ..\\ExternalDependencies\\Include\\FMod\\library\\fmod.dll ..\\Solution\\",
              }

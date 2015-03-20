@echo off
set /p shaderName=Shader Name:
set /p type= Shader Type(vs, frag, gs, cs):

copy /y template\ShaderTemplate.%type%.glsl ..\..\Resources\Shaders\%shaderName%.%type%.glsl

cd ..\premake
.\premake5 vs2013
pause
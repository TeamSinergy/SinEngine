@echo off
set /p objectName=Component Name:
set /p type= Component Type(Audio, Graphics, Physics):

copy /y template\ComponentTemplate.cpp..\source\Engine\System\%type%\%objectName%.cpp
copy /y template\ComponentTemplate.h ..\include\Engine\System\%type%\%objectName%.h

cd ..\premake
.\premake5 vs2013
pause
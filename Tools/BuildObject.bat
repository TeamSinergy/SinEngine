@echo off
set /p objectName=Object Name:
set /p type= Object Type(Audio, Graphics, Physics):

copy /y template\ObjectTemplate.cpp ..\source\Engine\System\%type%\%objectName%.cpp
copy /y template\ObjectTemplate.h ..\include\Engine\System\%type%\%objectName%.h

cd ..\premake
.\premake5 vs2013
pause
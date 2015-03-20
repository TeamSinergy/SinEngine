@echo off
set /p objectName=Name:
set /p loc= Object Location:

copy /y template\HeaderTemplate.h ..\include\%loc%\%objectName%.h

cd ..\premake
.\premake5 vs2013
pause
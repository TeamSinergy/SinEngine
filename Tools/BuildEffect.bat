@echo off
set /p objectName=Effect Name:

copy /y template\ObjectTemplate.cpp ..\source\Engine\System\WindowSystems\Graphics\ParticleSystem\Effects\%objectName%.cpp
copy /y template\ObjectTemplate.h ..\include\Engine\System\WindowSystems\Graphics\ParticleSystem\Effects\%objectName%.h

cd ..\premake
.\premake5 vs2013
pause
@echo off
set /p objectName=Scene Name:

copy /y template\SceneTemplate.cpp ..\source\Engine\System\SceneManager\Scenes\%objectName%.cpp
copy /y template\SceneTemplate.h ..\include\Engine\System\SceneManager\Scenes\%objectName%.h

cd ..\premake
.\premake5 vs2013
pause
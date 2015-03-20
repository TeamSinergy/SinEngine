@echo off
set /p systemName=System Name:

md ..\source\Engine\System\systemName
copy /y template\SystemTemplate.cpp ..\source\Engine\System\%systemName%.cpp
copy /y template\SystemTemplate.h ..\include\Engine\System\%systemName%.h

pause

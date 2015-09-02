@echo off
cd Premake
del *.vcxproj*
cd ..
del *.sdf
del *.opensdf
del *.sln
del *.suo
cd Solution
rmdir /S /Q ObjectFiles
rmdir /S /Q BinaryFiles
pause
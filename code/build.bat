@echo off
if not exist ..\build mkdir ..\build
pushd ..\build
      cl /Fenuwa -nologo -FC -Zi ..\code\nuwa_win32.cpp user32.lib gdi32.lib
popd

@REM This assumes you've executed `vcvarsall.bat x64` before running this batch script
@echo off

set raylib_dir=third_party\raylib-5.0_win64_msvc16\

cl.exe ^
src\platform_raylib_win32.c ^
/Zi /nologo ^
/I"%raylib_dir%\include" "%raylib_dir%\lib\raylib.lib" ^
"opengl32.lib" "kernel32.lib" "user32.lib" "shell32.lib" "gdi32.lib" "winmm.lib" "msvcrt.lib"

echo Build complete.
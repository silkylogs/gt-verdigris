@echo off

cl.exe src\platform_raylib.c src\GTV_Arena.c /Zi /nologo /I"third_party\raylib-5.0_win64_msvc16\include" "third_party\raylib-5.0_win64_msvc16\lib\raylib.lib" opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib 
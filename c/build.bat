@echo off

REM Build the game code, with full warnings
cl.exe src\GTV_game.c /MD /Zi /W4 /c /nologo ^
/link /MACHINE:X64 /OUT:"GTV_game.obj" /DEBUG:FULL /PDB:"GTV_game.pdb"

REM Build the main application, with relaxed warnings
cl.exe src\platform_raylib.c GTV_game.obj ^
/I"third_party\raylib-5.0_win64_msvc16\include" ^
/MD /Zi /nologo ^
/link /MACHINE:X64 /OUT:"main.exe" /DEBUG:FULL /PDB:"main.pdb" ^
"third_party\raylib-5.0_win64_msvc16\lib\raylib.lib" opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib 
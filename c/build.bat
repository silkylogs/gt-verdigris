cl.exe src\main.c ^
/I"third_party\raylib-5.0_win64_msvc16\include" ^
/MD /link /MACHINE:X64 /OUT:"main.exe" ^
"third_party\raylib-5.0_win64_msvc16\lib\raylib.lib" opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib 

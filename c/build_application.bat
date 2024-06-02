@echo off
REM cl /c /Zi /nologo application.c /Foapplication.obj
REM link /nologo /DLL /DEBUG /OUT:application.dll application.obj

cl.exe /c /MD /Zi /W4 /nologo src\GTV_game.c /FoGTV_game.obj 

link /nologo /DLL /DEBUG:FULL /PDB:"GTV_game.pdb" /OUT:GTV_game.dll GTV_game.obj
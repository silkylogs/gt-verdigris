REM This build file is a workaround of odin refusing to compile the updated source file
REM (which may have errors in them) due to the presence of the executable
del *.exe
odin run .
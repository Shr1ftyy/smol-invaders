@echo OFF
if not defined DevEnvDir ( 
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall" x64
)
echo "Starting Build for all Projects with proposed changes"
echo .
devenv "smol-invaders.sln" /build Release 

rem Check the return code of devenv
if %errorlevel% equ 0 (
    echo Compilation successful.
    echo .
    echo "Running"
    call "x64\Release\smol-invaders.exe"
    echo "All builds completed." 
) else (
    echo Compilation failed.
)
echo .
pause
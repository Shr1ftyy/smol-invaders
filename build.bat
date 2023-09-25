@echo OFF 
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall" x64
echo "Starting Build for all Projects with proposed changes"
echo .  
devenv "smol-invaders.sln" /build Release 
echo . 
echo "All builds completed." 
echo .
echo "Running"
call "x64\Release\smol-invaders.exe"
pause
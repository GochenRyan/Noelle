set SHADERS="shaders"
set PROJECT="./shaders"

rem ImGui
start /b shaderc --type v --platform windows -p vs_5_0 -O 3 --varyingdef %SHADERS%/varying.def.sc -f %SHADERS%/vs_imgui.sc -o %PROJECT%/vs_imgui.bin
start /b shaderc --type f --platform windows -p ps_5_0 -O 3 --varyingdef %SHADERS%/varying.def.sc -f %SHADERS%/fs_imgui.sc -o %PROJECT%/fs_imgui.bin

echo Finish compiling...
echo .bin files generated at %PROJECT%

pause

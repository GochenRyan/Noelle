rmdir /s/q build
rmdir /s/q lib
rmdir /s/q output

set VENDOR_PATH=%cd%\\Vendor

rem clean freetype
cd /d %VENDOR_PATH%\\freetype
cmake -E remove_directory build
cmake -E remove_directory lib

rem clean SDL
cd /d %VENDOR_PATH%\\SDL
cmake -E remove_directory build
cmake -E remove_directory lib

rem clean spdlog
cd /d %VENDOR_PATH%\\spdlog
cmake -E remove_directory build
cmake -E remove_directory lib
xmake f -c

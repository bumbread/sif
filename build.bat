@echo off

rm sif.exe

set optimizations=/Od
if "%1"=="release" (
    set optimizations=/O2
)

cl /MP /Zi /Fd %optimizations% /Iexternal src/main.cpp src/lexer.cpp src/parser.cpp src/checker.cpp src/c_backend.cpp src/basic.cpp src/os_windows.cpp src/common.cpp user32.lib advapi32.lib ole32.lib oleaut32.lib /EHsc /link /INCREMENTAL:no /OUT:sif.exe /DEBUG
del *.obj
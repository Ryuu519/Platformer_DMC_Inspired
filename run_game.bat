@echo off
title Devil May Cry - Castle of the Damned
echo =======================================================
echo   Launching: Devil May Cry - Castle of the Damned (Theme 2)
echo =======================================================
echo.

:: Configure compiler path in PATH to ensure DLLs load correctly (libstdc++ etc.)
set PATH=D:\CLion 2025.2.3\bin\mingw\bin;%PATH%

:: Run the game in interactive mode
.\cmake-build-debug\oop.exe

echo.
echo =======================================================
echo   The session has ended. Press any key to exit.
echo =======================================================
pause > nul

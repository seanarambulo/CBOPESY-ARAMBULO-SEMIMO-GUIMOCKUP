@echo off
setlocal enabledelayedexpansion
title CBOPESY OS Mockup Build Tool
color 0B

:: Define paths
set "PROJECT_DIR=%~dp0CBOPESY-ARAMBULO-SEMIMO-GUIMOCKUP\"
set "PROJECT_NAME=%PROJECT_DIR%CBOPESY-ARAMBULO-SEMIMO-GUIMOCKUP.vcxproj"

:menu
cls
echo ===================================================
echo     CBOPESY OS MOCKUP COMPILATION MANAGER
echo ===================================================
echo.
echo   [1] Build Debug (x64)
echo   [2] Build Release (x64)
echo   [3] Run Debug
echo   [4] Run Release
echo   [5] Clean Project
echo   [6] Rebuild Debug (x64)
echo   [7] Rebuild Release (x64)
echo   [8] Exit
echo.
echo ===================================================
set /p opt="Select an option (1-8): "

if "%opt%"=="1" goto build_debug
if "%opt%"=="2" goto build_release
if "%opt%"=="3" goto run_debug
if "%opt%"=="4" goto run_release
if "%opt%"=="5" goto clean_proj
if "%opt%"=="6" goto rebuild_debug
if "%opt%"=="7" goto rebuild_release
if "%opt%"=="8" goto end
goto menu

:find_msbuild
:: Locate MSBuild using vswhere
set "MSBUILD_PATH="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -property installationPath`) do (
        set "VS_PATH=%%i"
    )
)
if defined VS_PATH (
    if exist "!VS_PATH!\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=!VS_PATH!\MSBuild\Current\Bin\MSBuild.exe"
    )
)
:: Fallback to common path if vswhere doesn't find it
if not defined MSBUILD_PATH (
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
    )
)

if not defined MSBUILD_PATH (
    echo [ERROR] MSBuild.exe was not found on your system.
    echo Please make sure Visual Studio is installed.
    pause
    goto menu
)
goto :eof

:build_debug
call :find_msbuild
echo.
echo [+] Building Debug (x64)...
echo.
"!MSBUILD_PATH!" "%PROJECT_NAME%" /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Debug build completed successfully.
) else (
    echo.
    echo [ERROR] Debug build failed.
)
pause
goto menu

:build_release
call :find_msbuild
echo.
echo [+] Building Release (x64)...
echo.
"!MSBUILD_PATH!" "%PROJECT_NAME%" /p:Configuration=Release /p:Platform=x64
if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Release build completed successfully.
) else (
    echo.
    echo [ERROR] Release build failed.
)
pause
goto menu

:run_debug
echo.
set "EXE_PATH=%PROJECT_DIR%x64\Debug\CBOPESY-ARAMBULO-SEMIMO-GUIMOCKUP.exe"
if exist "%EXE_PATH%" (
    echo [!] Running Debug Build...
    start /D "%PROJECT_DIR%" "" "%EXE_PATH%"
) else (
    echo [ERROR] Debug executable not found! Build it first.
    pause
)
goto menu

:run_release
echo.
set "EXE_PATH=%PROJECT_DIR%x64\Release\CBOPESY-ARAMBULO-SEMIMO-GUIMOCKUP.exe"
if exist "%EXE_PATH%" (
    echo [!] Running Release Build...
    start /D "%PROJECT_DIR%" "" "%EXE_PATH%"
) else (
    echo [ERROR] Release executable not found! Build it first.
    pause
)
goto menu

:clean_proj
echo.
echo [+] Cleaning build directories...
if exist "%PROJECT_DIR%x64" (
    rmdir /s /q "%PROJECT_DIR%x64"
    echo [SUCCESS] Deleted x64 folder under project directory.
)
if exist "%PROJECT_DIR%..\x64" (
    rmdir /s /q "%PROJECT_DIR%..\x64"
    echo [SUCCESS] Deleted root x64 folder.
)
echo [INFO] Clean completed.
pause
goto menu

:rebuild_debug
call :clean_proj_quiet
call :build_debug_quiet
pause
goto menu

:rebuild_release
call :clean_proj_quiet
call :build_release_quiet
pause
goto menu

:clean_proj_quiet
if exist "%PROJECT_DIR%x64" (
    rmdir /s /q "%PROJECT_DIR%x64"
)
if exist "%PROJECT_DIR%..\x64" (
    rmdir /s /q "%PROJECT_DIR%..\x64"
)
goto :eof

:build_debug_quiet
call :find_msbuild
echo [+] Rebuilding Debug (x64)...
"!MSBUILD_PATH!" "%PROJECT_NAME%" /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] Rebuild completed successfully.
) else (
    echo [ERROR] Rebuild failed.
)
goto :eof

:build_release_quiet
call :find_msbuild
echo [+] Rebuilding Release (x64)...
"!MSBUILD_PATH!" "%PROJECT_NAME%" /p:Configuration=Release /p:Platform=x64
if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] Rebuild completed successfully.
) else (
    echo [ERROR] Rebuild failed.
)
goto :eof

:end
exit

@echo off
:: build.bat — Windows build script for SY-1000 FloorBoard.
:: Requires Qt (qmake on PATH) and either MSVC (nmake) or MinGW (mingw32-make).
:: Run from the repository root: tools\build.bat

setlocal EnableDelayedExpansion

set PRO_FILE=SY-1000FloorBoard.pro

:: ── Detect make tool ───────────────────────────────────────────────────────
where nmake >nul 2>&1
if %errorlevel% == 0 (
    set MAKE=nmake
) else (
    where mingw32-make >nul 2>&1
    if %errorlevel% == 0 (
        set MAKE=mingw32-make
    ) else (
        echo ERROR: Neither nmake nor mingw32-make found on PATH.
        echo Install Qt with MSVC or MinGW, or run from a Visual Studio developer prompt.
        exit /b 1
    )
)
echo Using make tool: %MAKE%

:: ── Read current version from .pro file ────────────────────────────────────
for /f "tokens=3" %%v in ('findstr /b "VERSION = " %PRO_FILE%') do set CURRENT_VERSION=%%v

:: Extract build number (4th dotted segment) and increment via PowerShell
for /f %%n in ('powershell -NoProfile -Command "('%CURRENT_VERSION%' -split '\.')[3]"') do set BUILD_NUM=%%n
for /f %%n in ('powershell -NoProfile -Command "[int]('%BUILD_NUM%') + 1"') do set NEXT_BUILD=%%n

:: Build today's date prefix
for /f %%d in ('powershell -NoProfile -Command "Get-Date -Format 'yyyy.MM.dd'"') do set TODAY=%%d
set NEW_VERSION=%TODAY%.%NEXT_BUILD%

:: ── Stamp version into .pro file ───────────────────────────────────────────
powershell -NoProfile -Command ^
    "(Get-Content '%PRO_FILE%') -replace '^VERSION = .*', 'VERSION = %NEW_VERSION%' | Set-Content '%PRO_FILE%'"

:: ── Stamp version into preferences.xml.dist ────────────────────────────────
:: Remove dots: 2026.03.20 → 20260320
for /f %%c in ('powershell -NoProfile -Command "'%NEW_VERSION%' -replace '\.', ''"') do set CLEAN_VERSION=%%c
powershell -NoProfile -Command ^
    "(Get-Content 'preferences.xml.dist') -replace '<Application version=""[^""]*""/>', '<Application version=""%CLEAN_VERSION%""/>' | Set-Content 'preferences.xml.dist'"

:: Touch preferences.xml.dist to bust rcc cache
powershell -NoProfile -Command "(Get-Item 'preferences.xml.dist').LastWriteTime = Get-Date"

:: Remove cached resource object to force rebuild
if exist build\release\qrc_SY-1000FloorBoard.obj del /f build\release\qrc_SY-1000FloorBoard.obj

echo ──────────────────────────────────────
echo Version: %CURRENT_VERSION% -^> %NEW_VERSION%
echo ──────────────────────────────────────

:: ── qmake + make ───────────────────────────────────────────────────────────
qmake SY-1000FloorBoard.pro
if %errorlevel% neq 0 ( echo qmake failed. & exit /b %errorlevel% )

%MAKE%
if %errorlevel% neq 0 ( echo Build failed. & exit /b %errorlevel% )

echo ──────────────────────────────────────
echo BUILD SUCCEEDED: %NEW_VERSION%
echo ──────────────────────────────────────
echo Binary: build\release\SY-1000FloorBoard.exe  (or build\debug\)
echo ──────────────────────────────────────
endlocal

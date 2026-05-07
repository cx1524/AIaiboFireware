@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ========================================
echo        AIaiboFireware load script
echo ========================================
echo.

:: set project root directory
set "PROJECT_ROOT=%~dp0"
set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

:: check venv directory
if exist "%PROJECT_ROOT%\.venv" (
    set "VENV_PATH=%PROJECT_ROOT%\.venv"
) else if exist "%PROJECT_ROOT%\..\.venv" (
    set "VENV_PATH=%PROJECT_ROOT%\..\.venv"
) else (
    echo ERROR: venv directory not found in project root or parent directory!
    pause
    exit /b 1
)

:: check build directory and elf file
if not exist "%PROJECT_ROOT%\build" (
    echo ERROR: build directory not found!
    echo Please compile project first
    pause
    exit /b 1
)

if not exist "%PROJECT_ROOT%\build\AIaiboFirmware.elf" (
    echo ERROR: build\AIaiboFirmware.elf file not found!
    echo Please compile project first
    pause
    exit /b 1
)

:: check pack file
if not exist "%PROJECT_ROOT%\pack\Geehy.APM32F4xx_DFP.1.0.11.pack" (
    echo ERROR: pack\Geehy.APM32F4xx_DFP.1.0.11.pack file not found!
    echo Please ensure pack file is exists.
    pause
    exit /b 1
)

:: activate venv environment
echo activating venv environment...
call "%VENV_PATH%\Scripts\activate.bat"
if errorlevel 1 (
    echo ERROR: activate venv environment failed!
    pause
    exit /b 1
)

:: check pyocd availability
echo checking pyocd tool...
pyocd --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: pyocd not installed or not in PATH environment!
    echo Use pip install pyocd to install it.
    pause
    exit /b 1
)

:: execute load command
echo.
echo ========================================
echo start load AIaiboFireware.elf to device
echo ========================================
echo.

cd /d "%PROJECT_ROOT%"
pyocd load -t APM32F402RB --pack=pack\Geehy.APM32F4xx_DFP.1.0.11.pack build\AIaiboFirmware.elf

if errorlevel 1 (
    echo.
    echo ERROR: load failed!
    pause
    exit /b 1
) else (
    echo.
    echo ========================================
    echo load success!
    echo ========================================
)

@echo off
setlocal

set "BUILD_DIR=build"
set "SOURCE_DIR=."

if "%1"=="" goto build
if "%1"=="clean" goto clean
if "%1"=="rebuild" goto rebuild
if "%1"=="config" goto config
goto usage

:config
echo [Configuring CMake with Ninja...]
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cmake -S "%SOURCE_DIR%" -B "%BUILD_DIR%" -G Ninja
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed!
    exit /b %ERRORLEVEL%
)
echo [Configuration complete.]
goto end

:build
echo [Building firmware...]
if not exist "%BUILD_DIR%" (
    echo [Build directory not found, configuring...]
    goto config
)
cmake --build "%BUILD_DIR%"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed!
    exit /b %ERRORLEVEL%
)
echo [Build complete.]
goto end

:clean
echo [Cleaning build directory...]
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    echo [Clean complete.]
) else (
    echo [Build directory not found.]
)
goto end

:rebuild
echo [Rebuilding firmware...]
call :clean
call :build
goto end

:usage
echo Usage: build.bat [command]
echo.
echo Commands:
echo   (none)   - Build firmware (configure if needed)
echo   config   - Configure CMake only
echo   clean    - Remove build directory
echo   rebuild  - Clean and build
echo.
exit /b 1

:end
endlocal

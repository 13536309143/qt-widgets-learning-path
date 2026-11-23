@echo off
setlocal
if "%~1"=="" (
  echo Usage: scripts\run.cmd 49_text_editor
  exit /b 1
)
if exist "%~dp0..\.qt-env.cmd" call "%~dp0..\.qt-env.cmd"
if defined QT_BIN set "PATH=%QT_BIN%;%PATH%"
cd /d "%~dp0.."
if exist "build\%~1.exe" (
  start "" "build\%~1.exe"
  exit /b 0
)
if exist "build\Release\%~1.exe" (
  start "" "build\Release\%~1.exe"
  exit /b 0
)
echo Example not built. Configure and build with CMake first.
exit /b 1

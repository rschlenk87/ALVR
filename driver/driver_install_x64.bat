@echo off
IF EXIST "C:\Program Files\Steam\steamapps\common\SteamVR\bin\win32\" (
"C:\Program Files\Steam\steamapps\common\SteamVR\bin\win32\vrpathreg.exe" adddriver "%~dp0
echo "Driver Probably Installed"
) ELSE (
echo "SteamVR not located in C:\\Program Files\\Steam\\steamapps\\common\\SteamVR - Installation Failed"
)
pause

@echo off
set "ENGINE_PATH=D:\Epic Games\UE_5.7"
set "PROJECT_PATH=%~dp0Defence_AtoK.uproject"

"%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%PROJECT_PATH%" -game -engine -progress

pause
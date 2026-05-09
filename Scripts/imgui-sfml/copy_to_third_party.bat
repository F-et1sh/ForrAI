:: copy imgui-sfml submodule to third party

@echo off

xcopy /y /i /s "..\\..\\External\\imgui-sfml\\imconfig-SFML.h" "..\\..\\ThirdParty\\imgui-sfml\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui-sfml\\imgui-SFML.cpp" "..\\..\\ThirdParty\\imgui-sfml\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui-sfml\\imgui-SFML.h" "..\\..\\ThirdParty\\imgui-sfml\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui-sfml\\imgui-SFML_export.h" "..\\..\\ThirdParty\\imgui-sfml\\include\\" >nul

copy "..\\..\\External\\imgui-sfml\\LICENSE" "..\\..\\ThirdParty\\imgui-sfml\\IMGUI_SFML_LICENSE.txt" >nul
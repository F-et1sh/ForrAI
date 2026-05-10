:: copy imgui submodule to third party

@echo off

xcopy /y /i /s "..\\..\\External\\imgui\\misc\\freetype\\imgui_freetype.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\misc\\freetype\\imgui_freetype.cpp" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imconfig.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui.cpp" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui_demo.cpp" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui_draw.cpp" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui_internal.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui_tables.cpp" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imgui_widgets.cpp" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imstb_rectpack.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imstb_textedit.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul
xcopy /y /i /s "..\\..\\External\\imgui\\imstb_truetype.h" "..\\..\\ThirdParty\\imgui\\include\\" >nul

copy "..\\..\\External\\imgui\\LICENSE.txt" "..\\..\\ThirdParty\\imgui\\IMGUI_LICENSE.txt" >nul
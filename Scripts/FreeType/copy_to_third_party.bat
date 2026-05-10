:: copy freetype to ThirdParty

@echo off

xcopy /y /i /s "..\\..\\External\\freetype\\include" "..\\..\\ThirdParty\\freetype\\include" >nul
xcopy /y /i /s "..\\..\\External\\freetype\\build\\Debug" "..\\..\\ThirdParty\\freetype\\win64_debug" >nul
xcopy /y /i /s "..\\..\\External\\freetype\\build\\Release" "..\\..\\ThirdParty\\freetype\\win64_release" >nul
xcopy /y /i /s "..\\..\\External\\freetype\\build\\MinSizeRel" "..\\..\\ThirdParty\\freetype\\win64_min_size_rel" >nul
xcopy /y /i /s "..\\..\\External\\freetype\\build\\RelWithDebInfo" "..\\..\\ThirdParty\\freetype\\win64_rel_with_deb_info" >nul

copy "..\\..\\External\\freetype\\LICENSE.TXT" "..\\..\\ThirdParty\\freetype\\FREETYPE_GPLV2_LICENSE.TXT" >nul

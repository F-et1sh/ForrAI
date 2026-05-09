:: copy SFML to ThirdParty

@echo off

xcopy /y /i /s "..\\..\\External\\SFML\\include" "..\\..\\ThirdParty\\SFML\\include" >nul
xcopy /y /i /s "..\\..\\External\\SFML\\build\\lib\\Debug" "..\\..\\ThirdParty\\SFML\\win64_debug" >nul
xcopy /y /i /s "..\\..\\External\\SFML\\build\\lib\\Release" "..\\..\\ThirdParty\\SFML\\win64_release" >nul
xcopy /y /i /s "..\\..\\External\\SFML\\build\\lib\\MinSizeRel" "..\\..\\ThirdParty\\SFML\\win64_min_size_rel" >nul
xcopy /y /i /s "..\\..\\External\\SFML\\build\\lib\\RelWithDebInfo" "..\\..\\ThirdParty\\SFML\\win64_rel_with_deb_info" >nul

copy "..\\..\\External\\SFML\\license.md" "..\\..\\ThirdParty\\SFML\\SFML_LICENSE.md" >nul

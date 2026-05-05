:: copy mnist ( simple C++ reader ) submodule to third party

@echo off

xcopy /y /i /s "..\\..\\External\\mnist\\include" "..\\..\\ThirdParty\\mnist\\include\\" >nul
copy "..\\..\\External\\mnist\\LICENSE" "..\\..\\ThirdParty\\mnist\\MNIST_READER_LICENSE.txt" >nul
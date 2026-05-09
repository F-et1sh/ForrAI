:: build SFML in debug mode

@echo off

pushd "%~dp0\..\..\External\SFML"

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug

popd
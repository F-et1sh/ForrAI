:: build freetype in minimal size release mode

@echo off

pushd "%~dp0\..\..\External\freetype"

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config MinSizeRel

popd
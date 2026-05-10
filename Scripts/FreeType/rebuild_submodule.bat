:: rebuild SFML submodule

@echo off

pushd "%~dp0\..\..\External\freetype"

rmdir /q /s build

popd

call build_submodule_debug.bat
call build_submodule_release.bat
call build_submodule_min_size_rel.bat
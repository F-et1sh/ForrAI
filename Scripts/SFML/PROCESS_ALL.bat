:: process all

@echo off

call build_submodule_debug.bat
call build_submodule_release.bat
call build_submodule_min_size_rel.bat
call build_submodule_rel_with_deb_info.bat
call copy_to_third_party.bat
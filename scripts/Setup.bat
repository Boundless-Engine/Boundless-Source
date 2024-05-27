@echo off

@echo -------------------------------------------------
@echo Project Generation Started
@echo -------------------------------------------------

rem Navigate to the parent directory
pushd ..


rem Run the premake5.exe command
scripts\premake5.exe vs2022

rem Return to the original directory
popd


@echo -------------------------------------------------
@echo Project Generation Completed
@echo -------------------------------------------------


@echo off

set mode=%1
if [%mode%] == [] set mode=d

if %mode%==d set out_dir=bin-debug
if %mode%==r set out_dir=bin-rel

if %mode%==d set install_dir=..\install\debug\
if %mode%==r set install_dir=..\install\rel\

pushd %out_dir%
    meson compile
    :: This is a workaround to handle windows dlls, but makes for a nice location
    :: to place everything. 
    meson install --quiet --no-rebuild --only-changed --destdir %install_dir%
popd
@echo off

set exe=ct-render-demo.exe

set mode=%1
if [%mode%] == [] set mode=d

if %mode%==d set exe_dir=install\debug\bin
if %mode%==r set exe_dir=install\rel\bin

call %exe_dir%\%exe%
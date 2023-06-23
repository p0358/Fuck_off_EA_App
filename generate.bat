@echo off
git submodule update --init
premake\premake5 %* vs2022

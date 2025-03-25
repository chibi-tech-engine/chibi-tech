@echo off

meson setup --reconfigure --buildtype debug bin-debug
meson setup --reconfigure --buildtype release bin-rel

pushd bin-debug
    ninja -t compdb > compile_commands.json
popd

pushd bin-rel
    ninja -t compdb > compile_commands.json
popd

xcopy bin-debug\compile_commands.json compile_commands.json /Y
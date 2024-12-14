# usm_string_hash_dictionary_tool

This tool convert string to hash code by using a function created by Lemon Haze.

## Usage

Type the input string in text box and smash the "Hash!" button.

<img src="images/img.png" alt="screenshot"/>

## Compiling

This tool is comiled with mingw compiler on linux. I have no idea how to do it on windows.

Make sure you have installed `mingw-w64-i686-dev` (necessary for this compilation) or `mingw-w64` (to install the full toolchain).

Type the following command to compile the tool.

``i686-w64-mingw32-g++ -static-libgcc -static-libstdc++ -o hashtool.exe main.cpp hashtool.res -mwindows -luxtheme``

If you modify the manifest file, you will need to recreate the res file first.
``i686-w64-mingw32-windres hashtool.rc -O coff -o hashtool.res``
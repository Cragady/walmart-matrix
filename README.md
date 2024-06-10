# Overview

This was just a fun small thing I wanted to do. It's basically the walmart version of The Matrix's code rain.

I may revisit this to make a better looking one, but I doubt that will happen. This was more of a for-fun kinda thing.

## Notes

This compiles on Windows, but doesn't work well with the terminal application used. The characters printed are not the one's used in the application.

`#include <unistd.h>` is problematic. Will need to use windows equiv and configure a makefile or cmake for cross-comp if wanted.

In WSL (Ubuntu), it works as expected, but to get the desired effect, you have to scroll to the top of the output. This may also be due to the terminal used. Will have to 

Tried with MSVC, instead of the gcc port from mingw, and it still had the same issue.

Powershell for VS or call cl.exe directly by using proper INCLUDE, LIBPATH, and LIB env vars set.

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe" main.c


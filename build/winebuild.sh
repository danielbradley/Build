#!/bin/bash

VERSION=0.3

OSTYPE=win32
CC="/local/shared/WinRoot/Software/MinGW/bin/gcc.exe"
CPPFLAGS=-Isource/include
CFLAGS="-Wall -c"
LFLAGS=
LDFLAGS=

DESTDIR=/local/software/Win32/build-$VERSION

COMPILE="$CPPFLAGS $CFLAGS"
LINK="$LFLAGS $LDFLAGS"

wine -- $CC $COMPILE source/src/win32/ProcessPSA.c
#set PSA_FILES=obj\IOPSA.o obj\ProcessPSA.o obj\linkify.o
#set CLASS_FILES=obj\Arguments.o obj\CurrentContext.o obj\Global.o obj\List.o obj\String.o
#set FUNCTION_FILES=obj\build.o obj\copyfiles.o obj\execute.o obj\main.o
#set OBJECTS=%PSA_FILES% %CLASS_FILES% %FUNCTION_FILES%

#echo on
#mkdir bin
#mkdir obj

#set FILE=ProcessPSA
#%COMPILE% source\src\%OSTYPE%\%FILE%.c -o obj\%FILE%.o

#REM FOR %%F in (ProcessPSA, IOPSA, linkify) DO %COMPILE% source\src\%OSTYPE%\%%F.c -o obj\%%F.o
#REM FOR %%F in (Arguments, CurrentContext, Global, List, String) DO %COMPILE% source\src\%%F.c -o obj\%%F.o
#REM FOR %%F in (build, copyfiles, execute, main) DO %COMPILE% source\src\%%F.c -o obj\%%F.o

#REM %LINK% %OBJECTS% -o bin\build


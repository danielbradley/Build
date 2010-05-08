@REM
@REM	Set the location of your C Compiler here
@REM
@
@set CC="gcc.exe"
@
@REM
@REM	Set the desired install location here
@REM
@
@set DEST="C:\Local\software\bin\build.exe"
@
@echo l
@echo l  Building build-0.3 with %CC%
@echo l         and installing to %DEST%
@echo l
@
@if not exist bin mkdir bin
@if not exist obj mkdir obj
@
@set VERSION=0.3
@set OSTYPE=win32
@set CPPFLAGS=-Isource/include
@set CFLAGS=-Wall -g -c
@
@set COMPILE=%CPPFLAGS% %CFLAGS%
@
@
@%CC% %COMPILE% source\src\%OSTYPE%\CharString.c      -o obj\CharString.o
@echo source\src\%OSTYPE%\CharString.o
@
@%CC% %COMPILE% source\src\%OSTYPE%\IOPSA.c      -o obj\IOPSA.o
@echo source\src\%OSTYPE%\IOPSA.c
@
@%CC% %COMPILE% source\src\%OSTYPE%\ProcessPSA.c -o obj\ProcessPSA.o
@echo source\src\%OSTYPE%\ProcessPSA.c
@
@%CC% %COMPILE% source\src\%OSTYPE%\Provider.c   -o obj\Provider.o
@echo source\src\%OSTYPE%\Provider.c
@
@%CC% %COMPILE% source\src\Arguments.c      -o obj\Arguments.o
@echo source\src\Arguments.c
@
@%CC% %COMPILE% source\src\CurrentContext.c -o obj\CurrentContext.o
@echo source\src\CurrentContext.c
@
@%CC% %COMPILE% source\src\Global.c         -o obj\Global.o
@echo source\src\Global.c
@
@%CC% %COMPILE% source\src\Link.c           -o obj\Link.o
@echo source\src\Link.c
@
@%CC% %COMPILE% source\src\List.c           -o obj\List.o
@echo source\src\List.c
@
@%CC% %COMPILE% source\src\ProviderRegistry.c -o obj\ProviderRegistry.o
@echo source\src\ProviderRegistry.c
@
@%CC% %COMPILE% source\src\Stack.c          -o obj\Stack.o
@echo source\src\Stack.c
@
@%CC% %COMPILE% source\src\String.c         -o obj\String.o
@echo source\src\String.c
@
@%CC% %COMPILE% source\src\build.c          -o obj\build.o
@echo source\src\build.c
@
@%CC% %COMPILE% source\src\copyfiles.c      -o obj\copyfiles.o
@echo source\src\copyfiles.c
@
@%CC% %COMPILE% source\src\execute.c        -o obj\execute.o
@echo source\src\execute.c
@
@%CC% %COMPILE% source\src\linkify.c        -o obj\linkify.o
@echo source\src\linkify.c
@
@%CC% %COMPILE% source\src\main.c           -o obj\main.o
@echo source\src\main.c
@
@set MY_PSA=obj\IOPSA.o obj\ProcessPSA.o obj\Provider.o
@set MY_OBJ=obj\Arguments.o obj\CharString.o obj\CurrentContext.o obj\Global.o obj\Link.o obj\List.o obj\ProviderRegistry.o obj\Stack.o obj\String.o
@set MY_FNC=obj\build.o obj\copyfiles.o obj\execute.o obj\linkify.o obj\main.o
@
@%CC% %MY_PSA% %MY_OBJ% %MY_FNC% -o bin/build 
@echo bin\build.exe
@
@copy bin/build.exe %DEST%
:END

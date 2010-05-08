@REM
@REM	Batch file to compile build
@REM	

@set OCL_VERSION=1.0.14
@set LIB_VERSION=0.8.0rc7
@set SUFFIX=8

@set OSTYPE=windows
@set PLATFORM=win32

@set CC=gcc
@set AR=ar

@set LIBX=..\libislabs_build-%LIB_VERSION%
@set BASE=..\..\OpenOCL_PSA\libopenocl_base_psa-%OCL_VERSION%
@set   IO=..\..\OpenOCL_PSA\libopenocl_io_psa-%OCL_VERSION%
@set PROV=..\..\OpenOCL_PSA\libopenocl_provider_psa-%OCL_VERSION%

@set OPENOCL_HEADERS=-I%BASE%\include -I%IO%\include -I%BASE%\source\include -I%IO%\source\include -I%PROV%\include 
@set CPPFLAGS=-I%LIBX%\include -I%LIBX%\source\include -Isource/include %OPENOCL_HEADERS% -DEXT_VERSION=\"%LIB_VERSION%\"
@set CFLAGS=-c -DEMBED_PROVIDERS
@
@set FLAGS=%CPPFLAGS% %CFLAGS%

@REM
@REM	Create directories if not already there
@REM

@if not exist bin mkdir bin
@if not exist obj mkdir obj
@if not exist %LIBX%\obj mkdir %LIBX%\obj
@if not exist %LIBX%\lib mkdir %LIBX%\lib
@if not exist providers\islabs_darwin_apple-0.1\obj mkdir providers\islabs_darwin_apple-0.1\obj
@if not exist providers\islabs_linux_gnu_lammpi-0.1\obj mkdir providers\islabs_linux_gnu_lammpi-0.1\obj
@if not exist providers\islabs_linux_gnu_gnu-0.1\obj mkdir providers\islabs_linux_gnu_gnu-0.1\obj
@if not exist providers\islabs_win32_gnu-0.1\obj mkdir providers\islabs_win32_gnu-0.1\obj
@if not exist providers\islabs_win32_ms-0.1\obj mkdir providers\islabs_win32_ms-0.1\obj


@echo !
@echo !
@echo !		Compiling and linking with GNU %CC% and %AR%
@echo !

@REM
@REM	Compile object files
@REM

	@REM	build

@%CC% %FLAGS% source\src\main.c   -o obj\main.obj
@echo main.o

	@REM	base

@set FILE=CharString
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Clock
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Environment
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Math
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Process
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c    -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=System
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c    -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Time
@%CC% %FLAGS% %BASE%\source\src\%PLATFORM%\%FILE%.c    -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Bag
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=CRuntime
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=CharStringBase
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=CharStringList
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=EnvironmentBase
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Hashtable
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Index
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Iterator
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=List
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Map
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Node
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Sequence
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Set
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Sort
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Stack
@%CC% %FLAGS% %BASE%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o


	@REM	io

@set FILE=Directory
@%CC% %FLAGS% %IO%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=File
@%CC% %FLAGS% %IO%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Path
@%CC% %FLAGS% %IO%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o

@set FILE=Link
@%CC% %FLAGS% %IO%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=DirectoryBase
@%CC% %FLAGS% %IO%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=FileBase
@%CC% %FLAGS% %IO%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=PathBase
@%CC% %FLAGS% %IO%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o


	@REM	provider

@set FILE=Provider
@%CC% %FLAGS% %PROV%\source\src\%PLATFORM%\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o


	@REM	libislabs_build

@set FILE=Arguments
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Buildable
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=BuildManager
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=BuildParameters
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Command
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=CommandPool
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=CurrentContext
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Global
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Package
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=ProviderContext
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=ProviderRegistry
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=Suite
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=TestPackage
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=TestSuite
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o
@set FILE=ToolSet
@%CC% %FLAGS% %LIBX%\source\src\%FILE%.c     -o %LIBX%\obj\%FILE%.obj
@echo %FILE%.o

@REM
@REM	Compile providers
@REM

@%CC% %FLAGS% providers/islabs_darwin_apple-0.1/source/src/islabs_darwin_apple.c -o %LIBX%/obj/islabs_darwin_apple.obj
@echo islabs_darwin_apple.obj

@%CC% %FLAGS% providers/islabs_linux_gnu_lammpi-0.1/source/src/islabs_linux_gnu_lammpi.c -o %LIBX%/obj/islabs_linux_gnu_lammpi.obj
@echo islabs_linux_gnu_lammpi.obj

@%CC% %FLAGS% providers/islabs_linux_gnu_gnu-0.1/source/src/islabs_linux_gnu_gnu.c -o %LIBX%/obj/islabs_linux_gnu_gnu.obj
@echo islabs_linux_gnu_gnu.obj

@%CC% %FLAGS% providers/islabs_win32_gnu-0.1/source/src/islabs_win32_gnu.c -o %LIBX%/obj/islabs_win32_gnu.obj
@echo islabs_win32_gnu.obj

@%CC% %FLAGS% providers/islabs_win32_ms-0.1/source/src/islabs_win32_ms.c -o %LIBX%/obj/islabs_win32_ms.obj
@echo islabs_win32_ms.obj

@%CC% %FLAGS% providers/islabs_jdk_sun-0.1/source/src/islabs_jdk_sun.c -o %LIBX%/obj/islabs_jdk_sun.obj
@echo islabs_jdk_sun.obj

@REM
@REM	Create libislabs_build.lib
@REM

@%AR% rcs %LIBX%\lib\libislabs_build.lib %LIBX%\obj\*.obj
@echo %LIBX%\lib\libislabs_build.lib

@REM
@REM	Create build.exe
@REM

@%CC% obj\*.obj %LIBX%\lib\libislabs_build.lib -o bin\build%SUFFIX%.exe
@echo bin\build%SUFFIX%.exe
@goto END

:END

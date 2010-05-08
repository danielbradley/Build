@REM
@REM	Batch file to compile build
@REM	

@set OCL_VERSION=1.0.14
@set LIB_VERSION=0.8.0rc7
@set SUFFIX=8

@set OSTYPE=windows
@set PLATFORM=win32

@set CC=cl
@set AR=lib
@set LINKER=link

@set LIBX=..\libislabs_build-%LIB_VERSION%
@set BASE=..\..\OpenOCL_PSA\libopenocl_base_psa-%OCL_VERSION%
@set   IO=..\..\OpenOCL_PSA\libopenocl_io_psa-%OCL_VERSION%
@set PROV=..\..\OpenOCL_PSA\libopenocl_provider_psa-%OCL_VERSION%

@set OPENOCL_HEADERS=/I%BASE%\include /I%IO%\include /I%BASE%\source\include /I%IO%\source\include /I%PROV%\include 
@set CPPFLAGS=/I%LIBX%\include /I%LIBX%\source\include /Isource/include %OPENOCL_HEADERS% /DEXT_VERSION=\"%LIB_VERSION%\"
@REM set CFLAGS=/c /Ox
@set CFLAGS=/c /ZI /DEBUG /D_CRT_SECURE_NO_DEPRECATE /DEMBED_PROVIDERS
@REM For debugging /Zi /ZI /DEBUG
@
@set FLAGS=%CPPFLAGS% %CFLAGS%
@set LFLAGS=/DEBUG
@REM For debugging /DEBUG

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
@if not exist providers\islabs_jdk_sun-0.1\obj mkdir providers\islabs_jdk_sun-0.1\obj

@echo !
@echo !
@echo !		Compiling and linking with Microsoft %CC% and %AR%
@echo !

@REM
@REM	Compile object files
@REM

	@REM	build

@%CC% /nologo %FLAGS% source\src\main.c      /Foobj\main.obj


	@REM	base

@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\CharString.c	/Fo%LIBX%\obj\CharString.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\Clock.c	/Fo%LIBX%\obj\Clock.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\Environment.c	/Fo%LIBX%\obj\Environment.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\Math.c		/Fo%LIBX%\obj\Math.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\Process.c	/Fo%LIBX%\obj\Process.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\System.c	/Fo%LIBX%\obj\System.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\%PLATFORM%\Time.c		/Fo%LIBX%\obj\Time.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Bag.c			/Fo%LIBX%\obj\Bag.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\CRuntime.c		/Fo%LIBX%\obj\CRuntime.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\CharStringBase.c	/Fo%LIBX%\obj\CharStringBase.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\CharStringList.c	/Fo%LIBX%\obj\CharStringList.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\EnvironmentBase.c	/Fo%LIBX%\obj\EnvironmentBase.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Hashtable.c		/Fo%LIBX%\obj\Hashtable.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Index.c			/Fo%LIBX%\obj\Index.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Iterator.c		/Fo%LIBX%\obj\Iterator.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\List.c			/Fo%LIBX%\obj\List.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Map.c			/Fo%LIBX%\obj\Map.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Node.c			/Fo%LIBX%\obj\Node.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Sequence.c		/Fo%LIBX%\obj\Sequence.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Set.c			/Fo%LIBX%\obj\Set.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Sort.c			/Fo%LIBX%\obj\Sort.obj
@%CC% /nologo %FLAGS% %BASE%\source\src\Stack.c			/Fo%LIBX%\obj\Stack.obj

	@REM	io

@%CC% /nologo %FLAGS% %IO%\source\src\%PLATFORM%\Directory.c	/Fo%LIBX%\obj\Directory.obj
@%CC% /nologo %FLAGS% %IO%\source\src\%PLATFORM%\File.c		/Fo%LIBX%\obj\File.obj
@%CC% /nologo %FLAGS% %IO%\source\src\%PLATFORM%\Path.c		/Fo%LIBX%\obj\Path.obj

@%CC% /nologo %FLAGS% %IO%\source\src\Link.c			/Fo%LIBX%\obj\Link.obj
@%CC% /nologo %FLAGS% %IO%\source\src\DirectoryBase.c		/Fo%LIBX%\obj\DirectoryBase.obj
@%CC% /nologo %FLAGS% %IO%\source\src\FileBase.c		/Fo%LIBX%\obj\FileBase.obj
@%CC% /nologo %FLAGS% %IO%\source\src\PathBase.c		/Fo%LIBX%\obj\PathBase.obj

	@REM	provider

@%CC% /nologo %FLAGS% %PROV%\source\src\%PLATFORM%\Provider.c     /Fo%LIBX%\obj\Provider.obj

	@REM	libislabs_build

@%CC% /nologo %FLAGS% %LIBX%\source\src\Arguments.c		/Fo%LIBX%\obj\Arguments.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\Buildable.c		/Fo%LIBX%\obj\Buildable.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\BuildManager.c		/Fo%LIBX%\obj\BuildManager.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\BuildParameters.c	/Fo%LIBX%\obj\BuildParameters.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\Command.c		/Fo%LIBX%\obj\Command.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\CommandPool.c		/Fo%LIBX%\obj\CommandPool.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\CurrentContext.c	/Fo%LIBX%\obj\CurrentContext.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\Global.c		/Fo%LIBX%\obj\Global.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\Package.c		/Fo%LIBX%\obj\Package.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\ProviderContext.c	/Fo%LIBX%\obj\ProviderContext.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\ProviderRegistry.c	/Fo%LIBX%\obj\ProviderRegistry.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\Suite.c			/Fo%LIBX%\obj\Suite.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\TestPackage.c		/Fo%LIBX%\obj\TestPackage.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\TestSuite.c		/Fo%LIBX%\obj\TestSuite.obj
@%CC% /nologo %FLAGS% %LIBX%\source\src\ToolSet.c		/Fo%LIBX%\obj\ToolSet.obj

@REM
@REM	Compile providers
@REM

@%CC% /nologo %FLAGS% providers/islabs_darwin_apple-0.1/source/src/islabs_darwin_apple.c /Fo%LIBX%\obj\islabs_darwin_apple.obj
@%CC% /nologo %FLAGS% providers/islabs_linux_gnu_lammpi-0.1/source/src/islabs_linux_gnu_lammpi.c /Fo%LIBX%\obj\islabs_linux_gnu_lammpi.obj
@%CC% /nologo %FLAGS% providers/islabs_linux_gnu_gnu-0.1/source/src/islabs_linux_gnu_gnu.c /Fo%LIBX%\obj\islabs_linux_gnu_gnu.obj
@%CC% /nologo %FLAGS% providers/islabs_win32_gnu-0.1/source/src/islabs_win32_gnu.c /Fo%LIBX%\obj\islabs_win32_gnu.obj
@%CC% /nologo %FLAGS% providers/islabs_win32_ms-0.1/source/src/islabs_win32_ms.c /Fo%LIBX%\obj\islabs_win32_ms.obj
@%CC% /nologo %FLAGS% providers/islabs_jdk_sun-0.1/source/src/islabs_jdk_sun.c /Fo%LIBX%\obj\islabs_jdk_sun.obj

@REM
@REM	Create libislabs_build.lib
@REM

@%AR% /nologo /OUT:%LIBX%\lib\libislabs_build.lib %LIBX%\obj\*.obj
@echo %LIBX%\lib\libislabs_build.lib

@REM
@REM	Create build.exe
@REM

@%LINKER% /nologo %LFLAGS% obj\main.obj %LIBX%\lib\libislabs_build.lib /OUT:bin\build.exe
@%LINKER% /nologo %LFLAGS% obj\main.obj %LIBX%\lib\libislabs_build.lib /OUT:bin\build%SUFFIX%.exe

@goto END

:END

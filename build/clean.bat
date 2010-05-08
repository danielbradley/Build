@REM
@REM	This script removes all the object files, libraries and
@REM	shared objects created by "compile"
@REM

@set RM=del

@set VERSION=0.5
@set SUFFIX=5

@set LIBX=lib\libislabs_build-%VERSION%
@set BASE=lib\libopenocl_base_psa-1.0.x
@set IO=lib\libopenocl_io_psa-1.0.x
@set PROV=lib\libopenocl_provider_psa-1.0.x

@echo !
@echo !		Cleaning
@echo !

	@REM	base

@set FILE=CharString
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Clock
@%RM% %LIBX%\obj\%FILE%.obj

@REM @set FILE=Environment
@REM @%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Math
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Process
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=System
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Time
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Bag
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=CRuntime
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=CharStringBase
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=CharStringList
@%RM% %LIBX%\obj\%FILE%.obj

@REM @set FILE=EnvironmentBase
@REM @%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Hashtable
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Iterator
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Index
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=List
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Map
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Node
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Sequence
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Set
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Sort
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Stack
@%RM% %LIBX%\obj\%FILE%.obj

	@REM	io

@set FILE=Directory
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=File
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Path
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Link
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=DirectoryBase
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=FileBase
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=PathBase
@%RM% %LIBX%\obj\%FILE%.obj

	@REM	provider

@set FILE=Provider
@%RM% %LIBX%\obj\%FILE%.obj

	@REM	libislabs_build

@set FILE=Arguments
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Buildable
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=BuildManager
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=BuildParameters
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Command
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=CommandPool
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=CurrentContext
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Global
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Package
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=ProviderContext
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=ProviderRegistry
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=Suite
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=TestPackage
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=TestSuite
@%RM% %LIBX%\obj\%FILE%.obj

@set FILE=ToolSet
@%RM% %LIBX%\obj\%FILE%.obj

	@REM	build

@%RM% obj\main.obj

@REM
@REM	Compile providers
@REM

@%RM% %LIBX%\obj\islabs_darwin_apple.obj
@%RM% %LIBX%\obj\islabs_linux_gnu_lammpi.obj
@%RM% %LIBX%\obj\islabs_linux_gnu_gnu.obj
@%RM% %LIBX%\obj\islabs_win32_gnu.obj
@%RM% %LIBX%\obj\islabs_win32_ms.obj

@REM
@REM	Create libislabs_build.lib
@REM

@%RM% %LIBX%\lib\libislabs_build.lib

@REM
@REM	Create build.exe
@REM

@%RM% bin\build5.exe

@echo DONE


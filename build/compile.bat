@REM
@REM	Batch file to compile build
@REM	

@if !%CC% == !gcc goto GNU
@if !%CC% == !gcc.exe goto GNU
@if !%CC% == !cl goto MSVC
@if !%CC% == !cl.exe goto MSVC

@echo compile: No compiler found! Set CC environment variable
@goto END

:GNU
compile.gnu.bat
@goto END

:MSVC
@compile.msvc.bat
@goto END

:END

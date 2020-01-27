@ECHO OFF

IF "%1"=="" goto :help
IF "%2"=="" goto :help
IF NOT "%3"=="" IF "%4"=="" goto :help

:: Soundboard executable
set EXEC=%1

:: Sets up the directory structure for binarycreator
set PACK=installer\packaged\pack
set DATA=%PACK%\data
mkdir %PACK%\data
mkdir %PACK%\meta
copy installer\auto_uninstall.qs %PACK%\meta
copy installer\installscript.qs %PACK%\meta
copy installer\package.xml %PACK%\meta
copy installer\targetwidget.ui %PACK%\meta

:: Copies all the libs using the windowsdeployqt tool
%2 %EXEC% --no-quick-import --no-translations --no-system-d3d-compiler --no-webkit2 --no-angle --no-opengl-sw --dir %DATA%
copy %EXEC% %DATA%

echo ----------------------------------------------------
echo The program will now run to check for DLL errors.
echo ----------------------------------------------------
pause
goto :check
:fail
if not exist lib mkdir lib
echo The errors will tell you the DLL files you need, and
echo you will need to copy those files to Soundboard\lib,
echo (found in this git repo).
echo You will likely find them in the bin for the compiler
echo you used, located in your Qt installation.
echo e.g. for the 64bit MingW compiler, you may find them
echo in C:\Qt\5.12.6\mingw73_64\bin
pause
:check

if exist lib xcopy /s /y lib %DATA%

:: This simply removes the path from the executable and runs it
for /f "tokens=* delims= " %%F in ('echo %EXEC%') do (
	%DATA%\%%~nxF
)

:choice
set /P c=Did the program run successfully? (y/n)?
if /I "%c%" EQU "Y" goto :finish
if /I "%c%" EQU "N" goto :fail
goto :choice

:finish

if "%~3"=="" goto :firstend

set OUTPUT=SoundboardInstaller.exe
%3 -t %4 -p installer\packaged -c installer\config.xml %OUTPUT%

echo Finished! Created installer Soundboard/%OUTPUT% and output files to Soundboard\%DATA%
goto :end

:firstend
echo Finished! Output files to Soundboard\%DATA%
goto :end

:help
echo "windows.bat <soundboard executable> <windowsdeployqt.exe> [binarycreator.exe] [installerbase.exe]"

:end
pause
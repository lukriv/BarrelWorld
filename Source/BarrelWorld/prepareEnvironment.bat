@echo off
set dest=%1

if [%dest%] EQU [Debug] set externSubDir=win32
if [%dest%] EQU [Release] set externSubDir=win32
if [%dest%] EQU [Debug64] set externSubDir=x64
if [%dest%] EQU [Release64] set externSubDir=x64

if not defined externSubDir (
	echo externSubDir not defined
	exit /B 5
)

if [%externSubDir%] EQU [win32] set mingwDir=C:\Program Files (x86)\mingw-w64\i686-7.3.0-posix-sjlj-rt_v5-rev0\mingw32\bin
if [%externSubDir%] EQU [x64] set mingwDir=C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin

if not exist .\%dest%\Urho3D.dll copy ..\..\extern\bin\%externSubDir%\Urho3D.dll .\%dest%

if [%externSubDir%] EQU [win32] set mingwList=libgcc_s_sjlj-1.dll libstdc++-6.dll libwinpthread-1.dll
if [%externSubDir%] EQU [x64] set mingwList=libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll

for %%g in (%mingwList%) do (if not exist .\%dest%\%%g copy "%mingwDir%\%%g" .\%dest%)

if not exist .\%dest%\CoreData  (
	mkdir .\%dest%\CoreData
	xcopy ..\..\Resources\urho3d\CoreData .\%dest%\CoreData /S /Y /Q 
 )

set source=..\..\Resources\urho3d


if not exist .\%dest%\Data mkdir .\%dest%\Data

::@echo on
set fileList=UI.png StoneDiffuse.dds StoneNormal.dds TerrainWeights.dds TerrainDetail2.dds TerrainDetail3.dds
call :CopyList %dest% Data Textures %fileList%
::@echo off

set fileList="Anonymous Pro.ttf"
call :CopyList %dest% Data Fonts %fileList%

set fileList=Box.mdl
call :CopyList %dest% Data Models %fileList%

set fileList=Stone.xml
call :CopyList %dest% Data Materials %fileList%

set fileList=DefaultStyle.xml
call :CopyList %dest% Data UI %fileList%

set source=..\..\Resources
set barrelData=BarrelData
if not exist .\%dest%\%barrelData% mkdir .\%dest%\%barrelData%

::set fileList=AvatarMaterial.xml TerrainX.xml BaseMat.xml HandMat.xml LegMat.xml
::call :CopyList %dest% %barrelData% Materials %fileList%

::set fileList=Avatar.mdl Skeleton.mdl BodyClear.mdl idle.ani run.ani LeftHand.mdl RightHand.mdl
::call :CopyList %dest% %barrelData% Models %fileList%

::set fileList=barrelTexPrep.png webGrass.dds avatarFull2.png avatarFull2LeftHand.png
::call :CopyList %dest% %barrelData% Textures %fileList%

xcopy ..\..\Resources\BarrelData .\%dest%\%barrelData% /S /Y
 
goto :EOF

:CopyList
setlocal
set dest=%1
shift
set resourceDest=%1
shift
set destSubdir=%1
shift 
set fileList=%1
shift
:CopyListLoop
if not [%1]==[] (
	set fileList=%fileList% %1
	shift
	goto :CopyListLoop
)

if not exist .\%dest%\%resourceDest%\%destSubdir% (
	mkdir .\%dest%\%resourceDest%\%destSubdir% 
)

for %%g in (%fileList%) do ( call :CopyFile %source%\%resourceDest%\%destSubdir% .\%dest%\%resourceDest%\%destSubdir% %%g )

endlocal
EXIT /B 0

:CopyFolder
setlocal
set dest=%1
shift
set resourceDest=%1
shift
set destSubdir=%1
shift 
set fileList=%1
shift
:CopyListLoop
if not [%1]==[] (
	set fileList=%fileList% %1
	shift
	goto :CopyListLoop
)

if not exist .\%dest%\%resourceDest%\%destSubdir% (
	mkdir .\%dest%\%resourceDest%\%destSubdir% 
)

for %%g in (%fileList%) do ( call :CopyFile %source%\%resourceDest%\%destSubdir% .\%dest%\%resourceDest%\%destSubdir% %%g )

endlocal
EXIT /B 0

:CopyFile
setlocal
set sourceDir=%1
set destDir=%2
set fileName=%3

if not exist .\%destDir%\%fileName% (
	copy %sourceDir%\%fileName% .\%destDir%
	echo Copied file .\%destDir%\%fileName%
)

endlocal
EXIT /B 0

::xcopy ..\..\Resources\urho3d\coredata .\%dest% /S /T
::xcopy ..\..\Resources\urho3d\data .\%dest% /S /T
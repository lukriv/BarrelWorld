set dest=%1

if [%dest%] EQU [Debug] set externSubDir=win32
if [%dest%] EQU [Release] set externSubDir=win32
if [%dest%] EQU [Debug64] set externSubDir=x64
if [%dest%] EQU [Release64] set externSubDir=x64

if not defined externSubDir exit /B 5

if [%externSubDir%] EQU [win32] set mingwDir=C:\Program Files (x86)\mingw-w64\i686-7.2.0-posix-sjlj-rt_v5-rev0\mingw32\bin
if [%externSubDir%] EQU [x64] set mingwDir=C:\Program Files\mingw-w64\x86_64-7.2.0-posix-seh-rt_v5-rev0\mingw64\bin

if not exist .\%dest%\Urho3D.dll copy ..\..\extern\bin\%externSubDir%\Urho3D.dll .\%dest%

if [%externSubDir%] EQU [win32] set mingwList=libgcc_s_sjlj-1.dll libstdc++-6.dll libwinpthread-1.dll
if [%externSubDir%] EQU [x64] set mingwList=libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll

for %%g in (%mingwList%) do (if not exist .\%dest%\%%g copy "%mingwDir%\%%g" .\%dest%)

if not exist .\%dest%\Autoload  xcopy ..\..\Resources\urho3d .\%dest% /S /Y /Q
::xcopy ..\..\Resources\urho3d\coredata .\%dest% /S /T
::xcopy ..\..\Resources\urho3d\data .\%dest% /S /T
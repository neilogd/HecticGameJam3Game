C:\Python27\python.exe ..\Psybrus\reflection_parse.py DevelopmentGame
..\Psybrus\Tools\genie\genie.exe --os=windows --platform=x32 vs2013

cd Build\vs2013

rem Build Exes.
call "%VS120COMNTOOLS%\vsvars32.bat"
call msbuild HecticGameJam3Game.sln /t:Build /p:Configuration=Debug
call msbuild HecticGameJam3Game.sln /t:Build /p:Configuration=Production
cd ../..

rem Build Data.
cd Dist

..\Build\vs2013\bin\Debug\HecticGameJam3Game.exe ImportPackages

cd ..
rem Deploy.

mkdir Deploy
copy Build\vs2013\bin\Production\*.exe Deploy\*.exe
copy Dist\*.dll Deploy\*.dll

xcopy Dist\PackedContent\* Deploy\PackedContent /s /i

pause

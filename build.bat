del cmakeBuild\* /Q
del bin\* /Q
rmdir /s /q cmakeBuild
rmdir /s /q bin

mkdir cmakeBuild
cd cmakeBuild
cmake ../

msbuild AC_DataConcentrator.sln /property:Configuration=Debug
msbuild AC_DataConcentrator.sln /property:Configuration=Release

cd ..

mkdir bin
mkdir bin\debug
mkdir bin\release
xcopy cmakeBuild\debug\AC_DataConcentrator.exe bin\debug\ /i
xcopy cmakeBuild\debug\AC_DataConcentrator.pdb bin\debug\ /i

xcopy cmakeBuild\release\AC_DataConcentrator.exe bin\release\ /i

echo AC_DataConcentrator.exe -d ../../data/network/ > bin\debug\run.bat
echo AC_DataConcentrator.exe -d ../../data/network/ > bin\release\run.bat
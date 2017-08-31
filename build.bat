del cmakeBuild\* /Q
rmdir /s /q cmakeBuild

mkdir cmakeBuild
cd cmakeBuild
cmake ../

msbuild AC_DataConcentrator.sln
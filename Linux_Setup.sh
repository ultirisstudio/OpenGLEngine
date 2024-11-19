#!/bin/bash

mkdir temp\

wget -P temp https://github.com/premake/premake-core/releases/download/v5.0.0-beta3/premake-5.0.0-beta3-linux.tar.gz

cd temp

tar -xzvf premake-5.0.0-beta3-linux.tar.gz

cd ../

mkdir vendor/bin
mkdir vendor/bin/premake

cp -p temp/premake5 vendor/bin/premake/
cp -p temp/premake5 QuasarEngine-Editor/Assets/premake/

chmod a+x vendor/bin/premake/premake5

rm -rf temp\

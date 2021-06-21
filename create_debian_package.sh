#! /bin/bash

VER=$(cat .version)
BUILD_NR=$(cat .buildnumber)

rm -rf pkg/bibestepicsioc-$VER.$BUILD_NR


cd ..
mkdir -p bibestepicsioc-temp
cp -r best-epics-ioc/* bibestepicsioc-temp/
mkdir -p best-epics-ioc/pkg/bibestepicsioc-$VER.$BUILD_NR
cp -r bibestepicsioc-temp/* best-epics-ioc/pkg/bibestepicsioc-$VER.$BUILD_NR/
rm -rf bibestepicsioc-temp

cd best-epics-ioc/pkg/bibestepicsioc-$VER.$BUILD_NR

rm -rf .git
rm -rf bestApp/op/.metadata
rm -rf create_debian_package.sh
rm -rf debian

cd ..

tar czvf bibestepicsioc_$VER.$BUILD_NR.orig.tar.gz bibestepicsioc-$VER.$BUILD_NR

cp -r ../debian/ bibestepicsioc-$VER.$BUILD_NR

cd bibestepicsioc-$VER.$BUILD_NR

#debuild -us -uc
dpkg-buildpackage -rfakeroot -D -us -uc


#! /bin/bash

VER=1.0
BUILD_NR=$(cat build_number)
BUILD_NR=$(echo $BUILD_NR+1 | bc)

ORIG_DIR=$(pwd)

rm -rf ../bestepicsioc-$VER.$BUILD_NR
mkdir  ../bestepicsioc-$VER.$BUILD_NR

cp -r . ../bestepicsioc-$VER.$BUILD_NR

cd ../bestepicsioc-$VER.$BUILD_NR

rm -rf .git
rm -rf bestApp/op/.metadata
rm -rf create_debian_package.sh

cd ..

tar czvf bestepicsioc_$VER.$BUILD_NR.orig.tar.gz bestepicsioc-$VER.$BUILD_NR

cd bestepicsioc-$VER.$BUILD_NR

debuild -us -uc -k51895D5F &&

echo "$BUILD_NR" > $ORIG_DIR/build_number

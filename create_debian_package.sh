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

debuild -k95926773 -b

if [ $? -ne 0 ]; then
	exit 1
fi

echo "$BUILD_NR" > $ORIG_DIR/build_number

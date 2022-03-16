#! /bin/bash

# exit if some command fails
set -e

VER=$(cat .version)
BUILDNR=$(cat .buildnumber)
CURRENT_DIR=$(pwd)
NEW_NAME=bestepicsioc-$VER-$BUILDNR
UBUNTU_VER=$(lsb_release -sr)

echo "Package: $NEW_NAME"

mkdir -p pkg/bestepicsioc-$VER-$BUILDNR

cp -ar 	bestApp/ \
		best_epics_ioc.desktop \
        best_epics_ioc.png \
        configure/ \
        .buildnumber \
        iocBoot/ \
        Makefile \
        .version \
        .travis.yml \
		pkg/bestepicsioc-$VER-$BUILDNR

rm -rf pkg/bestepicsioc-$VER-$BUILDNR/bestApp/op/.metadata

cd pkg 
tar czvf bestepicsioc-$VER-$BUILDNR.orig.tar.gz bestepicsioc-$VER-$BUILDNR/
mkdir -p bestepicsioc-$VER-$BUILDNR/debian
cd ..

if [ $UBUNTU_VER = "14.04" ]
then
    cp -r ./debian-14/* pkg/bestepicsioc-$VER-$BUILDNR/debian
    cd pkg/bestepicsioc-$VER-$BUILDNR 
    debuild -us -uc 
    debuild -kBA3FBE17BF2D656B1F93D1896D47F3FD51895D5F -S -p"gpg --batch --passphrase $GPGPSW"
else
    cp -r ./debian-20/* pkg/bestepicsioc-$VER-$BUILDNR/debian
    cd pkg/bestepicsioc-$VER-$BUILDNR 
    debuild -us -uc 
    debuild -kBA3FBE17BF2D656B1F93D1896D47F3FD51895D5F -S -p"gpg --batch --passphrase $GPGPSW --pinentry-mode loopback"
fi

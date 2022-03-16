#! /bin/bash

# exit if some command fails
set -e

VER=$(cat .version)
BUILDNR=$(cat .buildnumber)
CURRENT_DIR=$(pwd)
NEW_NAME=bibestepicsioc-$VER-$BUILDNR
UBUNTU_VER=$(lsb_release -sr)

echo "Package: $NEW_NAME"

mkdir -p pkg/bibestepicsioc-$VER-$BUILDNR

cp -ar 	bestApp/ \
		best_epics_ioc.desktop \
        best_epics_ioc.png \
        configure/ \
        .buildnumber \
        iocBoot/ \
        Makefile \
        .version \
        .travis.yml \
		pkg/bibestepicsioc-$VER-$BUILDNR

rm -rf pkg/bibestepicsioc-$VER-$BUILDNR/bestApp/op/.metadata

cd pkg 
tar czvf bibestepicsioc-$VER-$BUILDNR.orig.tar.gz bibestepicsioc-$VER-$BUILDNR/
mkdir -p bibestepicsioc-$VER-$BUILDNR/debian
cd ..

if [ $UBUNTU_VER = "14.04" ]
then
    cp -r ./debian-14/* pkg/bibestepicsioc-$VER-$BUILDNR/debian
    cd pkg/bibestepicsioc-$VER-$BUILDNR 
    debuild -us -uc 
    debuild -kBA3FBE17BF2D656B1F93D1896D47F3FD51895D5F -S -p"gpg --batch --passphrase $GPGPSW"
else
    cp -r ./debian-20/* pkg/bibestepicsioc-$VER-$BUILDNR/debian
    cd pkg/bibestepicsioc-$VER-$BUILDNR 
    debuild -us -uc 
    debuild -kBA3FBE17BF2D656B1F93D1896D47F3FD51895D5F -S -p"gpg --batch --passphrase $GPGPSW --pinentry-mode loopback"
fi

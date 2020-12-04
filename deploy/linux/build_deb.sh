#!/bin/bash
cd ../../
ARCH=`uname -m`
echo "Retrieving latest from Git";
#git pull
VER=3.0

echo "Building PowerDEVS DEB package for $ARCH";
echo "Building Binaries";
#make -f Makefile clean
#if [ $# -eq 1 ]; then
#  make -f Makefile MODE=release RTAI=yes 
#else
#  make -f Makefile MODE=release 
#fi
rm -rf tmp_deb
mkdir tmp_deb
mkdir tmp
mkdir ./tmp_deb/opt
mkdir ./tmp_deb/opt/powerdevs
mkdir ./tmp_deb/opt/powerdevs/bin
mkdir ./tmp_deb/opt/powerdevs/3rd-party
mkdir ./tmp_deb/opt/powerdevs/library
mkdir ./tmp_deb/opt/powerdevs/examples
mkdir ./tmp_deb/opt/powerdevs/atomics
mkdir ./tmp_deb/opt/powerdevs/doc

echo "Installing Scilab"
cd ./tmp_deb/opt/powerdevs/3rd-party
#wget https://www.scilab.org/download/5.5.2/scilab-5.5.2.bin.linux-x86_64.tar.gz
cp ../../../../scilab-5.5.2.bin.linux-x86_64.tar.gz .
tar -xvf scilab-5.5.2.bin.linux-x86_64.tar.gz
rm scilab-5.5.2.bin.linux-x86_64.tar.gz 
cd ../../../../
echo "Done"

echo "Archiving repo"
#git archive powerdevs-dev  | bzip2 >pdevs-src.tar.bz2
git archive update-config-files  | bzip2 >pdevs-src.tar.bz2
cd ./tmp
mv ../pdevs-src.tar.bz2 .
tar -xvf pdevs-src.tar.bz2
cd ..
echo "Done"

cp -r ./tmp/deploy/linux/deb/* ./tmp_deb/
chmod 0755 tmp_deb/DEBIAN/post*
cat ./tmp_deb/DEBIAN/control.amd64 | awk -v VERSION="$VER" '{ if(index($0,"Version: ")>=1) print "Version: " VERSION ; else print $0;}' >  ./tmp_deb/DEBIAN/control
rm ./tmp_deb/DEBIAN/control.amd64; 

cp -r ./bin/* ./tmp_deb/opt/powerdevs/bin/
chmod 0755 `find tmp_deb/opt/powerdevs/bin`
mv ./tmp_deb/opt/powerdevs/bin/original.ini ./tmp_deb/opt/powerdevs/bin/powerdevs.ini

cp COPYING ./tmp_deb/opt/powerdevs
cp -r ./tmp/build ./tmp_deb/opt/powerdevs/build
cp -r ./tmp/engine ./tmp_deb/opt/powerdevs/engine
if [ $# -eq 1 ]; then
  cat engine/Makefile.include \
   | awk '{if (match($0,'/^CXX/')) print $0, " -DRTAIOS"; else print $0}' \
   | awk '{if (match($0,'/^LIBS/')) print $0, " -L/usr/realtime/lib -lpthread -llxrt -lm "; else print $0}' \
   | awk '{if (match($0,'/^INCLUDE/')) print $0, "-I/usr/realtime/include"; else print $0}' > tmp_deb/opt/powerdevs/engine/Makefile.include 
fi
cp -r ./tmp/library ./tmp_deb/opt/powerdevs/library
cp -r ./tmp/examples ./tmp_deb/opt/powerdevs/examples
cp -r ./tmp/atomics ./tmp_deb/opt/powerdevs/atomics
cp -r ./tmp/doc ./tmp_deb/opt/powerdevs/doc
#Extra binaries
cp -r ./tmp/output ./tmp_deb/opt/powerdevs/output
chmod 0644 `find tmp_deb/ -iname *.cpp`
chmod 0644 `find tmp_deb/ -iname *.c`
chmod 0644 `find tmp_deb/ -iname *.sce`
chmod 0644 `find tmp_deb/ -iname *.h`
chmod 0644 `find tmp_deb/ -iname *.ini`
chmod 0644 `find tmp_deb/ -iname *.txt`
chmod 0644 `find tmp_deb/ -iname *.png`
chmod 0644 `find tmp_deb/opt/powerdevs/engine/ -type f`
chmod 0644 `find tmp_deb/opt/powerdevs/library/ -type f`
chmod 0644 `find tmp_deb/opt/powerdevs/atomics/ -type f`
chmod 0644 `find tmp_deb/opt/powerdevs/build/ -type f`
chmod 0644 `find tmp_deb/opt/powerdevs/examples/ -type f`
chmod 0755 `find tmp_deb/ -type d`
fakeroot chown -R root:root tmp_deb/*
dpkg -b tmp_deb powerdevs.deb
mv powerdevs.deb powerdevs_amd64.deb
lintian -EvIL +pedantic -i -v powerdevs_*> build.log 
rm -rf tmp
rm -rf tmp_deb

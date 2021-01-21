#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

rm -rf tmp
mkdir tmp
cd tmp

cmake ../vendor/github.com/erincatto/box2d.git  \
	-DBOX2D_BUILD_DOCS=ON \
	-DBOX2D_BUILD_TESTBED=ON \
	-DBOX2D_BUILD_UNIT_TESTS=ON \
	-DBOX2D_USER_SETTINGS=OFF \
	-DBUILD_SHARED_LIBS=OFF \
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_INSTALL_PREFIX=$(pwd)/../system

make VERBOSE=1 -j8
make install

cd -
rm -rf tmp
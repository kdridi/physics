#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

rm -rf tmp
mkdir tmp
cd tmp

cmake ../vendor/github.com/machinezone/IXWebSocket.git \
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_INSTALL_PREFIX=$(pwd)/../system/${PLATFORM} \
	-DUSE_TLS=ON \
	-DUSE_ZLIB=ON

make VERBOSE=1 -j8
make install

cd -
rm -rf tmp
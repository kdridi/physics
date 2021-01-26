#!/usr/bin/env bash

set -e

cd $(dirname $0)/..

cd vendor/github.com/protocolbuffers/protobuf.git/cmake
git submodule update --init --recursive
cd -

rm -rf tmp
mkdir tmp
cd tmp

cmake ../vendor/github.com/protocolbuffers/protobuf.git/cmake \
	-DCMAKE_BUILD_TYPE=Debug \
	-Dprotobuf_BUILD_CONFORMANCE=OFF \
	-Dprotobuf_BUILD_EXAMPLES=ON \
	-Dprotobuf_BUILD_LIBPROTOC=ON \
	-Dprotobuf_BUILD_PROTOC_BINARIES=ON \
	-Dprotobuf_BUILD_SHARED_LIBS=OFF \
	-Dprotobuf_BUILD_TESTS=OFF \
	-Dprotobuf_INSTALL_EXAMPLES=ON \
	-Dprotobuf_MSVC_STATIC_RUNTIME=ON \
	-Dprotobuf_WITH_ZLIB=ON \
	-DCMAKE_INSTALL_PREFIX=$(pwd)/../system/${PLATFORM}

make VERBOSE=1 -j8
make install

cd -
rm -rf tmp
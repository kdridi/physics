#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

scripts/XX-protobuf.sh

rm -rf system/$(uname)/makefile
mkdir -p system/$(uname)/makefile
cd system/$(uname)/makefile

cmake ../../.. \
	-G "Unix Makefiles"
make VERBOSE=1 -j8
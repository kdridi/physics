#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

scripts/XX-protobuf.sh

rm -rf system/$(uname)/xcode
mkdir -p system/$(uname)/xcode
cd system/$(uname)/xcode

cmake ../../.. \
	-G Xcode

open *.xcodeproj

# make install

# cd -
# rm -rf tmp
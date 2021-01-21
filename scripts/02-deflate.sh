#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

cd vendor/github.com/ebiggers/libdeflate.git

make PREFIX=$(pwd)/../system -j8 realclean
make PREFIX=$(pwd)/../system -j8 all
make PREFIX=$(pwd)/../system -j8 install

cd -
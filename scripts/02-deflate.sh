#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

rm -rf tmp
mkdir tmp
cd tmp

make -C ../vendor/github.com/ebiggers/libdeflate.git install PREFIX=$(pwd)/../system

cd -
rm -rf tmp
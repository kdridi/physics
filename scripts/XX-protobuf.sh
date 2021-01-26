#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

cd src
rm -rf proto
mkdir proto
rm -rf proto.js
mkdir -p proto.js
for file in *.proto; do
	../system/$(uname)/bin/protoc --cpp_out=proto "$file"
	cp "${file}" proto.js/
done
rm -rf ../assets/public/proto
mv proto.js ../assets/public/proto
cd ..
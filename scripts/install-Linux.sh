#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

export PLATFORM=$(uname)

rm -rf system/${PLATFORM}
scripts/01-box2d.sh
scripts/02-deflate.sh
scripts/03-IXWebSocket.sh
scripts/04-protobuf.sh
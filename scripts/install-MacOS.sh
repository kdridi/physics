#!/usr/bin/env bash

set -e
set -x

cd $(dirname $0)/..

export PLATFORM=$(uname)

rm -rf system/${PLATFORM}
scripts/01-box2d.sh
scripts/02-deflate.sh
scripts/03-IXWebSocket.sh

sed -i -e 's,-framework foundation;-framework security,-framework Foundation;-framework Security,' \
    system/${PLATFORM}/lib/cmake/ixwebsocket/ixwebsocket-config.cmake
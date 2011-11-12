#!/bin/sh
echo $0
pushd trunk
./autogen.sh && ./configure --enable-static
popd

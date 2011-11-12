#!/bin/bash
rm -Rf protobuf-j2me
rsync -aC debian/* protobuf-j2me
#cp ../../out/Default/protoc-gen-j2me protobuf-j2me/bin
cp ../../out/Default/protoc-j2me protobuf-j2me/bin
gzip --best protobuf-j2me/usr/share/doc/protobuf-j2me/changelog
gzip --best protobuf-j2me/usr/share/doc/protobuf-j2me/changelog.Debian
find protobuf-j2me -type d | xargs chmod 755
find protobuf-j2me/bin -type f | xargs chmod 755
find protobuf-j2me/usr/share/doc -type f | xargs chmod 644
fakeroot dpkg -b protobuf-j2me protobuf-j2me.deb
lintian protobuf-j2me.deb

#!/bin/bash
rm -Rf protobuf-j2me
rsync -aC debian/* protobuf-j2me
cp ../../out/Default/protoc-gen-j2me protobuf-j2me/usr/bin

pushd protobuf-j2me
sed -i "s/#ARCHITECTURE/amd64/g" DEBIAN/control
size=`du --apparent-size --block-size=1k --summarize usr/ | cut -f 1`
sed -i "s/#PACKAGE_SIZE/$size/g" DEBIAN/control
find usr/ -type f | xargs md5sum > DEBIAN/md5sums
find . -type f | xargs chmod 644
find . -type d | xargs chmod 755
find usr/bin -type f | xargs chmod 755
popd
fakeroot dpkg -b protobuf-j2me protobuf-j2me.deb
lintian protobuf-j2me.deb

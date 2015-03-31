The following intructions are known to work under Ubuntu 11.04 LTS.

## Requirements ##

Follow [depot\_tools installation instructions](http://dev.chromium.org/developers/how-tos/install-depot-tools).

Also, run the following command to install required debian packages:
```
sudo apt-get install ant ...
```

## Get source code ##

Setup a git clone:
```
git clone http://code.google.com/p/protobuf-j2me/ protobuf-j2me
cd protobuf-j2me
gclient sync
```

## Build ##
```
cd src
./gyp.sh
make
```

Output files can be found under `protobuf-j2me/src/out/Default`
#!/bin/sh
export PATH=out/Default:$PATH

out/Default/protoc-j2me \
  --proto_path=example \
  example/example1.proto \
  --j2me_out=example/example1 \

out/Default/protoc-j2me \
  --proto_path=example \
  example/example2.proto \
  --j2me_out=example/example2 \

{
  'targets': [
    {
      'target_name': 'protobuf-j2me',
      'type': 'none',
      'dependencies': [
        '../plugin/build.gyp:protoc-j2me',
      ],
      'actions': [
        {
          'action_name': 'ant',
          'inputs': [
            '../plugin/src/google/protobuf/j2me.proto',
            'src/com/google/protobuf/BlockingRpcChannel.java',
            'src/com/google/protobuf/ByteString.java',
            'src/com/google/protobuf/CodedInputStream.java',
            'src/com/google/protobuf/CodedOutputStream.java',
            'src/com/google/protobuf/EnumType.java',
            'src/com/google/protobuf/Extension.java',
            'src/com/google/protobuf/Message.java',
            'src/com/google/protobuf/RpcCallback.java',
            'src/com/google/protobuf/RpcChannel.java',
            'src/com/google/protobuf/RpcController.java',
            'src/com/google/protobuf/ServiceException.java',
            'src/com/google/protobuf/WireFormat.java',
          ],
          'outputs': [
            '<(PRODUCT_DIR)/protobuf-j2me.jar',
          ],
          'action': [
            'ant',
            '-Dprotoc=<(PRODUCT_DIR)/protoc-j2me',
            '-Dbuild_dir=<(INTERMEDIATE_DIR)/protobuf-j2me/src/runtime',
            '-Ddeploy_dir=<(PRODUCT_DIR)',
          ],
        },
      ],
    },
  ],
}

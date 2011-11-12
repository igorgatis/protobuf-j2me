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
          'inputs': [ ],
          'outputs': [ 'always_execute_this_action', ],
          'action': [
            'ant', 'preverify',
            '-Dbuild_dir=<(INTERMEDIATE_DIR)/protobuf-j2me/src/runtime',
            '-Ddeploy_dir=<(PRODUCT_DIR)',
          ],
        },
      ],
    },
    {
      'target_name': 'build',
      'type': 'none',
      'dependencies': [
        'protobuf-j2me',
      ],
    },
  ],
}

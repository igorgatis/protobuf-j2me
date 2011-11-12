# vim: ft=python
{
  'variables': {
    'THIRD_PARTY': '../../third_party',
  }
  'targets': [
    {
      'target_name': 'protobuf-j2me-tests',
      'type': 'none',
      'dependencies': [
        '../plugin/build.gyp:protoc-j2me',
        '../runtime/build.gyp:protobuf-j2me-runtime',
        '<(THIRD_PARTY)/protobuf/build.gyp:protobuf-java',
      ],
      'actions': [
        {
          'action_name': 'ant',
          'inputs': [ ],
          'outputs': [ 'always_execute_this_action' ],
          'action': [
            'ant',
            '-Dbuild_dir=<(INTERMEDIATE_DIR)/protobuf-j2me/src/tests',
            '-Ddeploy_dir=<(PRODUCT_DIR)',
          ],
        },
      ],
    },
    {
      'target_name': 'test',
      'type': 'none',
      'dependencies': [
        'protobuf-j2me-tests',
      ],
    },
  ],
}

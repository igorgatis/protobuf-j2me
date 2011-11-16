# vim: ft=python
{
  'variables': {
    'THIRD_PARTY': '../../third_party',
  },
  'targets': [
    {
      'target_name': 'protobuf-j2me-tests',
      'type': 'none',
      'dependencies': [
        '../plugin/build.gyp:protoc-j2me',
        '../runtime/build.gyp:protobuf-j2me',
        '<(THIRD_PARTY)/protobuf/build.gyp:protobuf-java',
      ],
      'actions': [
        {
          'action_name': 'ant',
          'inputs': [
            'common/res/HelloWorld.proto',
            'common/src/MANIFEST.MF',
            'common/src/TestDriver.java',
            'common/src/TestSet.java',
            'common/src/UnitTest.java',
            'j2me/src/HelloWorldTests.java',
            'j2me/src/J2meUnitTestAdapter.java',
            'j2me/src/Main.java',
            'java/src/HelloWorldTests.java',
            'java/src/JavaUnitTestAdapter.java',
            'java/src/Main.java',
          ],
          'outputs': [
            'protobuf-j2me.runtime-tests_j2me.jar',
            'protobuf-j2me.runtime-tests_java.jar',
          ],
          'action': [
            'ant',
            '-Dprotoc=<(PRODUCT_DIR)/protoc-j2me',
            '-Dbuild_dir=<(INTERMEDIATE_DIR)/protobuf-j2me/src/tests',
            '-Ddeploy_dir=<(PRODUCT_DIR)',
          ],
        },
      ],
    },
  ],
}

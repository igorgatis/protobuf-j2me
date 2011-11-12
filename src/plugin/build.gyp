# vim: ft=python
{
  'variables': {
    'THIRD_PARTY': '../../third_party',
  },
  'targets': [
    {
      'target_name': 'protobuf-descriptors',
      'type': 'none',
      'dependencies': [
        '<(THIRD_PARTY)/protobuf/build.gyp:protoc',
      ],
      'actions': [
        {
          'action_name': 'embed_descriptors',
          'inputs': [
            'src/google/protobuf/descriptor.proto',
            'src/google/protobuf/j2me.proto',
          ],
          'outputs': [
            'src/google/protobuf/descriptor.proto.h',
            'src/google/protobuf/j2me.proto.h',
          ],
          'action': [
            'python', './embed.py',
            'src/google/protobuf/descriptor.proto',
            'src/google/protobuf/j2me.proto',
          ],
        },
        {
          'action_name': 'compile_descriptors',
          'inputs': [
            'src/google/protobuf/j2me.proto',
          ],
          'outputs': [
            'src/google/protobuf/j2me.pb.cc',
            'src/google/protobuf/j2me.pb.h',
          ],
          'action': [
            '../out/Default/protoc',
            '--proto_path=src',
            'src/google/protobuf/j2me.proto',
            '--cpp_out=src',
          ],
        },
      ],
    },
    {
      'target_name': 'libprotobuf-j2me',
      'type': 'static_library',
      'include_dirs': [
        'src',
      ],
      'dependencies': [
        '<(THIRD_PARTY)/protobuf/build.gyp:libprotobuf',
        'protobuf-descriptors',
      ],
      'export_dependent_settings': [
        '<(THIRD_PARTY)/protobuf/build.gyp:libprotobuf',
        'protobuf-descriptors',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'src',
        ],
      },
      'sources': [
        'src/google/protobuf/compiler/j2me/j2me_enum.cc',
        'src/google/protobuf/compiler/j2me/j2me_enum.h',
        'src/google/protobuf/compiler/j2me/j2me_enum_field.cc',
        'src/google/protobuf/compiler/j2me/j2me_enum_field.h',
        'src/google/protobuf/compiler/j2me/j2me_extension.cc',
        'src/google/protobuf/compiler/j2me/j2me_extension.h',
        'src/google/protobuf/compiler/j2me/j2me_field.cc',
        'src/google/protobuf/compiler/j2me/j2me_field.h',
        'src/google/protobuf/compiler/j2me/j2me_file.cc',
        'src/google/protobuf/compiler/j2me/j2me_file.h',
        'src/google/protobuf/compiler/j2me/j2me_generator.cc',
        'src/google/protobuf/compiler/j2me/j2me_generator.h',
        'src/google/protobuf/compiler/j2me/j2me_helpers.cc',
        'src/google/protobuf/compiler/j2me/j2me_helpers.h',
        'src/google/protobuf/compiler/j2me/j2me_message.cc',
        'src/google/protobuf/compiler/j2me/j2me_message.h',
        'src/google/protobuf/compiler/j2me/j2me_message_field.cc',
        'src/google/protobuf/compiler/j2me/j2me_message_field.h',
        'src/google/protobuf/compiler/j2me/j2me_primitive_field.cc',
        'src/google/protobuf/compiler/j2me/j2me_primitive_field.h',
        'src/google/protobuf/compiler/j2me/j2me_service.cc',
        'src/google/protobuf/compiler/j2me/j2me_service.h',
        'src/google/protobuf/descriptor.proto.h',
        'src/google/protobuf/j2me.pb.cc',
        'src/google/protobuf/j2me.pb.h',
        'src/google/protobuf/j2me.proto.h',
      ],
    },
    {
      'target_name': 'protoc-gen-j2me',
      'type': 'executable',
      'cflags': [ '-O2', '-g', '-Wall' ],
      'include_dirs': [
        'src',
      ],
      'dependencies': [
        '<(THIRD_PARTY)/protobuf/build.gyp:libprotoc',
        'libprotobuf-j2me',
      ],
      'sources': [
        'src/google/protobuf/compiler/j2me/j2me_plugin_main.cc',
      ],
    },
    {
      'target_name': 'protoc-j2me',
      'type': 'executable',
      'cflags': [ '-O2', '-g', '-Wall' ],
      'include_dirs': [
        'src',
      ],
      'cflags': [ '-O2', '-g', '-Wall' ],
      'dependencies': [
        '<(THIRD_PARTY)/protobuf/build.gyp:libprotoc',
        'libprotobuf-j2me',
      ],
      'sources': [
        'src/google/protobuf/compiler/j2me/j2me_main.cc',
      ],
    },
    #{
    #  'target_name': 'protobuf-protos',
    #  'type': 'none',
    #  'copies': [
    #    {
    #      'destination': '<(PRODUCT_DIR)/descriptors/google/protobuf',
    #      'files': [
    #        'src/google/protobuf/descriptor.proto',
    #        'src/google/protobuf/j2me.proto',
    #      ],
    #    },
    #  ],
    #},
    {
      'target_name': 'build',
      'type': 'none',
      'dependencies': [
        'protoc-gen-j2me',
        'protoc-j2me',
      ],
    },
  ],
}

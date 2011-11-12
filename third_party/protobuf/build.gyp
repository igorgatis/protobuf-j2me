{
  'variables': {
    'nobase_include_HEADERS': [
      'trunk/config.h',
      'trunk/src/google/protobuf/stubs/common.h',
      'trunk/src/google/protobuf/stubs/once.h',
      'trunk/src/google/protobuf/descriptor.h',
      'trunk/src/google/protobuf/descriptor.pb.h',
      'trunk/src/google/protobuf/descriptor_database.h',
      'trunk/src/google/protobuf/dynamic_message.h',
      'trunk/src/google/protobuf/extension_set.h',
      'trunk/src/google/protobuf/generated_message_util.h',
      'trunk/src/google/protobuf/generated_message_reflection.h',
      'trunk/src/google/protobuf/message.h',
      'trunk/src/google/protobuf/message_lite.h',
      'trunk/src/google/protobuf/reflection_ops.h',
      'trunk/src/google/protobuf/repeated_field.h',
      'trunk/src/google/protobuf/service.h',
      'trunk/src/google/protobuf/text_format.h',
      'trunk/src/google/protobuf/unknown_field_set.h',
      'trunk/src/google/protobuf/wire_format.h',
      'trunk/src/google/protobuf/wire_format_lite.h',
      'trunk/src/google/protobuf/wire_format_lite_inl.h',
      'trunk/src/google/protobuf/io/coded_stream.h',
      'trunk/src/google/protobuf/io/gzip_stream.h', #GZHEADERS
      'trunk/src/google/protobuf/io/printer.h',
      'trunk/src/google/protobuf/io/tokenizer.h',
      'trunk/src/google/protobuf/io/zero_copy_stream.h',
      'trunk/src/google/protobuf/io/zero_copy_stream_impl.h',
      'trunk/src/google/protobuf/io/zero_copy_stream_impl_lite.h',
      'trunk/src/google/protobuf/compiler/code_generator.h',
      'trunk/src/google/protobuf/compiler/command_line_interface.h',
      'trunk/src/google/protobuf/compiler/importer.h',
      'trunk/src/google/protobuf/compiler/parser.h',
      'trunk/src/google/protobuf/compiler/plugin.h',
      'trunk/src/google/protobuf/compiler/plugin.pb.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_generator.h',
      'trunk/src/google/protobuf/compiler/java/java_generator.h',
      'trunk/src/google/protobuf/compiler/python/python_generator.h',
    ],
    'libprotobuf_lite_la_SOURCES': [
      'trunk/src/google/protobuf/stubs/common.cc',
      'trunk/src/google/protobuf/stubs/once.cc',
      'trunk/src/google/protobuf/stubs/hash.h',
      'trunk/src/google/protobuf/stubs/map-util.h',
      'trunk/src/google/protobuf/stubs/stl_util-inl.h',
      'trunk/src/google/protobuf/extension_set.cc',
      'trunk/src/google/protobuf/generated_message_util.cc',
      'trunk/src/google/protobuf/message_lite.cc',
      'trunk/src/google/protobuf/repeated_field.cc',
      'trunk/src/google/protobuf/wire_format_lite.cc',
      'trunk/src/google/protobuf/io/coded_stream.cc',
      'trunk/src/google/protobuf/io/coded_stream_inl.h',
      'trunk/src/google/protobuf/io/zero_copy_stream.cc',
      'trunk/src/google/protobuf/io/zero_copy_stream_impl_lite.cc',
    ],
    'libprotobuf_la_SOURCES': [
      '<@(libprotobuf_lite_la_SOURCES)',
      'trunk/src/google/protobuf/stubs/strutil.cc',
      'trunk/src/google/protobuf/stubs/strutil.h',
      'trunk/src/google/protobuf/stubs/substitute.cc',
      'trunk/src/google/protobuf/stubs/substitute.h',
      'trunk/src/google/protobuf/stubs/structurally_valid.cc',
      'trunk/src/google/protobuf/descriptor.cc',
      'trunk/src/google/protobuf/descriptor.pb.cc',
      'trunk/src/google/protobuf/descriptor_database.cc',
      'trunk/src/google/protobuf/dynamic_message.cc',
      'trunk/src/google/protobuf/extension_set_heavy.cc',
      'trunk/src/google/protobuf/generated_message_reflection.cc',
      'trunk/src/google/protobuf/message.cc',
      'trunk/src/google/protobuf/reflection_ops.cc',
      'trunk/src/google/protobuf/service.cc',
      'trunk/src/google/protobuf/text_format.cc',
      'trunk/src/google/protobuf/unknown_field_set.cc',
      'trunk/src/google/protobuf/wire_format.cc',
      'trunk/src/google/protobuf/io/gzip_stream.cc',
      'trunk/src/google/protobuf/io/printer.cc',
      'trunk/src/google/protobuf/io/tokenizer.cc',
      'trunk/src/google/protobuf/io/zero_copy_stream_impl.cc',
      'trunk/src/google/protobuf/compiler/importer.cc',
      'trunk/src/google/protobuf/compiler/parser.cc',
    ],
    'libprotoc_la_SOURCES': [
      'trunk/src/google/protobuf/compiler/code_generator.cc',
      'trunk/src/google/protobuf/compiler/command_line_interface.cc',
      'trunk/src/google/protobuf/compiler/plugin.cc',
      'trunk/src/google/protobuf/compiler/plugin.pb.cc',
      'trunk/src/google/protobuf/compiler/subprocess.cc',
      'trunk/src/google/protobuf/compiler/subprocess.h',
      'trunk/src/google/protobuf/compiler/zip_writer.cc',
      'trunk/src/google/protobuf/compiler/zip_writer.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_enum.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_enum.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_enum_field.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_enum_field.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_extension.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_extension.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_field.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_field.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_file.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_file.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_generator.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_helpers.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_helpers.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_message.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_message.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_message_field.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_message_field.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_primitive_field.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_primitive_field.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_service.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_service.h',
      'trunk/src/google/protobuf/compiler/cpp/cpp_string_field.cc',
      'trunk/src/google/protobuf/compiler/cpp/cpp_string_field.h',
      'trunk/src/google/protobuf/compiler/java/java_enum.cc',
      'trunk/src/google/protobuf/compiler/java/java_enum.h',
      'trunk/src/google/protobuf/compiler/java/java_enum_field.cc',
      'trunk/src/google/protobuf/compiler/java/java_enum_field.h',
      'trunk/src/google/protobuf/compiler/java/java_extension.cc',
      'trunk/src/google/protobuf/compiler/java/java_extension.h',
      'trunk/src/google/protobuf/compiler/java/java_field.cc',
      'trunk/src/google/protobuf/compiler/java/java_field.h',
      'trunk/src/google/protobuf/compiler/java/java_file.cc',
      'trunk/src/google/protobuf/compiler/java/java_file.h',
      'trunk/src/google/protobuf/compiler/java/java_generator.cc',
      'trunk/src/google/protobuf/compiler/java/java_helpers.cc',
      'trunk/src/google/protobuf/compiler/java/java_helpers.h',
      'trunk/src/google/protobuf/compiler/java/java_message.cc',
      'trunk/src/google/protobuf/compiler/java/java_message.h',
      'trunk/src/google/protobuf/compiler/java/java_message_field.cc',
      'trunk/src/google/protobuf/compiler/java/java_message_field.h',
      'trunk/src/google/protobuf/compiler/java/java_primitive_field.cc',
      'trunk/src/google/protobuf/compiler/java/java_primitive_field.h',
      'trunk/src/google/protobuf/compiler/java/java_service.cc',
      'trunk/src/google/protobuf/compiler/java/java_service.h',
      'trunk/src/google/protobuf/compiler/java/java_string_field.cc',
      'trunk/src/google/protobuf/compiler/java/java_string_field.h',
      'trunk/src/google/protobuf/compiler/python/python_generator.cc',
    ],
  },
  'targets': [
    {
      'target_name': 'protobuf_config',
      'type': 'none',
      'actions': [
        {
          'action_name': 'config',
          'inputs': [
            'trunk/configure.ac',
          ],
          'outputs': [
            'trunk/Makefile',
            'trunk/Makefile.in',
            'trunk/config.h',
            'trunk/config.h.in',
            'trunk/configure',
            #'trunk/gtest/Makefile',
            #'trunk/gtest/build-aux/config.h',
            #'trunk/gtest/scripts/gtest-config',
            'trunk/protobuf-lite.pc',
            'trunk/protobuf.pc',
            'trunk/src/Makefile',
          ],
          'action': ['/bin/sh', 'config.sh'],
        },
      ],
    },
    {
      'target_name': 'libprotobuf_lite',
      'type': 'static_library',
      'cflags': [
        '-Wno-deprecated',
      ],
      'ldflags': [
        '-pthread',
        '-no-undefined',
        '-export-dynamic',
        '-version-info 7:0:0',
      ],
      'include_dirs': [
        'trunk',
        'trunk/src',
      ],
      'sources': [
        '<@(nobase_include_HEADERS)',
        '<@(libprotobuf_lite_la_SOURCES)',
      ],
      'dependencies': [
        ':protobuf_config',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'trunk',
          'trunk/src',
        ],
        'cflags': [
          '-Wno-deprecated',
        ],
        'ldflags': [
          '-pthread',
        ],
      },
    },
    {
      'target_name': 'libprotobuf',
      'type': 'static_library',
      'cflags': [
        '-Wno-deprecated',
      ],
      'ldflags': [
        '-pthread',
        '-no-undefined',
        '-export-dynamic',
        '-version-info 7:0:0',
      ],
      'include_dirs': [
        'trunk',
        'trunk/src',
      ],
      'sources': [
        '<@(nobase_include_HEADERS)',
        '<@(libprotobuf_la_SOURCES)',
      ],
      'dependencies': [
        ':protobuf_config',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'trunk',
          'trunk/src',
        ],
        'cflags': [
          '-Wno-deprecated',
        ],
        'ldflags': [
          '-pthread',
        ],
      },
    },
    {
      'target_name': 'libprotoc',
      'type': 'static_library',
      'include_dirs': [
        'trunk',
        'trunk/src',
      ],
      'sources': [
        '<@(nobase_include_HEADERS)',
        '<@(libprotoc_la_SOURCES)',
      ],
      'dependencies': [
        ':libprotobuf',
      ],
      'export_dependent_settings': [
        ':libprotobuf',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'trunk',
          'trunk/src',
        ],
      },
    },
    {
      'target_name': 'protoc',
      'type': 'executable',
      'include_dirs': [
        'trunk',
        'trunk/src',
      ],
      'sources': [
        'trunk/src/google/protobuf/compiler/main.cc',
      ],
      'dependencies': [
        ':libprotoc',
      ],
    },
    {
      'target_name': 'protobuf-java',
      'type': 'none',
      'dependencies': [
        ':protoc',
      ],
      'actions': [
        {
          'action_name': 'ant',
          'inputs': [ ],
          'outputs': [ 'always_execute_this_action', ],
          'action': [
            'ant',
            '-Dprotoc=<(PRODUCT_DIR)/protoc',
            '-Dbuild_dir=<(INTERMEDIATE_DIR)/protobuf/trunk/java',
            '-Ddeploy_dir=<(PRODUCT_DIR)',
          ],
        },
      ],
    },
  ],
}

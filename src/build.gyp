# vim: ft=python
{
  'targets': [
    {
      'target_name': 'plugin',
      'type': 'none',
      'dependencies': [
        'plugin/build.gyp:*',
      ],
    },
    {
      'target_name': 'runtime',
      'type': 'none',
      'dependencies': [
        'runtime/build.gyp:*',
      ],
    },
    {
      'target_name': 'build',
      'type': 'none',
      'dependencies': [
        'plugin',
        'runtime',
      ],
    },
    {
      'target_name': 'test',
      'type': 'none',
      'dependencies': [
        'tests/build.gyp:*',
      ],
    },
  ],
}

#!/usr/bin/python

import os
import os.path
import re
import sys

mapping = {
  '\\': '\\\\',
  '"': '\\"',
  '\n': '\\n',
}

for file in sys.argv[1:]:
  out = open('%s.h' % file, 'w')
  out.write('/* automatically generated - do not edit. */\n')
  name = re.sub(r'\W', r'_', os.path.basename(file))
  out.write('#include <string>\n')
  out.write('static const std::string k_%s_content =\n' % name)
  for line in open(file):
    out.write('  "')
    for ch in line:
      if ch in mapping:
        out.write(mapping[ch])
      else:
        out.write(ch)
    out.write('"\n')
  out.write('  "";\n')

// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <map>
#include <string>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/j2me.pb.h>
#include "j2me_enum.h"
#include "j2me_helpers.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

using google::protobuf::j2me::cstyle_enum;

EnumGenerator::EnumGenerator(const EnumDescriptor* descriptor)
  : descriptor_(descriptor) {
  for (int i = 0; i < descriptor_->value_count(); i++) {
    const EnumValueDescriptor* value = descriptor_->value(i);
    const EnumValueDescriptor* canonical_value =
      descriptor_->FindValueByNumber(value->number());

    if (value == canonical_value) {
      canonical_values_.push_back(value);
    } else {
      Alias alias;
      alias.value = value;
      alias.canonical_value = canonical_value;
      aliases_.push_back(alias);
    }
  }
}

EnumGenerator::~EnumGenerator() {}

void EnumGenerator::Generate(io::Printer* printer) {
  if (descriptor_->options().GetExtension(cstyle_enum)) {
    printer->Print(
      "// $classname$ enum.\n",
      "classname", descriptor_->name());
    for (int i = 0; i < canonical_values_.size(); i++) {
      printer->Print(
        "public static final int $name$ = $number$;\n",
        "name", canonical_values_[i]->name(),
        "number", SimpleItoa(canonical_values_[i]->number()));
    }
    printer->Print("\n");
    return;
  }

  bool is_own_file =
      descriptor_->containing_type() == NULL &&
      descriptor_->file()->options().java_multiple_files();
  printer->Print(
    "public $static$ final class $classname$\n"
    "    extends com.google.protobuf.EnumType {\n",
    "classname", descriptor_->name(),
    "static", is_own_file ? "" : "static");
  printer->Indent();

  for (int i = 0; i < canonical_values_.size(); i++) {
    map<string, string> vars;
    vars["name"] = canonical_values_[i]->name();
    vars["number"] = SimpleItoa(canonical_values_[i]->number());
    vars["classname"] = descriptor_->name();
    printer->Print(vars,
      "public static final int $name$_VALUE = $number$;\n"
      "public static final $classname$ $name$ = \n"
      "    new $classname$($name$_VALUE, \"$name$\");\n");
  }

  // -----------------------------------------------------------------

  for (int i = 0; i < aliases_.size(); i++) {
    map<string, string> vars;
    vars["classname"] = descriptor_->name();
    vars["name"] = aliases_[i].value->name();
    vars["canonical_name"] = aliases_[i].canonical_value->name();
    printer->Print(vars,
      "public static final $classname$ $name$ = $canonical_name$;\n");
  }

  // -----------------------------------------------------------------

  printer->Print(
    "public static $classname$ valueOf(int value) {\n"
    "  switch (value) {\n",
    "classname", descriptor_->name());
  printer->Indent();
  printer->Indent();

  for (int i = 0; i < canonical_values_.size(); i++) {
    printer->Print(
      "case $name$_VALUE: return $name$;\n",
      "name", canonical_values_[i]->name());
  }

  printer->Outdent();
  printer->Outdent();
  printer->Print(
    "    default: throw new IllegalArgumentException(Integer.toString(value));\n"
    "  }\n"
    "}\n"
    "\n");

  // -----------------------------------------------------------------

  printer->Print(
    "private static $classname$[] _VALUES = new $classname$[] {\n",
    "classname", descriptor_->name());
  printer->Indent();
  for (int i = 0; i < canonical_values_.size(); i++) {
    printer->Print("$name$,\n", "name", canonical_values_[i]->name());
  }
  printer->Outdent();
  printer->Print(
    "};\n");

  printer->Print(
    "public static $classname$[] values() {\n"
    "  return _VALUES;\n"
    "}\n"
    "\n",
    "classname", descriptor_->name());

  // -----------------------------------------------------------------

  printer->Print(
    "private $classname$(int value, String name) {\n"
    "  super(value, name);\n"
    "}\n"
    "\n"
    "public com.google.protobuf.EnumType getEnum(int value) {\n"
    "  return $classname$.valueOf(value);\n"
    "}\n",
    "classname", descriptor_->name());

  printer->Print(
    "\n"
    "// @@protoc_insertion_point(enum_scope:$full_name$)\n",
    "full_name", descriptor_->full_name());

  printer->Outdent();
  printer->Print("}\n\n");
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

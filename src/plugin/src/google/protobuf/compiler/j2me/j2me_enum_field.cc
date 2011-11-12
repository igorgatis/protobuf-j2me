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

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/j2me.pb.h>
#include "j2me_enum_field.h"
#include "j2me_helpers.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

namespace {

using google::protobuf::j2me::cstyle_enum;

// TODO(kenton):  Factor out a "SetCommonFieldVariables()" to get rid of
//   repeat code between this and the other field types.
void SetEnumVariables(const FieldDescriptor* descriptor,
                      map<string, string>* variables) {
  bool cstyle = descriptor->enum_type()->options().GetExtension(cstyle_enum);
  (*variables)["name"] =
    UnderscoresToCamelCase(descriptor);
  (*variables)["capitalized_name"] =
    UnderscoresToCapitalizedCamelCase(descriptor);
  (*variables)["number"] = SimpleItoa(descriptor->number());
  (*variables)["type"] = cstyle ? "int" : ClassName(descriptor->enum_type());
  (*variables)["get_value"] = cstyle ? "" : ".getValue()";
  (*variables)["default"] = DefaultValue(descriptor);
  (*variables)["tag"] = SimpleItoa(internal::WireFormat::MakeTag(descriptor));
  (*variables)["tag_size"] = SimpleItoa(
      internal::WireFormat::TagSize(descriptor->number(), GetType(descriptor)));
  (*variables)["set_mask"] =
      "set_mask_" + SimpleItoa(descriptor->index() / 32) + "_";
  (*variables)["mask_bit"] = SimpleItoa(1 << (descriptor->index() % 32));
}

}  // namespace

// ===================================================================

EnumFieldGenerator::
EnumFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetEnumVariables(descriptor, &variables_);
}

EnumFieldGenerator::~EnumFieldGenerator() {}

void EnumFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  bool cstyle = descriptor_->enum_type()->options().GetExtension(cstyle_enum);
  printer->Print(variables_,
    "private $type$ $name$_;\n"
    "public $type$ get$capitalized_name$() { return $name$_; }\n"
    "public boolean has$capitalized_name$() {"
    " return ($set_mask$ & $mask_bit$) != 0; }\n"
    "public void clear$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  $set_mask$ &= ~$mask_bit$;\n"
    "  $name$_ = $default$;\n"
    "}\n"
    "public void set$capitalized_name$($type$ value) {\n"
    "  assertNotReadOnly();\n");
  if (!cstyle) {
    printer->Print(variables_,
      "  if (value == null) {\n"
      "    throw new NullPointerException();\n"
      "  }\n");
  }
  printer->Print(variables_,
    "  $set_mask$ |= $mask_bit$;\n"
    "  $name$_ = value;\n"
    "}\n");
}

void EnumFieldGenerator::
GenerateInitializationCode(io::Printer* printer) const {
  printer->Print(variables_, "$name$_ = $default$;\n");
}

void EnumFieldGenerator::
GenerateReturnFalseIfDifferent(io::Printer* printer) const {
  printer->Print(variables_,
    //"if (has$capitalized_name$() != msg.has$capitalized_name$() ||\n"
    //"    (has$capitalized_name$() && ($name$_ != msg.$name$_))) {\n"
    "if ($name$_ != msg.$name$_) {\n"
    "  return false;\n"
    "}\n");
}

void EnumFieldGenerator::
GenerateCalculateHash(io::Printer* printer) const {
  bool cstyle = descriptor_->enum_type()->options().GetExtension(cstyle_enum);
  printer->Print(variables_,
    "if (has$capitalized_name$())");
  if (cstyle) {
    printer->Print(variables_,
      "  hash += 37 * $name$_;\n");
  } else {
    printer->Print(variables_,
      "  hash += 37 * $name$_.hashCode();\n");
  }
}

void EnumFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  bool cstyle = descriptor_->enum_type()->options().GetExtension(cstyle_enum);
  if (!cstyle) {
    printer->Print(variables_,
      "int rawValue = input.readEnum();\n"
      "$type$ value = $type$.valueOf(rawValue);\n"
      "if (value != null) {\n"
      "  set$capitalized_name$(value);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "set$capitalized_name$(input.readEnum());\n");
  }
  //if (HasUnknownFields(descriptor_->containing_type())) {
  //  printer->Print(variables_,
  //    "if (value == null) {\n"
  //    "  unknownFields.mergeVarintField($number$, rawValue);\n"
  //    "} else {\n");
  //}
}

void EnumFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n"
    "  output.writeEnum($number$, get$capitalized_name$()$get_value$);\n"
    "}\n");
}

void EnumFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n"
    "  size += com.google.protobuf.CodedOutputStream\n"
    "    .computeEnumSize($number$, get$capitalized_name$()$get_value$);\n"
    "}\n");
}

string EnumFieldGenerator::GetBoxedType() const {
  return ClassName(descriptor_->enum_type());
}

// ===================================================================

RepeatedEnumFieldGenerator::
RepeatedEnumFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetEnumVariables(descriptor, &variables_);
}

RepeatedEnumFieldGenerator::~RepeatedEnumFieldGenerator() {}

void RepeatedEnumFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  printer->Print(variables_,
    "private $type$[] $name$_ = new $type$[0];\n"
    "private int $name$Count_;\n"
    "public int get$capitalized_name$Count() { return $name$Count_; }\n"
    "public $type$ get$capitalized_name$(int index) {\n"
    "  return $name$_[index];\n"
    "}\n"
    "public void reserve$capitalized_name$(int size) {\n"
    "  if (size >= $name$_.length) {\n"
    "    $type$[] copy = new $type$[size];\n"
    "    System.arraycopy($name$_, 0, copy, 0, $name$Count_);\n"
    "    $name$_ = copy;\n"
    "  }\n"
    "}\n"
    "public void set$capitalized_name$(int index, $type$ value) {\n"
    "  assertNotReadOnly();\n"
    "  $name$_[index] = value;\n"
    "}\n"
    "public void add$capitalized_name$($type$ value) {\n"
    "  assertNotReadOnly();\n"
    "  reserve$capitalized_name$($name$Count_ + 1);\n"
    "  $name$_[$name$Count_++] = value;\n"
    "}\n"
    "public void swap$capitalized_name$(int index1, int index2) {\n"
    "  assertNotReadOnly();\n"
    "  $type$ swp = $name$_[index1];\n"
    "  $name$_[index1] = $name$_[index2];\n"
    "  $name$_[index2] = swp;\n"
    "}\n"
    "public void removeLast$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  if ($name$Count_ > 0) {\n"
    "    $name$_[$name$Count_--] = $default$;\n"
    "  }\n"
    "}\n"
    "public void clear$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  while ($name$Count_ > 0) {\n"
    "    $name$_[$name$Count_--] = $default$;\n"
    "  }\n"
    "}\n");

  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "private int $name$MemoizedSerializedSize;\n");
  }
}

void RepeatedEnumFieldGenerator::
GenerateInitializationCode(io::Printer* printer) const {
  // Initialized inline.
}

void RepeatedEnumFieldGenerator::
GenerateReturnFalseIfDifferent(io::Printer* printer) const {
  printer->Print(variables_,
      "if ($name$_.length != msg.$name$_.length) return false;\n"
      "for (int j = 0; j < $name$_.length; j++) {\n"
      "  if ($name$_[j] != msg.$name$_[j]) {\n"
      "    return false;\n"
      "  }\n"
      "}\n");
}

void RepeatedEnumFieldGenerator::
GenerateCalculateHash(io::Printer* printer) const {
  printer->Print(variables_,
      "for (int j = 0; j < $name$_.length; j++) {\n"
      "  hash += 13 * $name$_[j].getValue();\n"
      "}\n");
}

void RepeatedEnumFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  // Read and store the enum
  printer->Print(variables_,
    "int rawValue = input.readEnum();\n"
    "$type$ value = $type$.valueOf(rawValue);\n");
  //if (HasUnknownFields(descriptor_->containing_type())) {
  //  printer->Print(variables_,
  //    "if (value == null) {\n"
  //    "  unknownFields.mergeVarintField($number$, rawValue);\n"
  //    "} else {\n");
  //} else {
    printer->Print(variables_,
      "if (value != null) {\n");
  //}
  printer->Print(variables_,
    "  add$capitalized_name$(value);\n"
    "}\n");
}

void RepeatedEnumFieldGenerator::
GenerateParsingCodeFromPacked(io::Printer* printer) const {
  // Wrap GenerateParsingCode's contents with a while loop.

  printer->Print(variables_,
    "int length = input.readRawVarint32();\n"
    "int oldLimit = input.pushLimit(length);\n"
    "while(input.getBytesUntilLimit() > 0) {\n");
  printer->Indent();

  GenerateParsingCode(printer);

  printer->Outdent();
  printer->Print(variables_,
    "}\n"
    "input.popLimit(oldLimit);\n");
}

void RepeatedEnumFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "if (get$capitalized_name$Count() > 0) {\n"
      "  output.writeRawVarint32($tag$);\n"
      "  output.writeRawVarint32($name$MemoizedSerializedSize);\n"
      "}\n");
    printer->Print(variables_,
      "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
      "  $type$ element = get$capitalized_name$(i);\n"
      "  output.writeEnumNoTag(element$get_value$);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
      "  $type$ element = get$capitalized_name$(i);\n"
      "  output.writeEnum($number$, element$get_value$);\n"
      "}\n");
  }
}

void RepeatedEnumFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "{\n"
    "  int dataSize = 0;\n");
  printer->Indent();

  printer->Print(variables_,
    "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
    "  $type$ element = get$capitalized_name$(i);\n"
    "  dataSize += com.google.protobuf.CodedOutputStream\n"
    "    .computeEnumSizeNoTag(element$get_value$);\n"
    "}\n"
    "size += dataSize;\n");
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "if (get$capitalized_name$Count() > 0) {\n"
      "  size += $tag_size$;\n"
      "  size += com.google.protobuf.CodedOutputStream\n"
      "    .computeRawVarint32Size(dataSize);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "size += $tag_size$ * get$capitalized_name$Count();\n");
  }

  // cache the data size for packed fields.
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "$name$MemoizedSerializedSize = dataSize;\n");
  }

  printer->Outdent();
  printer->Print("}\n");
}

string RepeatedEnumFieldGenerator::GetBoxedType() const {
  return ClassName(descriptor_->enum_type());
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

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
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>

#include "j2me_message_field.h"
#include "j2me_helpers.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

namespace {

// TODO(kenton):  Factor out a "SetCommonFieldVariables()" to get rid of
//   repeat code between this and the other field types.
void SetMessageVariables(const FieldDescriptor* descriptor,
                         map<string, string>* variables) {
  (*variables)["name"] =
    UnderscoresToCamelCase(descriptor);
  (*variables)["capitalized_name"] =
    UnderscoresToCapitalizedCamelCase(descriptor);
  (*variables)["number"] = SimpleItoa(descriptor->number());
  (*variables)["type"] = ClassName(descriptor->message_type());
  (*variables)["group_or_message"] =
    (GetType(descriptor) == FieldDescriptor::TYPE_GROUP) ?
    "Group" : "Message";
  (*variables)["set_mask"] =
      "set_mask_" + SimpleItoa(descriptor->index() / 32) + "_";
  (*variables)["mask_bit"] = SimpleItoa(1 << (descriptor->index() % 32));
}

}  // namespace

// ===================================================================

MessageFieldGenerator::
MessageFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetMessageVariables(descriptor, &variables_);
}

MessageFieldGenerator::~MessageFieldGenerator() {}

void MessageFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  printer->Print(variables_,
    "private $type$ $name$_;\n"
    "public $type$ get$capitalized_name$() { return $name$_; }\n"
    "public boolean has$capitalized_name$() {"
    " return ($set_mask$ & $mask_bit$) != 0; }\n"
    "public void clear$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  $set_mask$ &= ~$mask_bit$;\n"
    "  $name$_ = $type$.getDefaultInstance();\n"
    "}\n"
    "public void set$capitalized_name$($type$ value) {\n"
    "  assertNotReadOnly();\n"
    "  if (value == null) {\n"
    "    throw new NullPointerException();\n"
    "  }\n"
    "  $set_mask$ |= $mask_bit$;\n"
    "  $name$_ = value;\n"
    "}\n");
}

void MessageFieldGenerator::
GenerateInitializationCode(io::Printer* printer) const {
  printer->Print(variables_, "$name$_ = $type$.getDefaultInstance();\n");
}

void MessageFieldGenerator::
GenerateReturnFalseIfDifferent(io::Printer* printer) const {
  printer->Print(variables_,
    //"if (has$capitalized_name$() != msg.has$capitalized_name$() ||\n"
    //"    (has$capitalized_name$() && !$name$_.equals(msg.$name$_))) {\n"
    "if (!$name$_.equals(msg.$name$_)) {\n"
    "  return false;\n"
    "}\n");
}

void MessageFieldGenerator::
GenerateCalculateHash(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) "
    "  hash += 31 * $name$_.hashCode();\n");
}

void MessageFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (!has$capitalized_name$()) {\n"
    "  set$capitalized_name$(new $type$());\n"
    "}\n"
    "input.readMessage(get$capitalized_name$());\n");
}

void MessageFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n"
    "  output.write$group_or_message$($number$, get$capitalized_name$());\n"
    "}\n");
}

void MessageFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n"
    "  size += com.google.protobuf.CodedOutputStream\n"
    "    .compute$group_or_message$Size($number$, get$capitalized_name$());\n"
    "}\n");
}

string MessageFieldGenerator::GetBoxedType() const {
  return ClassName(descriptor_->message_type());
}

// ===================================================================

RepeatedMessageFieldGenerator::
RepeatedMessageFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetMessageVariables(descriptor, &variables_);
}

RepeatedMessageFieldGenerator::~RepeatedMessageFieldGenerator() {}

void RepeatedMessageFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  printer->Print(variables_,
    "private java.util.Vector $name$_ = new java.util.Vector();\n"
    "public java.util.Enumeration get$capitalized_name$Enum() {\n"
    "  return $name$_.elements();\n"   // note:  unmodifiable list
    "}\n"
    "public int get$capitalized_name$Count() { return $name$_.size(); }\n"
    "public $type$ get$capitalized_name$(int index) {\n"
    "  return ($type$) $name$_.elementAt(index);\n"
    "}\n"
    "public void set$capitalized_name$(int index, $type$ value) {\n"
    "  assertNotReadOnly();\n"
    "  if (value == null) {\n"
    "    throw new NullPointerException();\n"
    "  }\n"
    "  $name$_.setElementAt(value, index);\n"
    "}\n"
    "public void add$capitalized_name$($type$ value) {\n"
    "  assertNotReadOnly();\n"
    "  if (value == null) {\n"
    "    throw new NullPointerException();\n"
    "  }\n"
    "  $name$_.addElement(value);\n"
    "}\n"
    "public void swap$capitalized_name$(int index1, int index2) {\n"
    "  assertNotReadOnly();\n"
    "  Object swp = $name$_.elementAt(index1);\n"
    "  $name$_.setElementAt($name$_.elementAt(index2), index1);\n"
    "  $name$_.setElementAt(swp, index2);\n"
    "}\n"
    "public void removeLast$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  if ($name$_.size() > 0) {\n"
    "    $name$_.removeElementAt($name$_.size() - 1);\n"
    "  }\n"
    "}\n"
    "public void clear$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  $name$_.removeAllElements();\n"
    "}\n");
}

void RepeatedMessageFieldGenerator::
GenerateInitializationCode(io::Printer* printer) const {
  // Initialized inline.
}

void RepeatedMessageFieldGenerator::
GenerateReturnFalseIfDifferent(io::Printer* printer) const {
  printer->Print(variables_,
      "if ($name$_.size() != msg.$name$_.size()) return false;\n"
      "for (int j = 0; j < $name$_.size(); j++) {\n"
      "  if (!$name$_.elementAt(j).equals(msg.$name$_.elementAt(j))) {\n"
      "    return false;\n"
      "  }\n"
      "}\n");
}

void RepeatedMessageFieldGenerator::
GenerateCalculateHash(io::Printer* printer) const {
  printer->Print(variables_,
     "for (int j = 0; j < $name$_.size(); j++) {\n"
     "  hash += 17 * $name$_.elementAt(j).hashCode();\n"
     "}\n");
}

void RepeatedMessageFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  //if (GetType(descriptor_) == FieldDescriptor::TYPE_GROUP) {
  //  printer->Print(variables_,
  //    "input.readGroup($number$, subBuilder, extensionRegistry);\n");
  //} else {
  //  printer->Print(variables_,
  //    "input.readMessage(subBuilder, extensionRegistry);\n");
  //}
  printer->Print(variables_,
    "$type$ msg = new $type$();\n"
    "input.readMessage(msg);\n"
    "add$capitalized_name$(msg);\n");
}

void RepeatedMessageFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  printer->Print(variables_,
    "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
    "  $type$ element = get$capitalized_name$(i);\n"
    "  output.write$group_or_message$($number$, element);\n"
    "}\n");
}

void RepeatedMessageFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
    "  $type$ element = get$capitalized_name$(i);\n"
    "  size += com.google.protobuf.CodedOutputStream\n"
    "    .compute$group_or_message$Size($number$, element);\n"
    "}\n");
}

string RepeatedMessageFieldGenerator::GetBoxedType() const {
  return ClassName(descriptor_->message_type());
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

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
#include "j2me_helpers.h"
#include "j2me_primitive_field.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

using internal::WireFormat;
using internal::WireFormatLite;

namespace {

const char* PrimitiveTypeName(JavaType type) {
  switch (type) {
    case JAVATYPE_INT    : return "int";
    case JAVATYPE_LONG   : return "long";
    case JAVATYPE_FLOAT  : return "float";
    case JAVATYPE_DOUBLE : return "double";
    case JAVATYPE_BOOLEAN: return "boolean";
    case JAVATYPE_STRING : return "java.lang.String";
    case JAVATYPE_BYTES  : return "com.google.protobuf.ByteString";
    case JAVATYPE_ENUM   : return NULL;
    case JAVATYPE_MESSAGE: return NULL;

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return NULL;
}

bool IsReferenceType(JavaType type) {
  switch (type) {
    case JAVATYPE_INT    : return false;
    case JAVATYPE_LONG   : return false;
    case JAVATYPE_FLOAT  : return false;
    case JAVATYPE_DOUBLE : return false;
    case JAVATYPE_BOOLEAN: return false;
    case JAVATYPE_STRING : return true;
    case JAVATYPE_BYTES  : return true;
    case JAVATYPE_ENUM   : return true;
    case JAVATYPE_MESSAGE: return true;

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return false;
}

const char* GetCapitalizedType(const FieldDescriptor* field) {
  switch (GetType(field)) {
    case FieldDescriptor::TYPE_INT32   : return "Int32"   ;
    case FieldDescriptor::TYPE_UINT32  : return "UInt32"  ;
    case FieldDescriptor::TYPE_SINT32  : return "SInt32"  ;
    case FieldDescriptor::TYPE_FIXED32 : return "Fixed32" ;
    case FieldDescriptor::TYPE_SFIXED32: return "SFixed32";
    case FieldDescriptor::TYPE_INT64   : return "Int64"   ;
    case FieldDescriptor::TYPE_UINT64  : return "UInt64"  ;
    case FieldDescriptor::TYPE_SINT64  : return "SInt64"  ;
    case FieldDescriptor::TYPE_FIXED64 : return "Fixed64" ;
    case FieldDescriptor::TYPE_SFIXED64: return "SFixed64";
    case FieldDescriptor::TYPE_FLOAT   : return "Float"   ;
    case FieldDescriptor::TYPE_DOUBLE  : return "Double"  ;
    case FieldDescriptor::TYPE_BOOL    : return "Bool"    ;
    case FieldDescriptor::TYPE_STRING  : return "String"  ;
    case FieldDescriptor::TYPE_BYTES   : return "Bytes"   ;
    case FieldDescriptor::TYPE_ENUM    : return "Enum"    ;
    case FieldDescriptor::TYPE_GROUP   : return "Group"   ;
    case FieldDescriptor::TYPE_MESSAGE : return "Message" ;

    // No default because we want the compiler to complain if any new
    // types are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return NULL;
}

// For encodings with fixed sizes, returns that size in bytes.  Otherwise
// returns -1.
int FixedSize(FieldDescriptor::Type type) {
  switch (type) {
    case FieldDescriptor::TYPE_INT32   : return -1;
    case FieldDescriptor::TYPE_INT64   : return -1;
    case FieldDescriptor::TYPE_UINT32  : return -1;
    case FieldDescriptor::TYPE_UINT64  : return -1;
    case FieldDescriptor::TYPE_SINT32  : return -1;
    case FieldDescriptor::TYPE_SINT64  : return -1;
    case FieldDescriptor::TYPE_FIXED32 : return WireFormatLite::kFixed32Size;
    case FieldDescriptor::TYPE_FIXED64 : return WireFormatLite::kFixed64Size;
    case FieldDescriptor::TYPE_SFIXED32: return WireFormatLite::kSFixed32Size;
    case FieldDescriptor::TYPE_SFIXED64: return WireFormatLite::kSFixed64Size;
    case FieldDescriptor::TYPE_FLOAT   : return WireFormatLite::kFloatSize;
    case FieldDescriptor::TYPE_DOUBLE  : return WireFormatLite::kDoubleSize;

    case FieldDescriptor::TYPE_BOOL    : return WireFormatLite::kBoolSize;
    case FieldDescriptor::TYPE_ENUM    : return -1;

    case FieldDescriptor::TYPE_STRING  : return -1;
    case FieldDescriptor::TYPE_BYTES   : return -1;
    case FieldDescriptor::TYPE_GROUP   : return -1;
    case FieldDescriptor::TYPE_MESSAGE : return -1;

    // No default because we want the compiler to complain if any new
    // types are added.
  }
  GOOGLE_LOG(FATAL) << "Can't get here.";
  return -1;
}

void SetPrimitiveVariables(const FieldDescriptor* descriptor,
                           map<string, string>* variables) {
  (*variables)["name"] =
    UnderscoresToCamelCase(descriptor);
  (*variables)["capitalized_name"] =
    UnderscoresToCapitalizedCamelCase(descriptor);
  (*variables)["number"] = SimpleItoa(descriptor->number());
  (*variables)["type"] = PrimitiveTypeName(GetJavaType(descriptor));
  (*variables)["boxed_type"] = BoxedPrimitiveTypeName(GetJavaType(descriptor));
  (*variables)["default"] = DefaultValue(descriptor);
  (*variables)["capitalized_type"] = GetCapitalizedType(descriptor);
  (*variables)["tag"] = SimpleItoa(WireFormat::MakeTag(descriptor));
  (*variables)["tag_size"] = SimpleItoa(
      WireFormat::TagSize(descriptor->number(), GetType(descriptor)));
  if (IsReferenceType(GetJavaType(descriptor))) {
    (*variables)["null_check"] =
        "  if (value == null) {\n"
        "    throw new NullPointerException();\n"
        "  }\n";
  } else {
    (*variables)["null_check"] = "";
  }
  int fixed_size = FixedSize(GetType(descriptor));
  if (fixed_size != -1) {
    (*variables)["fixed_size"] = SimpleItoa(fixed_size);
  }
  (*variables)["set_mask"] =
      "set_mask_" + SimpleItoa(descriptor->index() / 32) + "_";
  (*variables)["mask_bit"] = SimpleItoa(1 << (descriptor->index() % 32));
}
}  // namespace

// ===================================================================

PrimitiveFieldGenerator::
PrimitiveFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetPrimitiveVariables(descriptor, &variables_);
}

PrimitiveFieldGenerator::~PrimitiveFieldGenerator() {}

void PrimitiveFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  printer->Print(variables_,
    "private $type$ $name$_ = $default$;\n"
    "public $type$ get$capitalized_name$() { return $name$_; }\n"
    "public boolean has$capitalized_name$() {"
    " return ($set_mask$ & $mask_bit$) != 0; }\n"
    "public void clear$capitalized_name$() {\n"
    "  assertNotReadOnly();\n"
    "  $set_mask$ &= ~$mask_bit$;\n"
    "  $name$_ = $default$;\n"
    "}\n"
    "public void set$capitalized_name$($type$ value) {\n"
    "  assertNotReadOnly();\n"
    "  $set_mask$ |= $mask_bit$;\n"
    "  $name$_ = value;\n"
    "}\n");
}

void PrimitiveFieldGenerator::
GenerateInitializationCode(io::Printer* printer) const {
  // Initialized inline.
}

void PrimitiveFieldGenerator::
GenerateReturnFalseIfDifferent(io::Printer* printer) const {
  //printer->Print(variables_,
  //  "if (has$capitalized_name$() != msg.has$capitalized_name$() ||\n"
  //  "    (has$capitalized_name$() &&");
  if (IsReferenceType(GetJavaType(descriptor_))) {
    printer->Print(variables_,
      "if (!$name$_.equals(msg.$name$_)) {\n");
  } else {
    printer->Print(variables_,
      "if ($name$_ != msg.$name$_) {\n");
  }
  printer->Print(variables_,
    "  return false;\n"
    "}\n");
}

void PrimitiveFieldGenerator::
GenerateCalculateHash(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) ");
  if (IsReferenceType(GetJavaType(descriptor_))) {
    printer->Print(variables_,
        "hash += 31 * $name$_.hashCode();\n");
  } else if (GetJavaType(descriptor_) == JAVATYPE_BOOLEAN) {
    printer->Print(variables_,
        "hash += 33 * ($name$_ ? 1 : 0);\n");
  } else {
    printer->Print(variables_,
        "hash += 33 * $name$_;\n");
  }
}

void PrimitiveFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  printer->Print(variables_,
    "set$capitalized_name$(input.read$capitalized_type$());\n");
}

void PrimitiveFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n"
    "  output.write$capitalized_type$($number$, get$capitalized_name$());\n"
    "}\n");
}

void PrimitiveFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n"
    "  size += com.google.protobuf.CodedOutputStream\n"
    "    .compute$capitalized_type$Size($number$, get$capitalized_name$());\n"
    "}\n");
}

string PrimitiveFieldGenerator::GetBoxedType() const {
  return BoxedPrimitiveTypeName(GetJavaType(descriptor_));
}

bool PrimitiveFieldGenerator::ShouldUseStringEncodingCache() const {
  return GetType(descriptor_) == FieldDescriptor::TYPE_STRING &&
      descriptor_->file()->options().optimize_for() == FileOptions::SPEED;
}

// ===================================================================

RepeatedPrimitiveFieldGenerator::
RepeatedPrimitiveFieldGenerator(const FieldDescriptor* descriptor)
  : descriptor_(descriptor) {
  SetPrimitiveVariables(descriptor, &variables_);
}

RepeatedPrimitiveFieldGenerator::~RepeatedPrimitiveFieldGenerator() {}

void RepeatedPrimitiveFieldGenerator::
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
      "private int $name$MemoizedSerializedSize = -1;\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateInitializationCode(io::Printer* printer) const {
  // Initialized inline.
}

void RepeatedPrimitiveFieldGenerator::
GenerateReturnFalseIfDifferent(io::Printer* printer) const {
  if (IsReferenceType(GetJavaType(descriptor_))) {
    printer->Print(variables_,
        "if ($name$_.length != msg.$name$_.length) return false;\n"
        "for (int j = 0; j < $name$_.length; j++) {\n"
        "  if (!$name$_[j].equals(msg.$name$_[j])) {\n"
        "    return false;\n"
        "  }\n"
        "}\n");
  } else {
    printer->Print(variables_,
        "if ($name$_.length != msg.$name$_.length) return false;\n"
        "for (int j = 0; j < $name$_.length; j++) {\n"
        "  if ($name$_[j] != msg.$name$_[j]) {\n"
        "    return false;\n"
        "  }\n"
        "}\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateCalculateHash(io::Printer* printer) const {
  if (IsReferenceType(GetJavaType(descriptor_))) {
    printer->Print(variables_,
        "for (int j = 0; j < $name$_.length; j++) {\n"
        "  hash += 19 * $name$_[j].hashCode();\n"
        "}\n");
  } else if (GetJavaType(descriptor_) == JAVATYPE_BOOLEAN) {
    printer->Print(variables_,
        "for (int j = 0; j < $name$_.length; j++) {\n"
        "  hash += $name$_[j] ? 19 : 17;\n"
        "}\n");
  } else {
    printer->Print(variables_,
        "for (int j = 0; j < $name$_.length; j++) {\n"
        "  hash += 19 * $name$_[j];\n"
        "}\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  printer->Print(variables_,
    "add$capitalized_name$(input.read$capitalized_type$());\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateParsingCodeFromPacked(io::Printer* printer) const {
  printer->Print(variables_,
    "int length = input.readRawVarint32();\n"
    "reserve$capitalized_name$(length);\n"
    "int limit = input.pushLimit(length);\n"
    "while (input.getBytesUntilLimit() > 0) {\n"
    "  add$capitalized_name$(input.read$capitalized_type$());\n"
    "}\n"
    "input.popLimit(limit);\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "if (get$capitalized_name$Count() > 0) {\n"
      "  output.writeRawVarint32($tag$);\n"
      "  output.writeRawVarint32($name$MemoizedSerializedSize);\n"
      "}\n"
      "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
      "  $type$ element = get$capitalized_name$(i);\n"
      "  output.write$capitalized_type$NoTag(element);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
      "  $type$ element = get$capitalized_name$(i);\n"
      "  output.write$capitalized_type$($number$, element);\n"
      "}\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "{\n"
    "  int dataSize = 0;\n");
  printer->Indent();

  if (FixedSize(GetType(descriptor_)) == -1) {
    printer->Print(variables_,
      "for (int i = 0; i < get$capitalized_name$Count(); i++) {\n"
      "  $type$ element = get$capitalized_name$(i);\n"
      "  dataSize += com.google.protobuf.CodedOutputStream\n"
      "    .compute$capitalized_type$SizeNoTag(element);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "dataSize = $fixed_size$ * get$capitalized_name$Count();\n");
  }

  printer->Print(
      "size += dataSize;\n");

  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "if (get$capitalized_name$Count() > 0) {\n"
      "  size += $tag_size$;\n"
      "  size += com.google.protobuf.CodedOutputStream\n"
      "      .computeInt32SizeNoTag(dataSize);\n"
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

string RepeatedPrimitiveFieldGenerator::GetBoxedType() const {
  return BoxedPrimitiveTypeName(GetJavaType(descriptor_));
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

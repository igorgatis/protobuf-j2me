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

#include <algorithm>
#include <google/protobuf/stubs/hash.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/j2me.pb.h>
#include "j2me_message.h"
#include "j2me_extension.h"
#include "j2me_enum.h"
#include "j2me_helpers.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

using internal::WireFormat;
using internal::WireFormatLite;
using google::protobuf::j2me::generate_builder;

namespace {

void PrintFieldComment(io::Printer* printer, const FieldDescriptor* field) {
  // Print the field's proto-syntax definition as a comment.  We don't want to
  // print group bodies so we cut off after the first line.
  string def = field->DebugString();
  printer->Print("// $def$\n",
    "def", def.substr(0, def.find_first_of('\n')));
}

struct FieldOrderingByNumber {
  inline bool operator()(const FieldDescriptor* a,
                         const FieldDescriptor* b) const {
    return a->number() < b->number();
  }
};

// Sort the fields of the given Descriptor by number into a new[]'d array
// and return it.
const FieldDescriptor** SortFieldsByNumber(const Descriptor* descriptor) {
  const FieldDescriptor** fields =
    new const FieldDescriptor*[descriptor->field_count()];
  for (int i = 0; i < descriptor->field_count(); i++) {
    fields[i] = descriptor->field(i);
  }
  sort(fields, fields + descriptor->field_count(),
       FieldOrderingByNumber());
  return fields;
}

// Get an identifier that uniquely identifies this type within the file.
// This is used to declare static variables related to this type at the
// outermost file scope.
string UniqueFileScopeIdentifier(const Descriptor* descriptor) {
  return "static_" + StringReplace(descriptor->full_name(), ".", "_", true);
}

// Returns true if the message type has any required fields.  If it doesn't,
// we can optimize out calls to its isInitialized() method.
//
// already_seen is used to avoid checking the same type multiple times
// (and also to protect against recursion).
static bool HasRequiredFields(
    const Descriptor* type,
    hash_set<const Descriptor*>* already_seen) {
  if (already_seen->count(type) > 0) {
    // The type is already in cache.  This means that either:
    // a. The type has no required fields.
    // b. We are in the midst of checking if the type has required fields,
    //    somewhere up the stack.  In this case, we know that if the type
    //    has any required fields, they'll be found when we return to it,
    //    and the whole call to HasRequiredFields() will return true.
    //    Therefore, we don't have to check if this type has required fields
    //    here.
    return false;
  }
  already_seen->insert(type);

  // If the type has extensions, an extension with message type could contain
  // required fields, so we have to be conservative and assume such an
  // extension exists.
  if (type->extension_range_count() > 0) return true;

  for (int i = 0; i < type->field_count(); i++) {
    const FieldDescriptor* field = type->field(i);
    if (field->is_required()) {
      return true;
    }
    if (GetJavaType(field) == JAVATYPE_MESSAGE) {
      if (HasRequiredFields(field->message_type(), already_seen)) {
        return true;
      }
    }
  }

  return false;
}

static bool HasRequiredFields(const Descriptor* type) {
  hash_set<const Descriptor*> already_seen;
  return HasRequiredFields(type, &already_seen);
}

}  // namespace

// ===================================================================

MessageGenerator::MessageGenerator(const Descriptor* descriptor)
  : descriptor_(descriptor),
    field_generators_(descriptor) {
}

MessageGenerator::~MessageGenerator() {}

void MessageGenerator::GenerateStaticVariables(io::Printer* printer) {
  // Generate static members for all nested types.
  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    // TODO(kenton):  Reuse MessageGenerator objects?
    MessageGenerator(descriptor_->nested_type(i))
      .GenerateStaticVariables(printer);
  }
}

void MessageGenerator::GenerateStaticVariableInitializers(
    io::Printer* printer) {
  // Generate static member initializers for all nested types.
  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    // TODO(kenton):  Reuse MessageGenerator objects?
    MessageGenerator(descriptor_->nested_type(i))
      .GenerateStaticVariableInitializers(printer);
  }

  for (int i = 0; i < descriptor_->extension_count(); i++) {
    // TODO(kenton):  Reuse ExtensionGenerator objects?
    ExtensionGenerator(descriptor_->extension(i))
      .GenerateInitializationCode(printer);
  }
}

void MessageGenerator::Generate(io::Printer* printer) {
  string modifiers;
  bool is_own_file =
    descriptor_->containing_type() == NULL &&
    descriptor_->file()->options().java_multiple_files();
  if (!is_own_file) {
    modifiers += " static";
  }
  if (!descriptor_->options().GetExtension(generate_builder)) {
    modifiers += " final";
  }

  printer->Print(
      "public $modifiers$ class $classname$ extends\n"
      "    com.google.protobuf.Message {\n",
      "modifiers", modifiers,
      "classname", descriptor_->name());
  printer->Indent();
  for (int i = 0; i < (descriptor_->field_count() + 31) / 32; ++i) {
    printer->Print("private int set_mask_$bucket$_;\n",
                   "bucket", SimpleItoa(i));
  }
  printer->Print(
    "public $classname$() {\n"
    "  super(\"$fullname$\");\n"
    "  initFields();\n"
    "}\n"
    // Used when constructing the default instance, which cannot be initialized
    // immediately because it may cyclically refer to other default instances.
    "private $classname$(boolean noInit) { super(true); }\n"
    "\n"
    "private static final $classname$ defaultInstance;\n"
    "public static $classname$ getDefaultInstance() {\n"
    "  return defaultInstance;\n"
    "}\n"
    "\n"
    "public $classname$ getDefaultInstanceForType() {\n"
    "  return defaultInstance;\n"
    "}\n"
    "\n",
    "classname", descriptor_->name(),
    "fullname", descriptor_->full_name());

  // Nested types and extensions
  for (int i = 0; i < descriptor_->enum_type_count(); i++) {
    EnumGenerator(descriptor_->enum_type(i)).Generate(printer);
  }

  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    MessageGenerator(descriptor_->nested_type(i)).Generate(printer);
  }

  // Fields
  for (int i = 0; i < descriptor_->field_count(); i++) {
    PrintFieldComment(printer, descriptor_->field(i));
    //printer->Print("public static final int $constant_name$ = $number$;\n",
    //  "constant_name", FieldConstantName(descriptor_->field(i)),
    //  "number", SimpleItoa(descriptor_->field(i)->number()));
    field_generators_.get(descriptor_->field(i)).GenerateMembers(printer);
    printer->Print("\n");
  }

  // Called by the constructor, except in the case of the default instance,
  // in which case this is called by static init code later on.
  printer->Print("private void initFields() {\n");
  printer->Indent();
  if (descriptor_->extension_range_count() > 0) {
    printer->Print("initExtensionSupport();\n");
  }
  for (int i = 0; i < descriptor_->field_count(); i++) {
    field_generators_.get(descriptor_->field(i))
                     .GenerateInitializationCode(printer);
  }
  printer->Outdent();
  printer->Print("}\n");

  GenerateIsInitialized(printer);
  GenerateHashingMethods(printer);
  GenerateMessageSerializationMethods(printer);
  GenerateParseFromMethods(printer);
  GenerateParsingMethods(printer);
  if (descriptor_->options().GetExtension(generate_builder)) {
    GenerateBuilder(printer);
  }

  // Force initialization of outer class.  Otherwise, nested extensions may
  // not be initialized.  Also carefully initialize the default instance in
  // such a way that it doesn't conflict with other initialization.
  printer->Print(
    "\n"
    "static {\n"
    "  defaultInstance = new $classname$(true);\n"
    "  $file$.internalForceInit();\n"
    "  defaultInstance.initFields();\n"
    "}\n",
    "file", ClassName(descriptor_->file()),
    "classname", descriptor_->name());

  for (int i = 0; i < descriptor_->extension_count(); i++) {
    ExtensionGenerator(descriptor_->extension(i)).Generate(printer);
  }

  printer->Print(
    "\n"
    "// @@protoc_insertion_point(class_scope:$full_name$)\n",
    "full_name", descriptor_->full_name());

  printer->Outdent();
  printer->Print("}\n\n");

}

// ===================================================================

void MessageGenerator::
GenerateMessageSerializationMethods(io::Printer* printer) {
  scoped_array<const FieldDescriptor*> sorted_fields(
    SortFieldsByNumber(descriptor_));

  printer->Print(
    "public void writeTo(com.google.protobuf.CodedOutputStream output)\n"
    "                    throws java.io.IOException {\n");
  printer->Indent();

  for (int i = 0; i < descriptor_->field_count(); ++i) {
      GenerateSerializeOneField(printer, sorted_fields[i]);
  }
  if (descriptor_->extension_range_count() > 0) {
    printer->Print("writeExtensions(output);\n");
  }

  printer->Outdent();
  printer->Print(
      "}\n"
      "\n");

  printer->Print(
    "public int getSerializedSize() {\n"
    "  int size = 0;\n");
  printer->Indent();
  for (int i = 0; i < descriptor_->field_count(); i++) {
    field_generators_.get(sorted_fields[i]).GenerateSerializedSizeCode(printer);
  }
  if (descriptor_->extension_range_count() > 0) {
    printer->Print("size += extensionsSerializedSize();\n");
  }
  printer->Outdent();
  printer->Print(
    "  return size;\n"
    "}\n"
    "\n");
}

void MessageGenerator::
GenerateParseFromMethods(io::Printer* printer) {
  printer->Print(
      "public static $classname$ parseFrom(\n"
      "    java.io.InputStream input)\n"
      "    throws java.io.IOException {\n"
      "  com.google.protobuf.CodedInputStream codedInput =\n"
      "      com.google.protobuf.CodedInputStream.newInstance(input);\n"
      "  return parseFrom(codedInput);\n"
      "}\n"
      "\n"
      "public static $classname$ parseFrom(\n"
      "    com.google.protobuf.CodedInputStream codedInput)\n"
      "    throws java.io.IOException {\n"
      "  $classname$ proto = new $classname$();\n"
      "  proto.mergeFrom(codedInput);\n"
      "  return proto;\n"
      "}\n"
      "\n",
      "classname", ClassName(descriptor_));
}

void MessageGenerator::GenerateSerializeOneField(
    io::Printer* printer, const FieldDescriptor* field) {
  field_generators_.get(field).GenerateSerializationCode(printer);
}

void MessageGenerator::GenerateBuilder(io::Printer* printer) {
  printer->Print(
    "public static Builder newBuilder() { return Builder.create(); }\n"
    "public Builder newBuilderForType() { return newBuilder(); }\n"
    "\n",
    "classname", ClassName(descriptor_));

  printer->Print(
    "public static final class Builder extends $classname$ implements com.google.protobuf.Message.Builder {\n",
    "classname", ClassName(descriptor_));
  printer->Indent();

  GenerateCommonBuilderMethods(printer);

  printer->Print(
    "\n"
    "// @@protoc_insertion_point(builder_scope:$full_name$)\n",
    "full_name", descriptor_->full_name());

  printer->Outdent();
  printer->Print("}\n");
}

// ===================================================================

void MessageGenerator::GenerateCommonBuilderMethods(io::Printer* printer) {
  printer->Print(
    "// Construct using $classname$.newBuilder()\n"
    "private Builder() {}\n"
    "\n"
    "private static Builder create() {\n"
    "  return new Builder();\n"
    "}\n"
    "\n",
    "classname", ClassName(descriptor_));

  // -----------------------------------------------------------------

  printer->Print(
    "public com.google.protobuf.Message build() {\n"
    " return this;\n"
    "}\n",
    "classname", ClassName(descriptor_));
}

// ===================================================================

void MessageGenerator::GenerateParsingMethods(io::Printer* printer) {
  scoped_array<const FieldDescriptor*> sorted_fields(
    SortFieldsByNumber(descriptor_));

  printer->Print(
    "public com.google.protobuf.Message newInstance() {\n"
    "  return new $classname$();\n"
    "}\n"
    "\n",
    "classname", ClassName(descriptor_));

  printer->Print(
    "public void mergeFrom(\n"
    "    com.google.protobuf.CodedInputStream input)\n"
    "    throws java.io.IOException {\n"
    "  assertNotReadOnly();\n");
  printer->Indent();

  printer->Print(
    "while (true) {\n");
  printer->Indent();

  printer->Print(
    "int tag = input.readTag();\n"
    "switch (tag) {\n");
  printer->Indent();

  const string parseMethod = (descriptor_->extension_range_count() > 0) ?
      "parseUnknownFieldWithExtensions" : "parseUnknownField";
  printer->Print(
    "case 0:\n"          // zero signals EOF / limit reached
    "  return;\n"
    "default: {\n"
    "  if (!$parse_method$(input, tag)) {\n"
    "    return;\n"   // it's an endgroup tag
    "  }\n"
    "  break;\n"
    "}\n",
    "parse_method", parseMethod.c_str());

  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* field = sorted_fields[i];
    uint32 tag = WireFormatLite::MakeTag(field->number(),
      WireFormat::WireTypeForFieldType(field->type()));

    printer->Print(
      "case $tag$: {\n",
      "tag", SimpleItoa(tag));
    printer->Indent();

    field_generators_.get(field).GenerateParsingCode(printer);

    printer->Outdent();
    printer->Print(
      "  break;\n"
      "}\n");

    if (field->is_packable()) {
      // To make packed = true wire compatible, we generate parsing code from a
      // packed version of this field regardless of field->options().packed().
      uint32 packed_tag = WireFormatLite::MakeTag(field->number(),
        WireFormatLite::WIRETYPE_LENGTH_DELIMITED);
      printer->Print(
        "case $tag$: {\n",
        "tag", SimpleItoa(packed_tag));
      printer->Indent();

      field_generators_.get(field).GenerateParsingCodeFromPacked(printer);

      printer->Outdent();
      printer->Print(
        "  break;\n"
        "}\n");
    }
  }

  printer->Outdent();
  printer->Outdent();
  printer->Outdent();
  printer->Print(
    "    }\n"     // switch (tag)
    "  }\n"       // while (true)
    "}\n"
    "\n");
}

// ===================================================================

void MessageGenerator::GenerateIsInitialized(io::Printer* printer) {
  printer->Print(
    "public final boolean isInitialized() {\n");
  printer->Indent();

  // Check that all required fields in this message are set.
  // TODO(kenton):  We can optimize this when we switch to putting all the
  //   "has" fields into a single bitfield.
  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* field = descriptor_->field(i);

    if (field->is_required()) {
      printer->Print(
        "if (!has$name$()) return false;\n",
        "name", UnderscoresToCapitalizedCamelCase(field));
    }
  }

  // Now check that all embedded messages are initialized.
  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* field = descriptor_->field(i);
    if (GetJavaType(field) == JAVATYPE_MESSAGE &&
        HasRequiredFields(field->message_type())) {
      switch (field->label()) {
        case FieldDescriptor::LABEL_REQUIRED:
          printer->Print(
            "if (!get$name$().isInitialized()) return false;\n",
            "type", ClassName(field->message_type()),
            "name", UnderscoresToCapitalizedCamelCase(field));
          break;
        case FieldDescriptor::LABEL_OPTIONAL:
          printer->Print(
            "if (has$name$()) {\n"
            "  if (!get$name$().isInitialized()) return false;\n"
            "}\n",
            "type", ClassName(field->message_type()),
            "name", UnderscoresToCapitalizedCamelCase(field));
          break;
        case FieldDescriptor::LABEL_REPEATED:
          printer->Print(
            "for (int i = 0; i < get$name$Count(); i++) {\n"
            "  $type$ element = get$name$(i);\n"
            "  if (!element.isInitialized()) return false;\n"
            "}\n",
            "type", ClassName(field->message_type()),
            "name", UnderscoresToCapitalizedCamelCase(field));
          break;
      }
    }
  }

  if (descriptor_->extension_range_count() > 0) {
    printer->Print(
      "if (!extensionsAreInitialized()) return false;\n");
  }

  printer->Outdent();
  printer->Print(
    "  return true;\n"
    "}\n"
    "\n");
}

// ===================================================================

void MessageGenerator::GenerateHashingMethods(io::Printer* printer) {
  // Equals.
  printer->Print(
    "public boolean equals(Object obj) {\n"
    "  if (this == obj) return true;\n"
    "  if (!(obj instanceof $classname$)) return false;\n"
    "  $classname$ msg = ($classname$)obj;\n",
    "classname", ClassName(descriptor_));
  printer->Indent();
  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* field = descriptor_->field(i);
    field_generators_.get(field).GenerateReturnFalseIfDifferent(printer);
  }
  if (descriptor_->extension_range_count() > 0) {
    printer->Print(
      "if (!extensionsEquals(msg)) return false;\n");
  }
  printer->Outdent();
  printer->Print(
    "  return true;\n"
    "}\n\n");
  // Hash Code.
  printer->Print(
    "public int hashCode() {\n"
    "  int hash = 41 * getClass().getName().hashCode();\n");
  printer->Indent();
  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* field = descriptor_->field(i);
    field_generators_.get(field).GenerateCalculateHash(printer);
  }
  if (descriptor_->extension_range_count() > 0) {
    printer->Print(
      "hash += extensionsHashCode();\n");
  }
  printer->Outdent();
  printer->Print(
    "  return hash;\n"
    "}\n\n");
}

// ===================================================================

void MessageGenerator::GenerateExtensionRegistrationCode(io::Printer* printer) {
  for (int i = 0; i < descriptor_->extension_count(); i++) {
    ExtensionGenerator(descriptor_->extension(i))
      .GenerateRegistrationCode(printer);
  }

  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    MessageGenerator(descriptor_->nested_type(i))
      .GenerateExtensionRegistrationCode(printer);
  }
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

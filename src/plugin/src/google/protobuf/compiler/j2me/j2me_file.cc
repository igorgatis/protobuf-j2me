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

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>
#include "j2me_enum.h"
#include "j2me_extension.h"
#include "j2me_file.h"
#include "j2me_helpers.h"
#include "j2me_message.h"
#include "j2me_service.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

namespace {

// Recursively searches the given message to see if it contains any extensions.
bool UsesExtensions(const Message& message) {
  const Reflection* reflection = message.GetReflection();

  // We conservatively assume that unknown fields are extensions.
  if (reflection->GetUnknownFields(message).field_count() > 0) return true;

  vector<const FieldDescriptor*> fields;
  reflection->ListFields(message, &fields);

  for (int i = 0; i < fields.size(); i++) {
    if (fields[i]->is_extension()) return true;

    if (GetJavaType(fields[i]) == JAVATYPE_MESSAGE) {
      if (fields[i]->is_repeated()) {
        int size = reflection->FieldSize(message, fields[i]);
        for (int j = 0; j < size; j++) {
          const Message& sub_message =
            reflection->GetRepeatedMessage(message, fields[i], j);
          if (UsesExtensions(sub_message)) return true;
        }
      } else {
        const Message& sub_message = reflection->GetMessage(message, fields[i]);
        if (UsesExtensions(sub_message)) return true;
      }
    }
  }

  return false;
}


}  // namespace

FileGenerator::FileGenerator(const FileDescriptor* file)
  : file_(file),
    java_package_(FileJavaPackage(file)),
    classname_(FileClassName(file)) {}

FileGenerator::~FileGenerator() {}

bool FileGenerator::Validate(string* error) {
  // Check that no class name matches the file's class name.  This is a common
  // problem that leads to Java compile errors that can be hard to understand.
  // It's especially bad when using the java_multiple_files, since we would
  // end up overwriting the outer class with one of the inner ones.

  bool found_conflict = false;
  for (int i = 0; i < file_->enum_type_count() && !found_conflict; i++) {
    if (file_->enum_type(i)->name() == classname_) {
      found_conflict = true;
    }
  }
  for (int i = 0; i < file_->message_type_count() && !found_conflict; i++) {
    if (file_->message_type(i)->name() == classname_) {
      found_conflict = true;
    }
  }
  for (int i = 0; i < file_->service_count() && !found_conflict; i++) {
    if (file_->service(i)->name() == classname_) {
      found_conflict = true;
    }
  }

  if (found_conflict) {
    error->assign(file_->name());
    error->append(
      ": Cannot generate Java output because the file's outer class name, \"");
    error->append(classname_);
    error->append(
      "\", matches the name of one of the types declared inside it.  "
      "Please either rename the type or use the java_outer_classname "
      "option to specify a different outer class name for the .proto file.");
    return false;
  }

  return true;
}

void FileGenerator::Generate(io::Printer* printer) {
  // We don't import anything because we refer to all classes by their
  // fully-qualified names in the generated source.
  printer->Print(
    "// Generated by the protocol buffer compiler.  DO NOT EDIT!\n"
    "// source: $filename$\n"
    "\n",
    "filename", file_->name());
  if (!java_package_.empty()) {
    printer->Print(
      "package $package$;\n"
      "\n",
      "package", java_package_);
  }
  printer->Print(
    "public final class $classname$ {\n"
    "  private $classname$() {}\n",
    "classname", classname_);
  printer->Indent();

  // -----------------------------------------------------------------

  if (!file_->options().java_multiple_files()) {
    for (int i = 0; i < file_->enum_type_count(); i++) {
      EnumGenerator(file_->enum_type(i)).Generate(printer);
    }
    for (int i = 0; i < file_->message_type_count(); i++) {
      MessageGenerator(file_->message_type(i)).Generate(printer);
    }
    if (HasGenericServices(file_)) {
      for (int i = 0; i < file_->service_count(); i++) {
        ServiceGenerator(file_->service(i)).Generate(printer);
      }
    }
  }

  // Extensions must be generated in the outer class since they are values,
  // not classes.
  for (int i = 0; i < file_->extension_count(); i++) {
    ExtensionGenerator(file_->extension(i)).Generate(printer);
  }

  // Static variables.
  for (int i = 0; i < file_->message_type_count(); i++) {
    // TODO(kenton):  Reuse MessageGenerator objects?
    MessageGenerator(file_->message_type(i)).GenerateStaticVariables(printer);
  }

  printer->Print("\n");

  printer->Print(
    "static {\n");
  printer->Indent();

  for (int i = 0; i < file_->message_type_count(); i++) {
    // TODO(kenton):  Reuse MessageGenerator objects?
    MessageGenerator(file_->message_type(i))
      .GenerateStaticVariableInitializers(printer);
  }

  for (int i = 0; i < file_->extension_count(); i++) {
    // TODO(kenton):  Reuse ExtensionGenerator objects?
    ExtensionGenerator(file_->extension(i))
      .GenerateInitializationCode(printer);
  }

  for (int i = 0; i < file_->extension_count(); i++) {
    ExtensionGenerator(file_->extension(i)).GenerateRegistrationCode(printer);
  }

  for (int i = 0; i < file_->message_type_count(); i++) {
    MessageGenerator(file_->message_type(i))
      .GenerateExtensionRegistrationCode(printer);
  }

  printer->Outdent();
  printer->Print(
    "}\n");

  // Dummy function we can use to force the static initialization block to
  // run.  Needed by inner classes.  Cannot be private due to
  // java_multiple_files option.
  printer->Print(
    "\n"
    "public static void internalForceInit() {}\n");

  printer->Print(
    "\n"
    "// @@protoc_insertion_point(outer_class_scope)\n");

  printer->Outdent();
  printer->Print("}\n");
}

void FileGenerator::GenerateEmbeddedDescriptor(io::Printer* printer) {
  // There is no embeded Descriptor for J2ME implementation.
}

template<typename GeneratorClass, typename DescriptorClass>
static void GenerateSibling(const string& package_dir,
                            const string& java_package,
                            const DescriptorClass* descriptor,
                            GeneratorContext* context,
                            vector<string>* file_list) {
  string filename = package_dir + descriptor->name() + ".java";
  file_list->push_back(filename);

  scoped_ptr<io::ZeroCopyOutputStream> output(context->Open(filename));
  io::Printer printer(output.get(), '$');

  printer.Print(
    "// Generated by the protocol buffer compiler.  DO NOT EDIT!\n"
    "\n");
  if (!java_package.empty()) {
    printer.Print(
      "package $package$;\n"
      "\n",
      "package", java_package);
  }

  GeneratorClass(descriptor).Generate(&printer);
}

void FileGenerator::GenerateSiblings(const string& package_dir,
                                     GeneratorContext* context,
                                     vector<string>* file_list) {
  if (file_->options().java_multiple_files()) {
    for (int i = 0; i < file_->enum_type_count(); i++) {
      GenerateSibling<EnumGenerator>(package_dir, java_package_,
                                     file_->enum_type(i),
                                     context, file_list);
    }
    for (int i = 0; i < file_->message_type_count(); i++) {
      GenerateSibling<MessageGenerator>(package_dir, java_package_,
                                        file_->message_type(i),
                                        context, file_list);
    }
    if (HasGenericServices(file_)) {
      for (int i = 0; i < file_->service_count(); i++) {
        GenerateSibling<ServiceGenerator>(package_dir, java_package_,
                                          file_->service(i),
                                          context, file_list);
      }
    }
  }
}

bool FileGenerator::ShouldIncludeDependency(const FileDescriptor* descriptor) {
  return true;
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

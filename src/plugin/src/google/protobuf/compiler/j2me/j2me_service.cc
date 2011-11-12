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

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>
#include "j2me_service.h"
#include "j2me_helpers.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace j2me {

ServiceGenerator::ServiceGenerator(const ServiceDescriptor* descriptor)
  : descriptor_(descriptor) {}

ServiceGenerator::~ServiceGenerator() {}

void ServiceGenerator::Generate(io::Printer* printer) {
  bool is_own_file = descriptor_->file()->options().java_multiple_files();
  map<string, string> vars;
  vars["static"] = is_own_file ? "" : "static";
  vars["classname"] = descriptor_->name();
  printer->Print(vars,
    "public $static$ abstract class $classname$ {\n");
  printer->Indent();

  printer->Print(
      "protected $classname$() {}\n\n",
      "classname", descriptor_->name());

  GenerateInterface(printer);
  GenerateBlockingInterface(printer);
  GenerateStub(printer);
  GenerateBlockingStub(printer);

  printer->Outdent();
  printer->Print("}\n\n");
}

void ServiceGenerator::GenerateInterface(io::Printer* printer) {
  printer->Print("public interface Interface {\n");
  printer->Indent();
  GenerateAbstractMethods(printer);
  printer->Outdent();
  printer->Print("}\n\n");
}

void ServiceGenerator::GenerateAbstractMethods(io::Printer* printer) {
  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    GenerateMethodSignature(printer, method, IS_ABSTRACT);
    printer->Print(";\n\n");
  }
}

void ServiceGenerator::GenerateStub(io::Printer* printer) {
  printer->Print(
    "public static Stub newStub(\n"
    "    com.google.protobuf.RpcChannel channel) {\n"
    "  return new Stub(channel);\n"
    "}\n"
    "\n"
    "public static final class Stub extends $classname$ implements Interface {"
    "\n",
    "classname", ClassName(descriptor_));
  printer->Indent();

  printer->Print(
    "private Stub(com.google.protobuf.RpcChannel channel) {\n"
    "  this.channel = channel;\n"
    "}\n"
    "\n"
    "private final com.google.protobuf.RpcChannel channel;\n"
    "\n"
    "public com.google.protobuf.RpcChannel getChannel() {\n"
    "  return channel;\n"
    "}\n");

  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    printer->Print("\n");
    GenerateMethodSignature(printer, method, IS_CONCRETE);
    printer->Print(" {\n");
    printer->Indent();

    map<string, string> vars;
    vars["service"] = descriptor_->full_name();
    vars["name"] = method->name();
    vars["output"] = ClassName(method->output_type());
    printer->Print(vars,
      "channel.callMethod(\n"
      "  \"$service$.$name$\",\n"
      "  controller,\n"
      "  request,\n"
      "  $output$.getDefaultInstance(),\n"
      "    done);\n");

    printer->Outdent();
    printer->Print("}\n");
  }

  printer->Outdent();
  printer->Print(
    "}\n"
    "\n");
}

void ServiceGenerator::GenerateBlockingInterface(io::Printer* printer) {
  printer->Print(
    "public interface BlockingInterface {");
  printer->Indent();

  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    GenerateBlockingMethodSignature(printer, method);
    printer->Print(";\n");
  }

  printer->Outdent();
  printer->Print(
    "}\n"
    "\n");
}

void ServiceGenerator::GenerateBlockingStub(io::Printer* printer) {
  printer->Print(
    "public static BlockingInterface newBlockingStub(\n"
    "    com.google.protobuf.BlockingRpcChannel channel) {\n"
    "  return new BlockingStub(channel);\n"
    "}\n"
    "\n");

  printer->Print(
    "private static final class BlockingStub implements BlockingInterface {\n");
  printer->Indent();

  printer->Print(
    "private BlockingStub(com.google.protobuf.BlockingRpcChannel channel) {\n"
    "  this.channel = channel;\n"
    "}\n"
    "\n"
    "private final com.google.protobuf.BlockingRpcChannel channel;\n");

  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor* method = descriptor_->method(i);
    GenerateBlockingMethodSignature(printer, method);
    printer->Print(" {\n");
    printer->Indent();

    map<string, string> vars;
    vars["service"] = descriptor_->full_name();
    vars["name"] = method->name();
    vars["output"] = ClassName(method->output_type());
    printer->Print(vars,
      "return ($output$) channel.callBlockingMethod(\n"
      "  \"$service$.$name$\",\n"
      "  controller,\n"
      "  request,\n"
      "  $output$.getDefaultInstance());\n");

    printer->Outdent();
    printer->Print(
      "}\n"
      "\n");
  }

  printer->Outdent();
  printer->Print("}\n");
}

void ServiceGenerator::GenerateMethodSignature(io::Printer* printer,
                                               const MethodDescriptor* method,
                                               IsAbstract is_abstract) {
  map<string, string> vars;
  vars["name"] = UnderscoresToCamelCase(method);
  vars["input"] = ClassName(method->input_type());
  vars["abstract"] = (is_abstract == IS_ABSTRACT) ? "abstract" : "";
  printer->Print(vars,
    "public $abstract$ void $name$(\n"
    "    com.google.protobuf.RpcController controller,\n"
    "    $input$ request,\n"
    "    com.google.protobuf.RpcCallback done)");
}

void ServiceGenerator::GenerateBlockingMethodSignature(
    io::Printer* printer,
    const MethodDescriptor* method) {
  map<string, string> vars;
  vars["method"] = UnderscoresToCamelCase(method);
  vars["input"] = ClassName(method->input_type());
  vars["output"] = ClassName(method->output_type());
  printer->Print(vars,
    "\n"
    "public $output$ $method$(\n"
    "    com.google.protobuf.RpcController controller,\n"
    "    $input$ request)\n"
    "    throws com.google.protobuf.ServiceException");
}

}  // namespace j2me
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

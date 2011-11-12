#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <string>
#include <vector>

#include <google/protobuf/compiler/command_line_interface.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/compiler/java/java_generator.h>
#include <google/protobuf/compiler/python/python_generator.h>
#include <google/protobuf/descriptor.proto.h>
#include <google/protobuf/j2me.proto.h>

#include "j2me_generator.h"

using namespace std;

bool appendPath(string* path, const string& name) {
#ifdef _WIN32
  static const string kFileSep = "\\";
#else
  static const string kFileSep = "/";
#endif
  path->append(kFileSep);
  path->append(name);
  return true;
}

bool create_dir(const string& parent, const string& name, string* path) {
  path->assign(parent);
  if (!appendPath(path, name)) {
    return false;
  }
  struct stat st;
  if (stat(path->c_str(), &st) != 0) {
    // Path does not exist.
    if (mkdir(path->c_str(), S_IRWXU | S_IRWXG) != 0) {
      cerr << "Could not create folder: " << *path << endl;
      return false;
    }
  } else if (!S_ISDIR(st.st_mode)) {
    cerr << "Path '" << *path << "' is not a directory.";
    return false;
  }
  return true;
}

bool dump_descriptor(const string& path,
                     const string& filename,
                     const string& content) {
  string file_path(path);
  if (!appendPath(&file_path, filename)) {
    return false;
  }
  FILE* proto_file = fopen(file_path.c_str(), "w");
  fprintf(proto_file, "%s", content.c_str());
  fflush(proto_file);
  fclose(proto_file);
  return true;
}

bool dump_descriptors(const string& root) {
  string path;
  if (!create_dir(root, "google", &path) ||
      !create_dir(path, "protobuf", &path)) {
    return false;
  }
  if (!dump_descriptor(path, "descriptor.proto", k_descriptor_proto_content)) {
    return false;
  }
  if (!dump_descriptor(path, "j2me.proto", k_j2me_proto_content)) {
    return false;
  }
  return true;
}

bool get_temp_path(string* path) {
  // TODO(igorgatis@gmail.com): make this portable and correct.
#ifdef _WIN32
  *path = "c:\\temp";
#else
  *path = "/tmp";
#endif
  appendPath(path, "protobuf-j2me");
  mkdir(path->c_str(), S_IRWXU | S_IRWXG);
  return true;
}

int main(int argc, char* argv[]) {
  google::protobuf::compiler::CommandLineInterface cli;
  cli.AllowPlugins("protoc-");

  // Proto2 C++
  google::protobuf::compiler::cpp::CppGenerator cpp_generator;
  cli.RegisterGenerator("--cpp_out", &cpp_generator,
                        "Generate C++ header and source.");

  // Proto2 Java
  google::protobuf::compiler::java::JavaGenerator java_generator;
  cli.RegisterGenerator("--java_out", &java_generator,
                        "Generate Java source file.");

  // Proto2 Python
  google::protobuf::compiler::python::Generator py_generator;
  cli.RegisterGenerator("--python_out", &py_generator,
                        "Generate Python source file.");

  // Proto2 J2ME
  google::protobuf::compiler::j2me::J2meGenerator j2me_generator;
  cli.RegisterGenerator("--j2me_out", &j2me_generator,
                        "Generate J2ME source file.");

  // Dumping built-in protos.
  string path;
  if (!get_temp_path(&path)) {
    cerr << "Could not resolve temporary folder." << endl;
    return -1;
  }
  if (!dump_descriptors(path)) {
    cerr << "Could not dump built-in protos under '" << path << "'." << endl;
    return -1;
  }

  // Rewriting args to include built-in protos.
  const char** new_argv = new const char*[argc + 1];
  const string built_in_protos_argv = "--proto_path=" + path;
  for (int i = 0; i < argc; i++) {
    new_argv[i] = argv[i];
  }
  new_argv[argc] = built_in_protos_argv.c_str();
  const int result = cli.Run(argc + 1, new_argv);
  delete[] new_argv;
  return result;
}

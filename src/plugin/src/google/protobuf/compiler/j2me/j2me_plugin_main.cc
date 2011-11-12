#include <google/protobuf/compiler/plugin.h>
#include "j2me_generator.h"

int main(int argc, char* argv[]) {
  google::protobuf::compiler::j2me::J2meGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}

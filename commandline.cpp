#include "llvm/Support/CommandLine.h"

static llvm::cl::opt<bool>
    MyOption("my-option", llvm::cl::desc("An option named '-my-option'"));

int main(int argc, char **argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv);
  return 0;
}

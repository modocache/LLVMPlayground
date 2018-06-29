#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/Utils.h"
#include <iostream>
#include <vector>

int main() {
  std::vector<const char *> Args;
  // The Clang invocation name is the name of this file:
  Args.push_back(__FILE__);
  // The invocation takes as input one file to compile -- this one:
  Args.push_back(__FILE__);

  std::unique_ptr<clang::CompilerInvocation> Invocation =
      clang::createInvocationFromCommandLine(Args);

  std::cout << "Module hash: " << Invocation->getModuleHash() << std::endl;
  return 0;
}

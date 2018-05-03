#include "llvm/Support/Path.h"
#include <iostream>

int main() {
  std::cout << llvm::sys::path::filename("<unknown>").str() << std::endl;
  std::cout << llvm::sys::path::filename("-").str() << std::endl;
  std::cout << llvm::sys::path::filename("").str() << std::endl;
  std::cout << llvm::sys::path::filename("/path/to/foo.swift").str() << std::endl;
  std::cout << llvm::sys::path::filename("~/foo.swift").str() << std::endl;
  std::cout << llvm::sys::path::filename("./path/to/foo.swift").str() << std::endl;
  std::cout << llvm::sys::path::filename("foo.swift").str() << std::endl;
}

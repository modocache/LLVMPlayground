#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include <iostream>

class MyRefCountedClass : public llvm::RefCountedBase<MyRefCountedClass> {
public:
  void sayHello() { std::cout << "Hello!" << std::endl; }
  ~MyRefCountedClass() { std::cout << "Good-bye!" << std::endl; }
};

int main() {
  {
    llvm::IntrusiveRefCntPtr<MyRefCountedClass> M = new MyRefCountedClass();
    M->sayHello();
  }

  return 0;
}

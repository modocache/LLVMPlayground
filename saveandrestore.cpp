#include "llvm/Support/SaveAndRestore.h"
#include <cassert>


static int Value = 0;

static void doSomething() {
  llvm::SaveAndRestore<int> S(Value, 10);
  assert(Value == 10);
}

int main() {
  assert(Value == 0);
  doSomething();
  assert(Value == 0);
  return 0;
}

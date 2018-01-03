#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MemoryBuffer.h"

#include <iostream>
#include <memory>
#include <system_error>

using namespace llvm;

int main() {
  auto FileName = __FILE__;
  std::cout << "Opening file \"" << FileName << "\"..." << std::endl;

  ErrorOr<std::unique_ptr<MemoryBuffer>> MemBufferOrErr =
      MemoryBuffer::getFile(FileName);
  if (!MemBufferOrErr) {
    std::error_code ErrorCode = MemBufferOrErr.getError();
    std::cerr << "An error occurred when opening file \"" << FileName
              << "\": " << ErrorCode << std::endl;
    return 1;
  }

  std::unique_ptr<MemoryBuffer> MemBuffer = std::move(MemBufferOrErr.get());
  std::cout << "Opened a buffer with identifier \""
            << MemBuffer->getBufferIdentifier().str() << "\", and a size of "
            << MemBuffer->getBufferSize() << " bytes." << std::endl;

  StringRef Buffer = MemBuffer->getBuffer();
  std::cout << "The buffer contains " << Buffer.count('\n') << " lines.";
  return 0;
}

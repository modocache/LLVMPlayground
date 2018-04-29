#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"

#include <iostream>
#include <memory>
#include <system_error>

using namespace llvm;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "error: incorrect number of arguments.\n\nUsage:\n"
              << argv[0] << " <file-to-open-in-memory-buffer>"
              << std::endl;
    return 1;
  }

  StringRef FileName = argv[1];
  std::cout << "Opening file \"" << FileName.str() << "\"..." << std::endl;

  ErrorOr<std::unique_ptr<MemoryBuffer>> MemBufferOrErr =
      MemoryBuffer::getFileOrSTDIN(FileName);

  int FD;
  SmallString<128> RealPath;
  std::error_code OpenErrorCode =
    sys::fs::openFileForRead(FileName, FD, &RealPath);

  if (!MemBufferOrErr) {
    std::error_code ErrorCode = MemBufferOrErr.getError();
    std::cerr << "An error occurred when opening file \"" << FileName.str()
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

#include <cerrno>
#include <iostream>
#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  // I'll open this file itself and read it into memory.
  auto FileName = __FILE__;

  // The system call open(2) gets a file descriptor
  // representing the open file.
  int OpenFlags = O_RDONLY;
  int FD = open(FileName, OpenFlags);

  // open(2) returns a -1 if the file could not be opened.
  // In this case, print an error and return.
  if (FD < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not open file \""
              << FileName << "\": " << Err.message()
              << std::endl;
    return 1;
  }

  // Syscall fstat populates the struct stat pointer
  // with information about the given file descriptor,
  // including the file's size in bytes.
  struct stat Stat;
  if (fstat(FD, &Stat) < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not acquire information "
              << "on file descriptor \"" << FD
              << "\": " << Err.message() << std::endl;
    return 1;
  }

  off_t FileSize = Stat.st_size;
  std::cout << "[NOTE] File size: " << FileSize << " bytes"
            << std::endl;

  // Allocate memory in size equal to the number of bytes
  // in the file.
  char *Memory = static_cast<char *>(operator new(
      FileSize + 1, std::nothrow));
  Memory[FileSize] = 0;

  // Use syscall read(2) to read in bytes from the given
  // file descriptor, into the prepared buffer, 16 bytes
  // at a time.
  const ssize_t ChunkSize = 16;
  ssize_t Offset = 0;
  ssize_t ReadBytes = 0;
  do {
    ReadBytes = read(FD, Memory + Offset, ChunkSize);
    if (ReadBytes < 0) {
      std::error_code Err(errno, std::generic_category());
      std::cerr << "[ERROR] Could not read from file "
                   "descriptor \""
                << FD << "\": " << Err.message()
                << std::endl;
      delete Memory;
      return 1;
    }
    Offset += ReadBytes;
  } while (ReadBytes != 0);

  // I've now read the file into memory. To demonstrate:
  std::cout << "[NOTE] Here's the first line "
            << "of the file: \"";
  char *Ptr = Memory;
  while (*Ptr != '\n' && *Ptr != '\0') {
    std::cout << *Ptr;
    ++Ptr;
  }
  std::cout << "\"" << std::endl;

  // Once I'm done with the file, I need to delete the
  // memory I allocated, otherwise this is a memory leak.
  delete Memory;

  // Finally, I need to close the open file descriptor,
  // using the system call close(2).
  if (close(FD) < 0) {
    std::error_code Err(errno, std::generic_category());
    std::cerr << "[ERROR] Could not close file "
              << "descriptor \"" << FD << "\":"
              << Err.message() << std::endl;
    return 1;
  }

  return 0;
}

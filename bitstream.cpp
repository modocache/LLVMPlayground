#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Bitcode/BitstreamWriter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include <cstring>
#include <iostream>
#include <system_error>

using namespace llvm;

int main() {
  SmallVector<char, 0> Buffer;
  BitstreamWriter Writer(Buffer);

  const unsigned char Signature[] = {0xde, 0xad, 0xbe, 0xef};
  for (unsigned char Byte : Signature)
    Writer.Emit(Byte, 8);

  {
    Writer.EnterSubblock(llvm::bitc::BLOCKINFO_BLOCK_ID, 2);

    {
      SmallVector<unsigned, 1> BlockIDBuffer;
      BlockIDBuffer.push_back(8);
      Writer.EmitRecord(llvm::bitc::BLOCKINFO_CODE_SETBID, BlockIDBuffer);
    }

    {
      SmallVector<unsigned char, 64> NameBuffer;
      StringRef Name;

      {
        Name = "PHILOSOPHICAL_BLOCK";
        NameBuffer.resize(Name.size());
        memcpy(NameBuffer.data(), Name.data(), Name.size());
        Writer.EmitRecord(llvm::bitc::BLOCKINFO_CODE_BLOCKNAME, NameBuffer);
      }

      {
        Name = "PHILOSOPHER_NAME";
        NameBuffer.resize(Name.size() + 1);
        NameBuffer[0] = 0;
        memcpy(NameBuffer.data() + 1, Name.data(), Name.size());
        Writer.EmitRecord(llvm::bitc::BLOCKINFO_CODE_SETRECORDNAME, NameBuffer);

        Name = "YEAR_OF_BIRTH";
        NameBuffer.resize(Name.size() + 1);
        NameBuffer[0] = 1;
        memcpy(NameBuffer.data() + 1, Name.data(), Name.size());
        Writer.EmitRecord(llvm::bitc::BLOCKINFO_CODE_SETRECORDNAME, NameBuffer);
      }
    }

    Writer.ExitBlock();
  }

  {
    Writer.EnterSubblock(8, 3);

    {
      SmallVector<unsigned char, 64> NameBuffer;
      SmallVector<unsigned, 1> DOBBuffer;

      StringRef Name = "Georg Wilhelm Friedrich Hegel";
      NameBuffer.resize(Name.size());
      memcpy(NameBuffer.data(), Name.data(), Name.size());
      Writer.EmitRecord(0, NameBuffer);

      DOBBuffer.push_back(1770);
      Writer.EmitRecord(1, DOBBuffer);
    }
    {
      SmallVector<unsigned char, 64> NameBuffer;
      SmallVector<unsigned, 1> DOBBuffer;

      StringRef Name = "Friedrich Wilhelm Nietzsche";
      NameBuffer.resize(Name.size());
      memcpy(NameBuffer.data(), Name.data(), Name.size());
      Writer.EmitRecord(0, NameBuffer);

      DOBBuffer.push_back(1844);
      Writer.EmitRecord(1, DOBBuffer);
    }

    Writer.ExitBlock();
  }

  std::error_code EC;
  raw_fd_ostream Out("build/bitstream.bc", EC, sys::fs::F_None);
  if (EC) {
    std::cerr << "Could not open file: " << EC << std::endl;
    return -1;
  }

  Out.write((char *)&Buffer.front(), Buffer.size());
  return 0;
}

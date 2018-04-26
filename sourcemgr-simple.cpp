#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"

int main() {
  llvm::StringRef IncludeInput = "import foo";
  llvm::SourceMgr SourceManager;
  SourceManager.AddNewSourceBuffer(
      std::move(
        llvm::MemoryBuffer::getMemBuffer(IncludeInput,
                                         "include.swift")),
      /*IncludeLoc*/ llvm::SMLoc());
  llvm::SMLoc IncludeLocation =
    llvm::SMLoc::getFromPointer(
        SourceManager.getMemoryBuffer(1)->getBufferStart());

  // This string will represent our source program.
  llvm::StringRef Input =
    "func foo() {\n"
    "  print(\"Hello!\")\n"
    "}";

  // The llvm::MemoryBuffer class is used to store
  // large strings, along with metadata such as a
  // buffer or file name. Here we instantiate a
  // llvm::MemoryBuffer to store the contents of
  // our source program.
  std::unique_ptr<llvm::MemoryBuffer> InputBuffer =
    llvm::MemoryBuffer::getMemBuffer(Input, "foo.swift");

  // The llvm::SourceMgr class is used to emit
  // diagnostics for one or more llvm::MemoryBuffer
  // instances. Here we instantiate a new
  // llvm::SourceMgr and transfer ownership of our
  // input buffer over to it.
  SourceManager.AddNewSourceBuffer(
      std::move(InputBuffer),
      /*IncludeLoc*/ IncludeLocation);

  // Here we grab a pointer into the buffer.
  // Incrementing and decrementing this pointer
  // allows us to traverse the source program.
  const llvm::MemoryBuffer *SourceBuffer =
    SourceManager.getMemoryBuffer(2);
  const char *CurrentCharacter =
    SourceBuffer->getBufferStart();

  // The llvm::SMLoc class is used to represent a
  // location in an llvm::MemoryBuffer that is managed
  // by llvm::SourceMgr. We instantiate an llvm::SMLoc
  // here, for the starting location.
  llvm::SMLoc BufferStartLocation =
    llvm::SMLoc::getFromPointer(CurrentCharacter);

  // The llvm::SourceMgr::PrintMessage function allows
  // us to print a caret ^ at a specific llvm::SMLoc
  // location.
  SourceManager.PrintMessage(
      BufferStartLocation,
      llvm::SourceMgr::DiagKind::DK_Remark,
      "This is the very beginning of the "
      "source buffer.");

  // Let's increment our buffer pointer until we find
  // the first quotation mark character: the first "
  // in the source text line 'print("Hello!")'. Then
  // let's record that in an llvm::SMLoc location.
  while (*CurrentCharacter != '"')
    ++CurrentCharacter;
  llvm::SMLoc StartLocation =
    llvm::SMLoc::getFromPointer(CurrentCharacter);

  // Next, let's get the llvm::SMLoc location
  // representing the end of the string "Hello!",
  // by finding the first character past the last
  // quotation mark.
  while (*CurrentCharacter != ')')
    ++CurrentCharacter;
  llvm::SMLoc EndLocation =
    llvm::SMLoc::getFromPointer(CurrentCharacter);

  // The llvm::SMRange class represents a range: a
  // beginning and an end llvm::SMLoc location.
  llvm::SMRange Range = llvm::SMRange(StartLocation,
                                      EndLocation);

  // We can print a warning that points to this
  // llvm::SMRange range.
  SourceManager.PrintMessage(
      StartLocation,
      llvm::SourceMgr::DiagKind::DK_Warning,
      "This is the range of source text in which "
      "a string literal appears.",
      Range);

  // The llvm::SMFixIt class allows us to print
  // a replacement suggestion underneath the
  // caret ^ output.
  SourceManager.PrintMessage(
      StartLocation,
      llvm::SourceMgr::DiagKind::DK_Note,
      "This is a fix-it that suggests an "
      "alternative string.",
      llvm::None,
      llvm::SMFixIt(Range, "\"Good-bye!\""));

  return 0;
}

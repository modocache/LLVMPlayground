#include "llvm/ADT/ArrayRef.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Option/Option.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <memory>
#include <vector>

// These option IDs are used to uniquely identify option groups and options
// themselves. The order here appears to be significant: invalid as zero,
// followed by groups, followed by input and then unknown, followed by all
// other options.
enum PlaygroundOptionID {
  Invalid = 0,
  All_Group,
  Input,
  Unknown,
  Help,
  X,
};

static const char *const NoPrefix[] = {nullptr};
static const char *const SingleDashPrefix[] = {"-", nullptr};

static const llvm::opt::OptTable::Info InfoTable[] = {
    // Option Groups
    {/* Prefixes */ NoPrefix,
     /* Name */ "<all options>",
     /* HelpText */ nullptr,
     /* MetaVar */ nullptr,
     /* ID */ PlaygroundOptionID::All_Group,
     /* Kind */ llvm::opt::Option::GroupClass,
     /* Param */ 0,
     /* Flags */ 0,
     /* GroupID */ PlaygroundOptionID::Invalid,
     /* AliasID */ PlaygroundOptionID::Invalid,
     /* AliasArgs */ nullptr,
     /* Values */ nullptr},
    // Options
    {/* Prefixes */ NoPrefix,
     /* Name */ "<input>",
     /* HelpText */ nullptr,
     /* MetaVar */ nullptr,
     /* ID */ PlaygroundOptionID::Input,
     /* Kind */ llvm::opt::Option::InputClass,
     /* Param */ 0,
     /* Flags */ 0,
     /* GroupID */ PlaygroundOptionID::Invalid,
     /* AliasID */ PlaygroundOptionID::Invalid,
     /* AliasArgs */ nullptr,
     /* Values */ nullptr},
    {/* Prefixes */ NoPrefix,
     /* Name */ "<unknown>",
     /* HelpText */ nullptr,
     /* MetaVar */ nullptr,
     /* ID */ PlaygroundOptionID::Unknown,
     /* Kind */ llvm::opt::Option::UnknownClass,
     /* Param */ 0,
     /* Flags */ 0,
     /* GroupID */ PlaygroundOptionID::Invalid,
     /* AliasID */ PlaygroundOptionID::Invalid,
     /* AliasArgs */ nullptr,
     /* Values */ nullptr},
    {/* Prefixes */ SingleDashPrefix,
     /* Name */ "help",
     /* HelpText */ "Print this help message and exit",
     /* MetaVar */ nullptr,
     /* ID */ PlaygroundOptionID::Help,
     /* Kind */ llvm::opt::Option::FlagClass,
     /* Param */ 0,
     /* Flags */ 0,
     /* GroupID */ PlaygroundOptionID::All_Group,
     /* AliasID */ PlaygroundOptionID::Invalid,
     /* AliasArgs */ nullptr,
     /* Values */ nullptr},
    {/* Prefixes */ SingleDashPrefix,
     /* Name */ "x",
     /* HelpText */ "An option named '-x'",
     /* MetaVar */ nullptr,
     /* ID */ PlaygroundOptionID::X,
     /* Kind */ llvm::opt::Option::FlagClass,
     /* Param */ 0,
     /* Flags */ 0,
     /* GroupID */ PlaygroundOptionID::All_Group,
     /* AliasID */ PlaygroundOptionID::Invalid,
     /* AliasArgs */ nullptr,
     /* Values */ nullptr},
};

// We're required to implement this subclass because the OptTable constructor
// is protected.
class PlaygroundOptTable : public llvm::opt::OptTable {
public:
  PlaygroundOptTable() : llvm::opt::OptTable(InfoTable) {}
};

int main(int argc, char **argv) {
  PlaygroundOptTable T;

  unsigned IncludedFlagsBitmask = 0;
  unsigned ExcludedFlagsBitmask = 0;
  unsigned MissingArgIndex, MissingArgCount;
  auto ArgumentList = std::make_unique<llvm::opt::InputArgList>(T.ParseArgs(
      llvm::ArrayRef<const char *>(argv + 1, argc - 1), MissingArgIndex,
      MissingArgCount, IncludedFlagsBitmask, ExcludedFlagsBitmask));

  if (ArgumentList->hasArg(PlaygroundOptionID::Help)) {
    T.PrintHelp(llvm::outs(), "commandline",
                "LLVM Playground Executable 'commandline'");
    return 0;
  }

  if (MissingArgCount) {
    std::cerr << "Missing argument" << std::endl;
    return 1;
  }

  for (const llvm::opt::Arg *A :
       ArgumentList->filtered(PlaygroundOptionID::Input)) {
    std::cout << "Input argument: " << A->getAsString(*ArgumentList)
              << std::endl;
  }

  for (const llvm::opt::Arg *A :
       ArgumentList->filtered(PlaygroundOptionID::Unknown)) {
    auto Argument = A->getAsString(*ArgumentList);
    std::string Nearest;
    T.findNearest(Argument, Nearest);
    if (Nearest.empty()) {
      std::cerr << "Unknown argument: " << Argument << std::endl;
    } else {
      std::cerr << "Unknown argument '" << Argument << "', did you mean '"
                << Nearest << "'?" << std::endl;
    }
  }

  return 0;
}

# LLVMPlayground

This is a CMake project I use to experiment with LLVM and Clang libraries,
by writing small example programs that use them.

To use it yourself, build and install into your path LLVM, Clang, and
clang-format, then run:

```
git clone https://github.com/modocache/LLVMPlayground.git
make -C LLVMPlayground
```

The example programs are in `LLVMPlayground/build/playground`.
For example, to run a program that uses `llvm::MemoryBuffer`, run:

```
./LLVMPlayground/build/playground/memorybuffer
```

To add your own example program, add a call to `add_playground_executable`
to the `CMakeLists.txt` file.

## How this project is built

The source files in this LLVMPlayground repository are configured and built
using CMake. As explained in [this article](https://modocache.io/the-swift-compilers-build-system),
LLVM, Clang, and Swift also use CMake.

Projects that use CMake require two invocations of the `cmake` executable:
one to configure the project, and one to build it based on that configuration.

The Makefile in this project handles this "configure and then build" dance for
the LLVMPlayground project. Running `make` does the following:

1. Runs `cmake` in order to configure the LLVMPlayground project. When
   configuring, CMake automatically discovers LLVM and Clang if they exist in
   your PATH. This allows the LLVMPlayground executables to be linked against
   LLVM and Clang libraries.
2. Runs `cmake --build` in order to build the executables in LLVMPlayground.

# LLVMPlayground

This is a CMake project I use to experiment with LLVM libraries,
by writing small example programs that use them.

To use it yourself:

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
LLVM and Clang, and then for this LLVMPlayground project itself. Running
`make` does the following:

1. Clones LLVM and Clang as Git submodules, placing them in the `external`
   directory.
2. Runs `cmake` in order to configure the LLVM project. LLVM's CMake allows
   Clang to be configured as well, by using its `LLVM_ENABLE_PROJECTS` CMake
   option.
3. Runs `cmake --build` in order to build LLVM and Clang.
4. Runs `cmake` in order to configure the LLVMPlayground project. When
   configuring, the Makefile passes in the paths to LLVM and Clang CMake files.
   This allows the LLVMPlayground executables to be linked against LLVM and
   Clang libraries.
5. Runs `cmake --build` in order to build the executables in LLVMPlayground.

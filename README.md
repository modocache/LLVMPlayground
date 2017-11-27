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

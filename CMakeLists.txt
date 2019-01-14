cmake_minimum_required(VERSION 3.6.0)
project(LLVMPlayground)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
string(APPEND CMAKE_C_FLAGS " -pedantic -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra")

find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(LLVM_LIBRARIES all)

find_package(CLANG REQUIRED CONFIG)

set(CLANG_LIBRARIES
    clangAnalysis
    clangAST
    clangBasic
    clangCodeGen
    clangDriver
    clangEdit
    clangFormat
    clangFrontend
    clangFrontendTool
    clangIndex
    clangLex
    clangParse
    clangSema
    clangSerialization
    clangToolingCore
    clangRewrite
    clangRewriteFrontend
)


function(add_playground_executable target_name)
  add_executable(${target_name} ${ARGN})
  set_target_properties(
    ${target_name}
    PROPERTIES
      COMPILE_FLAGS
        "-fno-rtti -fno-omit-frame-pointer -fsanitize=address"
      LINK_FLAGS
        "-fno-omit-frame-pointer -fsanitize=address")
  target_include_directories(${target_name}
                             PUBLIC ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
  target_link_libraries(${target_name} ${LLVM_LIBRARIES} ${CLANG_LIBRARIES})
endfunction()


add_playground_executable(memorybuffer memorybuffer.cpp)
add_playground_executable(memorybuffer-argv memorybuffer-argv.cpp)
add_playground_executable(bitstream bitstream.cpp)
add_playground_executable(commandline commandline.cpp)
add_playground_executable(crtp crtp.cpp)
add_playground_executable(driver driver.cpp)
add_playground_executable(mmap-read mmap-read.cpp)
add_playground_executable(opttable opttable.cpp)
add_playground_executable(path path.cpp)
add_playground_executable(read read.cpp)
add_playground_executable(refcountedbase refcountedbase.cpp)
add_playground_executable(sourcemgr sourcemgr.cpp)
add_playground_executable(sourcemgr-simple sourcemgr-simple.cpp)
add_playground_executable(saveandrestore saveandrestore.cpp)

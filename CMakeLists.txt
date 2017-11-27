cmake_minimum_required(VERSION 3.6.0)
project(LLVMPlayground)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
string(APPEND CMAKE_C_FLAGS " -pedantic -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra")


set("${LLVM_CMAKE_CONFIG_PATH}" CACHE STRING
    "Path to the CMake config in an LLVM build directory.")

find_package(LLVM REQUIRED CONFIG NO_DEFAULT_PATH
             PATHS "${LLVM_CMAKE_CONFIG_PATH}")
include("${LLVM_DIR}/LLVMConfig.cmake")
llvm_map_components_to_libnames(LLVM_LIBRARIES all)


function(add_playground_executable target_name)
  add_executable(${target_name} ${ARGN})
  target_include_directories(${target_name} PUBLIC ${LLVM_INCLUDE_DIRS})
  target_link_libraries(${target_name} ${LLVM_LIBRARIES})
endfunction()


add_playground_executable(memorybuffer memorybuffer.cpp)

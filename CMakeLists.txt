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
  add_dependencies(${target_name} GenerateSourceList)
endfunction()

set(find_cmd find ${CMAKE_SOURCE_DIR} -name "*.cpp")
execute_process(COMMAND ${find_cmd} OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/source.txt)
add_custom_target(CurrentListing COMMAND ${find_cmd} > ${CMAKE_CURRENT_BINARY_DIR}/current_listing.txt)
add_custom_target(GenerateSourceList
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/current_listing.txt ${CMAKE_CURRENT_BINARY_DIR}/source.txt
                  DEPENDS CurrentListing)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/source.txt)

file(GLOB programs "*.cpp")
foreach(program ${programs})
  get_filename_component(prog_name ${program} NAME_WE)
  add_playground_executable(${prog_name} ${program})
endforeach()


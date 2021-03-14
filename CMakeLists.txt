cmake_minimum_required (VERSION 3.5)

option (USE_CLANG "build application with clang" ON) 
if(USE_CLANG)
  set (CMAKE_CXX_COMPILER "clang++")
endif(USE_CLANG)

project (yaff)


set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)



include_directories ( 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/fields
)


add_subdirectory(src)

add_custom_target ( copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}
)

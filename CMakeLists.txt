
cmake_minimum_required(VERSION 3.16)
project(mini_compiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

add_executable(mini_compiler
    src/main.cpp
    src/driver.cpp
    src/ast.cpp
    src/sema.cpp
    src/llvm_codegen.cpp
    src/lexer.cpp
    src/parser.cpp
)

target_include_directories(mini_compiler PRIVATE src ${LLVM_INCLUDE_DIRS})
target_link_libraries(mini_compiler ${LLVM_LIBS})
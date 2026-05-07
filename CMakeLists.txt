
cmake_minimum_required(VERSION 3.20)

project(mini_compiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Bison
find_package(BISON REQUIRED)

# re2c
find_program(RE2C_EXECUTABLE re2c REQUIRED)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Parser generation
BISON_TARGET(
    Parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
    ${GENERATED_DIR}/parser.cpp
    DEFINES_FILE ${GENERATED_DIR}/parser.hpp
)

# Lexer generation
set(LEXER_RE ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.re)
set(LEXER_CPP ${GENERATED_DIR}/lexer.cpp)

add_custom_command(
    OUTPUT ${LEXER_CPP}
    COMMAND ${RE2C_EXECUTABLE}
            -o ${LEXER_CPP}
            ${LEXER_RE}
    DEPENDS ${LEXER_RE}
    COMMENT "Generating lexer with re2c"
)

# LLVM components
llvm_map_components_to_libnames(
    LLVM_LIBS
    core
    support
    irreader
    native
    passes
)

add_executable(mini_compiler
    src/main.cpp
    src/driver.cpp
    src/ast.cpp
    src/sema.cpp
    src/llvm_codegen.cpp
    src/lexer.cpp
    src/parser.cpp

    ${BISON_Parser_OUTPUTS}
    ${LEXER_CPP}
)

target_include_directories(mini_compiler PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${GENERATED_DIR}
)

target_link_libraries(mini_compiler PRIVATE
    ${LLVM_LIBS}
)

target_compile_options(mini_compiler PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fpermissive
)


message(STATUS "LLVM_INCLUDE_DIRS = ${LLVM_INCLUDE_DIRS}")
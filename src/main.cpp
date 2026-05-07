// src/main.cpp
#include "driver.hpp"
#include <iostream>
#include <cstring>

void print_usage(const char *progname)
{
    std::cout << "Usage: " << progname << " [options] <source_file>\n"
              << "Options:\n"
              << "  --emit-ir    Save LLVM IR to .ll file\n"
              << "  --emit-asm   Generate ARM64 assembly (.s file)\n"
              << "  --emit-obj   Generate object file (.o file) [default]\n"
              << "  --trace      Print all compilation phases\n"
              << "  -h, --help   Show this help\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    std::string filename;
    bool emit_ir = false;
    bool emit_asm = false;
    bool emit_obj = true; // по умолчанию генерируем .o
    bool trace = false;

    // Разбор аргументов командной строки
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--emit-ir") == 0)
        {
            emit_ir = true;
        }
        else if (strcmp(argv[i], "--emit-asm") == 0)
        {
            emit_asm = true;
            emit_obj = false;
        }
        else if (strcmp(argv[i], "--emit-obj") == 0)
        {
            emit_obj = true;
            emit_asm = false;
        }
        else if (strcmp(argv[i], "--trace") == 0)
        {
            trace = true;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_usage(argv[0]);
            return 0;
        }
        else if (filename.empty())
        {
            filename = argv[i];
        }
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            print_usage(argv[0]);
            return 1;
        }
    }

    if (filename.empty())
    {
        std::cerr << "Error: no source file specified" << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    Driver driver;
    bool success = driver.compile(filename, emit_ir, emit_asm, emit_obj, trace);

    return success ? 0 : 1;
}
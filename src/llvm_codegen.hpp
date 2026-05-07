// src/llvm_codegen.hpp
#pragma once

#include <filesystem>
#include <memory>
#include <string>

class Driver;
struct Program;

enum class TargetFileKind
{
    Assembly, // .s файл
    Object    // .o файл
};

class LlvmCodeGenerator
{
public:
    explicit LlvmCodeGenerator(Driver &driver);
    ~LlvmCodeGenerator();

    // Генерация LLVM IR из AST
    bool generate(const Program &program);

    // Сохранение LLVM IR в текстовый файл (.ll)
    bool write_ir_file(const std::filesystem::path &path);

    // Строковое представление LLVM IR (для отладки)
    std::string ir_string() const;

    // Выпуск целевого файла (.s или .o) для ARM64
    bool emit_target_file(const std::filesystem::path &path, TargetFileKind kind);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
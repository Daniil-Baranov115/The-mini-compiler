// src/driver.hpp
#pragma once

#include "ast.hpp"
#include "parser.hpp" // ← ВАЖНО: включаем сгенерированный заголовок парсера
#include <memory>
#include <string>
#include <vector>

class Lexer;

class Driver
{
public:
    Driver();
    ~Driver();

    bool compile(const std::string &filename,
                 bool emit_ir = false,
                 bool emit_asm = false,
                 bool emit_obj = true,
                 bool trace = false);

    void set_program(std::unique_ptr<Program> program);

    // Теперь yy::Parser известен, потому что мы включили parser.hpp
    yy::Parser::symbol_type next_token();

    void error(const std::string &msg);

    void dump_ast() const;
    void dump_ir() const;

    std::string get_ir() const { return ir_string_; }

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    std::unique_ptr<Program> program_;
    std::unique_ptr<Lexer> lexer_;
    std::string ir_string_;
    std::vector<std::string> errors_;
    bool trace_mode_ = false;
};
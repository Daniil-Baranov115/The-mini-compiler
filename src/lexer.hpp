// src/lexer.hpp
#pragma once

#include "parser.hpp" // для yy::Parser
#include <string>

class Driver;

class Lexer
{
public:
    Lexer(Driver &driver, const std::string &input);
    yy::Parser::symbol_type next_token();

private:
    Driver &driver_;
    std::string buffer_;
    const char *cursor_;
    const char *marker_;
    const char *limit_;
};
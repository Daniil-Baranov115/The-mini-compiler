// src/lexer.re
#include "driver.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <string>
#include <cstdlib>

namespace {
std::string unescape_string(const std::string& s) {
    std::string result;
    for (size_t i = 1; i + 1 < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char c = s[++i];
            switch (c) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '\\': result += '\\'; break;
                case '"': result += '"'; break;
                default: result += c; break;
            }
        } else {
            result += s[i];
        }
    }
    return result;
}
}

// ТОЛЬКО РЕАЛИЗАЦИЯ МЕТОДОВ, БЕЗ ОПРЕДЕЛЕНИЯ КЛАССА!
Lexer::Lexer(Driver& driver, const std::string& input)
    : driver_(driver), buffer_(input + '\0')
{
    cursor_ = buffer_.c_str();
    marker_ = cursor_;
    limit_ = cursor_ + buffer_.size() - 1;
}

yy::Parser::symbol_type Lexer::next_token() {
    for (;;) {
        const char* anchor = cursor_;

        /*!re2c
        re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = "char";
        re2c:define:YYCURSOR = cursor_;
        re2c:define:YYMARKER = marker_;
        re2c:define:YYLIMIT = limit_;

        [ \t\r\n\v\f]+  { continue; }
        "//" [^\r\n]*   { continue; }
        
        "func"      { return yy::Parser::make_FUNC(); }
        "return"    { return yy::Parser::make_RETURN(); }
        "var"       { return yy::Parser::make_VAR(); }
        "if"        { return yy::Parser::make_IF(); }
        "else"      { return yy::Parser::make_ELSE(); }
        "for"       { return yy::Parser::make_FOR(); }
        "switch"    { return yy::Parser::make_SWITCH(); }
        "case"      { return yy::Parser::make_CASE(); }
        "default"   { return yy::Parser::make_DEFAULT(); }
        "break"     { return yy::Parser::make_BREAK(); }
        "continue"  { return yy::Parser::make_CONTINUE(); }
        "int"       { return yy::Parser::make_INT_TYPE(); }
        "float"     { return yy::Parser::make_FLOAT_TYPE(); }
        "bool"      { return yy::Parser::make_BOOL_TYPE(); }
        "true"      { return yy::Parser::make_BOOL_LITERAL(true); }
        "false"     { return yy::Parser::make_BOOL_LITERAL(false); }

        ":="        { return yy::Parser::make_DEFINE_ASSIGN(); }
        "="         { return yy::Parser::make_ASSIGN(); }
        "+="        { return yy::Parser::make_PLUS_ASSIGN(); }
        "-="        { return yy::Parser::make_MINUS_ASSIGN(); }
        "++"        { return yy::Parser::make_INCREMENT(); }
        "--"        { return yy::Parser::make_DECREMENT(); }
        "=="        { return yy::Parser::make_EQUAL(); }
        "!="        { return yy::Parser::make_NOT_EQUAL(); }
        "<="        { return yy::Parser::make_LESS_EQUAL(); }
        ">="        { return yy::Parser::make_GREATER_EQUAL(); }
        "&&"        { return yy::Parser::make_AND(); }
        "||"        { return yy::Parser::make_OR(); }
        "+"         { return yy::Parser::make_PLUS(); }
        "-"         { return yy::Parser::make_MINUS(); }
        "*"         { return yy::Parser::make_STAR(); }
        "/"         { return yy::Parser::make_SLASH(); }
        "%"         { return yy::Parser::make_PERCENT(); }
        "!"         { return yy::Parser::make_NOT(); }
        "&"         { return yy::Parser::make_AMP(); }
        "<"         { return yy::Parser::make_LESS(); }
        ">"         { return yy::Parser::make_GREATER(); }

        "("         { return yy::Parser::make_LPAREN(); }
        ")"         { return yy::Parser::make_RPAREN(); }
        "{"         { return yy::Parser::make_LBRACE(); }
        "}"         { return yy::Parser::make_RBRACE(); }
        "["         { return yy::Parser::make_LBRACKET(); }
        "]"         { return yy::Parser::make_RBRACKET(); }
        ","         { return yy::Parser::make_COMMA(); }
        ";"         { return yy::Parser::make_SEMICOLON(); }
        ":"         { return yy::Parser::make_COLON(); }

        [0-9]+ {
            std::string value(anchor, cursor_);
            return yy::Parser::make_INT_LITERAL(std::stoll(value));
        }
        [0-9]+"."[0-9]+ {
            std::string value(anchor, cursor_);
            return yy::Parser::make_FLOAT_LITERAL(std::stod(value));
        }
        "\"" [^"\n]* "\"" {
            std::string value(anchor, cursor_);
            return yy::Parser::make_STRING_LITERAL(unescape_string(value));
        }

        [a-zA-Z_][a-zA-Z0-9_]* {
            std::string name(anchor, cursor_);
            return yy::Parser::make_IDENTIFIER(name);
        }

        "\000" { return yy::Parser::make_END(); }

        * {
            driver_.error(std::string("unknown character: '") + *anchor + "'");
            return yy::Parser::make_INVALID();
        }
        */
    }
}
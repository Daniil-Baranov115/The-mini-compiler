// src/sema.hpp
#pragma once

#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

class Driver;

// ================================================================
// СЕМАНТИЧЕСКИЙ АНАЛИЗАТОР
// ================================================================
// Задачи:
// 1. Проверка объявления переменных перед использованием
// 2. Проверка типов операндов
// 3. Проверка уникальности case-значений в switch
// 4. Проверка break/continue в правильном контексте
// 5. Вывод типов для auto-переменных (x := 10)
// ================================================================

class SemanticAnalyzer
{
public:
    explicit SemanticAnalyzer(Driver &driver);

    // Главный метод: анализирует всю программу
    // Возвращает true, если ошибок нет
    bool analyze(const Program &program);

private:
    // ============================================================
    // СТРУКТУРЫ ДЛЯ ТАБЛИЦЫ СИМВОЛОВ
    // ============================================================
    struct Symbol
    {
        TypeSpec type;     // тип переменной
        bool is_parameter; // является ли параметром функции
        bool is_mutable;   // можно ли изменять (все переменные mutable)
    };

    // ============================================================
    // КОНТЕКСТ ДЛЯ BREAK/CONTINUE/SWITCH
    // ============================================================
    struct Context
    {
        bool in_loop = false;     // внутри цикла (for/while)
        bool in_switch = false;   // внутри switch
        bool has_default = false; // в текущем switch уже есть default
    };

    Driver &driver_;
    int error_count_ = 0;

    // Стек областей видимости (вектор словарей)
    std::vector<std::unordered_map<std::string, Symbol>> scopes_;

    // Стек контекстов (для проверки break/continue)
    std::vector<Context> context_stack_;

    // Коллекция функций (имя -> количество параметров)
    std::unordered_map<std::string, size_t> functions_;

    // ============================================================
    // ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ
    // ============================================================
    void report_error(const std::string &msg);
    bool has_errors() const { return error_count_ > 0; }

    void push_scope();
    void pop_scope();
    bool declare_symbol(const std::string &name, const Symbol &sym);
    const Symbol *lookup_symbol(const std::string &name) const;

    void push_context(bool in_loop, bool in_switch);
    void pop_context();
    Context &current_context();

    // ============================================================
    // ПРОВЕРКИ ТИПОВ И ПРИВЕДЕНИЙ
    // ============================================================
    bool types_compatible(TypeSpec a, TypeSpec b) const;
    bool can_assign(TypeSpec lhs_type, TypeSpec rhs_type) const;
    TypeSpec common_arithmetic_type(TypeSpec a, TypeSpec b) const;

    // ============================================================
    // ОСНОВНЫЕ МЕТОДЫ АНАЛИЗА
    // ============================================================
    void collect_functions(const Program &program);
    void check_entry_point() const;
    void analyze_function(FunctionDecl &func);
    void analyze_block(BlockStmt &block, bool create_scope);
    void analyze_statement(Stmt &stmt);
    TypeSpec analyze_expression(Expr &expr);

    // ============================================================
    // СПЕЦИАЛЬНЫЕ МЕТОДЫ ДЛЯ SWITCH/CASE
    // ============================================================
    void analyze_switch(SwitchStmt &stmt);
    bool is_constant_integer(Expr *expr, std::int64_t *value = nullptr) const;
    void check_case_duplicates(const StmtList &cases);
    void check_case_types(TypeSpec switch_type, const StmtList &cases);
};
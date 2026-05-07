// src/sema.cpp
#include "sema.hpp"
#include "driver.hpp"
#include <iostream>
#include <sstream>

SemanticAnalyzer::SemanticAnalyzer(Driver &driver) : driver_(driver) {}

void SemanticAnalyzer::report_error(const std::string &msg)
{
    driver_.error("semantic error: " + msg);
    error_count_++;
}

bool SemanticAnalyzer::analyze(const Program &program)
{
    error_count_ = 0;
    scopes_.clear();
    context_stack_.clear();
    functions_.clear();

    // 1. Собираем все функции (чтобы проверять вызовы)
    collect_functions(program);

    // 2. Проверяем наличие точки входа
    check_entry_point();

    // 3. Анализируем каждую функцию
    for (const auto &func : program.functions)
    {
        analyze_function(*func);
    }

    return error_count_ == 0;
}

void SemanticAnalyzer::collect_functions(const Program &program)
{
    for (const auto &func : program.functions)
    {
        auto [it, inserted] = functions_.emplace(func->name, func->parameters.size());
        if (!inserted)
        {
            report_error("duplicate function: " + func->name);
        }
    }
}

void SemanticAnalyzer::check_entry_point() const
{
    // Точка входа: функция compiled_fn с одним параметром
    auto it = functions_.find("compiled_fn");
    if (it == functions_.end())
    {
        const_cast<SemanticAnalyzer *>(this)->report_error(
            "missing entry function: compiled_fn(int) must be defined");
        return;
    }

    if (it->second != 1)
    {
        const_cast<SemanticAnalyzer *>(this)->report_error(
            "compiled_fn must have exactly one parameter");
    }
}

// ================================================================
// УПРАВЛЕНИЕ ОБЛАСТЯМИ ВИДИМОСТИ
// ================================================================
void SemanticAnalyzer::push_scope()
{
    scopes_.emplace_back();
}

void SemanticAnalyzer::pop_scope()
{
    if (!scopes_.empty())
    {
        scopes_.pop_back();
    }
}

bool SemanticAnalyzer::declare_symbol(const std::string &name, const Symbol &sym)
{
    if (scopes_.empty())
    {
        push_scope();
    }

    auto &scope = scopes_.back();

    // Проверка на повторное объявление в той же области
    if (scope.find(name) != scope.end())
    {
        report_error("duplicate declaration: " + name);
        return false;
    }

    // Проверка конфликта с именем функции
    if (functions_.find(name) != functions_.end())
    {
        report_error("variable name conflicts with function: " + name);
        return false;
    }

    scope[name] = sym;
    return true;
}

const SemanticAnalyzer::Symbol *SemanticAnalyzer::lookup_symbol(const std::string &name) const
{
    // Поиск от самой внутренней области к внешней
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            return &found->second;
        }
    }
    return nullptr;
}

// ================================================================
// УПРАВЛЕНИЕ КОНТЕКСТОМ (для break/continue)
// ================================================================
void SemanticAnalyzer::push_context(bool in_loop, bool in_switch)
{
    context_stack_.push_back({in_loop, in_switch, false});
}

void SemanticAnalyzer::pop_context()
{
    if (!context_stack_.empty())
    {
        context_stack_.pop_back();
    }
}

SemanticAnalyzer::Context &SemanticAnalyzer::current_context()
{
    static Context empty_context;
    if (context_stack_.empty())
    {
        return empty_context;
    }
    return context_stack_.back();
}

// ================================================================
// ПРОВЕРКИ ТИПОВ
// ================================================================
bool SemanticAnalyzer::types_compatible(TypeSpec a, TypeSpec b) const
{
    if (a == TypeSpec::Auto || b == TypeSpec::Auto)
        return true;
    if (a == b)
        return true;
    // int может быть неявно приведён к float/double
    if (a == TypeSpec::Int && b == TypeSpec::Float)
        return true;
    if (a == TypeSpec::Float && b == TypeSpec::Int)
        return true;
    return false;
}

bool SemanticAnalyzer::can_assign(TypeSpec lhs_type, TypeSpec rhs_type) const
{
    if (lhs_type == TypeSpec::Auto)
        return true;
    if (lhs_type == rhs_type)
        return true;
    // int -> float разрешено, float -> int запрещено (нужен as)
    if (lhs_type == TypeSpec::Float && rhs_type == TypeSpec::Int)
        return true;
    return false;
}

TypeSpec SemanticAnalyzer::common_arithmetic_type(TypeSpec a, TypeSpec b) const
{
    if (a == TypeSpec::Float || b == TypeSpec::Float)
        return TypeSpec::Float;
    if (a == TypeSpec::Int || b == TypeSpec::Int)
        return TypeSpec::Int;
    return TypeSpec::Int;
}

// ================================================================
// АНАЛИЗ ФУНКЦИИ
// ================================================================
void SemanticAnalyzer::analyze_function(FunctionDecl &func)
{
    scopes_.clear();
    push_scope();

    // Объявляем параметры как символы
    for (const auto &param : func.parameters)
    {
        // Параметры всегда типа Int в учебном компиляторе
        Symbol sym{param.type, true, true};
        declare_symbol(param.name, sym);
    }

    // Анализируем тело функции
    if (func.body)
    {
        analyze_block(*func.body, false);
    }

    pop_scope();
}

void SemanticAnalyzer::analyze_block(BlockStmt &block, bool create_scope)
{
    if (create_scope)
    {
        push_scope();
    }

    for (auto &stmt : block.statements)
    {
        analyze_statement(*stmt);
        if (has_errors())
            break;
    }

    if (create_scope)
    {
        pop_scope();
    }
}

// ================================================================
// АНАЛИЗ ОПЕРАТОРОВ
// ================================================================
void SemanticAnalyzer::analyze_statement(Stmt &stmt)
{
    // Пустой оператор
    if (dynamic_cast<EmptyStmt *>(&stmt))
    {
        return;
    }

    // Выражение-оператор
    if (auto *expr_stmt = dynamic_cast<ExprStmt *>(&stmt))
    {
        if (expr_stmt->expr)
        {
            analyze_expression(*expr_stmt->expr);
        }
        return;
    }

    // Return
    if (auto *return_stmt = dynamic_cast<ReturnStmt *>(&stmt))
    {
        if (return_stmt->expr)
        {
            analyze_expression(*return_stmt->expr);
            // В compiled_fn возвращаемый тип должен быть Int
            // (проверяем по контексту, но для простоты считаем Int)
        }
        else
        {
            report_error("return statement must have an expression");
        }
        return;
    }

    // Break
    if (dynamic_cast<BreakStmt *>(&stmt))
    {
        if (!current_context().in_loop && !current_context().in_switch)
        {
            report_error("break statement outside loop or switch");
        }
        return;
    }

    // Continue
    if (dynamic_cast<ContinueStmt *>(&stmt))
    {
        if (!current_context().in_loop)
        {
            report_error("continue statement outside loop");
        }
        return;
    }

    // Объявление переменной
    if (auto *var_decl = dynamic_cast<VarDeclStmt *>(&stmt))
    {
        TypeSpec init_type = TypeSpec::Int;
        if (var_decl->initializer)
        {
            init_type = analyze_expression(*var_decl->initializer);
        }

        // Вывод типа для Auto
        TypeSpec final_type = var_decl->type;
        if (final_type == TypeSpec::Auto)
        {
            final_type = init_type;
        }
        else if (var_decl->initializer && !can_assign(final_type, init_type))
        {
            report_error("cannot assign " + std::to_string(static_cast<int>(init_type)) +
                         " to variable of type " + std::to_string(static_cast<int>(final_type)));
        }

        Symbol sym{final_type, false, true};
        declare_symbol(var_decl->name, sym);
        return;
    }

    // If
    if (auto *if_stmt = dynamic_cast<IfStmt *>(&stmt))
    {
        if (if_stmt->condition)
        {
            TypeSpec cond_type = analyze_expression(*if_stmt->condition);
            // Условие должно быть bool или int (0=false, не 0=true)
            if (cond_type != TypeSpec::Bool && cond_type != TypeSpec::Int)
            {
                report_error("if condition must be bool or int");
            }
        }

        if (if_stmt->then_stmt)
        {
            analyze_statement(*if_stmt->then_stmt);
        }
        if (if_stmt->else_stmt)
        {
            analyze_statement(*if_stmt->else_stmt);
        }
        return;
    }

    // While (поддерживаем и for как while)
    if (auto *while_stmt = dynamic_cast<WhileStmt *>(&stmt))
    {
        if (while_stmt->condition)
        {
            TypeSpec cond_type = analyze_expression(*while_stmt->condition);
            if (cond_type != TypeSpec::Bool && cond_type != TypeSpec::Int)
            {
                report_error("while condition must be bool or int");
            }
        }

        push_context(true, false);
        if (while_stmt->body)
        {
            analyze_statement(*while_stmt->body);
        }
        pop_context();
        return;
    }

    // Block
    if (auto *block_stmt = dynamic_cast<BlockStmt *>(&stmt))
    {
        analyze_block(*block_stmt, true);
        return;
    }

    // ============================================================
    // SWITCH/CASTE (ВАША ОСНОВНАЯ ФИЧА)
    // ============================================================
    if (auto *switch_stmt = dynamic_cast<SwitchStmt *>(&stmt))
    {
        analyze_switch(*switch_stmt);
        return;
    }

    report_error("unknown statement type");
}

// ================================================================
// АНАЛИЗ SWITCH/CASE
// ================================================================
bool SemanticAnalyzer::is_constant_integer(Expr *expr, std::int64_t *value) const
{
    auto *int_lit = dynamic_cast<IntegerLiteralExpr *>(expr);
    if (int_lit)
    {
        if (value)
            *value = int_lit->value;
        return true;
    }
    return false;
}

void SemanticAnalyzer::check_case_duplicates(const StmtList &cases)
{
    std::set<std::int64_t> seen_values;
    bool has_default = false;

    for (const auto &case_stmt : cases)
    {
        if (auto *case_clause = dynamic_cast<CaseClause *>(case_stmt.get()))
        {
            std::int64_t value;
            if (is_constant_integer(case_clause->value.get(), &value))
            {
                if (seen_values.count(value))
                {
                    report_error("duplicate case value: " + std::to_string(value));
                }
                seen_values.insert(value);
            }
            else
            {
                report_error("case value must be an integer constant");
            }
        }
        else if (dynamic_cast<DefaultClause *>(case_stmt.get()))
        {
            if (has_default)
            {
                report_error("multiple default clauses in switch");
            }
            has_default = true;
        }
    }
}

void SemanticAnalyzer::check_case_types(TypeSpec switch_type, const StmtList &cases)
{
    for (const auto &case_stmt : cases)
    {
        if (auto *case_clause = dynamic_cast<CaseClause *>(case_stmt.get()))
        {
            TypeSpec case_type = analyze_expression(*case_clause->value);
            if (!types_compatible(switch_type, case_type))
            {
                report_error("case type does not match switch expression type");
            }
        }
    }
}

void SemanticAnalyzer::analyze_switch(SwitchStmt &stmt)
{
    // Проверяем тип switch-выражения
    TypeSpec switch_type = analyze_expression(*stmt.condition);
    if (switch_type != TypeSpec::Int)
    {
        report_error("switch expression must be integer");
    }

    // Проверяем уникальность case-значений
    check_case_duplicates(stmt.cases);
    check_case_types(switch_type, stmt.cases);

    // Входим в switch-контекст
    push_context(false, true);

    // Анализируем каждую ветку
    for (auto &case_stmt : stmt.cases)
    {
        if (auto *case_clause = dynamic_cast<CaseClause *>(case_stmt.get()))
        {
            for (auto &s : case_clause->statements)
            {
                analyze_statement(*s);
            }
        }
        else if (auto *default_clause = dynamic_cast<DefaultClause *>(case_stmt.get()))
        {
            for (auto &s : default_clause->statements)
            {
                analyze_statement(*s);
            }
        }
    }

    pop_context();
}

// ================================================================
// АНАЛИЗ ВЫРАЖЕНИЙ
// ================================================================
TypeSpec SemanticAnalyzer::analyze_expression(Expr &expr)
{
    // Литералы
    if (dynamic_cast<IntegerLiteralExpr *>(&expr))
    {
        expr.type = TypeSpec::Int;
        expr.is_lvalue = false;
        return TypeSpec::Int;
    }

    if (dynamic_cast<FloatLiteralExpr *>(&expr))
    {
        expr.type = TypeSpec::Float;
        expr.is_lvalue = false;
        return TypeSpec::Float;
    }

    if (dynamic_cast<BoolLiteralExpr *>(&expr))
    {
        expr.type = TypeSpec::Bool;
        expr.is_lvalue = false;
        return TypeSpec::Bool;
    }

    if (dynamic_cast<StringLiteralExpr *>(&expr))
    {
        expr.type = TypeSpec::Void; // строки не поддерживаем для вычислений
        expr.is_lvalue = false;
        return TypeSpec::Void;
    }

    // Идентификатор (переменная)
    if (auto *ident = dynamic_cast<IdentifierExpr *>(&expr))
    {
        const Symbol *sym = lookup_symbol(ident->name);
        if (!sym)
        {
            report_error("undeclared identifier: " + ident->name);
            expr.type = TypeSpec::Int;
            return TypeSpec::Int;
        }
        expr.type = sym->type;
        expr.is_lvalue = true; // переменные можно изменять
        return sym->type;
    }

    // Унарные операции
    if (auto *unary = dynamic_cast<UnaryExpr *>(&expr))
    {
        TypeSpec operand_type = analyze_expression(*unary->operand);

        switch (unary->op)
        {
        case UnaryOp::LogicalNot:
            if (operand_type != TypeSpec::Bool && operand_type != TypeSpec::Int)
            {
                report_error("logical not (!) requires bool or int operand");
            }
            expr.type = TypeSpec::Bool;
            expr.is_lvalue = false;
            return TypeSpec::Bool;

        case UnaryOp::Negate:
            if (operand_type != TypeSpec::Int && operand_type != TypeSpec::Float)
            {
                report_error("negate (-) requires numeric operand");
            }
            expr.type = operand_type;
            expr.is_lvalue = false;
            return operand_type;

        case UnaryOp::AddressOf:
            if (!unary->operand->is_lvalue)
            {
                report_error("address of (&) requires lvalue");
            }
            expr.type = TypeSpec::Int; // указатели как int
            expr.is_lvalue = false;
            return TypeSpec::Int;

        case UnaryOp::Dereference:
            expr.type = TypeSpec::Int;
            expr.is_lvalue = true;
            return TypeSpec::Int;
        }

        return TypeSpec::Int;
    }

    // Бинарные операции
    if (auto *binary = dynamic_cast<BinaryExpr *>(&expr))
    {
        TypeSpec lhs_type = analyze_expression(*binary->lhs);
        TypeSpec rhs_type = analyze_expression(*binary->rhs);

        // Логические операции
        if (binary->op == BinaryOp::LogicalAnd || binary->op == BinaryOp::LogicalOr)
        {
            if (lhs_type != TypeSpec::Bool && lhs_type != TypeSpec::Int)
            {
                report_error("logical operator requires bool or int operands");
            }
            expr.type = TypeSpec::Bool;
            expr.is_lvalue = false;
            return TypeSpec::Bool;
        }

        // Операции сравнения
        if (binary->op >= BinaryOp::Less && binary->op <= BinaryOp::GreaterEqual)
        {
            if (!types_compatible(lhs_type, rhs_type))
            {
                report_error("comparison of incompatible types");
            }
            expr.type = TypeSpec::Bool;
            expr.is_lvalue = false;
            return TypeSpec::Bool;
        }

        // Арифметические операции
        TypeSpec common_type = common_arithmetic_type(lhs_type, rhs_type);
        expr.type = common_type;
        expr.is_lvalue = false;
        return common_type;
    }

    // Присваивание
    if (auto *assign = dynamic_cast<AssignExpr *>(&expr))
    {
        TypeSpec lhs_type = analyze_expression(*assign->lhs);
        TypeSpec rhs_type = analyze_expression(*assign->rhs);

        if (!assign->lhs->is_lvalue)
        {
            report_error("left-hand side of assignment must be an lvalue");
        }

        if (!can_assign(lhs_type, rhs_type))
        {
            report_error("cannot assign incompatible types");
        }

        expr.type = lhs_type;
        expr.is_lvalue = false;
        return lhs_type;
    }

    // Вызов функции
    if (auto *call = dynamic_cast<CallExpr *>(&expr))
    {
        auto *callee_id = dynamic_cast<IdentifierExpr *>(call->callee.get());
        if (!callee_id)
        {
            report_error("indirect calls are not supported");
            return TypeSpec::Int;
        }

        auto it = functions_.find(callee_id->name);
        if (it == functions_.end())
        {
            report_error("call to unknown function: " + callee_id->name);
            return TypeSpec::Int;
        }

        if (call->arguments.size() != it->second)
        {
            report_error("function " + callee_id->name + " expects " +
                         std::to_string(it->second) + " arguments, got " +
                         std::to_string(call->arguments.size()));
        }

        for (auto &arg : call->arguments)
        {
            analyze_expression(*arg);
        }

        expr.type = TypeSpec::Int;
        expr.is_lvalue = false;
        return TypeSpec::Int;
    }

    report_error("unknown expression type");
    return TypeSpec::Int;
}
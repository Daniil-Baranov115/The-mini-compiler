// src/ast.hpp
#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ================================================================
// ТИПЫ ДАННЫХ
// ================================================================
enum class TypeSpec {
    Auto,   // выводимый тип (для x := 10)
    Int,
    Float,
    Bool,
    Void
};

enum class BinaryOp {
    Add, Sub, Mul, Div, Mod,
    Less, LessEqual, Greater, GreaterEqual,
    Equal, NotEqual,
    LogicalAnd, LogicalOr,
    ShiftLeft, ShiftRight,
    BitwiseAnd, BitwiseOr, BitwiseXor
};

enum class UnaryOp {
    LogicalNot,
    Negate,
    AddressOf,
    Dereference
};

enum class IncDecOp {
    Increment,
    Decrement
};

// ================================================================
// БАЗОВЫЙ КЛАСС ВЫРАЖЕНИЙ
// ================================================================
struct Expr {
    virtual ~Expr() = default;
    virtual void dump(std::ostream& out, int indent) const = 0;
    
    TypeSpec type = TypeSpec::Auto;  // вычисленный тип (заполняется семантикой)
    bool is_lvalue = false;          // можно ли присвоить (для a = 10)
};

using ExprPtr = std::unique_ptr<Expr>;
using ExprList = std::vector<ExprPtr>;

// ================================================================
// ВЫРАЖЕНИЯ-ЛИТЕРАЛЫ
// ================================================================
struct IntegerLiteralExpr : Expr {
    std::int64_t value;
    explicit IntegerLiteralExpr(std::int64_t v) : value(v) {}
    void dump(std::ostream& out, int indent) const override;
};

struct FloatLiteralExpr : Expr {
    double value;
    explicit FloatLiteralExpr(double v) : value(v) {}
    void dump(std::ostream& out, int indent) const override;
};

struct BoolLiteralExpr : Expr {
    bool value;
    explicit BoolLiteralExpr(bool v) : value(v) {}
    void dump(std::ostream& out, int indent) const override;
};

struct StringLiteralExpr : Expr {
    std::string value;
    explicit StringLiteralExpr(std::string v) : value(std::move(v)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct IdentifierExpr : Expr {
    std::string name;
    explicit IdentifierExpr(std::string n) : name(std::move(n)) {}
    void dump(std::ostream& out, int indent) const override;
};

// ================================================================
// ОПЕРАТОРНЫЕ ВЫРАЖЕНИЯ
// ================================================================
struct UnaryExpr : Expr {
    UnaryOp op;
    ExprPtr operand;
    UnaryExpr(UnaryOp o, ExprPtr opnd) : op(o), operand(std::move(opnd)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct BinaryExpr : Expr {
    BinaryOp op;
    ExprPtr lhs, rhs;
    BinaryExpr(BinaryOp o, ExprPtr l, ExprPtr r) 
        : op(o), lhs(std::move(l)), rhs(std::move(r)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct AssignExpr : Expr {
    ExprPtr lhs, rhs;
    AssignExpr(ExprPtr l, ExprPtr r) : lhs(std::move(l)), rhs(std::move(r)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct PreIncDecExpr : Expr {
    IncDecOp op;
    ExprPtr operand;
    PreIncDecExpr(IncDecOp o, ExprPtr opnd) : op(o), operand(std::move(opnd)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct PostIncDecExpr : Expr {
    IncDecOp op;
    ExprPtr operand;
    PostIncDecExpr(IncDecOp o, ExprPtr opnd) : op(o), operand(std::move(opnd)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct CallExpr : Expr {
    ExprPtr callee;
    ExprList arguments;
    CallExpr(ExprPtr c, ExprList args) : callee(std::move(c)), arguments(std::move(args)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct IndexExpr : Expr {
    ExprPtr base, index;
    IndexExpr(ExprPtr b, ExprPtr i) : base(std::move(b)), index(std::move(i)) {}
    void dump(std::ostream& out, int indent) const override;
};

// ================================================================
// БАЗОВЫЙ КЛАСС ОПЕРАТОРОВ
// ================================================================
struct Stmt {
    virtual ~Stmt() = default;
    virtual void dump(std::ostream& out, int indent) const = 0;
};

using StmtPtr = std::unique_ptr<Stmt>;
using StmtList = std::vector<StmtPtr>;

// ================================================================
// ОПЕРАТОРЫ
// ================================================================
struct EmptyStmt : Stmt {
    void dump(std::ostream& out, int indent) const override;
};

struct ExprStmt : Stmt {
    ExprPtr expr;
    explicit ExprStmt(ExprPtr e) : expr(std::move(e)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct ReturnStmt : Stmt {
    ExprPtr expr;
    explicit ReturnStmt(ExprPtr e) : expr(std::move(e)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct BreakStmt : Stmt {
    void dump(std::ostream& out, int indent) const override;
};

struct ContinueStmt : Stmt {
    void dump(std::ostream& out, int indent) const override;
};

struct VarDeclStmt : Stmt {
    std::string name;
    TypeSpec type;
    ExprPtr initializer;
    VarDeclStmt(std::string n, TypeSpec t, ExprPtr init)
        : name(std::move(n)), type(t), initializer(std::move(init)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct IfStmt : Stmt {
    ExprPtr condition;
    StmtPtr then_stmt, else_stmt;
    IfStmt(ExprPtr cond, StmtPtr then_s) 
        : condition(std::move(cond)), then_stmt(std::move(then_s)) {}
    IfStmt(ExprPtr cond, StmtPtr then_s, StmtPtr else_s)
        : condition(std::move(cond)), then_stmt(std::move(then_s)), else_stmt(std::move(else_s)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct WhileStmt : Stmt {
    ExprPtr condition;
    StmtPtr body;
    WhileStmt(ExprPtr cond, StmtPtr b) : condition(std::move(cond)), body(std::move(b)) {}
    void dump(std::ostream& out, int indent) const override;
};

struct BlockStmt : Stmt {
    StmtList statements;
    void dump(std::ostream& out, int indent) const override;
};

// ================================================================
// SWITCH/CASE (ВАША ОСНОВНАЯ ФИЧА)
// ================================================================
struct CaseClause : Stmt {
    ExprPtr value;      // значение case (должно быть константой)
    StmtList statements; // операторы в этой ветке
    void dump(std::ostream& out, int indent) const override;
};

struct DefaultClause : Stmt {
    StmtList statements;
    void dump(std::ostream& out, int indent) const override;
};

struct SwitchStmt : Stmt {
    ExprPtr condition;   // выражение, которое проверяется
    StmtList cases;      // содержит CaseClause* и DefaultClause*
    SwitchStmt(ExprPtr cond, StmtList c) : condition(std::move(cond)), cases(std::move(c)) {}
    void dump(std::ostream& out, int indent) const override;
};

// ================================================================
// ФУНКЦИИ И ПРОГРАММА
// ================================================================
struct Parameter {
    std::string name;
    TypeSpec type;
};

using ParameterList = std::vector<Parameter>;

struct FunctionDecl {
    std::string name;
    ParameterList parameters;
    std::unique_ptr<BlockStmt> body;
    void dump(std::ostream& out, int indent) const;
};

using FunctionPtr = std::unique_ptr<FunctionDecl>;
using FunctionList = std::vector<FunctionPtr>;

struct Program {
    FunctionList functions;
    void dump(std::ostream& out) const;
};
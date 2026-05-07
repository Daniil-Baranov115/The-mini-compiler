// src/ast.cpp
#include "ast.hpp"

#define INDENT(out, n)            \
    for (int i = 0; i < (n); ++i) \
    out << "  "

void IntegerLiteralExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "IntegerLiteralExpr " << value << "\n";
}

void FloatLiteralExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "FloatLiteralExpr " << value << "\n";
}

void BoolLiteralExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "BoolLiteralExpr " << (value ? "true" : "false") << "\n";
}

void StringLiteralExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "StringLiteralExpr \"" << value << "\"\n";
}

void IdentifierExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "IdentifierExpr " << name << "\n";
}

void UnaryExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    const char *op_str = "";
    switch (op)
    {
    case UnaryOp::LogicalNot:
        op_str = "!";
        break;
    case UnaryOp::Negate:
        op_str = "-";
        break;
    case UnaryOp::AddressOf:
        op_str = "&";
        break;
    case UnaryOp::Dereference:
        op_str = "*";
        break;
    }
    out << "UnaryExpr " << op_str << "\n";
    if (operand)
        operand->dump(out, indent + 1);
}

void BinaryExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    const char *op_str = "";
    switch (op)
    {
    case BinaryOp::Add:
        op_str = "+";
        break;
    case BinaryOp::Sub:
        op_str = "-";
        break;
    case BinaryOp::Mul:
        op_str = "*";
        break;
    case BinaryOp::Div:
        op_str = "/";
        break;
    case BinaryOp::Mod:
        op_str = "%";
        break;
    case BinaryOp::Equal:
        op_str = "==";
        break;
    case BinaryOp::NotEqual:
        op_str = "!=";
        break;
    case BinaryOp::Less:
        op_str = "<";
        break;
    case BinaryOp::LessEqual:
        op_str = "<=";
        break;
    case BinaryOp::Greater:
        op_str = ">";
        break;
    case BinaryOp::GreaterEqual:
        op_str = ">=";
        break;
    case BinaryOp::LogicalAnd:
        op_str = "&&";
        break;
    case BinaryOp::LogicalOr:
        op_str = "||";
        break;
    default:
        op_str = "?";
        break;
    }
    out << "BinaryExpr " << op_str << "\n";
    if (lhs)
        lhs->dump(out, indent + 1);
    if (rhs)
        rhs->dump(out, indent + 1);
}

void AssignExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "AssignExpr =\n";
    if (lhs)
        lhs->dump(out, indent + 1);
    if (rhs)
        rhs->dump(out, indent + 1);
}

void PreIncDecExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "PreIncDecExpr " << (op == IncDecOp::Increment ? "++" : "--") << "\n";
    if (operand)
        operand->dump(out, indent + 1);
}

void PostIncDecExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "PostIncDecExpr " << (op == IncDecOp::Increment ? "++" : "--") << "\n";
    if (operand)
        operand->dump(out, indent + 1);
}

void CallExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "CallExpr\n";
    if (callee)
        callee->dump(out, indent + 1);
    for (const auto &arg : arguments)
    {
        arg->dump(out, indent + 1);
    }
}

void IndexExpr::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "IndexExpr []\n";
    if (base)
        base->dump(out, indent + 1);
    if (index)
        index->dump(out, indent + 1);
}

// Операторы
void EmptyStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "EmptyStmt\n";
}

void ExprStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "ExprStmt\n";
    if (expr)
        expr->dump(out, indent + 1);
}

void ReturnStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "ReturnStmt\n";
    if (expr)
        expr->dump(out, indent + 1);
}

void BreakStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "BreakStmt\n";
}

void ContinueStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "ContinueStmt\n";
}

void VarDeclStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    const char *type_str = "";
    switch (type)
    {
    case TypeSpec::Auto:
        type_str = "auto";
        break;
    case TypeSpec::Int:
        type_str = "int";
        break;
    case TypeSpec::Float:
        type_str = "float";
        break;
    case TypeSpec::Bool:
        type_str = "bool";
        break;
    default:
        type_str = "?";
        break;
    }
    out << "VarDeclStmt " << name << " : " << type_str << "\n";
    if (initializer)
        initializer->dump(out, indent + 1);
}

void IfStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "IfStmt\n";
    INDENT(out, indent + 1);
    out << "Condition:\n";
    if (condition)
        condition->dump(out, indent + 2);
    INDENT(out, indent + 1);
    out << "Then:\n";
    if (then_stmt)
        then_stmt->dump(out, indent + 2);
    if (else_stmt)
    {
        INDENT(out, indent + 1);
        out << "Else:\n";
        else_stmt->dump(out, indent + 2);
    }
}

void WhileStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "WhileStmt\n";
    INDENT(out, indent + 1);
    out << "Condition:\n";
    if (condition)
        condition->dump(out, indent + 2);
    INDENT(out, indent + 1);
    out << "Body:\n";
    if (body)
        body->dump(out, indent + 2);
}

void BlockStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "BlockStmt\n";
    for (const auto &stmt : statements)
    {
        stmt->dump(out, indent + 1);
    }
}

void CaseClause::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "CaseClause\n";
    if (value)
        value->dump(out, indent + 1);
    for (const auto &stmt : statements)
    {
        stmt->dump(out, indent + 1);
    }
}

void DefaultClause::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "DefaultClause\n";
    for (const auto &stmt : statements)
    {
        stmt->dump(out, indent + 1);
    }
}

void SwitchStmt::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "SwitchStmt\n";
    INDENT(out, indent + 1);
    out << "Condition:\n";
    if (condition)
        condition->dump(out, indent + 2);
    INDENT(out, indent + 1);
    out << "Cases:\n";
    for (const auto &c : cases)
    {
        c->dump(out, indent + 2);
    }
}

void FunctionDecl::dump(std::ostream &out, int indent) const
{
    INDENT(out, indent);
    out << "FunctionDecl " << name << "\n";
    INDENT(out, indent + 1);
    out << "Parameters:\n";
    for (const auto &p : parameters)
    {
        INDENT(out, indent + 2);
        const char *type_str = "";
        switch (p.type)
        {
        case TypeSpec::Int:
            type_str = "int";
            break;
        case TypeSpec::Float:
            type_str = "float";
            break;
        case TypeSpec::Bool:
            type_str = "bool";
            break;
        default:
            type_str = "?";
            break;
        }
        out << p.name << " : " << type_str << "\n";
    }
    if (body)
        body->dump(out, indent + 1);
}

void Program::dump(std::ostream &out) const
{
    out << "Program\n";
    for (const auto &fn : functions)
    {
        fn->dump(out, 1);
    }
}
/* src/parser.y - Упрощённая версия для мини-компилятора */

%skeleton "lalr1.cc"
%require "3.8"
%language "c++"

%define api.namespace {yy}
%define api.parser.class {Parser}
%define api.value.type variant
%define api.token.constructor
%define parse.error detailed
%define api.value.automove

%code requires {
    #include <memory>
    #include <string>
    #include <vector>
    #include "ast.hpp"
    class Driver;
}

%param { Driver& driver }

%code {
    #include "driver.hpp"
    static yy::Parser::symbol_type yylex(Driver& driver) {
        return driver.next_token();
    }
}

/* Токены */
%token END 0 "end of file"
%token INVALID

%token FUNC "func"
%token RETURN "return"
%token VAR "var"
%token IF "if"
%token ELSE "else"
%token FOR "for"
%token SWITCH "switch"
%token CASE "case"
%token DEFAULT "default"
%token BREAK "break"
%token CONTINUE "continue"
%token INT_TYPE "int"
%token FLOAT_TYPE "float"
%token BOOL_TYPE "bool"

%token <std::int64_t> INT_LITERAL
%token <double> FLOAT_LITERAL
%token <bool> BOOL_LITERAL
%token <std::string> STRING_LITERAL
%token <std::string> IDENTIFIER

%token DEFINE_ASSIGN ":="
%token ASSIGN "="
%token PLUS_ASSIGN "+="
%token MINUS_ASSIGN "-="
%token INCREMENT "++"
%token DECREMENT "--"
%token PLUS "+"
%token MINUS "-"
%token STAR "*"
%token SLASH "/"
%token PERCENT "%"
%token EQUAL "=="
%token NOT_EQUAL "!="
%token LESS "<"
%token LESS_EQUAL "<="
%token GREATER ">"
%token GREATER_EQUAL ">="
%token AND "&&"
%token OR "||"
%token NOT "!"
%token AMP "&"

%token LPAREN "("
%token RPAREN ")"
%token LBRACE "{"
%token RBRACE "}"
%token LBRACKET "["
%token RBRACKET "]"
%token COMMA ","
%token SEMICOLON ";"
%token COLON ":"

/* Приоритеты */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left LESS LESS_EQUAL GREATER GREATER_EQUAL
%left PLUS MINUS
%left STAR SLASH PERCENT
%right NOT

/* Типы */
%type <std::unique_ptr<Program>> program
%type <FunctionList> functions
%type <std::unique_ptr<FunctionDecl>> function
%type <ParameterList> parameters parameter_list
%type <std::unique_ptr<BlockStmt>> block
%type <StmtList> statements
%type <StmtPtr> statement declaration
%type <StmtList> switch_cases
%type <std::unique_ptr<CaseClause>> switch_case
%type <std::unique_ptr<DefaultClause>> switch_default
%type <ExprPtr> expression logical_or_expr logical_and_expr equality_expr relational_expr additive_expr multiplicative_expr unary_expr postfix_expr primary_expr
%type <ExprList> arguments argument_list
%type <TypeSpec> type

%%

program:
    functions
    {
        auto prog = std::make_unique<Program>();
        prog->functions = std::move($1);
        driver.set_program(std::move(prog));
    }
    ;

functions:
    %empty { $$ = FunctionList{}; }
    | functions function
    {
        $$ = std::move($1);
        $$.push_back(std::move($2));
    }
    ;

function:
    FUNC IDENTIFIER LPAREN parameter_list RPAREN block
    {
        auto fn = std::make_unique<FunctionDecl>();
        fn->name = std::move($2);
        fn->parameters = std::move($4);
        fn->body = std::move($6);
        $$ = std::move(fn);
    }
    ;

parameter_list:
    %empty { $$ = ParameterList{}; }
    | parameters { $$ = std::move($1); }
    ;

parameters:
    IDENTIFIER type
    {
        $$ = ParameterList{};
        $$.push_back({std::move($1), $2});
    }
    | parameters COMMA IDENTIFIER type
    {
        $$ = std::move($1);
        $$.push_back({std::move($3), $4});
    }
    ;

type:
    INT_TYPE   { $$ = TypeSpec::Int; }
    | FLOAT_TYPE { $$ = TypeSpec::Float; }
    | BOOL_TYPE  { $$ = TypeSpec::Bool; }
    ;

block:
    LBRACE statements RBRACE
    {
        auto b = std::make_unique<BlockStmt>();
        b->statements = std::move($2);
        $$ = std::move(b);
    }
    ;

statements:
    %empty { $$ = StmtList{}; }
    | statements statement
    {
        $$ = std::move($1);
        $$.push_back(std::move($2));
    }
    ;

statement:
    declaration
    | expression
    {
        $$ = std::make_unique<ExprStmt>(std::move($1));
    }
    | RETURN expression
    {
        $$ = std::make_unique<ReturnStmt>(std::move($2));
    }
    | IF expression block %prec LOWER_THAN_ELSE
    {
        $$ = std::make_unique<IfStmt>(std::move($2), std::move($3));
    }
    | IF expression block ELSE block
    {
        $$ = std::make_unique<IfStmt>(std::move($2), std::move($3), std::move($5));
    }
    | FOR expression block
    {
        $$ = std::make_unique<WhileStmt>(std::move($2), std::move($3));
    }
    | FOR block
    {
        auto always_true = std::make_unique<BoolLiteralExpr>(true);
        $$ = std::make_unique<WhileStmt>(std::move(always_true), std::move($2));
    }
    | BREAK { $$ = std::make_unique<BreakStmt>(); }
    | CONTINUE { $$ = std::make_unique<ContinueStmt>(); }
    | SWITCH expression LBRACE switch_cases RBRACE
    {
        $$ = std::make_unique<SwitchStmt>(std::move($2), std::move($4));
    }
    | block
    ;

switch_cases:
    %empty { $$ = StmtList{}; }
    | switch_cases switch_case
    {
        $$ = std::move($1);
        $$.push_back(std::move($2));
    }
    | switch_cases switch_default
    {
        $$ = std::move($1);
        $$.push_back(std::move($2));
    }
    ;

switch_case:
    CASE expression COLON statements
    {
        auto case_clause = std::make_unique<CaseClause>();
        case_clause->value = std::move($2);
        case_clause->statements = std::move($4);
        $$ = std::move(case_clause);
    }
    ;

switch_default:
    DEFAULT COLON statements
    {
        auto default_clause = std::make_unique<DefaultClause>();
        default_clause->statements = std::move($3);
        $$ = std::move(default_clause);
    }
    ;

declaration:
    VAR IDENTIFIER type ASSIGN expression
    {
        $$ = std::make_unique<VarDeclStmt>(std::move($2), $3, std::move($5));
    }
    | IDENTIFIER DEFINE_ASSIGN expression
    {
        $$ = std::make_unique<VarDeclStmt>(std::move($1), TypeSpec::Auto, std::move($3));
    }
    ;

expression:
    logical_or_expr
    ;

logical_or_expr:
    logical_and_expr
    | logical_or_expr OR logical_and_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::LogicalOr, std::move($1), std::move($3));
    }
    ;

logical_and_expr:
    equality_expr
    | logical_and_expr AND equality_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::LogicalAnd, std::move($1), std::move($3));
    }
    ;

equality_expr:
    relational_expr
    | equality_expr EQUAL relational_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Equal, std::move($1), std::move($3));
    }
    | equality_expr NOT_EQUAL relational_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::NotEqual, std::move($1), std::move($3));
    }
    ;

relational_expr:
    additive_expr
    | relational_expr LESS additive_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Less, std::move($1), std::move($3));
    }
    | relational_expr LESS_EQUAL additive_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::LessEqual, std::move($1), std::move($3));
    }
    | relational_expr GREATER additive_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Greater, std::move($1), std::move($3));
    }
    | relational_expr GREATER_EQUAL additive_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::GreaterEqual, std::move($1), std::move($3));
    }
    ;

additive_expr:
    multiplicative_expr
    | additive_expr PLUS multiplicative_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Add, std::move($1), std::move($3));
    }
    | additive_expr MINUS multiplicative_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Sub, std::move($1), std::move($3));
    }
    ;

multiplicative_expr:
    unary_expr
    | multiplicative_expr STAR unary_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Mul, std::move($1), std::move($3));
    }
    | multiplicative_expr SLASH unary_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Div, std::move($1), std::move($3));
    }
    | multiplicative_expr PERCENT unary_expr
    {
        $$ = std::make_unique<BinaryExpr>(BinaryOp::Mod, std::move($1), std::move($3));
    }
    ;

unary_expr:
    postfix_expr
    | NOT unary_expr
    {
        $$ = std::make_unique<UnaryExpr>(UnaryOp::LogicalNot, std::move($2));
    }
    | MINUS unary_expr
    {
        $$ = std::make_unique<UnaryExpr>(UnaryOp::Negate, std::move($2));
    }
    | AMP unary_expr
    {
        $$ = std::make_unique<UnaryExpr>(UnaryOp::AddressOf, std::move($2));
    }
    | STAR unary_expr
    {
        $$ = std::make_unique<UnaryExpr>(UnaryOp::Dereference, std::move($2));
    }
    | INCREMENT unary_expr
    {
        $$ = std::make_unique<PreIncDecExpr>(IncDecOp::Increment, std::move($2));
    }
    | DECREMENT unary_expr
    {
        $$ = std::make_unique<PreIncDecExpr>(IncDecOp::Decrement, std::move($2));
    }
    ;

postfix_expr:
    primary_expr
    | postfix_expr INCREMENT
    {
        $$ = std::make_unique<PostIncDecExpr>(IncDecOp::Increment, std::move($1));
    }
    | postfix_expr DECREMENT
    {
        $$ = std::make_unique<PostIncDecExpr>(IncDecOp::Decrement, std::move($1));
    }
    | postfix_expr LPAREN argument_list RPAREN
    {
        $$ = std::make_unique<CallExpr>(std::move($1), std::move($3));
    }
    | postfix_expr LBRACKET expression RBRACKET
    {
        $$ = std::make_unique<IndexExpr>(std::move($1), std::move($3));
    }
    ;

primary_expr:
    INT_LITERAL
    {
        $$ = std::make_unique<IntegerLiteralExpr>($1);
    }
    | FLOAT_LITERAL
    {
        $$ = std::make_unique<FloatLiteralExpr>($1);
    }
    | BOOL_LITERAL
    {
        $$ = std::make_unique<BoolLiteralExpr>($1);
    }
    | STRING_LITERAL
    {
        $$ = std::make_unique<StringLiteralExpr>(std::move($1));
    }
    | IDENTIFIER
    {
        $$ = std::make_unique<IdentifierExpr>(std::move($1));
    }
    | LPAREN expression RPAREN
    {
        $$ = std::move($2);
    }
    ;

argument_list:
    %empty { $$ = ExprList{}; }
    | arguments { $$ = std::move($1); }
    ;

arguments:
    expression
    {
        $$ = ExprList{};
        $$.push_back(std::move($1));
    }
    | arguments COMMA expression
    {
        $$ = std::move($1);
        $$.push_back(std::move($3));
    }
    ;

%%

void yy::Parser::error(const std::string& message) {
    driver.error("parser error: " + message);
}
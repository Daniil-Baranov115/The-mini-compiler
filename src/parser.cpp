// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hpp"


// Unqualified %code blocks.
#line 24 "src/parser.y"

    #include "driver.hpp"
    static yy::Parser::symbol_type yylex(Driver& driver) {
        return driver.next_token();
    }

#line 53 "src/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 7 "src/parser.y"
namespace yy {
#line 127 "src/parser.cpp"

  /// Build a parser object.
  Parser::Parser (Driver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_argument_list: // argument_list
      case symbol_kind::S_arguments: // arguments
        value.YY_MOVE_OR_COPY< ExprList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_assignment_expr: // assignment_expr
      case symbol_kind::S_logical_or_expr: // logical_or_expr
      case symbol_kind::S_logical_and_expr: // logical_and_expr
      case symbol_kind::S_equality_expr: // equality_expr
      case symbol_kind::S_relational_expr: // relational_expr
      case symbol_kind::S_additive_expr: // additive_expr
      case symbol_kind::S_multiplicative_expr: // multiplicative_expr
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_postfix_expr: // postfix_expr
      case symbol_kind::S_primary_expr: // primary_expr
        value.YY_MOVE_OR_COPY< ExprPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_functions: // functions
        value.YY_MOVE_OR_COPY< FunctionList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter_list: // parameter_list
      case symbol_kind::S_parameters: // parameters
        value.YY_MOVE_OR_COPY< ParameterList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_switch_cases: // switch_cases
        value.YY_MOVE_OR_COPY< StmtList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_declaration: // declaration
        value.YY_MOVE_OR_COPY< StmtPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_return_type_opt: // return_type_opt
      case symbol_kind::S_type: // type
        value.YY_MOVE_OR_COPY< TypeSpec > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOL_LITERAL: // BOOL_LITERAL
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.YY_MOVE_OR_COPY< std::int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.YY_MOVE_OR_COPY< std::unique_ptr<BlockStmt> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_switch_case: // switch_case
        value.YY_MOVE_OR_COPY< std::unique_ptr<CaseClause> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_switch_default: // switch_default
        value.YY_MOVE_OR_COPY< std::unique_ptr<DefaultClause> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_function: // function
        value.YY_MOVE_OR_COPY< std::unique_ptr<FunctionDecl> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
        value.YY_MOVE_OR_COPY< std::unique_ptr<Program> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_argument_list: // argument_list
      case symbol_kind::S_arguments: // arguments
        value.move< ExprList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_assignment_expr: // assignment_expr
      case symbol_kind::S_logical_or_expr: // logical_or_expr
      case symbol_kind::S_logical_and_expr: // logical_and_expr
      case symbol_kind::S_equality_expr: // equality_expr
      case symbol_kind::S_relational_expr: // relational_expr
      case symbol_kind::S_additive_expr: // additive_expr
      case symbol_kind::S_multiplicative_expr: // multiplicative_expr
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_postfix_expr: // postfix_expr
      case symbol_kind::S_primary_expr: // primary_expr
        value.move< ExprPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_functions: // functions
        value.move< FunctionList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter_list: // parameter_list
      case symbol_kind::S_parameters: // parameters
        value.move< ParameterList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_switch_cases: // switch_cases
        value.move< StmtList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_declaration: // declaration
        value.move< StmtPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_return_type_opt: // return_type_opt
      case symbol_kind::S_type: // type
        value.move< TypeSpec > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BOOL_LITERAL: // BOOL_LITERAL
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< std::int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.move< std::unique_ptr<BlockStmt> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_switch_case: // switch_case
        value.move< std::unique_ptr<CaseClause> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_switch_default: // switch_default
        value.move< std::unique_ptr<DefaultClause> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_function: // function
        value.move< std::unique_ptr<FunctionDecl> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
        value.move< std::unique_ptr<Program> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_argument_list: // argument_list
      case symbol_kind::S_arguments: // arguments
        value.copy< ExprList > (that.value);
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_assignment_expr: // assignment_expr
      case symbol_kind::S_logical_or_expr: // logical_or_expr
      case symbol_kind::S_logical_and_expr: // logical_and_expr
      case symbol_kind::S_equality_expr: // equality_expr
      case symbol_kind::S_relational_expr: // relational_expr
      case symbol_kind::S_additive_expr: // additive_expr
      case symbol_kind::S_multiplicative_expr: // multiplicative_expr
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_postfix_expr: // postfix_expr
      case symbol_kind::S_primary_expr: // primary_expr
        value.copy< ExprPtr > (that.value);
        break;

      case symbol_kind::S_functions: // functions
        value.copy< FunctionList > (that.value);
        break;

      case symbol_kind::S_parameter_list: // parameter_list
      case symbol_kind::S_parameters: // parameters
        value.copy< ParameterList > (that.value);
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_switch_cases: // switch_cases
        value.copy< StmtList > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_declaration: // declaration
        value.copy< StmtPtr > (that.value);
        break;

      case symbol_kind::S_return_type_opt: // return_type_opt
      case symbol_kind::S_type: // type
        value.copy< TypeSpec > (that.value);
        break;

      case symbol_kind::S_BOOL_LITERAL: // BOOL_LITERAL
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.copy< std::int64_t > (that.value);
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_block: // block
        value.copy< std::unique_ptr<BlockStmt> > (that.value);
        break;

      case symbol_kind::S_switch_case: // switch_case
        value.copy< std::unique_ptr<CaseClause> > (that.value);
        break;

      case symbol_kind::S_switch_default: // switch_default
        value.copy< std::unique_ptr<DefaultClause> > (that.value);
        break;

      case symbol_kind::S_function: // function
        value.copy< std::unique_ptr<FunctionDecl> > (that.value);
        break;

      case symbol_kind::S_program: // program
        value.copy< std::unique_ptr<Program> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_argument_list: // argument_list
      case symbol_kind::S_arguments: // arguments
        value.move< ExprList > (that.value);
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_assignment_expr: // assignment_expr
      case symbol_kind::S_logical_or_expr: // logical_or_expr
      case symbol_kind::S_logical_and_expr: // logical_and_expr
      case symbol_kind::S_equality_expr: // equality_expr
      case symbol_kind::S_relational_expr: // relational_expr
      case symbol_kind::S_additive_expr: // additive_expr
      case symbol_kind::S_multiplicative_expr: // multiplicative_expr
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_postfix_expr: // postfix_expr
      case symbol_kind::S_primary_expr: // primary_expr
        value.move< ExprPtr > (that.value);
        break;

      case symbol_kind::S_functions: // functions
        value.move< FunctionList > (that.value);
        break;

      case symbol_kind::S_parameter_list: // parameter_list
      case symbol_kind::S_parameters: // parameters
        value.move< ParameterList > (that.value);
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_switch_cases: // switch_cases
        value.move< StmtList > (that.value);
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_declaration: // declaration
        value.move< StmtPtr > (that.value);
        break;

      case symbol_kind::S_return_type_opt: // return_type_opt
      case symbol_kind::S_type: // type
        value.move< TypeSpec > (that.value);
        break;

      case symbol_kind::S_BOOL_LITERAL: // BOOL_LITERAL
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< std::int64_t > (that.value);
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_block: // block
        value.move< std::unique_ptr<BlockStmt> > (that.value);
        break;

      case symbol_kind::S_switch_case: // switch_case
        value.move< std::unique_ptr<CaseClause> > (that.value);
        break;

      case symbol_kind::S_switch_default: // switch_default
        value.move< std::unique_ptr<DefaultClause> > (that.value);
        break;

      case symbol_kind::S_function: // function
        value.move< std::unique_ptr<FunctionDecl> > (that.value);
        break;

      case symbol_kind::S_program: // program
        value.move< std::unique_ptr<Program> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_argument_list: // argument_list
      case symbol_kind::S_arguments: // arguments
        yylhs.value.emplace< ExprList > ();
        break;

      case symbol_kind::S_expression: // expression
      case symbol_kind::S_assignment_expr: // assignment_expr
      case symbol_kind::S_logical_or_expr: // logical_or_expr
      case symbol_kind::S_logical_and_expr: // logical_and_expr
      case symbol_kind::S_equality_expr: // equality_expr
      case symbol_kind::S_relational_expr: // relational_expr
      case symbol_kind::S_additive_expr: // additive_expr
      case symbol_kind::S_multiplicative_expr: // multiplicative_expr
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_postfix_expr: // postfix_expr
      case symbol_kind::S_primary_expr: // primary_expr
        yylhs.value.emplace< ExprPtr > ();
        break;

      case symbol_kind::S_functions: // functions
        yylhs.value.emplace< FunctionList > ();
        break;

      case symbol_kind::S_parameter_list: // parameter_list
      case symbol_kind::S_parameters: // parameters
        yylhs.value.emplace< ParameterList > ();
        break;

      case symbol_kind::S_statements: // statements
      case symbol_kind::S_switch_cases: // switch_cases
        yylhs.value.emplace< StmtList > ();
        break;

      case symbol_kind::S_statement: // statement
      case symbol_kind::S_declaration: // declaration
        yylhs.value.emplace< StmtPtr > ();
        break;

      case symbol_kind::S_return_type_opt: // return_type_opt
      case symbol_kind::S_type: // type
        yylhs.value.emplace< TypeSpec > ();
        break;

      case symbol_kind::S_BOOL_LITERAL: // BOOL_LITERAL
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        yylhs.value.emplace< std::int64_t > ();
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_block: // block
        yylhs.value.emplace< std::unique_ptr<BlockStmt> > ();
        break;

      case symbol_kind::S_switch_case: // switch_case
        yylhs.value.emplace< std::unique_ptr<CaseClause> > ();
        break;

      case symbol_kind::S_switch_default: // switch_default
        yylhs.value.emplace< std::unique_ptr<DefaultClause> > ();
        break;

      case symbol_kind::S_function: // function
        yylhs.value.emplace< std::unique_ptr<FunctionDecl> > ();
        break;

      case symbol_kind::S_program: // program
        yylhs.value.emplace< std::unique_ptr<Program> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: functions
#line 119 "src/parser.y"
    {
        auto prog = std::make_unique<Program>();
        prog->functions = std::move(YY_MOVE (yystack_[0].value.as < FunctionList > ()));
        driver.set_program(std::move(prog));
    }
#line 913 "src/parser.cpp"
    break;

  case 3: // functions: %empty
#line 127 "src/parser.y"
           { yylhs.value.as < FunctionList > () = FunctionList{}; }
#line 919 "src/parser.cpp"
    break;

  case 4: // functions: functions function
#line 129 "src/parser.y"
    {
        yylhs.value.as < FunctionList > () = std::move(YY_MOVE (yystack_[1].value.as < FunctionList > ()));
        yylhs.value.as < FunctionList > ().push_back(std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<FunctionDecl> > ())));
    }
#line 928 "src/parser.cpp"
    break;

  case 5: // function: "func" IDENTIFIER "(" parameter_list ")" return_type_opt block
#line 137 "src/parser.y"
    {
        auto fn = std::make_unique<FunctionDecl>();
        fn->name = std::move(YY_MOVE (yystack_[5].value.as < std::string > ()));
        fn->parameters = std::move(YY_MOVE (yystack_[3].value.as < ParameterList > ()));
        fn->body = std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<BlockStmt> > ()));
        yylhs.value.as < std::unique_ptr<FunctionDecl> > () = std::move(fn);
    }
#line 940 "src/parser.cpp"
    break;

  case 6: // return_type_opt: %empty
#line 147 "src/parser.y"
           { yylhs.value.as < TypeSpec > () = TypeSpec::Void; }
#line 946 "src/parser.cpp"
    break;

  case 7: // return_type_opt: type
#line 148 "src/parser.y"
           { yylhs.value.as < TypeSpec > () = YY_MOVE (yystack_[0].value.as < TypeSpec > ()); }
#line 952 "src/parser.cpp"
    break;

  case 8: // parameter_list: %empty
#line 152 "src/parser.y"
           { yylhs.value.as < ParameterList > () = ParameterList{}; }
#line 958 "src/parser.cpp"
    break;

  case 9: // parameter_list: parameters
#line 153 "src/parser.y"
                 { yylhs.value.as < ParameterList > () = std::move(YY_MOVE (yystack_[0].value.as < ParameterList > ())); }
#line 964 "src/parser.cpp"
    break;

  case 10: // parameters: IDENTIFIER type
#line 158 "src/parser.y"
    {
        yylhs.value.as < ParameterList > () = ParameterList{};
        yylhs.value.as < ParameterList > ().push_back({std::move(YY_MOVE (yystack_[1].value.as < std::string > ())), YY_MOVE (yystack_[0].value.as < TypeSpec > ())});
    }
#line 973 "src/parser.cpp"
    break;

  case 11: // parameters: parameters "," IDENTIFIER type
#line 163 "src/parser.y"
    {
        yylhs.value.as < ParameterList > () = std::move(YY_MOVE (yystack_[3].value.as < ParameterList > ()));
        yylhs.value.as < ParameterList > ().push_back({std::move(YY_MOVE (yystack_[1].value.as < std::string > ())), YY_MOVE (yystack_[0].value.as < TypeSpec > ())});
    }
#line 982 "src/parser.cpp"
    break;

  case 12: // type: "int"
#line 170 "src/parser.y"
               { yylhs.value.as < TypeSpec > () = TypeSpec::Int; }
#line 988 "src/parser.cpp"
    break;

  case 13: // type: "float"
#line 171 "src/parser.y"
                 { yylhs.value.as < TypeSpec > () = TypeSpec::Float; }
#line 994 "src/parser.cpp"
    break;

  case 14: // type: "bool"
#line 172 "src/parser.y"
                 { yylhs.value.as < TypeSpec > () = TypeSpec::Bool; }
#line 1000 "src/parser.cpp"
    break;

  case 15: // block: "{" statements "}"
#line 177 "src/parser.y"
    {
        auto b = std::make_unique<BlockStmt>();
        b->statements = std::move(YY_MOVE (yystack_[1].value.as < StmtList > ()));
        yylhs.value.as < std::unique_ptr<BlockStmt> > () = std::move(b);
    }
#line 1010 "src/parser.cpp"
    break;

  case 16: // statements: %empty
#line 185 "src/parser.y"
           { yylhs.value.as < StmtList > () = StmtList{}; }
#line 1016 "src/parser.cpp"
    break;

  case 17: // statements: statements statement
#line 187 "src/parser.y"
    {
        yylhs.value.as < StmtList > () = std::move(YY_MOVE (yystack_[1].value.as < StmtList > ()));
        yylhs.value.as < StmtList > ().push_back(std::move(YY_MOVE (yystack_[0].value.as < StmtPtr > ())));
    }
#line 1025 "src/parser.cpp"
    break;

  case 18: // statement: declaration
#line 194 "src/parser.y"
    { yylhs.value.as < StmtPtr > () = YY_MOVE (yystack_[0].value.as < StmtPtr > ()); }
#line 1031 "src/parser.cpp"
    break;

  case 19: // statement: expression
#line 196 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<ExprStmt>(std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1039 "src/parser.cpp"
    break;

  case 20: // statement: "return" expression
#line 200 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<ReturnStmt>(std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1047 "src/parser.cpp"
    break;

  case 21: // statement: "if" expression block
#line 204 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<IfStmt>(std::move(YY_MOVE (yystack_[1].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<BlockStmt> > ())));
    }
#line 1055 "src/parser.cpp"
    break;

  case 22: // statement: "if" expression block "else" block
#line 208 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<IfStmt>(std::move(YY_MOVE (yystack_[3].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[2].value.as < std::unique_ptr<BlockStmt> > ())), std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<BlockStmt> > ())));
    }
#line 1063 "src/parser.cpp"
    break;

  case 23: // statement: "for" expression block
#line 212 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<WhileStmt>(std::move(YY_MOVE (yystack_[1].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<BlockStmt> > ())));
    }
#line 1071 "src/parser.cpp"
    break;

  case 24: // statement: "for" block
#line 216 "src/parser.y"
    {
        auto always_true = std::make_unique<BoolLiteralExpr>(true);
        yylhs.value.as < StmtPtr > () = std::make_unique<WhileStmt>(std::move(always_true), std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<BlockStmt> > ())));
    }
#line 1080 "src/parser.cpp"
    break;

  case 25: // statement: "break"
#line 220 "src/parser.y"
            { yylhs.value.as < StmtPtr > () = std::make_unique<BreakStmt>(); }
#line 1086 "src/parser.cpp"
    break;

  case 26: // statement: "continue"
#line 221 "src/parser.y"
               { yylhs.value.as < StmtPtr > () = std::make_unique<ContinueStmt>(); }
#line 1092 "src/parser.cpp"
    break;

  case 27: // statement: "switch" expression "{" switch_cases "}"
#line 223 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<SwitchStmt>(std::move(YY_MOVE (yystack_[3].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[1].value.as < StmtList > ())));
    }
#line 1100 "src/parser.cpp"
    break;

  case 29: // switch_cases: %empty
#line 230 "src/parser.y"
           { yylhs.value.as < StmtList > () = StmtList{}; }
#line 1106 "src/parser.cpp"
    break;

  case 30: // switch_cases: switch_cases switch_case
#line 232 "src/parser.y"
    {
        yylhs.value.as < StmtList > () = std::move(YY_MOVE (yystack_[1].value.as < StmtList > ()));
        yylhs.value.as < StmtList > ().push_back(std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<CaseClause> > ())));
    }
#line 1115 "src/parser.cpp"
    break;

  case 31: // switch_cases: switch_cases switch_default
#line 237 "src/parser.y"
    {
        yylhs.value.as < StmtList > () = std::move(YY_MOVE (yystack_[1].value.as < StmtList > ()));
        yylhs.value.as < StmtList > ().push_back(std::move(YY_MOVE (yystack_[0].value.as < std::unique_ptr<DefaultClause> > ())));
    }
#line 1124 "src/parser.cpp"
    break;

  case 32: // switch_case: "case" expression ":" statements
#line 245 "src/parser.y"
    {
        auto case_clause = std::make_unique<CaseClause>();
        case_clause->value = std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ()));
        case_clause->statements = std::move(YY_MOVE (yystack_[0].value.as < StmtList > ()));
        yylhs.value.as < std::unique_ptr<CaseClause> > () = std::move(case_clause);
    }
#line 1135 "src/parser.cpp"
    break;

  case 33: // switch_default: "default" ":" statements
#line 255 "src/parser.y"
    {
        auto default_clause = std::make_unique<DefaultClause>();
        default_clause->statements = std::move(YY_MOVE (yystack_[0].value.as < StmtList > ()));
        yylhs.value.as < std::unique_ptr<DefaultClause> > () = std::move(default_clause);
    }
#line 1145 "src/parser.cpp"
    break;

  case 34: // declaration: "var" IDENTIFIER type "=" expression
#line 264 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<VarDeclStmt>(std::move(YY_MOVE (yystack_[3].value.as < std::string > ())), YY_MOVE (yystack_[2].value.as < TypeSpec > ()), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1153 "src/parser.cpp"
    break;

  case 35: // declaration: IDENTIFIER ":=" expression
#line 268 "src/parser.y"
    {
        yylhs.value.as < StmtPtr > () = std::make_unique<VarDeclStmt>(std::move(YY_MOVE (yystack_[2].value.as < std::string > ())), TypeSpec::Auto, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1161 "src/parser.cpp"
    break;

  case 36: // expression: assignment_expr
#line 274 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1167 "src/parser.cpp"
    break;

  case 37: // assignment_expr: logical_or_expr
#line 278 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1173 "src/parser.cpp"
    break;

  case 38: // assignment_expr: postfix_expr "=" assignment_expr
#line 280 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<AssignExpr>(std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1181 "src/parser.cpp"
    break;

  case 39: // logical_or_expr: logical_and_expr
#line 286 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1187 "src/parser.cpp"
    break;

  case 40: // logical_or_expr: logical_or_expr "||" logical_and_expr
#line 288 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::LogicalOr, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1195 "src/parser.cpp"
    break;

  case 41: // logical_and_expr: equality_expr
#line 294 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1201 "src/parser.cpp"
    break;

  case 42: // logical_and_expr: logical_and_expr "&&" equality_expr
#line 296 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::LogicalAnd, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1209 "src/parser.cpp"
    break;

  case 43: // equality_expr: relational_expr
#line 302 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1215 "src/parser.cpp"
    break;

  case 44: // equality_expr: equality_expr "==" relational_expr
#line 304 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Equal, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1223 "src/parser.cpp"
    break;

  case 45: // equality_expr: equality_expr "!=" relational_expr
#line 308 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::NotEqual, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1231 "src/parser.cpp"
    break;

  case 46: // relational_expr: additive_expr
#line 314 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1237 "src/parser.cpp"
    break;

  case 47: // relational_expr: relational_expr "<" additive_expr
#line 316 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Less, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1245 "src/parser.cpp"
    break;

  case 48: // relational_expr: relational_expr "<=" additive_expr
#line 320 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::LessEqual, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1253 "src/parser.cpp"
    break;

  case 49: // relational_expr: relational_expr ">" additive_expr
#line 324 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Greater, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1261 "src/parser.cpp"
    break;

  case 50: // relational_expr: relational_expr ">=" additive_expr
#line 328 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::GreaterEqual, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1269 "src/parser.cpp"
    break;

  case 51: // additive_expr: multiplicative_expr
#line 334 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1275 "src/parser.cpp"
    break;

  case 52: // additive_expr: additive_expr "+" multiplicative_expr
#line 336 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Add, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1283 "src/parser.cpp"
    break;

  case 53: // additive_expr: additive_expr "-" multiplicative_expr
#line 340 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Sub, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1291 "src/parser.cpp"
    break;

  case 54: // multiplicative_expr: unary_expr
#line 346 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1297 "src/parser.cpp"
    break;

  case 55: // multiplicative_expr: multiplicative_expr "*" unary_expr
#line 348 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Mul, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1305 "src/parser.cpp"
    break;

  case 56: // multiplicative_expr: multiplicative_expr "/" unary_expr
#line 352 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Div, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1313 "src/parser.cpp"
    break;

  case 57: // multiplicative_expr: multiplicative_expr "%" unary_expr
#line 356 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BinaryExpr>(BinaryOp::Mod, std::move(YY_MOVE (yystack_[2].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1321 "src/parser.cpp"
    break;

  case 58: // unary_expr: postfix_expr
#line 362 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1327 "src/parser.cpp"
    break;

  case 59: // unary_expr: "!" unary_expr
#line 364 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<UnaryExpr>(UnaryOp::LogicalNot, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1335 "src/parser.cpp"
    break;

  case 60: // unary_expr: "-" unary_expr
#line 368 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<UnaryExpr>(UnaryOp::Negate, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1343 "src/parser.cpp"
    break;

  case 61: // unary_expr: "&" unary_expr
#line 372 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<UnaryExpr>(UnaryOp::AddressOf, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1351 "src/parser.cpp"
    break;

  case 62: // unary_expr: "*" unary_expr
#line 376 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<UnaryExpr>(UnaryOp::Dereference, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1359 "src/parser.cpp"
    break;

  case 63: // unary_expr: "++" unary_expr
#line 380 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<PreIncDecExpr>(IncDecOp::Increment, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1367 "src/parser.cpp"
    break;

  case 64: // unary_expr: "--" unary_expr
#line 384 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<PreIncDecExpr>(IncDecOp::Decrement, std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1375 "src/parser.cpp"
    break;

  case 65: // postfix_expr: primary_expr
#line 390 "src/parser.y"
    { yylhs.value.as < ExprPtr > () = YY_MOVE (yystack_[0].value.as < ExprPtr > ()); }
#line 1381 "src/parser.cpp"
    break;

  case 66: // postfix_expr: postfix_expr "++"
#line 392 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<PostIncDecExpr>(IncDecOp::Increment, std::move(YY_MOVE (yystack_[1].value.as < ExprPtr > ())));
    }
#line 1389 "src/parser.cpp"
    break;

  case 67: // postfix_expr: postfix_expr "--"
#line 396 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<PostIncDecExpr>(IncDecOp::Decrement, std::move(YY_MOVE (yystack_[1].value.as < ExprPtr > ())));
    }
#line 1397 "src/parser.cpp"
    break;

  case 68: // postfix_expr: postfix_expr "(" argument_list ")"
#line 400 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<CallExpr>(std::move(YY_MOVE (yystack_[3].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[1].value.as < ExprList > ())));
    }
#line 1405 "src/parser.cpp"
    break;

  case 69: // postfix_expr: postfix_expr "[" expression "]"
#line 404 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<IndexExpr>(std::move(YY_MOVE (yystack_[3].value.as < ExprPtr > ())), std::move(YY_MOVE (yystack_[1].value.as < ExprPtr > ())));
    }
#line 1413 "src/parser.cpp"
    break;

  case 70: // primary_expr: INT_LITERAL
#line 411 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<IntegerLiteralExpr>(YY_MOVE (yystack_[0].value.as < std::int64_t > ()));
    }
#line 1421 "src/parser.cpp"
    break;

  case 71: // primary_expr: FLOAT_LITERAL
#line 415 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<FloatLiteralExpr>(YY_MOVE (yystack_[0].value.as < double > ()));
    }
#line 1429 "src/parser.cpp"
    break;

  case 72: // primary_expr: BOOL_LITERAL
#line 419 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<BoolLiteralExpr>(YY_MOVE (yystack_[0].value.as < bool > ()));
    }
#line 1437 "src/parser.cpp"
    break;

  case 73: // primary_expr: STRING_LITERAL
#line 423 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<StringLiteralExpr>(std::move(YY_MOVE (yystack_[0].value.as < std::string > ())));
    }
#line 1445 "src/parser.cpp"
    break;

  case 74: // primary_expr: IDENTIFIER
#line 427 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::make_unique<IdentifierExpr>(std::move(YY_MOVE (yystack_[0].value.as < std::string > ())));
    }
#line 1453 "src/parser.cpp"
    break;

  case 75: // primary_expr: "(" expression ")"
#line 431 "src/parser.y"
    {
        yylhs.value.as < ExprPtr > () = std::move(YY_MOVE (yystack_[1].value.as < ExprPtr > ()));
    }
#line 1461 "src/parser.cpp"
    break;

  case 76: // argument_list: %empty
#line 437 "src/parser.y"
           { yylhs.value.as < ExprList > () = ExprList{}; }
#line 1467 "src/parser.cpp"
    break;

  case 77: // argument_list: arguments
#line 438 "src/parser.y"
                { yylhs.value.as < ExprList > () = std::move(YY_MOVE (yystack_[0].value.as < ExprList > ())); }
#line 1473 "src/parser.cpp"
    break;

  case 78: // arguments: expression
#line 443 "src/parser.y"
    {
        yylhs.value.as < ExprList > () = ExprList{};
        yylhs.value.as < ExprList > ().push_back(std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1482 "src/parser.cpp"
    break;

  case 79: // arguments: arguments "," expression
#line 448 "src/parser.y"
    {
        yylhs.value.as < ExprList > () = std::move(YY_MOVE (yystack_[2].value.as < ExprList > ()));
        yylhs.value.as < ExprList > ().push_back(std::move(YY_MOVE (yystack_[0].value.as < ExprPtr > ())));
    }
#line 1491 "src/parser.cpp"
    break;


#line 1495 "src/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "INVALID", "func", "return",
  "var", "if", "else", "for", "switch", "case", "default", "break",
  "continue", "int", "float", "bool", "INT_LITERAL", "FLOAT_LITERAL",
  "BOOL_LITERAL", "STRING_LITERAL", "IDENTIFIER", ":=", "=", "+=", "-=",
  "++", "--", "+", "-", "*", "/", "%", "==", "!=", "<", "<=", ">", ">=",
  "&&", "||", "!", "&", "(", ")", "{", "}", "[", "]", ",", ";", ":",
  "LOWER_THAN_ELSE", "$accept", "program", "functions", "function",
  "return_type_opt", "parameter_list", "parameters", "type", "block",
  "statements", "statement", "switch_cases", "switch_case",
  "switch_default", "declaration", "expression", "assignment_expr",
  "logical_or_expr", "logical_and_expr", "equality_expr",
  "relational_expr", "additive_expr", "multiplicative_expr", "unary_expr",
  "postfix_expr", "primary_expr", "argument_list", "arguments", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -78;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -78,     2,    19,   -78,     8,   -78,     6,    24,    54,    -4,
      12,   -78,   -78,   -78,   -78,    54,    55,    32,   -78,    54,
     -78,   -78,   -78,   107,    61,    62,    61,   179,    61,   -78,
     -78,   -78,   -78,   -78,   -78,    63,    61,    61,    61,    61,
      61,    61,    61,   -78,   -78,   -78,   -78,   -78,   -78,    44,
      47,    -8,    -3,    14,    43,   -78,     1,   -78,   -78,   -78,
      54,    32,   -78,    32,    48,    61,   -78,     4,   -78,   -78,
     -78,   -78,   -78,    45,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,   -78,   -78,
      61,    61,    71,    88,   -78,   -78,   -78,   -78,    47,    -8,
      -3,    -3,    14,    14,    14,    14,    43,    43,   -78,   -78,
     -78,   -78,   -78,    64,    49,    51,    61,    32,     9,   -78,
      61,   -78,   -78,   -78,    61,    56,   -78,   -78,   -78,   -78,
      66,   -78,   -78,   150,   150
  };

  const signed char
  Parser::yydefact_[] =
  {
       3,     0,     2,     1,     0,     4,     0,     8,     0,     0,
       9,    12,    13,    14,    10,     6,     0,     0,     7,     0,
      16,     5,    11,     0,     0,     0,     0,     0,     0,    25,
      26,    70,    71,    72,    73,    74,     0,     0,     0,     0,
       0,     0,     0,    15,    28,    17,    18,    19,    36,    37,
      39,    41,    43,    46,    51,    54,    58,    65,    74,    20,
       0,     0,    24,     0,     0,     0,    63,    58,    64,    60,
      62,    59,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      76,     0,     0,    21,    23,    29,    35,    75,    40,    42,
      44,    45,    47,    48,    49,    50,    52,    53,    55,    56,
      57,    38,    78,     0,    77,     0,     0,     0,     0,    68,
       0,    69,    34,    22,     0,     0,    27,    30,    31,    79,
       0,    16,    16,    33,    32
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -78,   -78,   -78,   -78,   -78,   -78,   -78,     3,   -10,   -77,
     -78,   -78,   -78,   -78,   -78,   -18,    28,   -78,    50,    57,
     -19,   -41,   -17,   -25,   -23,   -78,   -78,   -78
  };

  const unsigned char
  Parser::yydefgoto_[] =
  {
       0,     1,     2,     5,    17,     9,    10,    14,    44,    23,
      45,   118,   127,   128,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    67,    57,   113,   114
  };

  const unsigned char
  Parser::yytable_[] =
  {
      56,    56,     3,    56,    56,    56,    59,    21,    61,    63,
      64,    66,    68,    69,    70,    71,    72,    62,    18,    56,
     124,   125,    22,     4,    73,    87,    76,    77,    88,    89,
       6,    88,    89,    78,    79,    80,    81,   102,   103,   104,
     105,    15,    56,    82,    83,    90,     8,    96,    90,    91,
       7,    93,    91,    94,   133,   134,   126,   100,   101,   108,
     109,   110,    16,    92,    56,   106,   107,    56,    56,    11,
      12,    13,   112,   115,    84,    85,    86,    19,    20,    31,
      32,    33,    34,    58,    60,    74,    65,    75,    36,    37,
      97,    38,    39,    56,    95,   116,   117,    56,   122,   120,
     121,    56,   129,    40,    41,    42,   130,   123,   131,   119,
      56,    56,    24,    25,    26,   111,    27,    28,   132,     0,
      29,    30,     0,     0,    98,    31,    32,    33,    34,    35,
       0,     0,    99,     0,    36,    37,     0,    38,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,    42,     0,    20,    43,    24,    25,    26,     0,    27,
      28,     0,     0,    29,    30,     0,     0,     0,    31,    32,
      33,    34,    35,     0,     0,     0,     0,    36,    37,     0,
      38,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,    42,     0,    20,    31,    32,    33,
      34,    58,     0,     0,     0,     0,    36,    37,     0,    38,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    40,    41,    42,     0,    20
  };

  const short
  Parser::yycheck_[] =
  {
      23,    24,     0,    26,    27,    28,    24,    17,    26,    27,
      28,    36,    37,    38,    39,    40,    41,    27,    15,    42,
      11,    12,    19,     4,    42,    24,    34,    35,    27,    28,
      22,    27,    28,    36,    37,    38,    39,    78,    79,    80,
      81,    45,    65,    29,    30,    44,    22,    65,    44,    48,
      44,    61,    48,    63,   131,   132,    47,    76,    77,    84,
      85,    86,    50,    60,    87,    82,    83,    90,    91,    15,
      16,    17,    90,    91,    31,    32,    33,    22,    46,    18,
      19,    20,    21,    22,    22,    41,    23,    40,    27,    28,
      45,    30,    31,   116,    46,    24,     8,   120,   116,    50,
      49,   124,   120,    42,    43,    44,   124,   117,    52,    45,
     133,   134,     5,     6,     7,    87,     9,    10,    52,    -1,
      13,    14,    -1,    -1,    74,    18,    19,    20,    21,    22,
      -1,    -1,    75,    -1,    27,    28,    -1,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    46,    47,     5,     6,     7,    -1,     9,
      10,    -1,    -1,    13,    14,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    27,    28,    -1,
      30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    46,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    27,    28,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    46
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    55,    56,     0,     4,    57,    22,    44,    22,    59,
      60,    15,    16,    17,    61,    45,    50,    58,    61,    22,
      46,    62,    61,    63,     5,     6,     7,     9,    10,    13,
      14,    18,    19,    20,    21,    22,    27,    28,    30,    31,
      42,    43,    44,    47,    62,    64,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    22,    69,
      22,    69,    62,    69,    69,    23,    77,    78,    77,    77,
      77,    77,    77,    69,    41,    40,    34,    35,    36,    37,
      38,    39,    29,    30,    31,    32,    33,    24,    27,    28,
      44,    48,    61,    62,    62,    46,    69,    45,    72,    73,
      74,    74,    75,    75,    75,    75,    76,    76,    77,    77,
      77,    70,    69,    80,    81,    69,    24,     8,    65,    45,
      50,    49,    69,    62,    11,    12,    47,    66,    67,    69,
      69,    52,    52,    63,    63
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    54,    55,    56,    56,    57,    58,    58,    59,    59,
      60,    60,    61,    61,    61,    62,    63,    63,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    65,
      65,    65,    66,    67,    68,    68,    69,    70,    70,    71,
      71,    72,    72,    73,    73,    73,    74,    74,    74,    74,
      74,    75,    75,    75,    76,    76,    76,    76,    77,    77,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    78,
      79,    79,    79,    79,    79,    79,    80,    80,    81,    81
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     7,     0,     1,     0,     1,
       2,     4,     1,     1,     1,     3,     0,     2,     1,     1,
       2,     3,     5,     3,     2,     1,     1,     5,     1,     0,
       2,     2,     4,     3,     5,     3,     1,     1,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     2,
       2,     2,     2,     2,     2,     1,     2,     2,     4,     4,
       1,     1,     1,     1,     1,     3,     0,     1,     1,     3
  };




#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   118,   118,   127,   128,   136,   147,   148,   152,   153,
     157,   162,   170,   171,   172,   176,   185,   186,   194,   195,
     199,   203,   207,   211,   215,   220,   221,   222,   226,   230,
     231,   236,   244,   254,   263,   267,   274,   278,   279,   286,
     287,   294,   295,   302,   303,   307,   314,   315,   319,   323,
     327,   334,   335,   339,   346,   347,   351,   355,   362,   363,
     367,   371,   375,   379,   383,   390,   391,   395,   399,   403,
     410,   414,   418,   422,   426,   430,   437,   438,   442,   447
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 7 "src/parser.y"
} // yy
#line 2025 "src/parser.cpp"

#line 454 "src/parser.y"


void yy::Parser::error(const std::string& message) {
    driver.error("parser error: " + message);
}

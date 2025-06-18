%{
#include "ast.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>

// External function from the lexer
extern int yylex();
// External function for error reporting
extern int yyerror(const char*);

// Location tracking variables from the lexer
extern int lin;
extern int col;

// Global flag to halt compilation on error
extern bool compilation_has_error;

// The root of the AST, which the parser will build
ProgramNode *root_ast_node;

%}

/* Define the union of possible semantic values for tokens and non-terminals */
%union {
    Node* pNode;
    ProgramNode* pProgramNode;
    IdentifierList* pIdentifierList;
    IdentNode* pIdentNode;
    Declarations* pDeclarations;
    VarDecl* pVarDecl;
    TypeNode* pTypeNode;
    StandardTypeNode* pStandardTypeNode;
    ArrayTypeNode* pArrayTypeNode;
    SubprogramDeclarations* pSubprogramDeclarations;
    SubprogramDeclaration* pSubprogramDeclaration;
    SubprogramHead* pSubprogramHead;
    ArgumentsNode* pArgumentsNode;
    ParameterList* pParameterList;
    ParameterDeclaration* pParameterDeclaration;
    CompoundStatementNode* pCompoundStatementNode;
    StatementList* pStatementList;
    StatementNode* pStatementNode;
    VariableNode* pVariableNode;
    ProcedureCallStatementNode* pProcedureCallStatementNode;
    ExprNode* pExprNode;
    ExpressionList* pExpressionList;
    IntNumNode* pIntNumNode;
    RealNumNode* pRealNumNode;
    BooleanLiteralNode* pBooleanLiteralNode;
    StringLiteralNode* pStringLiteralNode;

    // Raw types from lexer before they become AST nodes
    Num* rawNum;
    RealLit* rawRealLit;
    Ident* rawIdent;

    // For string literals
    char* str_val;
}

/* --- Token Declarations --- */
%token <rawNum> NUM
%token <rawRealLit> REAL_LITERAL
%token <rawIdent> IDENT
%token <str_val> STRING_LITERAL

%token TRUE_KEYWORD FALSE_KEYWORD PROGRAM VAR ARRAY OF INTEGER_TYPE REAL_TYPE BOOLEAN_TYPE FUNCTION PROCEDURE
%token BEGIN_TOKEN END_TOKEN IF THEN ELSE WHILE DO NOT_OP AND_OP OR_OP DIV_OP RETURN_KEYWORD
%token ASSIGN_OP EQ_OP NEQ_OP LT_OP LTE_OP GT_OP GTE_OP DOTDOT

/* --- Type Declarations for Non-Terminals --- */
%type <pProgramNode> program_rule
%type <pIdentNode> id_node
%type <pIntNumNode> int_num_node
%type <pRealNumNode> real_num_node
%type <pIdentifierList> identifier_list
%type <pDeclarations> declarations var_declaration_list_non_empty
%type <pVarDecl> var_declaration_item
%type <pTypeNode> type
%type <pStandardTypeNode> standard_type
%type <pSubprogramDeclarations> subprogram_declarations
%type <pSubprogramDeclaration> subprogram_declaration_block subprogram_declaration
%type <pSubprogramHead> subprogram_head
%type <pArgumentsNode> arguments
%type <pParameterList> parameter_list
%type <pParameterDeclaration> parameter_declaration_group
%type <pCompoundStatementNode> compound_statement
%type <pStatementList> optional_statements statement_list statement_list_terminated
%type <pStatementNode> statement return_statement
%type <pVariableNode> variable
%type <pProcedureCallStatementNode> procedure_statement
%type <pExpressionList> expression_list
%type <pExprNode> expr logical_or_expr logical_and_expr not_expr relational_expr additive_expr multiplicative_expr unary_expr primary

/* --- Operator Precedence and Associativity --- */
%right ASSIGN_OP
%left OR_OP
%left AND_OP
%right NOT_OP
%nonassoc EQ_OP NEQ_OP LT_OP GT_OP LTE_OP GTE_OP
%left '+' '-'
%left '*' '/' DIV_OP
%right UMINUS /* Unary minus has higher precedence */

/* Precedence for resolving the dangling-else conflict. */
/* A rule with DANGLING_IF has lower precedence than ELSE. */
%nonassoc DANGLING_IF
%nonassoc ELSE
%%

/* --- Grammar Rules --- */

program_rule: PROGRAM id_node ';' declarations subprogram_declarations compound_statement '.'
    { $$ = new ProgramNode($2, $4, $5, $6, lin, col); root_ast_node = $$; }
    ;

id_node: IDENT
    { $$ = new IdentNode($1->name, $1->line, $1->column); delete $1; }
    ;

identifier_list: id_node
    { $$ = new IdentifierList($1, lin, col); }
    | identifier_list ',' id_node
    { $1->addIdentifier($3); $$ = $1; }
    ;

declarations:
    { $$ = new Declarations(lin, col); }
    | VAR var_declaration_list_non_empty
    { $$ = $2; }
    ;

var_declaration_list_non_empty: var_declaration_item
    { $$ = new Declarations(lin, col); $$->addVarDecl($1); }
    | var_declaration_list_non_empty var_declaration_item
    { $1->addVarDecl($2); $$ = $1; }
    ;

var_declaration_item: identifier_list ':' type ';'
    { $$ = new VarDecl($1, $3, lin, col); }
    ;

type: standard_type
    { $$ = $1; }
    | ARRAY '[' int_num_node DOTDOT int_num_node ']' OF standard_type
    { $$ = new ArrayTypeNode($3, $5, $8, lin, col); }
    ;

int_num_node: NUM
    { $$ = new IntNumNode($1->value, $1->line, $1->column); delete $1; }
    ;

real_num_node: REAL_LITERAL
    { $$ = new RealNumNode($1->value, $1->line, $1->column); delete $1; }
    ;

standard_type: INTEGER_TYPE
    { $$ = new StandardTypeNode(StandardTypeNode::TYPE_INTEGER, lin, col); }
    | REAL_TYPE
    { $$ = new StandardTypeNode(StandardTypeNode::TYPE_REAL, lin, col); }
    | BOOLEAN_TYPE
    { $$ = new StandardTypeNode(StandardTypeNode::TYPE_BOOLEAN, lin, col); }
    ;

subprogram_declarations:
    { $$ = new SubprogramDeclarations(lin, col); }
    | subprogram_declarations subprogram_declaration_block
    { $1->addSubprogramDeclaration($2); $$ = $1; }
    ;

subprogram_declaration_block: subprogram_declaration ';'
    { $$ = $1; }
    ;

subprogram_declaration: subprogram_head declarations compound_statement
    { $$ = new SubprogramDeclaration($1, $2, $3, lin, col); }
    ;

subprogram_head: FUNCTION id_node arguments ':' standard_type ';'
    { $$ = new FunctionHeadNode($2, $3, $5, lin, col); }
    | PROCEDURE id_node arguments ';'
    { $$ = new ProcedureHeadNode($2, $3, lin, col); }
    ;

arguments:
    { $$ = new ArgumentsNode(lin, col); }
    | '(' parameter_list ')'
    { $$ = new ArgumentsNode($2, lin, col); }
    ;

parameter_list: parameter_declaration_group
    { $$ = new ParameterList($1, lin, col); }
    | parameter_list ';' parameter_declaration_group
    { $1->addParameterDeclarationGroup($3); $$ = $1; }
    ;

parameter_declaration_group: identifier_list ':' type
    { $$ = new ParameterDeclaration($1, $3, lin, col); }
    ;

compound_statement: BEGIN_TOKEN optional_statements END_TOKEN
    { $$ = new CompoundStatementNode($2, lin, col); }
    ;

optional_statements:
    { $$ = new StatementList(lin, col); }
    | statement_list_terminated
    { $$ = $1; }
    ;

statement_list_terminated: statement_list
    { $$ = $1; }
    | statement_list ';'
    { $$ = $1; }
    ;

statement_list: statement
    { $$ = new StatementList(lin, col); $$->addStatement($1); }
    | statement_list ';' statement
    { $1->addStatement($3); $$ = $1; }
    ;

statement: variable ASSIGN_OP expr
    { $$ = new AssignStatementNode($1, $3, lin, col); }
    | procedure_statement
    { $$ = $1; }
    | compound_statement
    { $$ = $1; }
    | IF expr THEN statement %prec DANGLING_IF
    { $$ = new IfStatementNode($2, $4, nullptr, lin, col); }
    | IF expr THEN statement ELSE statement
    { $$ = new IfStatementNode($2, $4, $6, lin, col); }
    | WHILE expr DO statement
    { $$ = new WhileStatementNode($2, $4, lin, col); }
    | return_statement
    { $$ = $1; }
    ;

return_statement: RETURN_KEYWORD expr
    { $$ = new ReturnStatementNode($2, lin, col); }
    ;

variable: id_node
    { $$ = new VariableNode($1, nullptr, lin, col); }
    | id_node '[' expr ']'
    { $$ = new VariableNode($1, $3, lin, col); }
    ;

procedure_statement: id_node
    { $$ = new ProcedureCallStatementNode($1, new ExpressionList(lin,col), lin, col); }
    | id_node '(' expression_list ')'
    { $$ = new ProcedureCallStatementNode($1, $3, lin, col); }
    ;

expression_list: expr
    { $$ = new ExpressionList(lin, col); $$->addExpression($1); }
    | expression_list ',' expr
    { $1->addExpression($3); $$ = $1; }
    ;

expr: logical_or_expr
    { $$ = $1; }
    ;

logical_or_expr: logical_and_expr
                 { $$ = $1; }
    | logical_or_expr OR_OP logical_and_expr
                 { $$ = new BinaryOpNode($1, "OR_OP", $3, lin, col); }
    ;

logical_and_expr: not_expr
                  { $$ = $1; }
    | logical_and_expr AND_OP not_expr
                  { $$ = new BinaryOpNode($1, "AND_OP", $3, lin, col); }
    ;

not_expr: relational_expr
          { $$ = $1; }
    | NOT_OP not_expr
          { $$ = new UnaryOpNode("NOT_OP", $2, lin, col); }
    ;

relational_expr: additive_expr
                 { $$ = $1; }
    | relational_expr EQ_OP additive_expr
                 { $$ = new BinaryOpNode($1, "EQ_OP", $3, lin, col); }
    | relational_expr NEQ_OP additive_expr
                 { $$ = new BinaryOpNode($1, "NEQ_OP", $3, lin, col); }
    | relational_expr LT_OP additive_expr
                 { $$ = new BinaryOpNode($1, "LT_OP", $3, lin, col); }
    | relational_expr LTE_OP additive_expr
                 { $$ = new BinaryOpNode($1, "LTE_OP", $3, lin, col); }
    | relational_expr GT_OP additive_expr
                 { $$ = new BinaryOpNode($1, "GT_OP", $3, lin, col); }
    | relational_expr GTE_OP additive_expr
                 { $$ = new BinaryOpNode($1, "GTE_OP", $3, lin, col); }
    ;

additive_expr: multiplicative_expr
               { $$ = $1; }
    | additive_expr '+' multiplicative_expr
               { $$ = new BinaryOpNode($1, "+", $3, lin, col); }
    | additive_expr '-' multiplicative_expr
               { $$ = new BinaryOpNode($1, "-", $3, lin, col); }
    ;

multiplicative_expr: unary_expr
                     { $$ = $1; }
    | multiplicative_expr '*' unary_expr
                     { $$ = new BinaryOpNode($1, "*", $3, lin, col); }
    | multiplicative_expr '/' unary_expr
                     { $$ = new BinaryOpNode($1, "/", $3, lin, col); }
    | multiplicative_expr DIV_OP unary_expr
                     { $$ = new BinaryOpNode($1, "DIV_OP", $3, lin, col); }
    ;

unary_expr: primary
            { $$ = $1; }
    | '-' primary %prec UMINUS
            { $$ = new UnaryOpNode("-", $2, lin, col); }
    ;

primary: id_node '[' expr ']'
           { $$ = new VariableNode($1, $3, $1->line, $1->column); }
    | id_node '(' expression_list ')'
           { $$ = new FunctionCallExprNode($1, $3, $1->line, $1->column); }
    | id_node
           { $$ = new IdExprNode($1, $1->line, $1->column); }
    | int_num_node
           { $$ = $1; }
    | real_num_node
           { $$ = $1; }
    | TRUE_KEYWORD
           { $$ = new BooleanLiteralNode(true, lin, col); }
    | FALSE_KEYWORD
           { $$ = new BooleanLiteralNode(false, lin, col); }
    | '(' expr ')'
           { $$ = $2; }
    | STRING_LITERAL
         { $$ = new StringLiteralNode($1, lin, col); }
    ;

%%

// Standardized yyerror function
int yyerror(const char* s) {
    compilation_has_error = true; 
    fprintf(stderr, "Syntax Error (L:%d, C:%d): %s\n", lin, col, s);
    return 0;
}
%{
#include "ast.h"
#include "parser.h" 
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cctype> // For tolower()
using namespace std;

int lin = 1;
int col = 1;

// Start location for multi-line comments for error reporting
int comment_start_lin = 0;
int comment_start_col = 0;

// An external reference to the global error flag, set on any compilation error
extern bool compilation_has_error;

// Helper function to convert a C-style string to a lowercase C++ string
std::string toLower(const char* c_str) {
    std::string str(c_str);
    for (char &c : str) {
        c = tolower(c);
    }
    return str;
}
// Function to update position tracking
void updatePosition(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            lin++;
            col = 1;
        } else {
            col++;
        }
    }
}

%}

%option noyywrap
%option yylineno

/* State for handling multi-line comments */
%x ML_COMMENT

/* --- Regular Definitions --- */
DIGIT       [0-9]
LETTER      [a-zA-Z]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
INTEGER_LIT 0|[1-9]{DIGIT}*
REAL_LIT1   {DIGIT}+\.{DIGIT}*([eE][+-]?{DIGIT}+)?
REAL_LIT2   {DIGIT}*\.{DIGIT}+([eE][+-]?{DIGIT}+)?
REAL_LIT3   {DIGIT}+[eE][+-]?{DIGIT}+
COMMENT    \/\/[^\n]*

WHITESPACE  [ \t\r]+
NEWLINE     \n

%%

{WHITESPACE}  { updatePosition(yytext); /* Ignore whitespace, update column */ }
{NEWLINE}     { lin++; col = 1; /* Newline increments line, resets column */ }

{COMMENT}      { updatePosition(yytext);/* Skip line comments */ }


"{"           {
                comment_start_lin = lin;
                comment_start_col = col;
                updatePosition(yytext);
                BEGIN(ML_COMMENT);
              }
<ML_COMMENT>"}"           { updatePosition(yytext); BEGIN(INITIAL); }
<ML_COMMENT>{NEWLINE}     { lin++; col = 1; }
<ML_COMMENT>.             { updatePosition(yytext); }
<ML_COMMENT><<EOF>>       {
                            std::cerr << "Lexical Error (L:" << lin << ", C:" << col << "): Unterminated comment that started at (L:" << comment_start_lin << ", C:" << comment_start_col << ")." << std::endl;
                            compilation_has_error = true;
                            yyterminate();
                          }

"'"[^'\n]*"'" { 
              int token_start_col = col;
              updatePosition(yytext);
              char* s_val = (char*)malloc(yyleng - 1); 
              if (s_val) {
                strncpy(s_val, yytext + 1, yyleng - 2);
                s_val[yyleng - 2] = '\0';  // Ensure null-termination
                yylval.str_val = s_val;
              } else {
                fprintf(stderr, "Lexer: Malloc failed for string literal at line %d, col %d\n", lin, token_start_col);
                yyterminate();
              }
              return STRING_LITERAL;
            }

{REAL_LIT1}|{REAL_LIT2}|{REAL_LIT3} {
    int token_start_col = col;
    updatePosition(yytext);
    yylval.rawRealLit = new RealLit(atof(yytext), lin, token_start_col);
    return REAL_LITERAL;
}

{INTEGER_LIT} { 
    int token_start_col = col;
    col+= yyleng;
    yylval.rawNum = new Num(atoi(yytext), lin, token_start_col);
    return NUM; 
}


{IDENTIFIER} {
    int token_start_col = col;
    updatePosition(yytext);
    std::string lowerId = toLower(yytext);

    // Check for keywords (case-insensitive)
    if (lowerId == "program")   return PROGRAM;
    if (lowerId == "var")       return VAR;
    if (lowerId == "integer")   return INTEGER_TYPE;
    if (lowerId == "real")      return REAL_TYPE;
    if (lowerId == "boolean")   return BOOLEAN_TYPE;
    if (lowerId == "function")  return FUNCTION;
    if (lowerId == "procedure") return PROCEDURE;
    if (lowerId == "begin")     return BEGIN_TOKEN;
    if (lowerId == "end")       return END_TOKEN;
    if (lowerId == "if")        return IF;
    if (lowerId == "then")      return THEN;
    if (lowerId == "else")      return ELSE;
    if (lowerId == "while")     return WHILE;
    if (lowerId == "do")        return DO;
    if (lowerId == "array")     return ARRAY;
    if (lowerId == "of")        return OF;
    if (lowerId == "div")       return DIV_OP;
    if (lowerId == "not")       return NOT_OP;
    if (lowerId == "or")        return OR_OP;
    if (lowerId == "and")       return AND_OP;
    if (lowerId == "true")      return TRUE_KEYWORD;
    if (lowerId == "false")     return FALSE_KEYWORD;
    if (lowerId == "return")    return RETURN_KEYWORD;

    // Not a keyword -> treat as an identifier
    yylval.rawIdent = new Ident(std::string(yytext), lin, token_start_col);
    return IDENT;
}


":="          { updatePosition(yytext); return ASSIGN_OP; }
"<>"          { updatePosition(yytext); return NEQ_OP; }
"<="          { updatePosition(yytext); return LTE_OP; }
">="          { updatePosition(yytext); return GTE_OP; }
".."          { updatePosition(yytext); return DOTDOT; }
"+"           { updatePosition(yytext); return '+'; }
"-"           { updatePosition(yytext); return '-'; }
"*"           { updatePosition(yytext); return '*'; }
"/"           { updatePosition(yytext); return '/'; }
"="           { updatePosition(yytext); return EQ_OP; }
"<"           { updatePosition(yytext); return LT_OP; }
">"           { updatePosition(yytext); return GT_OP; }
"("           { updatePosition(yytext); return '('; }
")"           { updatePosition(yytext); return ')'; }
"["           { updatePosition(yytext); return '['; }
"]"           { updatePosition(yytext); return ']'; }
":"           { updatePosition(yytext); return ':'; }
";"           { updatePosition(yytext); return ';'; }
","           { updatePosition(yytext); return ','; }
"."           { updatePosition(yytext); return '.'; }

.            { cerr << "Lexical error at line " << lin << ", column " << col << ": Invalid character '" << yytext << "'" << endl; updatePosition(yytext); }

%%
/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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


#line 95 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_REAL_LITERAL = 4,               /* REAL_LITERAL  */
  YYSYMBOL_IDENT = 5,                      /* IDENT  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_TRUE_KEYWORD = 7,               /* TRUE_KEYWORD  */
  YYSYMBOL_FALSE_KEYWORD = 8,              /* FALSE_KEYWORD  */
  YYSYMBOL_PROGRAM = 9,                    /* PROGRAM  */
  YYSYMBOL_VAR = 10,                       /* VAR  */
  YYSYMBOL_ARRAY = 11,                     /* ARRAY  */
  YYSYMBOL_OF = 12,                        /* OF  */
  YYSYMBOL_INTEGER_TYPE = 13,              /* INTEGER_TYPE  */
  YYSYMBOL_REAL_TYPE = 14,                 /* REAL_TYPE  */
  YYSYMBOL_BOOLEAN_TYPE = 15,              /* BOOLEAN_TYPE  */
  YYSYMBOL_FUNCTION = 16,                  /* FUNCTION  */
  YYSYMBOL_PROCEDURE = 17,                 /* PROCEDURE  */
  YYSYMBOL_BEGIN_TOKEN = 18,               /* BEGIN_TOKEN  */
  YYSYMBOL_END_TOKEN = 19,                 /* END_TOKEN  */
  YYSYMBOL_IF = 20,                        /* IF  */
  YYSYMBOL_THEN = 21,                      /* THEN  */
  YYSYMBOL_ELSE = 22,                      /* ELSE  */
  YYSYMBOL_WHILE = 23,                     /* WHILE  */
  YYSYMBOL_DO = 24,                        /* DO  */
  YYSYMBOL_NOT_OP = 25,                    /* NOT_OP  */
  YYSYMBOL_AND_OP = 26,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 27,                     /* OR_OP  */
  YYSYMBOL_DIV_OP = 28,                    /* DIV_OP  */
  YYSYMBOL_RETURN_KEYWORD = 29,            /* RETURN_KEYWORD  */
  YYSYMBOL_ASSIGN_OP = 30,                 /* ASSIGN_OP  */
  YYSYMBOL_EQ_OP = 31,                     /* EQ_OP  */
  YYSYMBOL_NEQ_OP = 32,                    /* NEQ_OP  */
  YYSYMBOL_LT_OP = 33,                     /* LT_OP  */
  YYSYMBOL_LTE_OP = 34,                    /* LTE_OP  */
  YYSYMBOL_GT_OP = 35,                     /* GT_OP  */
  YYSYMBOL_GTE_OP = 36,                    /* GTE_OP  */
  YYSYMBOL_DOTDOT = 37,                    /* DOTDOT  */
  YYSYMBOL_38_ = 38,                       /* '+'  */
  YYSYMBOL_39_ = 39,                       /* '-'  */
  YYSYMBOL_40_ = 40,                       /* '*'  */
  YYSYMBOL_41_ = 41,                       /* '/'  */
  YYSYMBOL_UMINUS = 42,                    /* UMINUS  */
  YYSYMBOL_DANGLING_IF = 43,               /* DANGLING_IF  */
  YYSYMBOL_44_ = 44,                       /* ';'  */
  YYSYMBOL_45_ = 45,                       /* '.'  */
  YYSYMBOL_46_ = 46,                       /* ','  */
  YYSYMBOL_47_ = 47,                       /* ':'  */
  YYSYMBOL_48_ = 48,                       /* '['  */
  YYSYMBOL_49_ = 49,                       /* ']'  */
  YYSYMBOL_50_ = 50,                       /* '('  */
  YYSYMBOL_51_ = 51,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_program_rule = 53,              /* program_rule  */
  YYSYMBOL_id_node = 54,                   /* id_node  */
  YYSYMBOL_identifier_list = 55,           /* identifier_list  */
  YYSYMBOL_declarations = 56,              /* declarations  */
  YYSYMBOL_var_declaration_list_non_empty = 57, /* var_declaration_list_non_empty  */
  YYSYMBOL_var_declaration_item = 58,      /* var_declaration_item  */
  YYSYMBOL_type = 59,                      /* type  */
  YYSYMBOL_int_num_node = 60,              /* int_num_node  */
  YYSYMBOL_real_num_node = 61,             /* real_num_node  */
  YYSYMBOL_standard_type = 62,             /* standard_type  */
  YYSYMBOL_subprogram_declarations = 63,   /* subprogram_declarations  */
  YYSYMBOL_subprogram_declaration_block = 64, /* subprogram_declaration_block  */
  YYSYMBOL_subprogram_declaration = 65,    /* subprogram_declaration  */
  YYSYMBOL_subprogram_head = 66,           /* subprogram_head  */
  YYSYMBOL_arguments = 67,                 /* arguments  */
  YYSYMBOL_parameter_list = 68,            /* parameter_list  */
  YYSYMBOL_parameter_declaration_group = 69, /* parameter_declaration_group  */
  YYSYMBOL_compound_statement = 70,        /* compound_statement  */
  YYSYMBOL_optional_statements = 71,       /* optional_statements  */
  YYSYMBOL_statement_list_terminated = 72, /* statement_list_terminated  */
  YYSYMBOL_statement_list = 73,            /* statement_list  */
  YYSYMBOL_statement = 74,                 /* statement  */
  YYSYMBOL_return_statement = 75,          /* return_statement  */
  YYSYMBOL_variable = 76,                  /* variable  */
  YYSYMBOL_procedure_statement = 77,       /* procedure_statement  */
  YYSYMBOL_expression_list = 78,           /* expression_list  */
  YYSYMBOL_expr = 79,                      /* expr  */
  YYSYMBOL_logical_or_expr = 80,           /* logical_or_expr  */
  YYSYMBOL_logical_and_expr = 81,          /* logical_and_expr  */
  YYSYMBOL_not_expr = 82,                  /* not_expr  */
  YYSYMBOL_relational_expr = 83,           /* relational_expr  */
  YYSYMBOL_additive_expr = 84,             /* additive_expr  */
  YYSYMBOL_multiplicative_expr = 85,       /* multiplicative_expr  */
  YYSYMBOL_unary_expr = 86,                /* unary_expr  */
  YYSYMBOL_primary = 87                    /* primary  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   147

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  150

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      50,    51,    40,    38,    46,    39,    45,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    47,    44,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    42,    43
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   115,   115,   119,   123,   125,   130,   131,   135,   137,
     141,   145,   147,   151,   155,   159,   161,   163,   168,   169,
     173,   177,   181,   183,   188,   189,   193,   195,   199,   203,
     208,   209,   213,   215,   219,   221,   225,   227,   229,   231,
     233,   235,   237,   241,   245,   247,   251,   253,   257,   259,
     263,   267,   269,   273,   275,   279,   281,   285,   287,   289,
     291,   293,   295,   297,   301,   303,   305,   309,   311,   313,
     315,   319,   321,   325,   327,   329,   331,   333,   335,   337,
     339,   341
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "REAL_LITERAL",
  "IDENT", "STRING_LITERAL", "TRUE_KEYWORD", "FALSE_KEYWORD", "PROGRAM",
  "VAR", "ARRAY", "OF", "INTEGER_TYPE", "REAL_TYPE", "BOOLEAN_TYPE",
  "FUNCTION", "PROCEDURE", "BEGIN_TOKEN", "END_TOKEN", "IF", "THEN",
  "ELSE", "WHILE", "DO", "NOT_OP", "AND_OP", "OR_OP", "DIV_OP",
  "RETURN_KEYWORD", "ASSIGN_OP", "EQ_OP", "NEQ_OP", "LT_OP", "LTE_OP",
  "GT_OP", "GTE_OP", "DOTDOT", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "DANGLING_IF", "';'", "'.'", "','", "':'", "'['", "']'", "'('", "')'",
  "$accept", "program_rule", "id_node", "identifier_list", "declarations",
  "var_declaration_list_non_empty", "var_declaration_item", "type",
  "int_num_node", "real_num_node", "standard_type",
  "subprogram_declarations", "subprogram_declaration_block",
  "subprogram_declaration", "subprogram_head", "arguments",
  "parameter_list", "parameter_declaration_group", "compound_statement",
  "optional_statements", "statement_list_terminated", "statement_list",
  "statement", "return_statement", "variable", "procedure_statement",
  "expression_list", "expr", "logical_or_expr", "logical_and_expr",
  "not_expr", "relational_expr", "additive_expr", "multiplicative_expr",
  "unary_expr", "primary", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-79)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-45)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      37,    10,    40,   -79,    13,   -79,    49,    10,   -79,   -79,
      36,    10,   -79,    71,    10,    79,   -79,    10,    10,    55,
     -79,    32,    49,    67,   -79,    70,   -79,   -79,   -79,    35,
     -79,    69,    69,    17,    17,    17,   -18,   -79,   101,   -79,
      77,   -79,   -79,    92,   -79,   -79,   105,   -79,   121,   -79,
      10,    78,    82,   -79,   -79,   -79,   -79,   -79,    17,    31,
      17,   -19,   -79,   -79,   106,   102,   104,   -79,    30,    57,
     -14,   -79,   -79,   107,   -79,    17,    17,   -79,    55,    17,
     -79,    91,    68,    -3,   -79,    93,   -79,   -79,   -79,    81,
      17,    17,    55,    17,    17,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    55,    84,    23,   -79,
     -79,   -79,   121,    79,    10,   -79,    90,   -79,    86,    34,
     114,   104,   -79,    57,    57,    57,    57,    57,    57,   -14,
     -14,   -79,   -79,   -79,   -79,   -79,    17,   -79,    88,   -79,
     -79,   -79,   -79,   -79,    55,   -79,   126,   -79,    93,   -79
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     3,     0,     1,     6,     0,    18,     4,
       0,     7,     8,     0,     0,     0,     9,     0,     0,    30,
      19,     0,     6,     0,     5,     0,    15,    16,    17,     0,
      11,    24,    24,     0,     0,     0,    46,    38,     0,    31,
      32,    34,    42,     0,    37,    20,     0,     2,     0,    10,
       0,     0,     0,    13,    14,    81,    78,    79,     0,     0,
       0,    75,    76,    77,     0,    50,    51,    53,    55,    57,
      64,    67,    71,     0,    43,     0,     0,    29,    33,     0,
      21,     0,     0,     0,    26,     0,    23,    56,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      35,    36,     0,     0,     0,    25,     0,    80,     0,     0,
      39,    52,    54,    58,    59,    60,    61,    62,    63,    65,
      66,    70,    68,    69,    41,    45,     0,    47,     0,    28,
      27,    22,    73,    74,     0,    49,     0,    40,     0,    12
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -79,   -79,    -1,   -46,   117,   -79,   129,    28,   -40,   -79,
     -78,   -79,   -79,   -79,   -79,   110,   -79,    33,    -2,   -79,
     -79,   -79,   -73,   -79,   -79,   -79,    53,   -32,   -79,    52,
     -49,   -79,     3,    15,     6,    87
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    61,    10,     8,    11,    12,    29,    62,    63,
      30,    13,    20,    21,    22,    51,    83,    84,    37,    38,
      39,    40,    41,    42,    43,    44,   108,   109,    65,    66,
      67,    68,    69,    70,    71,    72
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       4,    64,    73,    74,    82,   110,     9,   116,    81,    87,
       9,    23,   -44,    24,   103,     3,    31,    32,    36,   120,
      53,    54,     3,    55,    56,    57,   104,   105,    89,    90,
      75,    91,    76,   134,    53,    54,     3,    55,    56,    57,
       5,   114,    58,   107,    80,   122,     1,   111,   115,     9,
      53,    54,     3,    55,    56,    57,    59,     6,   118,     7,
       3,    95,    96,    97,    98,    99,   100,    60,    82,   136,
     149,   147,   138,    19,   137,    33,    45,    36,    34,    49,
     136,    60,    14,    15,    35,   143,    59,    17,    18,    19,
      25,    36,    26,    27,    28,   101,   102,    60,   123,   124,
     125,   126,   127,   128,   145,    36,    26,    27,    28,   131,
     132,   133,    47,     9,    14,   113,   129,   130,    48,    50,
      77,    78,    79,    19,    53,    85,    86,    92,   112,    93,
      94,   106,   117,   135,   141,   142,   144,   146,   148,    46,
      16,   139,    52,    36,   119,   121,    88,   140
};

static const yytype_uint8 yycheck[] =
{
       1,    33,    34,    35,    50,    78,     7,    85,    48,    58,
      11,    13,    30,    14,    28,     5,    17,    18,    19,    92,
       3,     4,     5,     6,     7,     8,    40,    41,    60,    48,
      48,    50,    50,   106,     3,     4,     5,     6,     7,     8,
       0,    44,    25,    75,    46,    94,     9,    79,    51,    50,
       3,     4,     5,     6,     7,     8,    39,    44,    90,    10,
       5,    31,    32,    33,    34,    35,    36,    50,   114,    46,
     148,   144,   112,    18,    51,    20,    44,    78,    23,    44,
      46,    50,    46,    47,    29,    51,    39,    16,    17,    18,
      11,    92,    13,    14,    15,    38,    39,    50,    95,    96,
      97,    98,    99,   100,   136,   106,    13,    14,    15,   103,
     104,   105,    45,   114,    46,    47,   101,   102,    48,    50,
      19,    44,    30,    18,     3,    47,    44,    21,    37,    27,
      26,    24,    51,    49,    44,    49,    22,    49,    12,    22,
      11,   113,    32,   144,    91,    93,    59,   114
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    53,     5,    54,     0,    44,    10,    56,    54,
      55,    57,    58,    63,    46,    47,    58,    16,    17,    18,
      64,    65,    66,    70,    54,    11,    13,    14,    15,    59,
      62,    54,    54,    20,    23,    29,    54,    70,    71,    72,
      73,    74,    75,    76,    77,    44,    56,    45,    48,    44,
      50,    67,    67,     3,     4,     6,     7,     8,    25,    39,
      50,    54,    60,    61,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    79,    79,    48,    50,    19,    44,    30,
      70,    60,    55,    68,    69,    47,    44,    82,    87,    79,
      48,    50,    21,    27,    26,    31,    32,    33,    34,    35,
      36,    38,    39,    28,    40,    41,    24,    79,    78,    79,
      74,    79,    37,    47,    44,    51,    62,    51,    79,    78,
      74,    81,    82,    84,    84,    84,    84,    84,    84,    85,
      85,    86,    86,    86,    74,    49,    46,    51,    60,    59,
      69,    44,    49,    51,    22,    79,    49,    74,    12,    62
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    55,    55,    56,    56,    57,    57,
      58,    59,    59,    60,    61,    62,    62,    62,    63,    63,
      64,    65,    66,    66,    67,    67,    68,    68,    69,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    74,    74,
      74,    74,    74,    75,    76,    76,    77,    77,    78,    78,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    83,
      83,    83,    83,    83,    84,    84,    84,    85,    85,    85,
      85,    86,    86,    87,    87,    87,    87,    87,    87,    87,
      87,    87
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     7,     1,     1,     3,     0,     2,     1,     2,
       4,     1,     8,     1,     1,     1,     1,     1,     0,     2,
       2,     3,     6,     4,     0,     3,     1,     3,     3,     3,
       0,     1,     1,     2,     1,     3,     3,     1,     1,     4,
       6,     4,     1,     2,     1,     4,     1,     4,     1,     3,
       1,     1,     3,     1,     3,     1,     2,     1,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     4,     4,     1,     1,     1,     1,     1,
       3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program_rule: PROGRAM id_node ';' declarations subprogram_declarations compound_statement '.'  */
#line 116 "parser.y"
    { (yyval.pProgramNode) = new ProgramNode((yyvsp[-5].pIdentNode), (yyvsp[-3].pDeclarations), (yyvsp[-2].pSubprogramDeclarations), (yyvsp[-1].pCompoundStatementNode), lin, col); root_ast_node = (yyval.pProgramNode); }
#line 1276 "parser.cpp"
    break;

  case 3: /* id_node: IDENT  */
#line 120 "parser.y"
    { (yyval.pIdentNode) = new IdentNode((yyvsp[0].rawIdent)->name, (yyvsp[0].rawIdent)->line, (yyvsp[0].rawIdent)->column); delete (yyvsp[0].rawIdent); }
#line 1282 "parser.cpp"
    break;

  case 4: /* identifier_list: id_node  */
#line 124 "parser.y"
    { (yyval.pIdentifierList) = new IdentifierList((yyvsp[0].pIdentNode), lin, col); }
#line 1288 "parser.cpp"
    break;

  case 5: /* identifier_list: identifier_list ',' id_node  */
#line 126 "parser.y"
    { (yyvsp[-2].pIdentifierList)->addIdentifier((yyvsp[0].pIdentNode)); (yyval.pIdentifierList) = (yyvsp[-2].pIdentifierList); }
#line 1294 "parser.cpp"
    break;

  case 6: /* declarations: %empty  */
#line 130 "parser.y"
    { (yyval.pDeclarations) = new Declarations(lin, col); }
#line 1300 "parser.cpp"
    break;

  case 7: /* declarations: VAR var_declaration_list_non_empty  */
#line 132 "parser.y"
    { (yyval.pDeclarations) = (yyvsp[0].pDeclarations); }
#line 1306 "parser.cpp"
    break;

  case 8: /* var_declaration_list_non_empty: var_declaration_item  */
#line 136 "parser.y"
    { (yyval.pDeclarations) = new Declarations(lin, col); (yyval.pDeclarations)->addVarDecl((yyvsp[0].pVarDecl)); }
#line 1312 "parser.cpp"
    break;

  case 9: /* var_declaration_list_non_empty: var_declaration_list_non_empty var_declaration_item  */
#line 138 "parser.y"
    { (yyvsp[-1].pDeclarations)->addVarDecl((yyvsp[0].pVarDecl)); (yyval.pDeclarations) = (yyvsp[-1].pDeclarations); }
#line 1318 "parser.cpp"
    break;

  case 10: /* var_declaration_item: identifier_list ':' type ';'  */
#line 142 "parser.y"
    { (yyval.pVarDecl) = new VarDecl((yyvsp[-3].pIdentifierList), (yyvsp[-1].pTypeNode), lin, col); }
#line 1324 "parser.cpp"
    break;

  case 11: /* type: standard_type  */
#line 146 "parser.y"
    { (yyval.pTypeNode) = (yyvsp[0].pStandardTypeNode); }
#line 1330 "parser.cpp"
    break;

  case 12: /* type: ARRAY '[' int_num_node DOTDOT int_num_node ']' OF standard_type  */
#line 148 "parser.y"
    { (yyval.pTypeNode) = new ArrayTypeNode((yyvsp[-5].pIntNumNode), (yyvsp[-3].pIntNumNode), (yyvsp[0].pStandardTypeNode), lin, col); }
#line 1336 "parser.cpp"
    break;

  case 13: /* int_num_node: NUM  */
#line 152 "parser.y"
    { (yyval.pIntNumNode) = new IntNumNode((yyvsp[0].rawNum)->value, (yyvsp[0].rawNum)->line, (yyvsp[0].rawNum)->column); delete (yyvsp[0].rawNum); }
#line 1342 "parser.cpp"
    break;

  case 14: /* real_num_node: REAL_LITERAL  */
#line 156 "parser.y"
    { (yyval.pRealNumNode) = new RealNumNode((yyvsp[0].rawRealLit)->value, (yyvsp[0].rawRealLit)->line, (yyvsp[0].rawRealLit)->column); delete (yyvsp[0].rawRealLit); }
#line 1348 "parser.cpp"
    break;

  case 15: /* standard_type: INTEGER_TYPE  */
#line 160 "parser.y"
    { (yyval.pStandardTypeNode) = new StandardTypeNode(StandardTypeNode::TYPE_INTEGER, lin, col); }
#line 1354 "parser.cpp"
    break;

  case 16: /* standard_type: REAL_TYPE  */
#line 162 "parser.y"
    { (yyval.pStandardTypeNode) = new StandardTypeNode(StandardTypeNode::TYPE_REAL, lin, col); }
#line 1360 "parser.cpp"
    break;

  case 17: /* standard_type: BOOLEAN_TYPE  */
#line 164 "parser.y"
    { (yyval.pStandardTypeNode) = new StandardTypeNode(StandardTypeNode::TYPE_BOOLEAN, lin, col); }
#line 1366 "parser.cpp"
    break;

  case 18: /* subprogram_declarations: %empty  */
#line 168 "parser.y"
    { (yyval.pSubprogramDeclarations) = new SubprogramDeclarations(lin, col); }
#line 1372 "parser.cpp"
    break;

  case 19: /* subprogram_declarations: subprogram_declarations subprogram_declaration_block  */
#line 170 "parser.y"
    { (yyvsp[-1].pSubprogramDeclarations)->addSubprogramDeclaration((yyvsp[0].pSubprogramDeclaration)); (yyval.pSubprogramDeclarations) = (yyvsp[-1].pSubprogramDeclarations); }
#line 1378 "parser.cpp"
    break;

  case 20: /* subprogram_declaration_block: subprogram_declaration ';'  */
#line 174 "parser.y"
    { (yyval.pSubprogramDeclaration) = (yyvsp[-1].pSubprogramDeclaration); }
#line 1384 "parser.cpp"
    break;

  case 21: /* subprogram_declaration: subprogram_head declarations compound_statement  */
#line 178 "parser.y"
    { (yyval.pSubprogramDeclaration) = new SubprogramDeclaration((yyvsp[-2].pSubprogramHead), (yyvsp[-1].pDeclarations), (yyvsp[0].pCompoundStatementNode), lin, col); }
#line 1390 "parser.cpp"
    break;

  case 22: /* subprogram_head: FUNCTION id_node arguments ':' standard_type ';'  */
#line 182 "parser.y"
    { (yyval.pSubprogramHead) = new FunctionHeadNode((yyvsp[-4].pIdentNode), (yyvsp[-3].pArgumentsNode), (yyvsp[-1].pStandardTypeNode), lin, col); }
#line 1396 "parser.cpp"
    break;

  case 23: /* subprogram_head: PROCEDURE id_node arguments ';'  */
#line 184 "parser.y"
    { (yyval.pSubprogramHead) = new ProcedureHeadNode((yyvsp[-2].pIdentNode), (yyvsp[-1].pArgumentsNode), lin, col); }
#line 1402 "parser.cpp"
    break;

  case 24: /* arguments: %empty  */
#line 188 "parser.y"
    { (yyval.pArgumentsNode) = new ArgumentsNode(lin, col); }
#line 1408 "parser.cpp"
    break;

  case 25: /* arguments: '(' parameter_list ')'  */
#line 190 "parser.y"
    { (yyval.pArgumentsNode) = new ArgumentsNode((yyvsp[-1].pParameterList), lin, col); }
#line 1414 "parser.cpp"
    break;

  case 26: /* parameter_list: parameter_declaration_group  */
#line 194 "parser.y"
    { (yyval.pParameterList) = new ParameterList((yyvsp[0].pParameterDeclaration), lin, col); }
#line 1420 "parser.cpp"
    break;

  case 27: /* parameter_list: parameter_list ';' parameter_declaration_group  */
#line 196 "parser.y"
    { (yyvsp[-2].pParameterList)->addParameterDeclarationGroup((yyvsp[0].pParameterDeclaration)); (yyval.pParameterList) = (yyvsp[-2].pParameterList); }
#line 1426 "parser.cpp"
    break;

  case 28: /* parameter_declaration_group: identifier_list ':' type  */
#line 200 "parser.y"
    { (yyval.pParameterDeclaration) = new ParameterDeclaration((yyvsp[-2].pIdentifierList), (yyvsp[0].pTypeNode), lin, col); }
#line 1432 "parser.cpp"
    break;

  case 29: /* compound_statement: BEGIN_TOKEN optional_statements END_TOKEN  */
#line 204 "parser.y"
    { (yyval.pCompoundStatementNode) = new CompoundStatementNode((yyvsp[-1].pStatementList), lin, col); }
#line 1438 "parser.cpp"
    break;

  case 30: /* optional_statements: %empty  */
#line 208 "parser.y"
    { (yyval.pStatementList) = new StatementList(lin, col); }
#line 1444 "parser.cpp"
    break;

  case 31: /* optional_statements: statement_list_terminated  */
#line 210 "parser.y"
    { (yyval.pStatementList) = (yyvsp[0].pStatementList); }
#line 1450 "parser.cpp"
    break;

  case 32: /* statement_list_terminated: statement_list  */
#line 214 "parser.y"
    { (yyval.pStatementList) = (yyvsp[0].pStatementList); }
#line 1456 "parser.cpp"
    break;

  case 33: /* statement_list_terminated: statement_list ';'  */
#line 216 "parser.y"
    { (yyval.pStatementList) = (yyvsp[-1].pStatementList); }
#line 1462 "parser.cpp"
    break;

  case 34: /* statement_list: statement  */
#line 220 "parser.y"
    { (yyval.pStatementList) = new StatementList(lin, col); (yyval.pStatementList)->addStatement((yyvsp[0].pStatementNode)); }
#line 1468 "parser.cpp"
    break;

  case 35: /* statement_list: statement_list ';' statement  */
#line 222 "parser.y"
    { (yyvsp[-2].pStatementList)->addStatement((yyvsp[0].pStatementNode)); (yyval.pStatementList) = (yyvsp[-2].pStatementList); }
#line 1474 "parser.cpp"
    break;

  case 36: /* statement: variable ASSIGN_OP expr  */
#line 226 "parser.y"
    { (yyval.pStatementNode) = new AssignStatementNode((yyvsp[-2].pVariableNode), (yyvsp[0].pExprNode), lin, col); }
#line 1480 "parser.cpp"
    break;

  case 37: /* statement: procedure_statement  */
#line 228 "parser.y"
    { (yyval.pStatementNode) = (yyvsp[0].pProcedureCallStatementNode); }
#line 1486 "parser.cpp"
    break;

  case 38: /* statement: compound_statement  */
#line 230 "parser.y"
    { (yyval.pStatementNode) = (yyvsp[0].pCompoundStatementNode); }
#line 1492 "parser.cpp"
    break;

  case 39: /* statement: IF expr THEN statement  */
#line 232 "parser.y"
    { (yyval.pStatementNode) = new IfStatementNode((yyvsp[-2].pExprNode), (yyvsp[0].pStatementNode), nullptr, lin, col); }
#line 1498 "parser.cpp"
    break;

  case 40: /* statement: IF expr THEN statement ELSE statement  */
#line 234 "parser.y"
    { (yyval.pStatementNode) = new IfStatementNode((yyvsp[-4].pExprNode), (yyvsp[-2].pStatementNode), (yyvsp[0].pStatementNode), lin, col); }
#line 1504 "parser.cpp"
    break;

  case 41: /* statement: WHILE expr DO statement  */
#line 236 "parser.y"
    { (yyval.pStatementNode) = new WhileStatementNode((yyvsp[-2].pExprNode), (yyvsp[0].pStatementNode), lin, col); }
#line 1510 "parser.cpp"
    break;

  case 42: /* statement: return_statement  */
#line 238 "parser.y"
    { (yyval.pStatementNode) = (yyvsp[0].pStatementNode); }
#line 1516 "parser.cpp"
    break;

  case 43: /* return_statement: RETURN_KEYWORD expr  */
#line 242 "parser.y"
    { (yyval.pStatementNode) = new ReturnStatementNode((yyvsp[0].pExprNode), lin, col); }
#line 1522 "parser.cpp"
    break;

  case 44: /* variable: id_node  */
#line 246 "parser.y"
    { (yyval.pVariableNode) = new VariableNode((yyvsp[0].pIdentNode), nullptr, lin, col); }
#line 1528 "parser.cpp"
    break;

  case 45: /* variable: id_node '[' expr ']'  */
#line 248 "parser.y"
    { (yyval.pVariableNode) = new VariableNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExprNode), lin, col); }
#line 1534 "parser.cpp"
    break;

  case 46: /* procedure_statement: id_node  */
#line 252 "parser.y"
    { (yyval.pProcedureCallStatementNode) = new ProcedureCallStatementNode((yyvsp[0].pIdentNode), new ExpressionList(lin,col), lin, col); }
#line 1540 "parser.cpp"
    break;

  case 47: /* procedure_statement: id_node '(' expression_list ')'  */
#line 254 "parser.y"
    { (yyval.pProcedureCallStatementNode) = new ProcedureCallStatementNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExpressionList), lin, col); }
#line 1546 "parser.cpp"
    break;

  case 48: /* expression_list: expr  */
#line 258 "parser.y"
    { (yyval.pExpressionList) = new ExpressionList(lin, col); (yyval.pExpressionList)->addExpression((yyvsp[0].pExprNode)); }
#line 1552 "parser.cpp"
    break;

  case 49: /* expression_list: expression_list ',' expr  */
#line 260 "parser.y"
    { (yyvsp[-2].pExpressionList)->addExpression((yyvsp[0].pExprNode)); (yyval.pExpressionList) = (yyvsp[-2].pExpressionList); }
#line 1558 "parser.cpp"
    break;

  case 50: /* expr: logical_or_expr  */
#line 264 "parser.y"
    { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1564 "parser.cpp"
    break;

  case 51: /* logical_or_expr: logical_and_expr  */
#line 268 "parser.y"
                 { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1570 "parser.cpp"
    break;

  case 52: /* logical_or_expr: logical_or_expr OR_OP logical_and_expr  */
#line 270 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "OR_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1576 "parser.cpp"
    break;

  case 53: /* logical_and_expr: not_expr  */
#line 274 "parser.y"
                  { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1582 "parser.cpp"
    break;

  case 54: /* logical_and_expr: logical_and_expr AND_OP not_expr  */
#line 276 "parser.y"
                  { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "AND_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1588 "parser.cpp"
    break;

  case 55: /* not_expr: relational_expr  */
#line 280 "parser.y"
          { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1594 "parser.cpp"
    break;

  case 56: /* not_expr: NOT_OP not_expr  */
#line 282 "parser.y"
          { (yyval.pExprNode) = new UnaryOpNode("NOT_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1600 "parser.cpp"
    break;

  case 57: /* relational_expr: additive_expr  */
#line 286 "parser.y"
                 { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1606 "parser.cpp"
    break;

  case 58: /* relational_expr: relational_expr EQ_OP additive_expr  */
#line 288 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "EQ_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1612 "parser.cpp"
    break;

  case 59: /* relational_expr: relational_expr NEQ_OP additive_expr  */
#line 290 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "NEQ_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1618 "parser.cpp"
    break;

  case 60: /* relational_expr: relational_expr LT_OP additive_expr  */
#line 292 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "LT_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1624 "parser.cpp"
    break;

  case 61: /* relational_expr: relational_expr LTE_OP additive_expr  */
#line 294 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "LTE_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1630 "parser.cpp"
    break;

  case 62: /* relational_expr: relational_expr GT_OP additive_expr  */
#line 296 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "GT_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1636 "parser.cpp"
    break;

  case 63: /* relational_expr: relational_expr GTE_OP additive_expr  */
#line 298 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "GTE_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1642 "parser.cpp"
    break;

  case 64: /* additive_expr: multiplicative_expr  */
#line 302 "parser.y"
               { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1648 "parser.cpp"
    break;

  case 65: /* additive_expr: additive_expr '+' multiplicative_expr  */
#line 304 "parser.y"
               { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "+", (yyvsp[0].pExprNode), lin, col); }
#line 1654 "parser.cpp"
    break;

  case 66: /* additive_expr: additive_expr '-' multiplicative_expr  */
#line 306 "parser.y"
               { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "-", (yyvsp[0].pExprNode), lin, col); }
#line 1660 "parser.cpp"
    break;

  case 67: /* multiplicative_expr: unary_expr  */
#line 310 "parser.y"
                     { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1666 "parser.cpp"
    break;

  case 68: /* multiplicative_expr: multiplicative_expr '*' unary_expr  */
#line 312 "parser.y"
                     { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "*", (yyvsp[0].pExprNode), lin, col); }
#line 1672 "parser.cpp"
    break;

  case 69: /* multiplicative_expr: multiplicative_expr '/' unary_expr  */
#line 314 "parser.y"
                     { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "/", (yyvsp[0].pExprNode), lin, col); }
#line 1678 "parser.cpp"
    break;

  case 70: /* multiplicative_expr: multiplicative_expr DIV_OP unary_expr  */
#line 316 "parser.y"
                     { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "DIV_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1684 "parser.cpp"
    break;

  case 71: /* unary_expr: primary  */
#line 320 "parser.y"
            { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1690 "parser.cpp"
    break;

  case 72: /* unary_expr: '-' primary  */
#line 322 "parser.y"
            { (yyval.pExprNode) = new UnaryOpNode("-", (yyvsp[0].pExprNode), lin, col); }
#line 1696 "parser.cpp"
    break;

  case 73: /* primary: id_node '[' expr ']'  */
#line 326 "parser.y"
           { (yyval.pExprNode) = new VariableNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExprNode), (yyvsp[-3].pIdentNode)->line, (yyvsp[-3].pIdentNode)->column); }
#line 1702 "parser.cpp"
    break;

  case 74: /* primary: id_node '(' expression_list ')'  */
#line 328 "parser.y"
           { (yyval.pExprNode) = new FunctionCallExprNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExpressionList), (yyvsp[-3].pIdentNode)->line, (yyvsp[-3].pIdentNode)->column); }
#line 1708 "parser.cpp"
    break;

  case 75: /* primary: id_node  */
#line 330 "parser.y"
           { (yyval.pExprNode) = new IdExprNode((yyvsp[0].pIdentNode), (yyvsp[0].pIdentNode)->line, (yyvsp[0].pIdentNode)->column); }
#line 1714 "parser.cpp"
    break;

  case 76: /* primary: int_num_node  */
#line 332 "parser.y"
           { (yyval.pExprNode) = (yyvsp[0].pIntNumNode); }
#line 1720 "parser.cpp"
    break;

  case 77: /* primary: real_num_node  */
#line 334 "parser.y"
           { (yyval.pExprNode) = (yyvsp[0].pRealNumNode); }
#line 1726 "parser.cpp"
    break;

  case 78: /* primary: TRUE_KEYWORD  */
#line 336 "parser.y"
           { (yyval.pExprNode) = new BooleanLiteralNode(true, lin, col); }
#line 1732 "parser.cpp"
    break;

  case 79: /* primary: FALSE_KEYWORD  */
#line 338 "parser.y"
           { (yyval.pExprNode) = new BooleanLiteralNode(false, lin, col); }
#line 1738 "parser.cpp"
    break;

  case 80: /* primary: '(' expr ')'  */
#line 340 "parser.y"
           { (yyval.pExprNode) = (yyvsp[-1].pExprNode); }
#line 1744 "parser.cpp"
    break;

  case 81: /* primary: STRING_LITERAL  */
#line 342 "parser.y"
         { (yyval.pExprNode) = new StringLiteralNode((yyvsp[0].str_val), lin, col); }
#line 1750 "parser.cpp"
    break;


#line 1754 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 345 "parser.y"


// Standardized yyerror function
int yyerror(const char* s) {
    compilation_has_error = true; 
    fprintf(stderr, "Syntax Error (L:%d, C:%d): %s\n", lin, col, s);
    return 0;
}

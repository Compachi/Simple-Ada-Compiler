/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PROJ2_TAB_H_INCLUDED
# define YY_YY_PROJ2_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     Identifier = 27,
     Constant = 28,
     Bool = 1,
     Int = 2,
     True = 3,
     False = 4,
     And = 5,
     Array = 6,
     Begin = 7,
     Declare = 8,
     Else = 9,
     Elsif = 10,
     End = 11,
     Exit = 12,
     For = 13,
     If = 14,
     In = 15,
     Is = 16,
     Loop = 17,
     Mod = 18,
     Not = 19,
     Of = 20,
     Or = 21,
     Procedure = 22,
     Then = 23,
     When = 24,
     While = 25,
     Xor = 26,
     Eq = 29,
     Ne = 30,
     Lt = 31,
     Gt = 32,
     Le = 33,
     Ge = 34,
     Plus = 35,
     Minus = 36,
     Star = 37,
     Divide = 38,
     LeftPar = 39,
     RightPar = 40,
     LeftBrac = 41,
     RightBrac = 42,
     Assign = 43,
     Range = 44,
     Semicolon = 45,
     Colon = 46,
     Comma = 47,
     UPlus = 48,
     UMinus = 49,
     NoType = 50
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 19 "proj2.y"
 tree p; int i; 

/* Line 2058 of yacc.c  */
#line 112 "proj2.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PROJ2_TAB_H_INCLUDED  */

/* Kyle Lessnau .y file for Project 2 */
%{
#include <stdlib.h>
#include "tree.h"
#include "y.tab.h"
extern tree root;
%}

%token <i> Identifier 27 Constant 28
%token <p> Bool 1 Int 2 True 3 False 4 And 5 Array 6 Begin 7 Declare 8 Else 9
%token <p> Elsif 10 End 11 Exit 12 For 13 If 14 In 15 Is 16 Loop 17 Mod 18 Not 19
%token <p> Of 20 Or 21 Procedure 22 Then 23 When 24 While 25 Xor 26 Eq 29 Ne 30
%token <p> Lt 31 Gt 32 Le 33 Ge 34 Plus 35 Minus 36 Star 37 Divide 38 LeftPar 39
%token <p> RightPar 40 LeftBrac 41 RightBrac 42 Assign 43 Range 44 Semicolon 45 Colon 46 Comma 47
%token <i> UPlus 48 UMinus 49 NoType 50

%start program

%union { tree p; int i; }

%type <p> program decls declaration id_list type const_range stmts statement range ref
%type <p> end_if expr relation sum prod factor basic
%type <i> sign 
%%
program
	: Procedure Identifier Is decls Begin stmts End Semicolon 
		{ root = buildTree (Procedure, buildIntTree(Identifier, $2), $4, $6); }
	;

decls
	:  /* empty */
		{ $$ = NULL; }
	| declaration Semicolon decls
		{ $$ = $1; $$->next = $3; }
	;

declaration 
	: id_list Colon type
		{ $$ = buildTree (Colon, $1, $3, NULL); }
	;

id_list
	: Identifier
		{ $$ = buildIntTree (Identifier, $1); } 
	| Identifier Comma id_list
		{ $$ = buildIntTree(Identifier, $1); $$->next = $3; }
	;

type
	: Int
		{ $$ = buildIntTree(Int, 1); } 
	| Bool 
		{ $$ = buildIntTree(Bool, 0); }
	| Array LeftBrac const_range RightBrac Of type
		{ $$ = buildTree(Array, $3, $6, NULL); }
	;

const_range
	: Constant Range Constant
		{ $$ = buildTree (Range, buildIntTree(Constant, $1), buildIntTree(Constant, $3), NULL); } 
	;

stmts
	: /* empty */
		{ $$ = NULL; }
	| statement Semicolon stmts
		{ $$ = $1; $$->next = $3; }
	;

statement 
	: ref Assign expr 
		{ $$ = buildTree (Assign, $1, $3, NULL); }
	| Declare decls Begin stmts End
		{ $$ = buildTree(Declare, $2, $4, NULL); }
	| For Identifier In range Loop stmts End Loop
		{ $$ = buildTree (For, buildIntTree(Identifier, $2), $4, $6); }
	| Exit 
		{ $$ = buildIntTree(Exit, 0); }
	| Exit When expr
		{ $$ = buildTree (Exit, $3, NULL, NULL); }
	| If expr Then stmts end_if
		{ $$ = buildTree(If, $2, $4, $5); }
	;

range
	: sum Range sum
		{ $$ = buildTree (Range, $1, $3, NULL); }
	;
 
ref
	: Identifier 
		{ $$ = buildIntTree (Identifier, $1); }
	| Identifier LeftBrac expr RightBrac
		{ $$ = buildTree(LeftBrac, buildIntTree(Identifier, $1), $3, NULL); }
	;

end_if
	: End If 
		{ $$ = NULL; }
	| Else stmts End If
		{ $$ = buildTree(Else, $2, NULL, NULL); } 
	| Elsif expr Then stmts end_if
		{ $$ = buildTree (Elsif, $2, $4, $5); }
	;

expr
	: relation Or relation
		{ $$ = buildTree (Or, $1, $3, NULL); } 
	| relation And relation
		{ $$ = buildTree (And, $1, $3, NULL); }
	| relation Xor relation 
		{ $$ = buildTree (Xor, $1, $3, NULL); }
	| relation 
		{ $$ = $1; }
	;

relation
	: sum 
		{ $$ = $1; }
	| sum Eq sum
		{ $$ = buildTree (Eq, $1, $3, NULL); } 
	| sum Ne sum
		{ $$ = buildTree (Ne, $1, $3, NULL); }
	| sum Lt sum 
		{ $$ = buildTree (Lt, $1, $3, NULL); }
	| sum Le sum
		{ $$ = buildTree (Le, $1, $3, NULL); }
	| sum Gt sum 
		{ $$ = buildTree (Gt, $1, $3, NULL); }
	| sum Ge sum
		{ $$ = buildTree (Ge, $1, $3, NULL); }
	;

sum
	: sign prod 
		{ if($1 == 0)
		  	$$ = $2;
		  else
			$$ = buildTree($1, $2, NULL, NULL); }
	| sum Plus prod 
		{ $$ = buildTree (Plus, $1, $3, NULL); }
	| sum Minus prod
		{ $$ = buildTree (Minus, $1, $3, NULL); }
	;

sign
	: Plus 
		{ $$ = UPlus; }
	| Minus 
		{ $$ = UMinus; }
	| /*   Default for Unary +/-  */
		{ $$ = 0; }
	;

prod
	: factor 
		{ $$ = $1; }
	| prod Star factor 
		{ $$ = buildTree (Star, $1, $3, NULL); }
	| prod Divide factor
		{ $$ = buildTree (Divide, $1, $3, NULL); }
	| prod Mod factor
		{ $$ = buildTree (Mod, $1, $3, NULL); }
	;

factor
	: Not basic 
		{ $$ = buildTree (Not, $2, NULL, NULL); }
	| basic
		{ $$ = $1; }
	;

basic
	: ref
		{ $$ = $1; } 
	| LeftPar expr RightPar
		{ $$ = $2; }
	| Constant
		{ $$ = buildIntTree (Constant, $1); } 
	| True 
		{ $$ = buildIntTree(True, 1); }
	| False
		{ $$ = buildIntTree (False, 0); }
	;
%%

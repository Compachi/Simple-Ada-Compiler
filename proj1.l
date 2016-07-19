%{
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "y.tab.h"
int yywrap(void);
void yyerror(char *s);
void printNames(void);
char *id_name(int);
int lookup(char []);
int line_no = 1, pos = 1;
%}

D	[0-9]
L	[a-zA-Z]

%%
[\t]                pos += yyleng;	
[\n]                pos += yyleng;
[ ]                 pos += yyleng;
BOOLEAN	            {pos += 7; return Bool;}
INTEGER             {pos += 7; return Int;}
TRUE	            {pos += 4; return True;}
FALSE	            {pos += 5; return False;}
and	            {pos += 3; return And;}
array	            {pos += 5; return Array;}
begin               {pos += 5; return Begin;}
declare             {pos += 7; return Declare;}
else                {pos += 4; return Else;}
elsif 	            {pos += 5; return Elsif;}
end 	            {pos += 3; return End;}
exit 	            {pos += 4; return Exit;}
for	            {pos += 3; return For;}
if 	            {pos += 2; return If;}
in	            {pos += 2; return In;}
is	            {pos += 2; return Is;}
loop	            {pos += 4; return Loop;}
mod	            {pos += 3; return Mod;}
not	            {pos += 3; return Not;}
of	            {pos += 2; return Of;}
or	            {pos += 2; return Or;}
procedure           {pos += 9; return Procedure;}
then	            {pos += 4; return Then;}
when	            {pos += 4; return When;}
while	            {pos += 5; return While;}
xor 	            {pos += 3; return Xor;}
{L}({D}|{L}|"_")*   {pos += yyleng; yylval.i = lookup(yytext); return Identifier;}
{D}+		    {pos += yyleng; yylval.i = atoi(yytext); return Constant;}
"="	            {pos += 1; return Eq;}
"/=" 	            {pos += 2; return Ne;}
"<"	            {pos += 1; return Lt;}
">"	            {pos += 1; return Gt;}
"<="	            {pos += 2; return Le;}
">="	            {pos += 2; return Ge;}
"+"	            {pos += 1; return Plus;}
"-"	            {pos += 1; return Minus;}
"*"	            {pos += 1; return Star;}
"/"	            {pos += 1; return Divide;}
"("	            {pos += 1; return LeftPar;}
")"	            {pos += 1; return RightPar;}
"["	            {pos += 1; return LeftBrac;}
"]"	            {pos += 1; return RightBrac;}
":="	            {pos += 2; return Assign;}
".."	            {pos += 2; return Range;}
";"	            {pos += 2; return Semicolon;}
":"	            {pos += 1; return Colon;}
","	            {pos += 1; return Comma;}
.	            {pos += 1; yyerror ("You have entered an invalid character");}
%%

void yyerror(char s[]) {
    printf("Error: %s at token %s.\n", s, yytext);
}

int yywrap(void) {
    return 1;
}

typedef char name[15];
static char Names[200][15] = {"<no name>"};
int top = 0;

int lookup (char s[]) {
        int     i;

        for (i = 1; i <= top; i ++)
                if (strcmp (s, Names[i]) == 0) return i;
        strcpy (Names[++top], s);
        return top;
}

void printNames (void) {
        int i = 0;

        for (; i <= top; i++)
                printf ("%d\t%s\n", i, Names[i]);
}

char *id_name (int i) {
        return Names[i];
}


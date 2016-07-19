#include <stdio.h>
#include "tree.h"
#include "y.tab.h"
#include "instr.h"
#include "code.h"
#include "st.h"

void init_decls(tree);
STentry DT[100] = {0, 0, 0, 0, 0, NoType};
STentry Dstack[100] = {0, 0, 0, 0, 0, NoType};
static struct FR exit_list[10];
static int num_exits = 0;
static int DC = 0;
extern int top;
static int dScope = 0;

void check_exits() {
	int i;
	for (i = 0; i < num_exits; i++) 
		fixFR(exit_list[i], LC);
	
	num_exits = 0;
}

static void gen_expr (tree t)
{
	int	n;
	if (t == NULL) fprintf (stderr, "Internal error 11\n");
	switch (n = t->kind) {
		case Eq: case Ne: case Lt: case Le: case Gt: case Ge:
			gen_expr (t->first); gen_expr (t->second);
			
			if (n == Ge || n == Le) 
				code (SWAPW);
		
			code (SUBI);
			code ( (n == Eq || n == Ne) ? TSTEQI : TSTLTI);
	
			if (n == Ne || n == Le || n == Ge) 
				code (NOTW);
			break;
		case Plus: case Minus: case Star: case Divide:
			gen_expr (t->first); 
			if(t->first->kind == LeftBrac)
				code(GETSW);
			gen_expr (t->second);
			code (n == Plus ? ADDI : n == Minus ? SUBI : n == Star ? MULI : Divide);
			break;
		case Mod:
			/* do nothing */
		break;
		case Identifier: 
			code1 (PUSHW, DT[t->value].addr); code (GETSW);
			break;
		case Constant:
			code1 (PUSHW, t->value);
			break;
		case True:
			code1 (PUSHW, -1);
			break;
		case False:
			code1 (PUSHW, 0);
			break;
		case LeftBrac:
			code1 (PUSHW, DT[t->first->value].addr);
			gen_expr(t->second); 
			code(DUPW); code1 (PUSHW, DT[t->first->value].lBound);
			code (SUBI); code (TSTLTI); code1 (RGOZ, 1); code (HALT); code (DUPW);
			code1(PUSHW, DT[t->first->value].uBound);
			code (SWAPW); code (SUBI); code (TSTLTI); code1 (RGOZ, 1); code (HALT);
			code1 (PUSHW, 2); code (MULI); code (ADDI); 
			break;
		case Or: case And: case Xor: case Not:
			if(t->first != NULL)
				gen_expr(t->first);
			gen_expr(t->second);
			if(n == Or)
				code(ORW);
			if(n == And)
				code(ANDW);
			if(n == Xor)
				code(XORW);
			if(n  == Not)
				code(NOTW);
			break;
		default:
			fprintf (stderr, "Internal error 12\n");
	}
}
static void generate (tree t)
{
	for (t; t != NULL; t = t->next) {
		switch (t->kind) 
		{
			case Int: case Bool: case Procedure: case End:
				/* Do nothing */ 
			break;
			case Declare:
			{ 
				dScope++;
				int curDC = DC;
				init_decls(t->first);
				pr_directive(".CODE");
				generate(t->second);
				check_exits();
				DC = curDC;
				popStack();
				dScope--;
			}
			break;
			case Assign :
				prLC();
				if(t->first->kind == Identifier)
				{
					code1(PUSHW, DT[t->first->value].addr);
					gen_expr(t->second);
					code(PUTSW); prNL();
				}
				else
				{
					gen_expr(t->first);
					gen_expr(t->second);
					code(PUTSW);
				}
			break;
			case Exit: 
			{
			 
				struct FR fix;
				prLC();
				if (t->first != NULL) 
				{
					gen_expr(t->first); 
					fix = codeFR (RGOZ); prNL(); 
					exit_list[num_exits] = fix; 
					num_exits++;
				}
				else 
				{
					fix = codeFR(RGOZ);
					exit_list[num_exits] = fix;
					num_exits++;
				}
			}
			break;
			case If : case Elsif :
			{
				fprintf(outfile, "/* Beginning of IF */"); prNL();
				struct FR	fix1, fix2;
				prLC ();
				gen_expr (t->first);
				fix1 = codeFR (RGOZ);
				prNL ();
				
				fprintf(outfile, "/* THEN */"); prNL();
				generate (t->second);
				if (t->third != NULL) 
				{
					prLC();
					fix2 = codeFR (RGOTO);
					fixFR (fix1, LC);
					prNL();
					generate (t->third);
					fixFR (fix2, LC);
				}
				else 
				{ 
					fixFR (fix1, LC);
					prNL();
				}
				fprintf(outfile, "/* End of IF */"); prNL();	
			}
			break;
			case Else:
				fprintf(outfile,"/* ELSE */"); prNL();
				generate(t->first);
				fprintf(outfile, "/* End of IF"); prNL();	
				break;
		
			case For: {
			
				struct FR fix1;
				dScope++;
				int position = t->first->value;
				DT[position].index = position;
				DT[position].addr = DC;
				DC += 2;
				DT[position].scope = dScope;
				fprintf(outfile, "/* Beginning of FOR */"); prNL();
				prLC();
				code1 (PUSHW, DT[t->first->value].addr);
				gen_expr(t->second->first);
				code (PUTSW);
				prNL();
				pr_directive(".DATA");
				fprintf(outfile, "%d: /*%s*/", DT[t->first->value].addr, id_name(t->first->value));
				prNL();
				pr_directive(".CODE");
				int top = LC;
				prLC();
				gen_expr(t->second->second); code (DUPW);
				prNL(); prLC();
				code1 (PUSHW, DT[t->first->value].addr); code (GETSW); code (SUBI); code (TSTEQI);
				fix1 = codeFR(RGOZ);
				prNL();
 				
				fprintf(outfile, "/* Body of FOR */"); prNL();
				generate(t->third);
				prNL(); 
				fprintf(outfile, "/* INCREMENT */"); prNL(); 
				prLC();
				code1 (PUSHW, DT[t->first->value].addr); 
				code (DUPW); code (GETSW); code1 (PUSHW, 1);
				code (ADDI); code (PUTSW);
				prNL();
		
				prLC();
				code (DUPW);
				code1  (RGOTO, LC - top - 1);
				fixFR (fix1, LC);
				prNL();
				check_exits();
				prLC();
				code1 (PUSHW, -1); code(CHSPS);
				prNL();
				fprintf(outfile, "/* End of FOR */"); prNL();
				DC -= 2;
				popStack();
				dScope--;
				break;
			}
		}
	}
}
void gen_program (tree t)
{
	
	pr_directive(".DATA");
	init_decls(t->second);
	pr_directive(".CODE");
	generate(t->third);
	prLC (); code (HALT); pr_directive(".ENTRY 0"); prNL ();
	prNL();
}

void init_decls (tree t)
{
	prNL();
        for(t; t != NULL; t = t->next)
        {
                int type = t->second->kind;
                tree p;
                for(p = t->first; p != NULL; p = p->next)
                {
                        int position = p->value;
                        if(DT[position].index != 0)
                        {
                                Dstack[top].index = DT[position].index;
                                Dstack[top].type = DT[position].type;
                                Dstack[top].scope = DT[position].scope;
                                Dstack[top].uBound = DT[position].uBound;
                                Dstack[top].lBound = DT[position].lBound;
                                top++;
                        }
                        DT[position].index = position;
                        DT[position].scope = dScope;
			DT[position].addr = DC;
                        if(type == Array)
                        {
                                DT[position].lBound = t->second->first->first->value;
                                DT[position].uBound = t->second->first->second->value;
				fprintf(outfile, "%d: /*%s*/ ", DC, id_name(DT[position].index));
				DC += 2 * (DT[position].uBound - DT[position].lBound) + 2;
                        }
			else
			{
				fprintf(outfile, "%d: /*%s*/ ", DC, id_name(DT[position].index));
				DC += 2;
			}
                }
        }
	prNL();
}

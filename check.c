#include <stdio.h>
#include "tree.h"
#include "y.tab.h"
#include "st.h"

extern int top;
STentry ST[100] = {0, 0, 0, 0, 0,  NoType};
int scope = 0;
STentry Stack[100] = {0, 0, 0, 0, 0, NoType};
int tos = 0;

void prST(void)
{
	int i;
	printf("Symbol Table\n");
	printf("------------\n");
	for(i = 0; i <= top; i++)
	{
		int t = ST[i].type;
		int s = ST[i].scope;
		if(ST[i].type == Int)
			printf("%3d      %-10s\t%s\t      %d\n", i, id_name(i), "Integer", s);
		else if(ST[i].type == Bool)
			printf("%3d      %-10s\t%s\t      %d\n", i, id_name(i), "Boolean", s);
		else if(ST[i].type == Array)
			printf("%3d      %-10s\t%s\t      %d  %d..%d\n" , i, id_name(i), "Array", s, ST[i].lBound, ST[i].uBound);
		else
			printf("%3d      %-10s\t%s\t      %d\n", i, id_name(i), TokName[t], s);
	}
	printf("\n");
}

static int check_expr(tree t)
{
	//Checks for a missing expression
	if(t == NULL)
	{
		fprintf(stderr, "You shouldn't be here; missing expression\n");
		return NoType;
	}
	switch(t->kind)
	{
		case Eq: case Ne: case Lt: case Le: case Gt: case Ge:
		{
			int toL = check_expr(t->first);
			int toR = check_expr(t->second);
			if(toL != toR)
			{
				fprintf(stderr, "Type mismatch in comparison\n");
				return (t->value = NoType); 
			}
			else
				return (t->value = Bool);
		}
		break;
		case Plus: case Range: case Minus: case Star: case Divide: case Mod: case UPlus: case UMinus:
		{
			int toL = check_expr(t->first);
			int toR = check_expr(t->second);
			if(toL != Int || toR!= Int)
			{
				fprintf(stderr, "Type mismatch binary arithmetic exp. \n");
				return (t->value = NoType);
			}
			else
				return (t->value = Int);
		}
		break;
		case LeftBrac:
		{
			int toL = check_expr(t->first);
			if(toL != Array)
			{
				fprintf(stderr, "Error in LeftBrac.\n");
				return(t->value = NoType);
			}
			else	
			{
				return(t->value = Int);
			}
		}
		break;
		case Or: case And: case Xor: case Not:
		{
			int toL = check_expr(t->first);
                        int toR = check_expr(t->second);
                        if(toL != Bool || toR!= Bool)
                        {
                                fprintf(stderr, "Type mismatch realtion exp. \n");
                                return (t->value = NoType);
                        }
                        else
				return Bool;
		}
		break;
		case True: case False:
		{
			return Bool;	
		}
		break;
		case Identifier:
		{
			int pos = t->value;
			if(ST[pos].index == 0)
			{
				ST[pos].index = pos;
				ST[pos].type = Int;
				ST[pos].scope = scope;
			}
			return ST[pos].type;
		}
		break;
		case Constant:
			return Int;
		
		default:
			fprintf(stderr, "You shouldn't be here!\n");
	}
}		
static void handle_decls (tree t)
{
	for(t; t != NULL; t = t->next)
	{
		int type = t->second->kind;
		tree p;
		if(type != Int && type != Bool && type != Array)
		{
			fprintf(stderr, "\nBad type in decl %s\n", TokName[t->kind]);
			return;
		}
		for(p = t->first; p != NULL; p = p->next)
		{
			int pos = p->value;
			if(ST[pos].index != 0)
			{
				Stack[tos].index = ST[pos].index;
				Stack[tos].type = ST[pos].type;
				Stack[tos].scope = ST[pos].scope;
				Stack[tos].uBound = ST[pos].uBound;
				Stack[tos].lBound = ST[pos].lBound;
				tos++;
			}
			ST[pos].index = pos;
			ST[pos].type = type;
			ST[pos].scope = scope;
			if(type == Array)
			{
				ST[pos].lBound = t->second->first->first->value;
				ST[pos].uBound = t->second->first->second->value;
			}
		}
	}
}
void check(tree t)
{
	for(t; t!= NULL; t = t->next)
	{
		switch(t->kind)
		{
			case Procedure:
			{
				scope++;
				handle_decls(t->second);
				check(t->third);
				printf("At scope %d\n", scope);
				prST();
			}
			break;

			case Assign:
			{
				int toL = check_expr(t->first);
				int toR = check_expr(t->second);
				if(toL != toR)
					fprintf(stderr, "Error in Assign.");
				else
				{
					int pos = t->first->value;
						
					if(ST[pos].index == 0)
					{
						ST[pos].index = pos;
						ST[pos].type = check_expr(t->first);
						ST[pos].scope = scope;
					}
				}
			}
			break;
			case If: case Elsif: 
			{
				if(check_expr(t->first) != Bool)
					fprintf(stderr, "Non-boolean in condition to IF/ELSIF.\n");
				check(t->second);
				check(t->third);
			}
			break;

			case Else:
			{
				check(t->first);
			}
			break;

			case For:
			{
				int t_start = check_expr(t->second->first);
				int t_end = check_expr(t->second->second);
				int pos = t->first->value;
				if(t_start != Int || t_end != Int || t_start != t_end)
					fprintf(stderr, "Error in condition FOR\n");
				else
				{
					scope++;
					if(ST[pos].index != 0)
					{
						Stack[tos].index = ST[pos].index;
						Stack[tos].type = ST[pos].type;
						Stack[tos].scope = ST[pos].scope;
						Stack[tos].uBound = ST[pos].uBound;
						Stack[tos].lBound = ST[pos].lBound;
						tos++;
					}
						ST[pos].index = pos;
						ST[pos].scope = scope;
						ST[pos].type = t_start;
				}
					check(t->third);
					printf("For at scope %d\n", scope);
					prST();
					printStack();
					popStack();
					scope--;
			}
			break;

			case Declare:
			{
				scope++;
				printf("Declare\n");
				handle_decls(t->first);
				check(t->second);
				printf("Declare at scope %d\n", scope);
				prST();
				printStack();
				popStack();
				scope--;	
			}
			break;
			case Exit:
			{
				if(t->first != 0) 
					if(check_expr(t->first) != Bool)
						fprintf(stderr, "Error in DECLARE.\n");
			}
			break;
			default:
				fprintf(stderr, "You shouldn't be here!!!!!!! \t%s\n", TokName[t->kind]);
		}
	}
}
//Function to print stack
void printStack(void)
{
 	int i;
        printf("Stack       \n");
	printf("------------\n");
        for(i = 0; i < tos; i++)
        {
                int t = Stack[i].type;
                int s = Stack[i].scope;
                if(Stack[i].type == Int)
                        printf("%3d %-10s\t%s\t %d\n", i, id_name(Stack[i].index), "Integer", s);
                else if(Stack[i].type == Bool)
                        printf("%3d %-10s\t%s\t %d\n", i, id_name(Stack[i].index), "Boolean", s);
                else if(Stack[i].type == Array)
                        printf("%3d %-10s\t%s\t %d  %d..%d\n" , i, id_name(Stack[i].index), "Array", s, Stack[i].lBound, Stack[i].uBound);
                else
                        printf("%3d %-10s\t%s\t %d\n", i, id_name(Stack[i].index), TokName[t], s);
        }
}
//Function to pop stack
void popStack(void)
{
int i, j;
for(i = top; i >= 0; i--){
                for(j = tos; j >= 0; j--){
                        if(ST[i].index == Stack[j].index && scope == ST[i].scope){
                                ST[i].index = Stack[j].index;
                                ST[i].type = Stack[j].type;
                                ST[i].scope = Stack[j].scope;
                                ST[i].lBound = Stack[j].lBound;
                                ST[i].uBound = Stack[j].uBound;
                                Stack[j].index = 0;
                                Stack[j].type = 0;
                                Stack[j].scope = 0;
                                Stack[j].lBound = 0;
                                Stack[j].uBound = 0;
                                tos--;
                        }
                }
        }
}


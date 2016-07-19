//Driver for file for project 2
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

extern FILE *yyin;
extern int yydebug;
tree root;

FILE *outfile;

main(int argc, char *argv[])
{
	//Checks for argument on command line 
	if(argc != 3)
	{
		fprintf(stderr, "%s: Insufficient Arguments\n", argv[0]);
		exit(1);
	}
	//Checks for valid file
	if((yyin = fopen(argv[1], "r")) == 0L) 
	{
		fprintf(stderr, "%s: Can't open Input File %s\n", argv[0], argv[1]);
		exit(1);
	}
	//Builds parse tree
	yyparse();
	//Prints tree
	printTree(root);
	check(root);
	if((outfile = fopen(argv[2], "w")) == 0L)
	{
		fprintf(stderr, "%s: Can't open Outfile %s\n", argv[0], argv[2]);
		exit(1);
	}
	gen_program(root);
	close(outfile);
	close(yyin);
}			

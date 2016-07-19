#Kyle Lessnau
#Makefile for project 4. 
all: ada clean
CC = gcc -g
FLEX = flex
BIS = bison -d -y

ada: y.tab.o lex.yy.o tree.o check.o code.o generate.o instr.o driver.o
	$(CC) -o ada y.tab.o lex.yy.o tree.o check.o code.o generate.o instr.o driver.o

instr.o: instr.c instr.h
	$(CC) -c instr.c

code.o: code.c tree.h y.tab.h instr.h code.h
	$(CC) -c code.c

generate.o: generate.c tree.h y.tab.h code.h instr.h
	$(CC) -c generate.c
 
check.o: check.c st.h tree.h y.tab.h
	$(CC) -c check.c

driver.o: driver.c tree.h
	$(CC) -c driver.c

tree.o: tree.c tree.h
	$(CC) -c tree.c

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

lex.yy.c: proj1.l tree.h y.tab.h
	$(FLEX) proj1.l

y.tab.o: y.tab.c
	$(CC) -c y.tab.c

y.tab.c: proj2.y tree.h
	$(BIS) proj2.y

clean:
	rm -f *.o
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h
	chmod 505 ada

CC = g++ -O2 -Wno-deprecated -Wall -Iheader_files

header_tag = -Iheader_files

SRC_DIR = src
SRCS = Record.cc Comparison.cc ComparisonEngine.cc Schema.cc File.cc DBFile.cc BigQ.cc Pipe.cc test.cc main.cc 
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

tag = -i.bak
tag2 = -ll -pthread
OStag = rest

ifdef linux
OStag = linux
tag = -n
tag2 = -lfl -lpthread
endif

test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o DBFile.o Pipe.o y.tab.o lex.yy.o test.o
	$(CC) -o test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o DBFile.o Pipe.o y.tab.o lex.yy.o test.o $(tag2)
	
main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o
	$(CC) -o main Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o $(tag2)
	
%.o: $(SRC_DIR)/%.cc
	$(CC) -g -c $< -o $@

	
ifdef linux
y.tab.o: Parser.y
	yacc -d Parser.y
	sed $(tag) y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c $(header_tag) y.tab.c
else 
y.tab.o: Parser.y
	yacc -d Parser.y
	sed $(tag) -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" y.tab.c
	g++ -c $(header_tag) y.tab.c
endif

lex.yy.o: Lexer.l
	lex  Lexer.l
	gcc -c $(header_tag) lex.yy.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.c
	rm -f y.tab.c.bak
	rm -f lex.yy.c
	rm -f y.tab.h

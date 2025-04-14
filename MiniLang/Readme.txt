flex lexer.l
bison -d parser.y
gcc -c lex.yy.c parser.tab.c symtab.c ast.c
gcc lex.yy.o parser.tab.o symtab.o ast.o -o minilang -lm
./minilang
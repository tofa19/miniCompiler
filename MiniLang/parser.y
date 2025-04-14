%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "symtab.h"
#include "ast.h"

void yyerror(const char *s);
int yylex(void);
extern FILE* yyin;

/* Root of the AST */
ASTNode *root = NULL;
ASTNode *current_program = NULL;
int interactive_mode = 0;  /* This variable should be visible to other files */
%}

%union {
    double fval;
    char *sval;
    ASTNode *node;
}

%token <fval> NUMBER
%token <sval> IDENTIFIER STRING
%token ASSIGN SEMICOLON PRINT IF ELSE WHILE FOR
%token PLUS MINUS MULTIPLY DIVIDE
%token EQUALS NEQUALS GREATER LESS GREATEREQ LESSEQ
%token MIN MAX COMMA

%type <node> program statement_list statement block assignment_statement
%type <node> print_statement if_statement while_statement for_statement
%type <node> expression minmax_function

%start program

%%

program:
    statement_list {
        root = $1;
    }
    | /* empty */ {
        root = create_block_node(); /* Create an empty program */
    }
    ;

statement_list:
    statement {
        $$ = create_block_node();
        add_statement_to_block($$, $1);
    }
    | statement_list statement {
        $$ = $1;
        add_statement_to_block($$, $2);
    }
    ;

statement:
    assignment_statement { $$ = $1; }
    | print_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | block { $$ = $1; }
    ;

block:
    '{' statement_list '}' { $$ = $2; }
    ;

assignment_statement:
    IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = create_assignment_node($1, $3);
        free($1);
    }
    ;

print_statement:
    PRINT expression SEMICOLON {
        $$ = create_print_node($2, NULL);
    }
    | PRINT STRING SEMICOLON {
        $$ = create_print_node(NULL, $2);
	free($2);
    }
    ;

if_statement:
    IF '(' expression ')' block {
        $$ = create_if_node($3, $5, NULL);
    }
    | IF '(' expression ')' block ELSE block {
        $$ = create_if_node($3, $5, $7);
    }
    ;

while_statement:
    WHILE '(' expression ')' block {
        $$ = create_while_node($3, $5);
    }
    ;

for_statement:
    FOR '(' assignment_statement expression SEMICOLON IDENTIFIER ASSIGN expression ')' block {
        ASTNode *update = create_assignment_node(strdup($6), 
                          create_binary_op_node(OP_ADD, 
                              create_variable_node($6), 
                              $8));
        $$ = create_for_node($3, $4, update, $10);
        free($6);
    }
    ;

expression:
    NUMBER {
        $$ = create_number_node($1);
    }
    | IDENTIFIER {
        $$ = create_variable_node($1);
        free($1);
    }
    | expression PLUS expression {
        $$ = create_binary_op_node(OP_ADD, $1, $3);
    }
    | expression MINUS expression {
        $$ = create_binary_op_node(OP_SUBTRACT, $1, $3);
    }
    | expression MULTIPLY expression {
        $$ = create_binary_op_node(OP_MULTIPLY, $1, $3);
    }
    | expression DIVIDE expression {
        $$ = create_binary_op_node(OP_DIVIDE, $1, $3);
    }
    | expression EQUALS expression {
        $$ = create_binary_op_node(OP_EQUALS, $1, $3);
    }
    | expression NEQUALS expression {
        $$ = create_binary_op_node(OP_NEQUALS, $1, $3);
    }
    | expression GREATER expression {
        $$ = create_binary_op_node(OP_GREATER, $1, $3);
    }
    | expression LESS expression {
        $$ = create_binary_op_node(OP_LESS, $1, $3);
    }
    | expression GREATEREQ expression {
        $$ = create_binary_op_node(OP_GREATEREQ, $1, $3);
    }
    | expression LESSEQ expression {
        $$ = create_binary_op_node(OP_LESSEQ, $1, $3);
    }
    | '(' expression ')' {
        $$ = $2;
    }
    | minmax_function {
        $$ = $1;
    }
    ;

minmax_function:
    MIN '(' expression COMMA expression ')' {
        $$ = create_minmax_node(FUNC_MIN, $3, $5);
    }
    | MAX '(' expression COMMA expression ')' {
        $$ = create_minmax_node(FUNC_MAX, $3, $5);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char **argv) {
    printf("..............................Welcome to MiniLang!..............................\n");
    
    int interactive_mode = 0;
    
    /* If a file is specified, read from it */
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    } else {
        /* Otherwise read from standard input */
        printf("Enter your code below (Press Ctrl+D when finished):\n");
        yyin = stdin;
        interactive_mode = 1;
    }
    
    /* Initialize the symbol table */
    init_symtab();
    
    /* Parse the input */
    int parse_result = yyparse();
    
    /* Execute the AST if parsing was successful */
    if (root != NULL) {
        printf("\nExecuting program...\n");
        fflush(stdout); /* Ensure the message is displayed immediately */
        
        interpret_ast(root);
        free_ast(root);
        
        /* In interactive mode, flush all outputs */
        if (interactive_mode) {
            fflush(stdout);
        }
    } else {
        printf("No valid program found or parsing failed.\n");
    }
    
    /* Clean up */
    if (yyin != stdin) {
        fclose(yyin);
    }
    free_symtab();
    return 0;
}
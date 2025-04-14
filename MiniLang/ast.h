#ifndef AST_H
#define AST_H

#include <stdlib.h>

/* Enumeration of possible AST node types */
typedef enum {
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_STRING,
    NODE_BINARY_OP,
    NODE_ASSIGNMENT,
    NODE_PRINT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_BLOCK,
    NODE_MINMAX
} ASTNodeType;

/* Enumeration of possible binary operations */
typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_EQUALS,
    OP_NEQUALS,
    OP_GREATER,
    OP_LESS,
    OP_GREATEREQ,
    OP_LESSEQ
} BinaryOpType;

/* Enumeration of possible min/max functions */
typedef enum {
    FUNC_MIN,
    FUNC_MAX
} MinMaxFuncType;

/* Forward declaration of AST node structure */
typedef struct ASTNode ASTNode;

/* Structure for a block of statements */
typedef struct {
    ASTNode **statements;
    int statement_count;
    int capacity;
} BlockNode;

/* Structure for binary operations */
typedef struct {
    BinaryOpType op;
    ASTNode *left;
    ASTNode *right;
} BinaryOpNode;

/* Structure for assignment operations */
typedef struct {
    char *var_name;
    ASTNode *value;
} AssignmentNode;

/* Structure for print statements */
typedef struct {
    ASTNode *expression;
    char *string_literal;
    char *var_name;  /* Name of the variable being printed (if applicable) */
} PrintNode;

/* Structure for if statements */
typedef struct {
    ASTNode *condition;
    ASTNode *if_branch;
    ASTNode *else_branch;  /* Can be NULL if no else branch */
} IfNode;

/* Structure for while loops */
typedef struct {
    ASTNode *condition;
    ASTNode *body;
} WhileNode;

/* Structure for for loops */
typedef struct {
    ASTNode *init;
    ASTNode *condition;
    ASTNode *update;
    ASTNode *body;
} ForNode;

/* Structure for min/max functions */
typedef struct {
    MinMaxFuncType func_type;
    ASTNode *arg1;
    ASTNode *arg2;
} MinMaxNode;

/* The main AST node structure */
struct ASTNode {
    ASTNodeType type;
    union {
        double number_value;       /* For NODE_NUMBER */
        char *variable_name;       /* For NODE_VARIABLE */
        char *string_value;        /* For NODE_STRING */
        BinaryOpNode binary_op;    /* For NODE_BINARY_OP */
        AssignmentNode assignment; /* For NODE_ASSIGNMENT */
        PrintNode print;           /* For NODE_PRINT */
        IfNode if_stmt;            /* For NODE_IF */
        WhileNode while_loop;      /* For NODE_WHILE */
        ForNode for_loop;          /* For NODE_FOR */
        BlockNode block;           /* For NODE_BLOCK */
        MinMaxNode minmax;         /* For NODE_MINMAX */
    } data;
};

/* Function to create a number node */
ASTNode* create_number_node(double value);

/* Function to create a variable node */
ASTNode* create_variable_node(char *name);

/* Function to create a string node */
ASTNode* create_string_node(char *value);

/* Function to create a binary operation node */
ASTNode* create_binary_op_node(BinaryOpType op, ASTNode *left, ASTNode *right);

/* Function to create an assignment node */
ASTNode* create_assignment_node(char *var_name, ASTNode *value);

/* Function to create a print node */
ASTNode* create_print_node(ASTNode *expr, char *string);

/* Function to create an if node */
ASTNode* create_if_node(ASTNode *condition, ASTNode *if_branch, ASTNode *else_branch);

/* Function to create a while node */
ASTNode* create_while_node(ASTNode *condition, ASTNode *body);

/* Function to create a for node */
ASTNode* create_for_node(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body);

/* Function to create a block node */
ASTNode* create_block_node();

/* Function to add a statement to a block */
void add_statement_to_block(ASTNode *block, ASTNode *statement);

/* Function to create a min/max function node */
ASTNode* create_minmax_node(MinMaxFuncType func_type, ASTNode *arg1, ASTNode *arg2);

/* Function to free an AST node and all its children */
void free_ast(ASTNode *node);

/* Execute an AST */
double interpret_ast(ASTNode *node);

/* Function to interpret a binary operation node */
double interpret_binary_op(BinaryOpNode *op_node);

#endif /* AST_H */
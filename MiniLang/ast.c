#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtab.h"

/* Create a number node */
ASTNode* create_number_node(double value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_NUMBER;
    node->data.number_value = value;
    return node;
}

/* Create a variable node */
ASTNode* create_variable_node(char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_VARIABLE;
    node->data.variable_name = strdup(name);
    if (node->data.variable_name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(node);
        exit(1);
    }
    return node;
}

/* Create a string node */
ASTNode* create_string_node(char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_STRING;
    node->data.string_value = strdup(value);
    if (node->data.string_value == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(node);
        exit(1);
    }
    return node;
}

/* Create a binary operation node */
ASTNode* create_binary_op_node(BinaryOpType op, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_BINARY_OP;
    node->data.binary_op.op = op;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

/* Create an assignment node */
ASTNode* create_assignment_node(char *var_name, ASTNode *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_ASSIGNMENT;
    node->data.assignment.var_name = strdup(var_name);
    if (node->data.assignment.var_name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(node);
        exit(1);
    }
    node->data.assignment.value = value;
    return node;
}

/* Create a print node */
ASTNode* create_print_node(ASTNode *expr, char *string) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_PRINT;
    node->data.print.expression = expr;
    node->data.print.string_literal = string ? strdup(string) : NULL;
    return node;
}

/* Create an if node */
ASTNode* create_if_node(ASTNode *condition, ASTNode *if_branch, ASTNode *else_branch) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_IF;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.if_branch = if_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

/* Create a while node */
ASTNode* create_while_node(ASTNode *condition, ASTNode *body) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_WHILE;
    node->data.while_loop.condition = condition;
    node->data.while_loop.body = body;
    return node;
}

/* Create a for node */
ASTNode* create_for_node(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_FOR;
    node->data.for_loop.init = init;
    node->data.for_loop.condition = condition;
    node->data.for_loop.update = update;
    node->data.for_loop.body = body;
    return node;
}

/* Create a block node */
ASTNode* create_block_node() {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_BLOCK;
    node->data.block.statements = NULL;
    node->data.block.statement_count = 0;
    node->data.block.capacity = 0;
    return node;
}

/* Add a statement to a block */
void add_statement_to_block(ASTNode *block, ASTNode *statement) {
    if (block == NULL || block->type != NODE_BLOCK) {
        fprintf(stderr, "Invalid block node\n");
        return;
    }

    /* Resize statements array if needed */
    if (block->data.block.statement_count >= block->data.block.capacity) {
        int new_capacity = block->data.block.capacity == 0 ? 4 : block->data.block.capacity * 2;
        ASTNode **new_statements = (ASTNode**)realloc(block->data.block.statements, 
                                                     new_capacity * sizeof(ASTNode*));
        if (new_statements == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        block->data.block.statements = new_statements;
        block->data.block.capacity = new_capacity;
    }

    /* Add the statement */
    block->data.block.statements[block->data.block.statement_count++] = statement;
}

/* Create a min/max function node */
ASTNode* create_minmax_node(MinMaxFuncType func_type, ASTNode *arg1, ASTNode *arg2) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = NODE_MINMAX;
    node->data.minmax.func_type = func_type;
    node->data.minmax.arg1 = arg1;
    node->data.minmax.arg2 = arg2;
    return node;
}

/* Free an AST node and all its children */
void free_ast(ASTNode *node) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case NODE_VARIABLE:
            free(node->data.variable_name);
            break;
        case NODE_STRING:
            free(node->data.string_value);
            break;
        case NODE_BINARY_OP:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
        case NODE_ASSIGNMENT:
            free(node->data.assignment.var_name);
            free_ast(node->data.assignment.value);
            break;
        case NODE_IF:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.if_branch);
            if (node->data.if_stmt.else_branch) {
                free_ast(node->data.if_stmt.else_branch);
            }
            break;
        case NODE_WHILE:
            free_ast(node->data.while_loop.condition);
            free_ast(node->data.while_loop.body);
            break;
        case NODE_FOR:
            free_ast(node->data.for_loop.init);
            free_ast(node->data.for_loop.condition);
            free_ast(node->data.for_loop.update);
            free_ast(node->data.for_loop.body);
            break;
        case NODE_BLOCK:
            for (int i = 0; i < node->data.block.statement_count; i++) {
                free_ast(node->data.block.statements[i]);
            }
            free(node->data.block.statements);
            break;
        case NODE_MINMAX:
            free_ast(node->data.minmax.arg1);
            free_ast(node->data.minmax.arg2);
            break;
        default:
            /* Nothing to free for NUMBER nodes */
            break;
    }

    free(node);
}

/* Function for interpreting binary operations */
double interpret_binary_op(BinaryOpNode *op_node) {
    if (!op_node || !op_node->left || !op_node->right) {
        fprintf(stderr, "Error: Invalid binary operation node\n");
        return 0.0;
    }

    /* Calculate the values of the left and right operands */
    double left_val = interpret_ast(op_node->left);
    double right_val = interpret_ast(op_node->right);
    
    /* Debug output - uncomment for debugging binary operations */
    // printf("Debug: Binary operation %d with values %f and %f\n", op_node->op, left_val, right_val);

    switch (op_node->op) {
        case OP_ADD:
            return left_val + right_val;
        case OP_SUBTRACT:
            return left_val - right_val;
        case OP_MULTIPLY:
            return left_val * right_val;
        case OP_DIVIDE:
            if (right_val != 0.0) {
                return left_val / right_val;
            } else {
                fprintf(stderr, "Error: Division by zero\n");
                return 0.0;
            }
        case OP_EQUALS:
            return (left_val == right_val) ? 1.0 : 0.0;
        case OP_NEQUALS:
            return (left_val != right_val) ? 1.0 : 0.0;
        case OP_GREATER:
            return (left_val > right_val) ? 1.0 : 0.0;
        case OP_LESS:
            return (left_val < right_val) ? 1.0 : 0.0;
        case OP_GREATEREQ:
            return (left_val >= right_val) ? 1.0 : 0.0;
        case OP_LESSEQ:
            return (left_val <= right_val) ? 1.0 : 0.0;
        default:
            fprintf(stderr, "Unknown binary operator: %d\n", op_node->op);
            return 0.0;
    }
}

/* Execute an AST - Implementation follows */
double interpret_ast(ASTNode *node) {
    if (node == NULL) {
        return 0.0;
    }

    double result = 0.0;
    double cond_value;
    double *var_value;

    switch (node->type) {
        case NODE_NUMBER:
            return node->data.number_value;

        case NODE_VARIABLE:
            var_value = get_symbol(node->data.variable_name);
            if (var_value != NULL) {
                return *var_value;
            } else {
                fprintf(stderr, "Undefined variable: %s\n", node->data.variable_name);
                return 0.0;
            }

        case NODE_STRING:
            /* Strings don't have a numeric value in this context */
            return 0.0;

        case NODE_BINARY_OP:
            return interpret_binary_op(&node->data.binary_op);

        case NODE_ASSIGNMENT:
            result = interpret_ast(node->data.assignment.value);
            add_symbol(node->data.assignment.var_name, result);
            return result;

        /* Modify the print node interpretation in interpret_ast() function */
/* This part replaces the corresponding code in the NODE_PRINT case */

        case NODE_PRINT:
            if (node->data.print.expression) {
                if (node->data.print.expression->type == NODE_VARIABLE) {
                /* For variable, print with name = value format */
                    char *var_name = node->data.print.expression->data.variable_name;
                    result = interpret_ast(node->data.print.expression);
                    printf("%s = %g\n", var_name, result);
                    fflush(stdout);
                 }else {
                 /* For expressions, evaluate and print */
                    result = interpret_ast(node->data.print.expression);
                    printf("%g\n", result);  /* Use %g format for clean number output */
                    fflush(stdout); /* Force output immediately */
                }
            } else if (node->data.print.string_literal) {
                /* Print string literals without "Output:" prefix */
                printf("%s\n", node->data.print.string_literal);
                fflush(stdout); /* Force output immediately */
            }
            return result;

        case NODE_IF:
            cond_value = interpret_ast(node->data.if_stmt.condition);
            /* Debug message - uncomment if you want to see condition evaluation */
            // printf("If condition evaluated to: %s\n", cond_value != 0.0 ? "true" : "false");
            
            if (cond_value != 0.0) {
                return interpret_ast(node->data.if_stmt.if_branch);
            } else if (node->data.if_stmt.else_branch) {
                // printf("Else block condition: true\n");
                return interpret_ast(node->data.if_stmt.else_branch);
            }
            return 0.0;

        case NODE_WHILE:
            {
                result = 0.0;
                while (1) {
                    cond_value = interpret_ast(node->data.while_loop.condition);
                    /* Debug message - uncomment if you want to see condition evaluation */
                    // printf("While condition evaluated to: %s\n", cond_value != 0.0 ? "true" : "false");
                    
                    if (cond_value == 0.0) {
                        // printf("Exiting while loop (condition false)\n");
                        break;
                    }
                    
                    // printf("Executing while loop body\n");
                    result = interpret_ast(node->data.while_loop.body);
                }
                return result;
            }

        case NODE_FOR:
            {
                result = 0.0;
                
                /* Initialize */
                if (node->data.for_loop.init) {
                    interpret_ast(node->data.for_loop.init);
                }
                
                /* Loop */
                while (1) {
                    /* Check condition */
                    if (node->data.for_loop.condition) {
                        cond_value = interpret_ast(node->data.for_loop.condition);
                        /* Debug message - uncomment if you want to see condition evaluation */
                        // printf("For loop condition evaluated to: %s\n", cond_value != 0.0 ? "true" : "false");
                        
                        if (cond_value == 0.0) {
                            // printf("Exiting for loop (condition false)\n");
                            break;
                        }
                    }
                    
                    /* Execute body */
                    // printf("Executing for loop body\n");
                    result = interpret_ast(node->data.for_loop.body);
                    
                    /* Update */
                    if (node->data.for_loop.update) {
                        interpret_ast(node->data.for_loop.update);
                    }
                }
                return result;
            }

        case NODE_BLOCK:
            {
                result = 0.0;
                for (int i = 0; i < node->data.block.statement_count; i++) {
                    result = interpret_ast(node->data.block.statements[i]);
                }
                return result;
            }

        case NODE_MINMAX:
            {
                double arg1_val = interpret_ast(node->data.minmax.arg1);
                double arg2_val = interpret_ast(node->data.minmax.arg2);
                
                if (node->data.minmax.func_type == FUNC_MIN) {
                    return (arg1_val < arg2_val) ? arg1_val : arg2_val;
                } else { /* FUNC_MAX */
                    return (arg1_val > arg2_val) ? arg1_val : arg2_val;
                }
            }

        default:
            fprintf(stderr, "Unknown node type: %d\n", node->type);
            return 0.0;
    }
}
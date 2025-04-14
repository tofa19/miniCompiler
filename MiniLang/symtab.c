#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* Symbol table structure */
typedef struct symbol {
    char *name;
    double value;
    struct symbol *next;
} symbol;

/* Head of the symbol table */
static symbol *symtab = NULL;

/* Initialize the symbol table */
void init_symtab() {
    /* Free any existing symbol table */
    free_symtab();
    
    /* Initialize to empty */
    symtab = NULL;
}

int add_symbol(char *name, double value) {
    if (name == NULL) {
        return 0;
    }

    /* Check if symbol already exists */
    symbol *sym = symtab;
    while (sym != NULL) {
        if (strcmp(sym->name, name) == 0) {
            /* Update existing symbol */
            sym->value = value;
            return 1;
        }
        sym = sym->next;
    }

    /* Create new symbol */
    sym = (symbol *)malloc(sizeof(symbol));
    if (sym == NULL) {
        return 0;
    }

    sym->name = strdup(name);
    if (sym->name == NULL) {
        free(sym);
        return 0;
    }

    sym->value = value;
    sym->next = symtab;
    symtab = sym;
    return 1;
}

double *get_symbol(char *name) {
    if (name == NULL) {
        return NULL;
    }

    symbol *sym = symtab;
    while (sym != NULL) {
        if (strcmp(sym->name, name) == 0) {
            return &(sym->value);
        }
        sym = sym->next;
    }

    return NULL;
}

void print_symtab() {
    printf("Symbol Table:\n");
    symbol *sym = symtab;
    if (sym == NULL) {
        printf("  <empty>\n");
        return;
    }

    while (sym != NULL) {
        printf("  %s = %f\n", sym->name, sym->value);
        sym = sym->next;
    }
}

void free_symtab() {
    symbol *sym = symtab;
    while (sym != NULL) {
        symbol *next = sym->next;
        free(sym->name);
        free(sym);
        sym = next;
    }
    symtab = NULL;
}
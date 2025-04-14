#ifndef SYMTAB_H
#define SYMTAB_H

/**
 * Symbol table for storing variables and their values
 */

/**
 * Initialize the symbol table
 */
void init_symtab();

/**
 * Add a symbol to the symbol table or update its value if it already exists
 * @param name Variable name
 * @param value Variable value
 * @return 1 if successful, 0 otherwise
 */
int add_symbol(char *name, double value);

/**
 * Get a symbol's value from the symbol table
 * @param name Variable name
 * @return Pointer to the value if found, NULL otherwise
 */
double *get_symbol(char *name);

/**
 * Print the contents of the symbol table
 */
void print_symtab();

/**
 * Clean up the symbol table and free allocated memory
 */
void free_symtab();

#endif /* SYMTAB_H */
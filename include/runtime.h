#ifndef RUNTIME_H
#define RUNTIME_H

#include "ast.h"

#define MAX_SYMBOLS 100

typedef struct {
    char *name;
    double value;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
    int break_flag;
} Environment;

// Function prototypes
void init_environment(Environment *env);
void set_variable(Environment *env, const char *name, double value);
double get_variable(Environment *env, const char *name);
double interpret(ASTNode *node, Environment *env);
void interpret_statement(ASTNode *node, Environment *env);
double interpret_expression(ASTNode *node, Environment *env);

#endif // RUNTIME_H
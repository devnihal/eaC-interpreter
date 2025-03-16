#ifndef RUNTIME_H
#define RUNTIME_H

#include "ast.h"

// Enum for runtime value types
typedef enum {
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_BOOL
} ValueType;

// Struct for runtime values
typedef struct {
    ValueType type;
    union {
        double number;
        char *string;
        int boolean;
    };
} RuntimeValue;

// Function prototypes
RuntimeValue evaluate(ASTNode *node); // Evaluate an AST node
void init_runtime();                  // Initialize the runtime environment
void free_runtime();                  // Free allocated memory

#endif // RUNTIME_H
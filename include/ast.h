#ifndef AST_H
#define AST_H

#include "lexer.h"

// Enum for AST node types
typedef enum {
    NODE_EXPRESSION, // Expression node (e.g., x + 5)
    NODE_STATEMENT,  // Statement node (e.g., variable declaration)
    NODE_NUMBER,     // Numeric literal
    NODE_IDENTIFIER, // Identifier (e.g., variable name)
    NODE_BINARY_OP,  // Binary operation (e.g., +, -, *, /)
    NODE_ASSIGNMENT  // Assignment (e.g., x = 42)
} NodeType;

// Struct for an AST node
typedef struct ASTNode {
    NodeType type;           // Type of the node
    union {
        struct {             // For binary operations
            struct ASTNode *left;
            struct ASTNode *right;
            char operator;   // Operator (+, -, *, /, etc.)
        } binary_op;

        struct {             // For assignments
            char *identifier;
            struct ASTNode *value;
        } assignment;

        char *identifier;    // For identifiers
        double number;       // For numeric literals
    };
} ASTNode;

// Function prototypes
ASTNode *parse(Token *tokens); // Parser function
void free_ast(ASTNode *node);  // Free allocated memory

#endif // AST_H
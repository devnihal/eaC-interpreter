#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum {
    NODE_PROGRAM,      // Root node containing all statements
    NODE_CONDITIONAL,  // Conditional (e.g., when ... otherwise ...)
    NODE_LOOP,         // Loop (e.g., loop { ... })
    NODE_ASSIGNMENT,   // Assignment (e.g., plant x = 42)
    NODE_BLOCK,        // Block of statements (e.g., { ... })
    NODE_BREAK,        // Break statement
    NODE_SKIP,         // Skip statement
    NODE_BINARY_OP,    // Binary operation (e.g., 1 + 2)
    NODE_NUMBER,       // Numeric literal
    NODE_IDENTIFIER    // Identifier (e.g., x)
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {             // For programs (multiple statements)
            struct ASTNode **statements;
            int statement_count;
        } program;

        struct {             // For conditionals
            struct ASTNode *condition;
            struct ASTNode *then_block;
            struct ASTNode *else_block;
        } conditional;

        struct {             // For loops
            struct ASTNode *body;
        } loop;

        struct {             // For assignments
            char *identifier;
            struct ASTNode *value;
        } assignment;

        struct {             // For blocks
            struct ASTNode **statements;
            int statement_count;
        } block;

        struct {             // For binary operations
            struct ASTNode *left;
            struct ASTNode *right;
            char operator;
        } binary_op;

        char *identifier;    // For identifiers
        double number;       // For numeric literals
    };
} ASTNode;
// Function prototypes
ASTNode *parse(Token *tokens); // Parser function
void free_ast(ASTNode *node);  // Free allocated memory

#endif // AST_H
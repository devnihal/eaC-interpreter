#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum {
    NODE_PROGRAM,
    NODE_CONDITIONAL,
    NODE_LOOP,
    NODE_ASSIGNMENT,
    NODE_BLOCK,
    NODE_BREAK,
    NODE_CONTINUE,  // Add this line
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_IDENTIFIER
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
            int is_compound;         // New field for compound operators
            char second_char;        // New field for second character
        } binary_op;

        char *identifier;    // For identifiers
        double number;       // For numeric literals
    };
} ASTNode;
// Function prototypes
ASTNode *parse(Token *tokens); // Parser function
void free_ast(ASTNode *node);  // Free allocated memory

#endif // AST_H
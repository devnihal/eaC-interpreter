#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to check if a token matches a specific type and value
int match_token(Token *token, TokenType type, const char *value) {
    return token->type == type && (value == NULL || strcmp(token->value, value) == 0);
}

// Parse a single expression (e.g., x + 5)
ASTNode *parse_expression(Token **tokens) {
    Token *current = (*tokens);

    // Parse the left-hand side (primary expression: identifier or number)
    ASTNode *left = malloc(sizeof(ASTNode));
    if (match_token(current, TOKEN_IDENTIFIER, NULL)) {
        left->type = NODE_IDENTIFIER;
        left->identifier = strdup(current->value);
        (*tokens)++;
    } else if (match_token(current, TOKEN_NUMBER, NULL)) {
        left->type = NODE_NUMBER;
        left->number = atof(current->value);
        (*tokens)++;
    } else {
        fprintf(stderr, "Error: Expected identifier or number\n");
        exit(1);
    }

    // Check for binary operator (e.g., + in "x + 5")
    current = (*tokens);
    while (match_token(current, TOKEN_OPERATOR, NULL)) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = NODE_BINARY_OP;
        node->binary_op.operator = current->value[0];
        (*tokens)++; // Advance past the operator

        // Parse the right-hand side (must be another expression)
        node->binary_op.left = left;
        node->binary_op.right = parse_expression(tokens);

        // Update the left-hand side to the new binary operation node
        left = node;

        // Move to the next token
        current = (*tokens);
    }

    return left;
}

// Parse a statement (e.g., plant x = 42;)
ASTNode *parse_statement(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    Token *current = (*tokens);

    // Check for variable declaration (e.g., plant x = 42;)
    if (match_token(current, TOKEN_IDENTIFIER, "plant")) {
        (*tokens)++; // Skip "plant"
        current = (*tokens);

        if (match_token(current, TOKEN_IDENTIFIER, NULL)) {
            node->type = NODE_ASSIGNMENT;
            node->assignment.identifier = strdup(current->value);
            (*tokens)++;
            current = (*tokens);

            if (match_token(current, TOKEN_OPERATOR, "=")) {
                (*tokens)++; // Skip "="
                node->assignment.value = parse_expression(tokens);
            } else {
                fprintf(stderr, "Error: Expected '=' after identifier\n");
                exit(1);
            }
        } else {
            fprintf(stderr, "Error: Expected identifier after 'plant'\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Error: Unsupported statement\n");
        exit(1);
    }

    // Ensure the statement ends with a semicolon
    current = (*tokens);
    if (!match_token(current, TOKEN_PUNCTUATION, ";")) {
        fprintf(stderr, "Error: Expected ';' at the end of statement\n");
        exit(1);
    }
    (*tokens)++; // Advance past the semicolon

    return node;
}

// Main parser function
ASTNode *parse(Token *tokens) {
    Token *current = tokens;

    // Parse the first statement
    ASTNode *ast = parse_statement(&current);

    // Ensure we've reached the end of the input
    if (current->type != TOKEN_EOF) {
        fprintf(stderr, "Error: Unexpected tokens after statement\n");
        exit(1);
    }

    return ast;
}

// Free allocated memory for the AST
void free_ast(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_EXPRESSION:
        case NODE_BINARY_OP:
            free_ast(node->binary_op.left);
            free_ast(node->binary_op.right);
            break;
        case NODE_ASSIGNMENT:
            free(node->assignment.identifier);
            free_ast(node->assignment.value);
            break;
        case NODE_IDENTIFIER:
            free(node->identifier);
            break;
        default:
            break;
    }
    free(node);
}
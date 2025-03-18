#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to check if a token matches a specific type and value
int match_token(Token *token, TokenType type, const char *value) {
    return token->type == type && (value == NULL || strcmp(token->value, value) == 0);
}
// Main parser function
ASTNode *parse(Token *tokens) {
    ASTNode *program_node = malloc(sizeof(ASTNode));
    program_node->type = NODE_PROGRAM;
    program_node->program.statements = malloc(100 * sizeof(ASTNode *));
    program_node->program.statement_count = 0;

    Token **current = &tokens; // Pointer to tokens array

    while ((*current)->type != TOKEN_EOF) {
        if (program_node->program.statement_count >= 100) {
            fprintf(stderr, "Error: Too many statements in program\n");
            exit(1);
        }
        program_node->program.statements[program_node->program.statement_count++] = parse_statement(current);
    }

    return program_node;
}
// Parse a single expression (e.g., x + 5)
ASTNode *parse_expression(Token **tokens) {
    Token *current = (*tokens);

    printf("Parsing expression: Type=%d, Value='%s'\n", current->type, current->value ? current->value : "NULL");

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

        printf("Found operator: '%c'\n", node->binary_op.operator);

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

ASTNode *parse_statement(Token **tokens) {
    Token *current = (*tokens);

    printf("Parsing statement: Type=%d, Value='%s'\n", current->type, current->value ? current->value : "NULL");

    if (match_token(current, TOKEN_KEYWORD, "when")) {
        return parse_conditional(tokens);
    } else if (match_token(current, TOKEN_KEYWORD, "loop")) {
        return parse_loop(tokens);
    } else if (match_token(current, TOKEN_KEYWORD, "plant")) {
        return parse_assignment(tokens);
    } else if (match_token(current, TOKEN_KEYWORD, "break")) {
        return parse_break(tokens);
    } else {
        fprintf(stderr, "Error: Unsupported statement\n");
        exit(1);
    }
}
// ASTNode *parse_conditional(Token **tokens) {
//     ASTNode *node = malloc(sizeof(ASTNode));
//     node->type = NODE_CONDITIONAL;

//     (*tokens)++; // Skip "when"
//     node->conditional.condition = parse_expression(tokens);

//     Token *current = (*tokens);
//     printf("After condition: Type=%d, Value='%s'\n", current->type, current->value ? current->value : "NULL");

//     if (!match_token(current, TOKEN_PUNCTUATION, "{")) {
//         fprintf(stderr, "Error: Expected '{' after condition\n");
//         exit(1);
//     }
//     (*tokens)++; // Skip "{"
//     node->conditional.then_block = parse_block(tokens);

//     current = (*tokens);
//     if (match_token(current, TOKEN_KEYWORD, "otherwise")) {
//         (*tokens)++; // Skip "otherwise"
//         current = (*tokens);
//         if (!match_token(current, TOKEN_PUNCTUATION, "{")) {
//             fprintf(stderr, "Error: Expected '{' after otherwise\n");
//             exit(1);
//         }
//         (*tokens)++; // Skip "{"
//         node->conditional.else_block = parse_block(tokens); // Parse as a block
//     } else {
//         node->conditional.else_block = NULL;
//     }

//     return node;
// }

ASTNode *parse_conditional(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_CONDITIONAL;

    (*tokens)++; // Skip "when"
    node->conditional.condition = parse_expression(tokens);

    Token *current = (*tokens);
    if (!match_token(current, TOKEN_PUNCTUATION, "{")) {
        fprintf(stderr, "Error: Expected '{' after condition\n");
        exit(1);
    }
    printf("Parsing then-block starting with '{'\n");
    node->conditional.then_block = parse_block(tokens);

    current = (*tokens);
    if (match_token(current, TOKEN_KEYWORD, "otherwise")) {
        (*tokens)++; // Skip "otherwise"
        current = (*tokens);
        if (!match_token(current, TOKEN_PUNCTUATION, "{")) {
            fprintf(stderr, "Error: Expected '{' after otherwise\n");
            exit(1);
        }
        printf("Parsing else-block starting with '{'\n");
        node->conditional.else_block = parse_block(tokens);
    } else {
        node->conditional.else_block = NULL;
    }

    printf("Finished parsing conditional\n");
    return node;
}
ASTNode *parse_loop(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_LOOP;

    (*tokens)++; // Skip "loop"
    node->loop.body = parse_block(tokens);

    return node;
}
ASTNode *parse_assignment(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGNMENT;

    (*tokens)++; // Skip "plant"
    Token *current = (*tokens);

    if (match_token(current, TOKEN_IDENTIFIER, NULL)) {
        printf("Capturing identifier: '%s'\n", current->value);
        node->assignment.identifier = strdup(current->value); // Capture the identifier
        (*tokens)++;
        current = (*tokens);

        if (match_token(current, TOKEN_OPERATOR, "=")) {
            (*tokens)++; // Skip "="
            node->assignment.value = parse_expression(tokens); // Parse the value
        } else {
            fprintf(stderr, "Error: Expected '=' after identifier\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Error: Expected identifier after 'plant'\n");
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

ASTNode *parse_break(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BREAK;

    (*tokens)++; // Skip "break"
    Token *current = (*tokens);

    // Ensure the statement ends with a semicolon
    if (!match_token(current, TOKEN_PUNCTUATION, ";")) {
        fprintf(stderr, "Error: Expected ';' at the end of break statement\n");
        exit(1);
    }
    (*tokens)++; // Advance past the semicolon

    return node;
}
// ASTNode *parse_block(Token **tokens) {
//     ASTNode *node = malloc(sizeof(ASTNode));
//     node->type = NODE_BLOCK;

//     // Allocate space for up to 100 statements (arbitrary limit)
//     node->block.statements = malloc(100 * sizeof(ASTNode *));
//     node->block.statement_count = 0;

//     (*tokens)++; // Skip "{"
//     Token *current = (*tokens);

//     while (!match_token(current, TOKEN_PUNCTUATION, "}")) {
//         if (node->block.statement_count >= 100) {
//             fprintf(stderr, "Error: Too many statements in block\n");
//             exit(1);
//         }
//         printf("Parsing block statement: Type=%d, Value='%s'\n", current->type, current->value ? current->value : "NULL");
//         node->block.statements[node->block.statement_count++] = parse_statement(tokens);
//         current = (*tokens);
//     }

//     (*tokens)++; // Skip "}"
//     return node;
// }
ASTNode *parse_block(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;

    node->block.statements = malloc(100 * sizeof(ASTNode *));
    node->block.statement_count = 0;

    (*tokens)++; // Skip "{"
    Token *current = (*tokens);

    while (!match_token(current, TOKEN_PUNCTUATION, "}")) {
        if (node->block.statement_count >= 100) {
            fprintf(stderr, "Error: Too many statements in block\n");
            exit(1);
        }
        node->block.statements[node->block.statement_count++] = parse_statement(tokens);
        current = (*tokens);
    }

    (*tokens)++; // Skip "}"
    return node;
}
// Free allocated memory for the AST
void free_ast(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
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
        case NODE_BLOCK:
            for (int i = 0; i < node->block.statement_count; i++) {
                free_ast(node->block.statements[i]);
            }
            free(node->block.statements);
            break;
        default:
            break;
    }
    free(node);
}
void print_tokens(Token *tokens) {
    printf("Tokens:\n");
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        printf("Type: %d, Value: '%s'\n", tokens[i].type, tokens[i].value ? tokens[i].value : "NULL");
    }
}
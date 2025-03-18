#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/lexer.h"
#include "../include/runtime.h"
#include <stdio.h>

void print_ast(ASTNode *node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    switch (node->type) {
        case NODE_PROGRAM:
            printf("PROGRAM:\n");
            for (int i = 0; i < node->program.statement_count; i++) {
                print_ast(node->program.statements[i], depth + 1);
            }
            break;
        case NODE_ASSIGNMENT:
            printf("ASSIGNMENT: %s =\n", node->assignment.identifier);
            print_ast(node->assignment.value, depth + 1);
            break;
        case NODE_BINARY_OP:
            printf("BINARY OP: %c\n", node->binary_op.operator);
            print_ast(node->binary_op.left, depth + 1);
            print_ast(node->binary_op.right, depth + 1);
            break;
        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s\n", node->identifier);
            break;
        case NODE_NUMBER:
            printf("NUMBER: %f\n", node->number);
            break;
        case NODE_CONDITIONAL:
            printf("CONDITIONAL:\n");
            print_ast(node->conditional.condition, depth + 1);
            printf("THEN BLOCK:\n");
            print_ast(node->conditional.then_block, depth + 1);
            if (node->conditional.else_block) {
                printf("ELSE BLOCK:\n");
                print_ast(node->conditional.else_block, depth + 1);
            }
            break;
        case NODE_LOOP:
            printf("LOOP:\n");
            print_ast(node->loop.body, depth + 1);
            break;
        case NODE_BLOCK:
            printf("BLOCK:\n");
            for (int i = 0; i < node->block.statement_count; i++) {
                print_ast(node->block.statements[i], depth + 1);
            }
            break;
        case NODE_BREAK:
            printf("BREAK\n");
            break;
        default:
            printf("UNKNOWN NODE\n");
            break;
    }
}
void print_tokens(Token *tokens);

int main() {
    const char *source = "when 1 > 0 { plant x = 42; } otherwise { plant y = 0; } loop { plant z = 10; break; }";
    printf("Source Code:\n%s\n\nTokens:\n", source);

    Token *tokens = lex(source);
    print_tokens(tokens);

    ASTNode *ast = parse(tokens);
    printf("\nAST:\n");
    print_ast(ast, 0);

    // Free the AST
    printf("\nFreeing AST...\n");
    free_ast(ast);

    // Free tokens
    free_tokens(tokens);
    return 0;
}
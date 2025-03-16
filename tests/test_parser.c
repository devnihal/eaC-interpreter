#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/lexer.h"
#include "../include/runtime.h"
#include <stdio.h>

void print_ast(ASTNode *node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    switch (node->type) {
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
        default:
            printf("UNKNOWN NODE\n");
            break;
    }
}

int main() {
    const char *source = "plant x = 42 + 5;";
    printf("Source Code:\n%s\n\nAST:\n", source);

    Token *tokens = lex(source);
    ASTNode *ast = parse(tokens);
    print_ast(ast, 0);

    // Evaluate the AST
    printf("\nEvaluation:\n");
    init_runtime();
    RuntimeValue result = evaluate(ast);
    if (result.type == VALUE_NUMBER) {
        printf("Result: %f\n", result.number);
    }

    free_tokens(tokens);
    free_ast(ast);
    free_runtime();
    return 0;
}
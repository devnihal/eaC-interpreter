#include "../include/lexer.h"
#include <stdio.h>

void print_token(Token *token) {
    switch (token->type) {
        case TOKEN_KEYWORD:
            printf("KEYWORD: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_PUNCTUATION:
            printf("PUNCTUATION: %s\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
    }
}

int main() {
    const char *source = "plant x = 42; give x + 5;";
    printf("Source Code:\n%s\n\nTokens:\n", source);

    Token *tokens = lex(source);
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        print_token(&tokens[i]);
    }

    free_tokens(tokens);
    return 0;
}
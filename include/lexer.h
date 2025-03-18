#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Enum for token types
typedef enum {
    TOKEN_EOF,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION
} TokenType;

// Struct for a token
typedef struct {
    TokenType type;  // Type of the token
    char *value;     // Value of the token (e.g., "plant", "42", "+")
} Token;

// Function prototypes
Token *lex(const char *source); // Lexer function
void free_tokens(Token *tokens); // Free allocated memory

#endif // LEXER_H
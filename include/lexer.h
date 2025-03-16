#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Enum for token types
typedef enum {
    TOKEN_KEYWORD,     // Keywords like 'plant', 'seal', 'when', etc.
    TOKEN_IDENTIFIER,  // Variable names or function names
    TOKEN_NUMBER,      // Numeric literals (e.g., 42, 3.14)
    TOKEN_STRING,      // String literals (e.g., "hello")
    TOKEN_OPERATOR,    // Operators like +, -, *, /, etc.
    TOKEN_PUNCTUATION, // Punctuation like {, }, (, ), ;
    TOKEN_EOF          // End of file marker
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
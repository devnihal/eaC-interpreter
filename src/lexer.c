#include "lexer.h"
#include "utils.h"

// Helper function to check if a character is a valid identifier start
int is_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

// Helper function to check if a character is part of an identifier
int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

// Lexer function
Token *lex(const char *source) {
    // Allocate memory for tokens (for simplicity, assume max 100 tokens)
    Token *tokens = malloc(100 * sizeof(Token));
    int token_count = 0;

    const char *ptr = source;
    while (*ptr) {
        // Skip whitespace
        if (isspace(*ptr)) {
            ptr++;
            continue;
        }

        // Check for numbers
        if (isdigit(*ptr)) {
            const char *start = ptr;
            while (isdigit(*ptr) || *ptr == '.') ptr++;
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].value = strndup(start, ptr - start);
            token_count++;
            continue;
        }

        // Check for identifiers or keywords
        if (is_identifier_start(*ptr)) {
            const char *start = ptr;
            while (is_identifier_char(*ptr)) ptr++;
            char *value = strndup(start, ptr - start);

            // Check for specific keywords
            if (strcmp(value, "plant") == 0 || strcmp(value, "when") == 0 ||
                strcmp(value, "otherwise") == 0 || strcmp(value, "loop") == 0 ||
                strcmp(value, "break") == 0 || strcmp(value, "skip") == 0 ||
                strcmp(value, "continue") == 0) { // Add this line
                tokens[token_count].type = TOKEN_KEYWORD;
            } else {
                tokens[token_count].type = TOKEN_IDENTIFIER;
            }
            tokens[token_count].value = value;
            token_count++;
            continue;
        }

        // Check for operators
        if (strchr("><=!+-*/", *ptr)) {
            // Check for multi-character operators
            if ((*ptr == '=' && *(ptr + 1) == '=') || 
                (*ptr == '!' && *(ptr + 1) == '=') ||
                (*ptr == '>' && *(ptr + 1) == '=') ||
                (*ptr == '<' && *(ptr + 1) == '=')) {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value = strndup(ptr, 2);
                ptr += 2;
            } else {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value = strndup(ptr, 1);
                ptr++;
            }
            token_count++;
            continue;
        }

        // Check for punctuation
        if (strchr("(){},;[]", *ptr)) {
            tokens[token_count].type = TOKEN_PUNCTUATION;
            tokens[token_count].value = strndup(ptr, 1);
            ptr++;
            token_count++;
            continue;
        }

        // Handle invalid characters
        fprintf(stderr, "Error: Invalid character '%c'\n", *ptr);
        exit(1);
    }

    // Add EOF token
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value = NULL;
    token_count++;

    return tokens;
}

// Free allocated memory for tokens
void free_tokens(Token *tokens) {
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}
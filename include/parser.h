#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

// Parser function prototypes
ASTNode *parse(Token *tokens);               // Main parser function
ASTNode *parse_expression(Token **tokens);   // Parse expressions
ASTNode *parse_statement(Token **tokens);    // Parse statements
ASTNode *parse_conditional(Token **tokens);  // Parse conditionals
ASTNode *parse_loop(Token **tokens);         // Parse loops
ASTNode *parse_assignment(Token **tokens);   // Parse assignments
ASTNode *parse_break(Token **tokens);        // Parse break statements
ASTNode *parse_block(Token **tokens);        // Parse blocks of statements

#endif // PARSER_H
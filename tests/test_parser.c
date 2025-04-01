#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Test source code with new comparison operators
    const char *source_code = 
        "plant x = 5;"
        "plant y = 10;"
        "when x >= 5 {"
            "plant a = 1;"
        "}"
        "when y <= 10 {"
            "plant b = 2;"
        "}"
        "when x != y {"
            "plant c = 3;"
        "}";

    // Tokenize
    Token *tokens = lex(source_code);

    // Parse
    ASTNode *ast = parse(tokens);

    // Interpret
    Environment env;
    init_environment(&env);
    interpret(ast, &env);

    // Print final variable values
    printf("\nFinal Variable Values:\n");
    for (int i = 0; i < env.count; i++) {
        printf("%s = %f\n", env.symbols[i].name, env.symbols[i].value);
    }

    // Free resources
    free_tokens(tokens);
    free_ast(ast);

    return 0;
}
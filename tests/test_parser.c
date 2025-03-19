#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Example source code
    const char *source_code = "when 1 > 0 { plant x = 42; } otherwise { plant y = 0; } loop { plant z = 10; break; }";

    // Tokenize
    Token *tokens = lex(source_code); // Use lex() instead of tokenize()

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
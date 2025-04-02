#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>

// Main test function for logical operators (&&, ||, !) and comparison operators (>=, <=, !=)
// Tests both simple and complex conditional expressions
int main() {
    // Test source code with logical operators
    const char *source_code = 
        "plant x = 5;"
        "plant y = 10;"
        "plant z = 15;"
        
        // Test AND operator
        "when x >= 5 && y <= 10 {"
            "plant a = 1;"  // Should be 1 (both conditions true)
        "}"
        
        // Test OR operator
        "when x > 10 || y == 10 {"
            "plant b = 2;"  // Should be 2 (second condition true)
        "}"
        
        // Test NOT operator
        "when !(x > 10) {"
            "plant c = 3;"  // Should be 3 (condition false, so NOT makes it true)
        "}"
        
        // Test complex logical expression
        "when (x >= 5 && y <= 10) || z == 15 {"
            "plant d = 4;"  // Should be 4 (first part true)
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
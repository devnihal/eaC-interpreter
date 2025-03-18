#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbol table (for simplicity, use a fixed-size array)
#define SYMBOL_TABLE_SIZE 100
typedef struct {
    char *name;
    RuntimeValue value;
} SymbolTableEntry;

SymbolTableEntry symbol_table[SYMBOL_TABLE_SIZE];
int symbol_table_size = 0;

// Helper function to find a variable in the symbol table
RuntimeValue *lookup_variable(const char *name) {
    for (int i = 0; i < symbol_table_size; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(1);
}

// Helper function to add a variable to the symbol table
void define_variable(const char *name, RuntimeValue value) {
    if (symbol_table_size >= SYMBOL_TABLE_SIZE) {
        fprintf(stderr, "Error: Symbol table overflow\n");
        exit(1);
    }
    symbol_table[symbol_table_size].name = strdup(name);
    symbol_table[symbol_table_size].value = value;
    symbol_table_size++;
}

// Evaluate an AST node
RuntimeValue evaluate(ASTNode *node) {
    RuntimeValue result;

    switch (node->type) {
        case NODE_NUMBER:
            result.type = VALUE_NUMBER;
            result.number = node->number;
            break;

        case NODE_IDENTIFIER:
            result = *lookup_variable(node->identifier);
            break;

        case NODE_BINARY_OP: {
            RuntimeValue left = evaluate(node->binary_op.left);
            RuntimeValue right = evaluate(node->binary_op.right);

            if (left.type != VALUE_NUMBER || right.type != VALUE_NUMBER) {
                fprintf(stderr, "Error: Binary operation requires numeric operands\n");
                exit(1);
            }

            result.type = VALUE_NUMBER;
            switch (node->binary_op.operator) {
                case '+': result.number = left.number + right.number; break;
                case '-': result.number = left.number - right.number; break;
                case '*': result.number = left.number * right.number; break;
                case '/': result.number = left.number / right.number; break;
                default:
                    fprintf(stderr, "Error: Unsupported operator '%c'\n", node->binary_op.operator);
                    exit(1);
            }
            break;
        }

        case NODE_ASSIGNMENT: {
            RuntimeValue value = evaluate(node->assignment.value);
            define_variable(node->assignment.identifier, value);
            result = value;
            break;
        }
        case NODE_CONDITIONAL: {
            RuntimeValue condition = evaluate(node->conditional.condition);
            if (condition.type != VALUE_BOOL) {
                fprintf(stderr, "Error: Conditional requires a boolean condition\n");
                exit(1);
            }
            if (condition.boolean) {
                evaluate(node->conditional.then_block);
            } else if (node->conditional.else_block) {
                evaluate(node->conditional.else_block);
            }
            break;
        }

        case NODE_LOOP: {
            while (1) {
                evaluate(node->loop.body);
            }
            break;
        }

        default:
            fprintf(stderr, "Error: Unsupported AST node type\n");
            exit(1);
    }

    return result;
}

// Initialize the runtime environment
void init_runtime() {
    symbol_table_size = 0;
}

// Free allocated memory
void free_runtime() {
    for (int i = 0; i < symbol_table_size; i++) {
        free(symbol_table[i].name);
    }
}
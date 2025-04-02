#include "runtime.h"
#include <string.h>
#include <stdlib.h>
#define MAX_SYMBOLS 100

// Forward declarations
double interpret(ASTNode *node, Environment *env);
void interpret_statement(ASTNode *node, Environment *env);
double interpret_expression(ASTNode *node, Environment *env);

// Initialize the environment
void init_environment(Environment *env) {
    env->count = 0;
    env->break_flag = 0;
    env->continue_flag = 0;
}

// Add or update a variable in the environment
void set_variable(Environment *env, const char *name, double value) {
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->symbols[i].name, name) == 0) {
            env->symbols[i].value = value;
            return;
        }
    }
    if (env->count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Too many variables\n");
        exit(1);
    }
    env->symbols[env->count].name = strdup(name);
    env->symbols[env->count].value = value;
    env->count++;
}

// Get the value of a variable from the environment
double get_variable(Environment *env, const char *name) {
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->symbols[i].name, name) == 0) {
            return env->symbols[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(1);
}
// Evaluate expressions
double interpret_expression(ASTNode *node, Environment *env) {
    if (!node) {
        fprintf(stderr, "Error: Null node in expression\n");
        exit(1);
    }

    switch (node->type) {
        case NODE_NUMBER:
            return node->number;
        case NODE_IDENTIFIER:
            return get_variable(env, node->identifier);
        case NODE_BINARY_OP: {
            // For NOT operator, we only need the right operand
            if (node->binary_op.is_logical && node->binary_op.operator == '!') {
                if (!node->binary_op.right) {
                    fprintf(stderr, "Error: Missing right operand for NOT operator\n");
                    exit(1);
                }
                double right = interpret_expression(node->binary_op.right, env);
                return (right == 0) ? 1 : 0;
            }

            // For all other operators, we need both operands
            if (!node->binary_op.left || !node->binary_op.right) {
                fprintf(stderr, "Error: Missing operand in binary operation\n");
                exit(1);
            }

            double left = interpret_expression(node->binary_op.left, env);
            double right = interpret_expression(node->binary_op.right, env);
            
            // Handle logical operators first
            // First evaluate compound operators
            double result;
            if (node->binary_op.is_compound) {
                switch (node->binary_op.operator) {
                    case '>':
                        result = (node->binary_op.second_char == '=') ? (left >= right) : (left > right);
                        break;
                    case '<':
                        result = (node->binary_op.second_char == '=') ? (left <= right) : (left < right);
                        break;
                    case '!':
                        result = (node->binary_op.second_char == '=') ? (left != right) : (!right);
                        break;
                    case '=':
                        result = (node->binary_op.second_char == '=') ? (left == right) : 0;
                        break;
                    default:
                        fprintf(stderr, "Error: Unsupported compound operator '%c%c'\n", 
                                node->binary_op.operator, node->binary_op.second_char);
                        exit(1);
                }
                return result ? 1 : 0;
            }

            // Then evaluate logical operators
            if (node->binary_op.is_logical) {
                // Convert numeric values to boolean (0 is false, non-zero is true)
                int left_bool = (left != 0);
                int right_bool = (right != 0);
                
                switch (node->binary_op.operator) {
                    case '&': return (left_bool && right_bool) ? 1 : 0;  // AND
                    case '|': return (left_bool || right_bool) ? 1 : 0;  // OR
                    case '!': return (!right_bool) ? 1 : 0;  // NOT (unary)
                    default:
                        fprintf(stderr, "Error: Unknown logical operator '%c'\n", node->binary_op.operator);
                        exit(1);
                }
            }
            
            // Handle compound operators
            if (node->binary_op.is_compound) {
                switch (node->binary_op.operator) {
                    case '>': return (node->binary_op.second_char == '=') ? 
                                   (left >= right ? 1 : 0) : (left > right ? 1 : 0);
                    case '<': return (node->binary_op.second_char == '=') ? 
                                   (left <= right ? 1 : 0) : (left < right ? 1 : 0);
                    case '!': return (node->binary_op.second_char == '=') ? 
                                   (left != right ? 1 : 0) : (!right ? 1 : 0);
                    case '=': return (node->binary_op.second_char == '=') ? 
                                   (left == right ? 1 : 0) : 0;
                    default:
                        fprintf(stderr, "Error: Unsupported compound operator '%c%c'\n", 
                                node->binary_op.operator, node->binary_op.second_char);
                        exit(1);
                }
            }
            
            // Handle simple operators
            switch (node->binary_op.operator) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return left / right;
                case '>': return left > right ? 1 : 0;
                case '<': return left < right ? 1 : 0;
                case '=': return left == right ? 1 : 0;
                default:
                    fprintf(stderr, "Error: Unsupported operator '%c'\n", node->binary_op.operator);
                    exit(1);
            }
        }
        default:
            fprintf(stderr, "Error: Unsupported expression type\n");
            exit(1);
    }
}
// Execute statements
void interpret_statement(ASTNode *node, Environment *env) {
    switch (node->type) {
        case NODE_ASSIGNMENT: {
            double value = interpret_expression(node->assignment.value, env);
            set_variable(env, node->assignment.identifier, value);
            break;
        }
        case NODE_CONDITIONAL: {
            double condition = interpret_expression(node->conditional.condition, env);
            if (condition) {
                interpret(node->conditional.then_block, env);
            } else if (node->conditional.else_block) {
                interpret(node->conditional.else_block, env);
            }
            break;
        }
        case NODE_LOOP: {
            while (1) {
                interpret(node->loop.body, env);

                // Exit the loop if a break statement was encountered
                if (env->break_flag) {
                    env->break_flag = 0; // Reset the break flag
                    break;
                }

                // Continue to the next iteration if a continue statement was encountered
                if (env->continue_flag) {
                    env->continue_flag = 0; // Reset the continue flag
                    continue;
                }
            }
            break;
        }
        case NODE_BREAK: {
            env->break_flag = 1; // Set the break flag
            break;
        }
        case NODE_CONTINUE: {
            env->continue_flag = 1; // Set the continue flag
            break;
        }
        default: {
            fprintf(stderr, "Error: Unsupported statement type\n");
            exit(1);
        }
    }
}

// Interpret the entire program
double interpret(ASTNode *node, Environment *env) {
    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->program.statement_count; i++) {
                interpret_statement(node->program.statements[i], env);
            }
            break;
        case NODE_BLOCK:
            for (int i = 0; i < node->block.statement_count; i++) {
                interpret_statement(node->block.statements[i], env);
            }
            break;
        default:
            fprintf(stderr, "Error: Unsupported node type in interpreter\n");
            exit(1);
    }
    return 0; // Return value is unused for now
}
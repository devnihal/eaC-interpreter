#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

// Custom implementation of strndup for Windows compatibility
char* strndup(const char* s, size_t n);

#endif // UTILS_H
#include <string.h>
#include <stdlib.h>

// Custom implementation of strndup for Windows compatibility
char* strndup(const char* s, size_t n) {
    size_t len = strnlen(s, n);
    char* new_str = (char*)malloc(len + 1);
    if (new_str == NULL) return NULL;
    memcpy(new_str, s, len);
    new_str[len] = '\0';
    return new_str;
}
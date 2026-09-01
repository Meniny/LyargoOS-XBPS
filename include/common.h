#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PROGNAME_XPT "xpt"
#define PROGNAME_XMAN "xman"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Helper macros
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define UNUSED(x) (void)(x)

// Safe string duplication
static inline char *safe_strdup(const char *s) {
    if (!s) return NULL;
    char *dup = strdup(s);
    if (!dup) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return dup;
}

#endif // COMMON_H

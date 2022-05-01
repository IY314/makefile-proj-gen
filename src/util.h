#pragma once

#include <stdlib.h>
#include <string.h>

#define is_null(ptr) (ptr == NULL)

#define malloc_check(name, size, stmt) \
    name = malloc(size);               \
    if (name == NULL) stmt

#define malloc_struct(name, type, stmt) malloc_check(name, sizeof(type), stmt)

#define malloc_struct_array(name, type, size, stmt) \
    malloc_check(name, sizeof(type) * size, stmt)

#define malloc_strcpy(dest, src, stmt)         \
    malloc_check(dest, strlen(src) + 1, stmt); \
    strcpy(dest, src)

#define realloc_str(str) str = realloc(str, strlen(str) + 1)

#define free_if_notnull(ptr) \
    if (is_null(ptr)) free(ptr)

#define str_eq(str1, str2) (strcmp(str1, str2) == 0)

#define str_empty(str) (is_null(str) || strcmp(str, "") == 0)

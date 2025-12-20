#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DYN_MIN_CAP 16

typedef struct DYNARRAY {
    void* str;
    size_t length;
    size_t capacity;
} array;



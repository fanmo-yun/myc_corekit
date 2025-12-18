#ifndef DYNSTRING_H
#define DYNSTRING_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRING_MIN_CAP 16

typedef struct STRING {
    char* str;
    size_t length;
    size_t capacity;
} string;

string* new_string(const char*);
string* new_string_with_size(size_t);
void free_string(string*);

int32_t string_grow(string*, size_t);

int32_t string_clear(string*);
int32_t string_write(string*, const char*);
int32_t string_append(string*, const char*);
int32_t string_insert(string*, uint32_t, const char*);
int32_t string_push_front(string*, const char*);
int32_t string_pop(string*, char*);
int32_t string_pop_front(string*, char*);

bool string_empty(const string*);
size_t string_length(const string*);
int32_t string_contains(const string*, const char*);
int32_t string_find(const string*, const char*);

const char* string_cstr(const string*);
char* string_to_cstr(string*);

#endif

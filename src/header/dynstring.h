#ifndef DYNSTRING_H
#define DYNSTRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef c_string c_string;

c_string* new_string(const char*);
c_string* new_string_reserve(size_t);
c_string* string_clone(const c_string*);

int32_t string_reserve(c_string*, size_t);
int32_t string_shrink(c_string*);
int32_t string_zero(c_string*);
int32_t string_clear(c_string*);
int32_t string_copy(c_string*, const c_string*);
int32_t string_write_str(c_string*, const char*);
int32_t string_push(c_string*, const char*);
int32_t string_insert(c_string*, size_t, const char*);
int32_t string_push_front(c_string*, const char*);
int32_t string_pop(c_string*, char*);
int32_t string_pop_front(c_string*, char*);
int32_t string_remove(c_string*, size_t, size_t);

bool string_empty(const c_string*);
size_t string_length(const c_string*);
bool string_contains(const c_string*, const char*);
int32_t string_find(const c_string*, const char*);

char* string_to_cstr(const c_string*);
const char* string_cstr(const c_string*);

#endif

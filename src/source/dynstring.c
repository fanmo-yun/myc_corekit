#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "src/utils/error.c"

static inline size_t __round_up_pow2(size_t x) {
    if (x <= 1) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return x + 1;
}

#define STRING_MIN_CAP 16

typedef struct DYN_STRING {
    char* str;
    size_t length;
    size_t capacity;
} c_string;

c_string* new_string(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);

    c_string* s = malloc(sizeof *s);
    if (!s) return NULL;

    size_t cap = __round_up_pow2(len+1);
    if (cap < STRING_MIN_CAP) cap = STRING_MIN_CAP;

    s->capacity = cap;

    s->str = malloc(s->capacity+1);
    if (!s->str) {
        free(s);
        return NULL;
    }
    memcpy(s->str, str, len);
    s->length = len;
    *(s->str + s->length) = '\0';
    return s;
}

c_string* new_string_reserve(size_t capacity) {
    c_string* s = malloc(sizeof *s);
    if (!s) return NULL;
    
    size_t cap = __round_up_pow2(capacity);
    if (cap < STRING_MIN_CAP) cap = STRING_MIN_CAP;

    s->capacity = cap;
    s->length = 0;

    s->str = malloc(s->capacity+1);
    if (!s->str) {
        free(s);
        return NULL;
    }
    *(s->str) = '\0';
    return s;
}

c_string* string_clone(const c_string* s) {
    if (!s) return NULL;

    c_string* clone = new_string_reserve(s->capacity);
    if (!clone) return NULL;
    
    memcpy(clone->str, s->str, s->length);
    clone->length = s->length;
    *(clone->str + clone->length) = '\0';
    return clone;
}

void free_string(c_string* s) {
    if (!s) return;
    free(s->str);
    free(s);
}

int32_t string_reserve(c_string* s, size_t need) {
    if (need <= s->capacity) return CSTR_EINVAL;
    size_t new_cap = __round_up_pow2(need);
    char* newbuf = realloc(s->str, new_cap+1);
    if (!newbuf) return CSTR_ENOMEM;

    s->str = newbuf;
    s->capacity = new_cap;
    return CSTR_OK;
}

int32_t string_shrink(c_string* s) {
    if (!s) return CSTR_EINVAL;
    size_t shrink_cap = __round_up_pow2(s->length);
    char* newbuf = realloc(s->str, shrink_cap+1);
    if (!newbuf) return CSTR_ENOMEM;

    s->str = newbuf;
    s->capacity = shrink_cap;
    return CSTR_OK;
}

int32_t string_zero(c_string* s) {
    if (!s) return CSTR_EINVAL;
    memset(s->str, 0, s->capacity+1);
    s->length = 0;
    return CSTR_OK;
}

int32_t string_clear(c_string* s) {
    if (!s) return CSTR_EINVAL;
    if (s->length == 0) return CSTR_OK;
    
    *(s->str) = '\0';
    s->length = 0;
    return CSTR_OK;
}

int32_t string_copy(c_string* dest, const c_string* src) {
    if (!dest || !src) return CSTR_EINVAL;
    if (dest->capacity < src->length)
        if (string_reserve(dest, src->length) == CSTR_ENOMEM)
            return CSTR_ENOMEM;
    
    memcpy(dest->str, src->str, src->length);
    dest->length = src->length;
    *(dest->str + dest->length) = '\0';
    return CSTR_OK;
}

int32_t string_write_str(c_string* s, const char* str) {
    if (!s || !str) return CSTR_EINVAL;

    size_t len = strlen(str);

    if (len > s->capacity)
        if (string_reserve(s, len) == CSTR_ENOMEM)
            return CSTR_ENOMEM;

    memcpy(s->str, str, len);
    s->length = len;
    *(s->str + s->length) = '\0';
    return 0;
}

int32_t string_push(c_string* s, const char* str) {
    if (!s || !str) return CSTR_EINVAL;

    size_t len = strlen(str);
    size_t total = s->length+len;
    
    if (total > s->capacity)
        if (string_reserve(s, total) == CSTR_ENOMEM)
            return CSTR_ENOMEM;

    memcpy(s->str+s->length, str, len);
    s->length = total;
    *(s->str + s->length) = '\0';
    return CSTR_OK;
}

int32_t string_insert(c_string* s, size_t index, const char* str) {
    if (!s || !str || index > s->length) return CSTR_EINVAL;

    if (index == s->length) return string_push(s, str);

    size_t len = strlen(str);
    size_t total = s->length+len;

    if (total > s->capacity)
        if (string_reserve(s, total) == CSTR_ENOMEM)
            return CSTR_ENOMEM;

    memmove(
        s->str+index+len,
        s->str+index,
        (s->length-index)+1
    );

    memcpy(s->str+index, str, len);

    s->length = total;
    return CSTR_OK;
}

int32_t string_push_front(c_string* s, const char* str) {
    return string_insert(s, 0, str);
}

int32_t string_pop(c_string* s, char* pop) {
    if (!s || !pop || s->length == 0) return CSTR_EINVAL;

    s->length--;
    *pop = *(s->str + s->length);
    *(s->str + s->length) = '\0';
    return CSTR_OK;
}

int32_t string_pop_front(c_string* s, char* pop) {
    if (!s || !pop || s->length == 0) return CSTR_EINVAL;

    s->length--;
    *pop = *(s->str);
    memmove(s->str, s->str+1, s->length+1);
    return CSTR_OK;
}

int32_t string_remove(c_string* s, size_t start, size_t end) {
    if (!s) return CSTR_EINVAL;

    if (start >= s->length || end > s->length || start >= end)
        return CSTR_EINVAL;

    if (start == 0 && end == s->length)
        return string_clear(s);

    size_t removed = end - start;
    
    memmove(
        s->str+start,
        s->str+end,
        (s->length - end) + 1
    );

    s->length -= removed;
    return CSTR_OK;
}

bool string_empty(const c_string* s) {
    return s && s->length == 0;
}

size_t string_length(const c_string* s) {
    return s ? s->length : 0;
}

bool string_contains(const c_string* s, const char* sub) {
    if (!s || !sub) return false;
    return strstr(s->str, sub) != NULL;
}

int32_t string_find(const c_string* s, const char* sub) {
    if (!s || !sub) return CSTR_EINVAL;
    const char* p = strstr(s->str, sub);
    if (!p) return CSTR_ENOTFOUND;
    return (int32_t)(p - s->str);
}

char* string_to_cstr(const c_string* s) {
    if (!s) return NULL;
    char* buf = malloc(s->length+1);
    if (!buf) return NULL;
    memcpy(buf, s->str, s->length);
    *(buf + s->length) = '\0';
    return buf;
}

const char* string_cstr(const c_string* s) {
    return s ? s->str : NULL;
}

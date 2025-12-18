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

static inline size_t __round_up_pow2(size_t x) {
    if (x <= 1) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

bool string_empty(const string* s) {
    return s && s->length == 0;
}

size_t string_length(const string* s) {
    return s ? s->length : 0;
}

char* string_to_cstr(string* s) {
    if (!s) return NULL;
    char* buf = malloc(s->length+1);
    if (!buf) return NULL;
    memcpy(buf, s->str, s->length);
    buf[s->length] = '\0';
    return buf;
}

const char* string_cstr(const string* s) {
    return s ? s->str : NULL;
}

int32_t string_grow(string* s, size_t need) {
    if (need <= s->capacity) return 0;
    size_t new_cap = __round_up_pow2(need);
    char* newbuf = realloc(s->str, new_cap+1);
    if (!newbuf) return 2;

    s->str = newbuf;
    s->capacity = new_cap;
    return 0;
}

string* new_string(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);

    string* s = malloc(sizeof *s);
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
    s->str[len] = '\0';
    s->length = len;
    return s;
}

string* new_string_with_size(size_t size) {
    string* s = malloc(sizeof *s);
    if (!s) return NULL;
    
    size_t cap = __round_up_pow2(size);
    if (cap < STRING_MIN_CAP) cap = STRING_MIN_CAP;

    s->capacity = cap;
    s->length = 0;

    s->str = malloc(s->capacity);
    if (!s->str) {
        free(s);
        return NULL;
    }
    s->str[0] = '\0';
    return s;
}

int32_t string_clear(string* s) {
    if (!s) return 1;
    if (s->length == 0) return 0;
    
    *(s->str) = '\0';
    s->length = 0;
    return 0;
}

void free_string(string* s) {
    if (!s) return;
    free(s->str);
    free(s);
}

int32_t string_write(string* s, const char* str) {
    if (!s || !str) return 1;

    size_t len = strlen(str);

    if (len > s->capacity)
        if (string_grow(s, len))
            return 2;

    memcpy(s->str, str, len);
    s->str[len] = '\0';
    s->length = len;
    return 0;
}

int32_t string_append(string* s, const char* str) {
    if (!s || !str) return 1;

    size_t len = strlen(str);
    size_t total = s->length+len;
    
    if (total > s->capacity)
        if (string_grow(s, total))
            return 2;

    memcpy(s->str+s->length, str, len);
    s->length = total;
    s->str[s->length] = '\0';
    return 0;
}

int32_t string_insert(string* s, uint32_t index, const char* str) {
    if (!s || !str) return 1;
    if (index > s->length) return 3;

    if (index == s->length) return string_append(s, str);

    size_t len = strlen(str);
    size_t total = s->length+len;

    if (total > s->capacity)
        if (string_grow(s, total))
            return 2;

    memmove(
        s->str+index+len,
        s->str+index,
        (s->length-index)+1
    );

    memcpy(s->str+index, str, len);

    s->length = total;
    return 0;
}

int32_t string_push_front(string* s, const char* str) {
    return string_insert(s, 0, str);
}

int32_t string_pop(string* s, char* pop) {
    if (!s || !pop) return 1;
    if (s->length == 0) return 2;

    s->length--;
    *pop = s->str[s->length];
    s->str[s->length] = '\0';
    return 0;
}

int32_t string_pop_front(string* s, char* pop) {
    if (!s || !pop) return 1;
    if (s->length == 0) return 2;

    s->length--;
    *pop = s->str[0];
    memmove(s->str, s->str+1, s->length+1);
    return 0;
}

int32_t string_contains(const string* s, const char* sub) {
    if (!s || !sub) return 1;
    return strstr(s->str, sub) != NULL;
}

int32_t string_find(const string* s, const char* sub) {
    if (!s || !sub) return 1;
    const char* p = strstr(s->str, sub);
    if (!p) return -1;
    return (int32_t)(p - s->str);
}

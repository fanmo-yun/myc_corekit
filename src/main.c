#include <stdio.h>
#include "dynstring.h"

int main(void) {
    int32_t result = 0;

    string* test1 = new_string("hello everybody!");
    if(!test1) return 1;
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test1->str, test1->length, test1->capacity, test1->str);
    free_string(test1);

    string* test = new_string_with_size(12);
    if(!test) return 1;

    result = string_write(test, "hello world");
    if (result == 2) {
        free_string(test);
        return result;
    }
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test->str, test->length, test->capacity, test->str);

    string_clear(test);
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test->str, test->length, test->capacity, test->str);

    result = string_write(test, "hello world! my name is Moryn.");
    if (result == 2) {
        free_string(test);
        return result;
    }
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test->str, test->length, test->capacity, test->str);

    result = string_append(test, " and my age is 20 years-old");
    if (result == 2) {
        free_string(test);
        return result;
    }
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test->str, test->length, test->capacity, test->str);

    result = string_insert(test, 12, " wow this is amazing!");
    if (result == 2) {
        free_string(test);
        return result;
    }
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test->str, test->length, test->capacity, test->str);

    char* test_cstr = string_to_cstr(test);
    printf("cstr -> %s\n", test_cstr);
    free(test_cstr);

    result = string_push_front(test, "hey this is a message from front. ");
    if (result == 2) {
        free_string(test);
        return result;
    }
    printf("str -> %s, length -> %ld, cap -> %ld %p\n", test->str, test->length, test->capacity, test->str);

    // size_t size = test->length;
    // for (size_t i = 0; i < size; i++) {
    //     char out;
    //     result = string_pop_front(test, &out);
    //     if (result) return result;
    //     printf("%c", out);
    // }
    // printf("\nstr -> %s, length -> %d, cap -> %d %p\n", test->str, test->length, test->capacity, test->str);

    printf(
        "empty -> %d, length -> %ld, readonly str -> %s, contains -> %d, find -> %d\n",
        string_empty(test),
        string_length(test),
        string_cstr(test),
        string_contains(test1, "fan"),
        string_find(test, "aaa")
    );
    
    free_string(test);
    test = NULL;
    return 0;
}

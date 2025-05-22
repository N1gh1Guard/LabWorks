#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*BinaryOperator)(const void* arg1, const void* arg2, void* result);
typedef void (*UnaryOperator)(const void* arg, void* result);

typedef struct {
    size_t size;
    BinaryOperator add;
    BinaryOperator subtract;
    BinaryOperator multiplication;
    UnaryOperator multiply;
    void (*print)(const void*);
} TypeInfo;

#endif // TYPEINFO_H
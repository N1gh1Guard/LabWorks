#ifndef COMPLEX_H
#define COMPLEX_H

#include "TypeInfo.h"

static TypeInfo* COMPLEX_TYPE_INFO = NULL;

typedef struct {
    double real;
    double imag;
} Complex;

void ComplexAdd(const void* arg1, const void* arg2, void* result);
void ComplexSubtract(const void* arg1, const void* arg2, void* result);
void ComplexMultiplication(const void* arg1, const void* arg2, void* result);
void ComplexMultiply(const void* arg, void* result);
void ComplexPrint(const void* data);
TypeInfo* GetComplexTypeInfo();

#endif // COMPLEX_H
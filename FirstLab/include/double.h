#ifndef DOUBLE_H
#define DOUBLE_H

#include "TypeInfo.h"

static TypeInfo* DOUBLE_TYPE_INFO = NULL;

void DoubleAdd(const void* arg1, const void* arg2, void* result);
void DoubleSubtract(const void* arg1, const void* arg2, void* result);
void DoubleMultiplication(const void* arg1, const void* arg2, void* result);
void DoubleMultiply(const void* arg, void* result);
void DoublePrint(const void* data);
TypeInfo* GetDoubleTypeInfo();

#endif // DOUBLE_H
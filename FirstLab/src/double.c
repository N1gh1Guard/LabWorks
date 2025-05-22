#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\double.h"
#include <stdio.h>

void DoubleAdd(const void* arg1, const void* arg2, void* result) {
    *(double*)result = *(double*)arg1 + *(double*)arg2;
}

void DoubleSubtract(const void* arg1, const void* arg2, void* result) {
    *(double*)result = *(double*)arg1 - *(double*)arg2;
}

void DoubleMultiplication(const void* arg1, const void* arg2, void* result) {
    *(double*)result = *(double*)arg1 * *(double*)arg2;
}

void DoubleMultiply(const void* arg, void* result) {
    *(double*)result *= *(double*)arg;
}

void DoublePrint(const void* data) {
    printf("%f", *(const double*)data);
}

TypeInfo* GetDoubleTypeInfo() {
    if (DOUBLE_TYPE_INFO == NULL) {
        DOUBLE_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        DOUBLE_TYPE_INFO->size = sizeof(double);
        DOUBLE_TYPE_INFO->add = DoubleAdd;
        DOUBLE_TYPE_INFO->subtract = DoubleSubtract;
        DOUBLE_TYPE_INFO->multiplication = DoubleMultiplication;
        DOUBLE_TYPE_INFO->multiply = DoubleMultiply;
        DOUBLE_TYPE_INFO->print = DoublePrint;
    }
    return DOUBLE_TYPE_INFO;
}
#include <stdlib.h>
#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\complex.h"

void ComplexAdd(const void* a, const void* b, void* result) {
    const Complex* ca = (const Complex*)a;
    const Complex* cb = (const Complex*)b;
    Complex* cr = (Complex*)result;
    cr->real = ca->real + cb->real;
    cr->imag = ca->imag + cb->imag;
}

void ComplexSubtract(const void* a, const void* b, void* result) {
    const Complex* ca = (const Complex*)a;
    const Complex* cb = (const Complex*)b;
    Complex* cr = (Complex*)result;
    cr->real = ca->real - cb->real;
    cr->imag = ca->imag - cb->imag;
}

void ComplexMultiplication(const void* a, const void* b, void* result){
    const Complex* ca = (const Complex*) a;
    const Complex* cb = (const Complex*) b;
    Complex* cr = (Complex*)result;
    cr->real = ca->real * cb->real - ca->imag * cb->imag;
    cr->imag = ca->real * cb->imag + ca->imag * cb->real;
}

void ComplexMultiply(const void* s, void* inout) {
    const Complex* scalar = (const Complex*)s;
    Complex* x = (Complex*)inout;
    double r = x->real * scalar->real - x->imag * scalar->imag;
    double i = x->real * scalar->imag + x->imag * scalar->real;
    x->real = r;
    x->imag = i;
}

void ComplexPrint(const void* a) {
    const Complex* c = (const Complex*)a;
    if (c->imag < 0)
        printf("%f - %fi", c->real, -c->imag);
    else
        printf("%f + %fi", c->real, c->imag);
}


TypeInfo* GetComplexTypeInfo() {
    if (COMPLEX_TYPE_INFO == NULL) {
        COMPLEX_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        COMPLEX_TYPE_INFO->size = sizeof(double _Complex);
        COMPLEX_TYPE_INFO->add = ComplexAdd;
        COMPLEX_TYPE_INFO->subtract = ComplexSubtract;
        COMPLEX_TYPE_INFO->multiplication = ComplexMultiplication;
        COMPLEX_TYPE_INFO->multiply = ComplexMultiply;
        COMPLEX_TYPE_INFO->print = ComplexPrint;
    }
    return COMPLEX_TYPE_INFO;
}
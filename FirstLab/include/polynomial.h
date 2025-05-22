#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "TypeInfo.h"
#include "polynomial_error.h"

typedef struct {
    void** coefficients;
    size_t size;
    TypeInfo* typeInfo;
} Polynomial;

Polynomial* createPolynomial(TypeInfo* typeInfo, size_t size, PolynomialErrors* operationResult);
void freePolynomial(Polynomial* poly);
PolynomialErrors addPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result);
PolynomialErrors subtractPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result);
PolynomialErrors multiplicationPolynominal(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result);
PolynomialErrors multiplyPolynomial(const Polynomial* poly, const void* scalar, Polynomial* result);
PolynomialErrors printPolynomial(const Polynomial* poly);
PolynomialErrors evaluatePolynomial(const Polynomial* poly, const void* xValues, void* result);

#endif // POLYNOMIAL_H
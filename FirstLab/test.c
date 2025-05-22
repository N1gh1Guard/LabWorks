#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/polynomial.h"
#include "include/double.h"
#include "include/complex.h"

void testDoublePolynomial() {
    printf("=== Test Double Polynomial ===\n");

    PolynomialErrors err;
    TypeInfo* type = GetDoubleTypeInfo();

    Polynomial* p1 = createPolynomial(type, 3, &err);
    double a[] = {1.0, 2.0, 3.0};
    for (int i = 0; i < 3; i++) memcpy(p1->coefficients[i], &a[i], sizeof(double));

    Polynomial* p2 = createPolynomial(type, 3, &err);
    double b[] = {3.0, 2.0, 1.0};
    for (int i = 0; i < 3; i++) memcpy(p2->coefficients[i], &b[i], sizeof(double));

    Polynomial* sum = createPolynomial(type, 3, &err);
    addPolynomials(p1, p2, sum);
    printf("Sum: ");
    printPolynomial(sum);

    Polynomial* diff = createPolynomial(type, 3, &err);
    subtractPolynomials(p1, p2, diff);
    printf("Difference: ");
    printPolynomial(diff);

    double scalar = 2.0;
    Polynomial* scaled = createPolynomial(type, 3, &err);
    multiplyPolynomial(p1, &scalar, scaled);
    printf("Scaled by 2.0: ");
    printPolynomial(scaled);

    double xVals[3] = {1.0, 1.0, 1.0};
    double result = 0.0;
    evaluatePolynomial(p1, xVals, &result);
    printf("Evaluation at x=1: %lf\n", result);

    freePolynomial(p1);
    freePolynomial(p2);
    freePolynomial(sum);
    freePolynomial(diff);
    freePolynomial(scaled);
}

void testComplexPolynomial() {
    printf("=== Test Complex Polynomial ===\n");

    PolynomialErrors err;
    TypeInfo* type = GetComplexTypeInfo();

    Polynomial* p1 = createPolynomial(type, 2, &err);
    Complex c1 = {1.0, 1.0}, c2 = {2.0, 2.0};
    memcpy(p1->coefficients[0], &c1, sizeof(Complex));
    memcpy(p1->coefficients[1], &c2, sizeof(Complex));

    Complex scalar = {2.0, 0.0};
    Polynomial* scaled = createPolynomial(type, 2, &err);
    multiplyPolynomial(p1, &scalar, scaled);
    printf("Complex scaled: ");
    printPolynomial(scaled);

    double xRealVals[2] = {1.0, 0.0};
    Complex evalResult = {0.0, 0.0};
    evaluatePolynomial(p1, xRealVals, &evalResult);
    printf("Evaluation at x=1 (real only): ");
    type->print(&evalResult);
    printf("\n");

    freePolynomial(p1);
    freePolynomial(scaled);
}

int main() {
    testDoublePolynomial();
    printf("\n");
    testComplexPolynomial();
    return 0;
}

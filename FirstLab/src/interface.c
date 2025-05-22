#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\interface.h"
#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\complex.h"
#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\double.h"

void printUsage(void) {
    printf("\nUsage:\n");
    printf("Enter polynomials as: <size> <type> <coefficients...>\n");
    printf("  size         - number of coefficients\n");
    printf("  type         - 'c' for complex or 'd' for double\n");
    printf("  coefficients - space-separated values\n");
    printf("Operations:\n");
    printf("  +   : add polynomials\n");
    printf("  -   : subtract polynomials\n");
    printf("  *   : multiply polynomials\n");
    printf("  *s  : multiply by scalar (real)\n");
    printf("  =   : print current polynomial\n");
    printf("  e   : evaluate polynomial at x (real)\n");
    printf("  q   : quit program\n\n");
}

TypeInfo* getTypeInfoFromChar(char type, PolynomialErrors* error) {
    switch (type) {
        case 'c': return GetComplexTypeInfo();
        case 'd': return GetDoubleTypeInfo();
        default:
            *error = INCOMPATIBLE_POLYNOMIAL_TYPES;
            return NULL;
    }
}

void readPolynomial(Polynomial** poly, PolynomialErrors* operationResult) {
    int size;
    char type;
    if (scanf("%d %c", &size, &type) != 2) {
        *operationResult = POLYNOMIAL_NOT_DEFINED;
        return;
    }
    TypeInfo* ti = getTypeInfoFromChar(type, operationResult);
    if (!ti) return;

    *poly = createPolynomial(ti, (size_t)size, operationResult);
    if (*operationResult != POLYNOMIAL_OPERATION_OK) return;

    for (int i = 0; i < size; ++i) {
        if (ti == GetComplexTypeInfo()) {
            Complex z;
            if (scanf("%lf %lf", &z.real, &z.imag) != 2) {
                *operationResult = POLYNOMIAL_NOT_DEFINED;
                return;
            }
            memcpy((*poly)->coefficients[i], &z, ti->size);
        } else {
            double v;
            if (scanf("%lf", &v) != 1) {
                *operationResult = POLYNOMIAL_NOT_DEFINED;
                return;
            }
            memcpy((*poly)->coefficients[i], &v, ti->size);
        }
    }
    *operationResult = POLYNOMIAL_OPERATION_OK;
}

void handleEvaluation(const Polynomial* poly) {
    double x_val;
    void* x      = malloc(poly->typeInfo->size);
    void* result = malloc(poly->typeInfo->size);
    if (!x || !result) {
        fprintf(stderr, "Memory allocation failed for evaluation\n");
        free(x); free(result);
        return;
    }

    printf("Enter real x: ");
    if (scanf("%lf", &x_val) != 1) {
        fprintf(stderr, "Invalid input for x\n");
        free(x); free(result);
        return;
    }

    if (poly->typeInfo == GetComplexTypeInfo()) {
        Complex z = { .real = x_val, .imag = 0.0 };
        memcpy(x, &z, poly->typeInfo->size);
    } else {
        memcpy(x, &x_val, poly->typeInfo->size);
    }

    PolynomialErrors err = evaluatePolynomial(poly, x, result);
    if (err == POLYNOMIAL_OPERATION_OK) {
        printf("Result: ");
        poly->typeInfo->print(result);
        printf("\n");
    } else {
        fprintf(stderr, "%s\n", polynomialErrorToString(err));
    }

    free(x);
    free(result);
}

void handleUserInput(void) {
    printUsage();
    PolynomialErrors opRes;
    Polynomial* poly = NULL;
    readPolynomial(&poly, &opRes);
    if (opRes != POLYNOMIAL_OPERATION_OK) {
        fprintf(stderr, "%s\n", polynomialErrorToString(opRes));
        exit(EXIT_FAILURE);
    }

    char cmd[3];
    while (1) {
        printf("Enter operation: ");
        if (scanf(" %2s", cmd) != 1) {
            fprintf(stderr, "Error reading operation\n");
            freePolynomial(poly);
            exit(EXIT_FAILURE);
        }

        if (strcmp(cmd, "q") == 0) {
            break;
        } else if (strcmp(cmd, "=") == 0) {
            printf("Current polynomial: ");
            printPolynomial(poly);
        } else if (strcmp(cmd, "e") == 0) {
            handleEvaluation(poly);
        } else if (strcmp(cmd, "+") == 0 || strcmp(cmd, "-") == 0) {
            Polynomial* other = NULL;
            readPolynomial(&other, &opRes);
            if (opRes != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(opRes));
                freePolynomial(poly);
                exit(EXIT_FAILURE);
            }
            size_t newSize = (poly->size > other->size) ? poly->size : other->size;
            Polynomial* resultPoly = createPolynomial(poly->typeInfo, newSize, &opRes);
            if (opRes != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(opRes));
                freePolynomial(poly); freePolynomial(other);
                exit(EXIT_FAILURE);
            }
            PolynomialErrors err = (cmd[0] == '+')
                ? addPolynomials(poly, other, resultPoly)
                : subtractPolynomials(poly, other, resultPoly);
            if (err != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(err));
                freePolynomial(poly); freePolynomial(other); freePolynomial(resultPoly);
                exit(EXIT_FAILURE);
            }
            freePolynomial(poly);
            freePolynomial(other);
            poly = resultPoly;
        } else if (strcmp(cmd, "*s") == 0) {
            double scalar_val;
            printf("Enter real scalar: ");
            if (scanf("%lf", &scalar_val) != 1) {
                fprintf(stderr, "Invalid scalar input\n");
                freePolynomial(poly);
                exit(EXIT_FAILURE);
            }
            void* scalar = malloc(poly->typeInfo->size);
            if (!scalar) {
                fprintf(stderr, "Memory error allocating scalar\n");
                freePolynomial(poly);
                exit(EXIT_FAILURE);
            }
            if (poly->typeInfo == GetComplexTypeInfo()) {
                Complex z = { .real = scalar_val, .imag = 0.0 };
                memcpy(scalar, &z, poly->typeInfo->size);
            } else {
                memcpy(scalar, &scalar_val, poly->typeInfo->size);
            }
            Polynomial* resultPoly = createPolynomial(poly->typeInfo, poly->size, &opRes);
            if (opRes != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(opRes));
                free(scalar); freePolynomial(poly);
                exit(EXIT_FAILURE);
            }
            PolynomialErrors err = multiplyPolynomial(poly, scalar, resultPoly);
            free(scalar);
            if (err != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(err));
                freePolynomial(poly); freePolynomial(resultPoly);
                exit(EXIT_FAILURE);
            }
            freePolynomial(poly);
            poly = resultPoly;
        } else if (strcmp(cmd, "*") == 0) {
            Polynomial* other = NULL;
            readPolynomial(&other, &opRes);
            if (opRes != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(opRes));
                freePolynomial(poly);
                exit(EXIT_FAILURE);
            }
            size_t sizeP = poly->size + other->size - 1;
            Polynomial* resultPoly = createPolynomial(poly->typeInfo, sizeP, &opRes);
            if (opRes != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(opRes));
                freePolynomial(poly); freePolynomial(other);
                exit(EXIT_FAILURE);
            }
            PolynomialErrors err = multiplyPolynomial(poly, other, resultPoly);
            if (err != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(err));
                freePolynomial(poly); freePolynomial(other); freePolynomial(resultPoly);
                exit(EXIT_FAILURE);
            }
            freePolynomial(poly);
            freePolynomial(other);
            poly = resultPoly;
        } else {
            fprintf(stderr, "Invalid operation: %s\n", cmd);
        }
    }
    freePolynomial(poly);
}

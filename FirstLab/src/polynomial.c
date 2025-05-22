#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\polynomial.h"

Polynomial* createPolynomial(TypeInfo* typeInfo, size_t size, PolynomialErrors* operationResult) {
    if (size == 0) {
        *operationResult = POLYNOMIAL_NOT_DEFINED;
        return NULL;
    }

    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
    if (!poly) {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    poly->coefficients = (void**)malloc(size * sizeof(void*));
    if (!poly->coefficients) {
        free(poly);
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    poly->size = size;
    poly->typeInfo = typeInfo;

    for (size_t i = 0; i < size; ++i) {
        poly->coefficients[i] = malloc(typeInfo->size);
        if (!poly->coefficients[i]) {
            for (size_t j = 0; j < i; ++j) free(poly->coefficients[j]);
            free(poly->coefficients);
            free(poly);
            *operationResult = MEMORY_ALLOCATION_FAILED;
            return NULL;
        }
        memset(poly->coefficients[i], 0, typeInfo->size);
    }

    *operationResult = POLYNOMIAL_OPERATION_OK;
    return poly;
}

void freePolynomial(Polynomial* poly) {
    if (!poly) return;
    for (size_t i = 0; i < poly->size; ++i) free(poly->coefficients[i]);
    free(poly->coefficients);
    free(poly);
}

PolynomialErrors addPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result) {
    if (!poly1 || !poly2 || !result) return POLYNOMIAL_NOT_DEFINED;
    if (poly1->typeInfo != poly2->typeInfo || poly1->typeInfo != result->typeInfo) 
        return INCOMPATIBLE_POLYNOMIAL_TYPES;
    if (!poly1->typeInfo->add) return OPERATION_NOT_DEFINED;

    size_t maxSize = (poly1->size > poly2->size) ? poly1->size : poly2->size;

    for (size_t i = 0; i < maxSize; ++i) {
        const void* coeff1 = (i < poly1->size) ? poly1->coefficients[i] : NULL;
        const void* coeff2 = (i < poly2->size) ? poly2->coefficients[i] : NULL;

        if (coeff1 && coeff2) {
            poly1->typeInfo->add(coeff1, coeff2, result->coefficients[i]);
        } else if (coeff1) {
            memcpy(result->coefficients[i], coeff1, poly1->typeInfo->size);
        } else if (coeff2) {
            memcpy(result->coefficients[i], coeff2, poly2->typeInfo->size);
        }
    }

    return POLYNOMIAL_OPERATION_OK;
}

PolynomialErrors subtractPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result) {
    if (poly1 == NULL || poly2 == NULL || result == NULL) return POLYNOMIAL_NOT_DEFINED;
    if (poly1->typeInfo != poly2->typeInfo || poly1->typeInfo != result->typeInfo) return INCOMPATIBLE_POLYNOMIAL_TYPES;
    if (poly1->typeInfo->subtract == NULL) return OPERATION_NOT_DEFINED;

    size_t maxSize = (poly1->size > poly2->size) ? poly1->size : poly2->size;

    // Resize the result polynomial to the maximum size
    for (size_t i = result->size; i < maxSize; ++i) {
        result->coefficients[i] = malloc(result->typeInfo->size);
        memset(result->coefficients[i], 0, result->typeInfo->size);
    }
    result->size = maxSize;

    for (size_t i = 0; i < maxSize; ++i) {
        const void* coeff1 = (i < poly1->size) ? poly1->coefficients[i] : result->coefficients[i];
        const void* coeff2 = (i < poly2->size) ? poly2->coefficients[i] : result->coefficients[i];

        poly1->typeInfo->subtract(coeff1, coeff2, result->coefficients[i]);
    }

    return POLYNOMIAL_OPERATION_OK;
}

PolynomialErrors multiplyPolynomial(const Polynomial* poly, const void* scalar, Polynomial* result) {
    if (!poly || !result) return POLYNOMIAL_NOT_DEFINED;
    if (!poly->typeInfo->multiply) return OPERATION_NOT_DEFINED;
    if (scalar == NULL) return SCALAR_NOT_DEFINED;

    for (size_t i = 0; i < poly->size; ++i) {
        memcpy(result->coefficients[i], poly->coefficients[i], poly->typeInfo->size);
        poly->typeInfo->multiply(scalar, result->coefficients[i]);
    }

    return POLYNOMIAL_OPERATION_OK;
}

PolynomialErrors printPolynomial(const Polynomial* poly) {
    if (!poly) return POLYNOMIAL_NOT_DEFINED;
    if (!poly->typeInfo->print) return OPERATION_NOT_DEFINED;

    printf("Polynomial: ");
    for (size_t i = 0; i < poly->size; ++i) {
        poly->typeInfo->print(poly->coefficients[i]);
        if (i < poly->size - 1)
            printf(" + ");
    }
    printf("\n");

    return POLYNOMIAL_OPERATION_OK;
}

PolynomialErrors evaluatePolynomial(const Polynomial* poly, const void* x, void* result) {
    if (!poly || !x || !result) return POLYNOMIAL_NOT_DEFINED;

    memset(result, 0, poly->typeInfo->size);
    void* temp = malloc(poly->typeInfo->size);

    if (!temp) return MEMORY_ALLOCATION_FAILED;

    for (size_t i = poly->size; i-- > 0; ) {
        memcpy(temp, result, poly->typeInfo->size);
        poly->typeInfo->multiply(x, temp);
        poly->typeInfo->add(temp, poly->coefficients[i], result);
    }

    free(temp);
    return POLYNOMIAL_OPERATION_OK;
}
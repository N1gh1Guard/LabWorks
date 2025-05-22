#include "C:\Users\misha\Documents\!programming\MEPhI\2 semestr\LabWorks\FirstLab\include\polynomial_error.h"

//честно, от балды
const char* polynomialErrorToString(PolynomialErrors error) {
    switch (error) {
        case POLYNOMIAL_OPERATION_OK: return "Operation successful";
        case MEMORY_ALLOCATION_FAILED: return "Memory allocation failed";
        case POLYNOMIAL_NOT_DEFINED: return "Polynomial not defined";
        case OPERATION_NOT_DEFINED: return "Operation not defined";
        case INCOMPATIBLE_POLYNOMIAL_TYPES: return "Incompatible polynomial types";
        case INVALID_ARGUMENTS: return "Invalid arguments";
        case SCALAR_NOT_DEFINED: return "Scalar not defined";
        default: return "Unknown error";
    }
}
#ifndef ALGEBRA_H
#define ALGEBRA_H

typedef struct matrix {
    float* mat;
    int rows;
    int cols;
} matrix;

void printMat(matrix *M);
inline void matInv(matrix* I, matrix* M);
inline void matSub(matrix* res, matrix* A, matrix* B);
inline void matAdd(matrix* res, matrix* A, matrix* B);
inline void matMul(matrix* res, matrix* A, matrix* B);
inline void matMul3(matrix* res, matrix* A, matrix* B, matrix* C);
inline void matMul3bis(matrix* res, matrix* A, matrix* B, matrix* C);

#endif

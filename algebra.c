#include <stdio.h>

#include "algebra.h"


void printMat(matrix *M) {
    int i, j;

    for(i=0; i < M->rows; i++) {
        printf("   ");
        for(j=0; j < M->cols; j++) {
            printf("%f ",M->mat[i*M->cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

inline void matInv(matrix* I, matrix* M) {

    // We only invert S which is 2x2
    float det = M->mat[0]*M->mat[3]-M->mat[1]*M->mat[2];
    
    I->mat[0] = M->mat[3]/det;
    I->mat[1] = -(M->mat[1]/det); 
    I->mat[2] = -(M->mat[2]/det); 
    I->mat[3] = M->mat[0]/det;
}

inline void matSub(matrix* res, matrix* A, matrix* B) {
    int i, j;

    for(i=0; i < res->rows; i++) {
        for(j=0; j < res->cols; j++) {
            res->mat[i*res->cols + j] = A->mat[i*res->cols + j] - B->mat[i*res->cols + j];
        }
    }
}

inline void matAdd(matrix* res, matrix* A, matrix* B) {
    int i, j;

    for(i=0; i < res->rows; i++) {
        for(j=0; j < res->cols; j++) {
            res->mat[i*res->cols + j] = A->mat[i*res->cols + j] + B->mat[i*res->cols + j];
        }
    }
}

inline void matMul(matrix* res, matrix* A, matrix* B) {
    int i, j, k;
    float sum;

    for(i=0; i < A->rows; i++) {
        for(j=0; j < B->cols; j++) {
            sum = 0;
            for(k=0; k < A->cols; k++) {
                sum += A->mat[i*A->cols + k] * B->mat[k* B->cols + j];
            }
            res->mat[i*B->cols + j] = sum;
        }
    }
}


inline void matMul3(matrix* res, matrix* A, matrix* B, matrix* C) {
    int i, j, k, l;
    float sum;


    for(i=0; i< res->rows; i++) {
        for(j=0; j < res->cols; j++) {
            res->mat[i*res->cols + j] = 0;
        }
    }
    
    for(i=0; i < A->rows; i++) {
        for(j=0; j < B->cols; j++) {
            sum = 0;
            for(k=0; k < A->cols; k++) {
                sum += A->mat[i*A->cols + k] * B->mat[k* B->cols + j];
            }
            for(l=0; l < C->cols; l++) {
                res->mat[i* C->cols + l] += sum * C->mat[j * C->cols + l];
            }
        }
    }
}


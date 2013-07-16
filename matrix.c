#include <stdio.h>

#define F_ROWS 4
#define F_COLS 4
#define H_ROWS 2
#define H_COLS 4
#define R_ROWS 2
#define R_COLS 2
#define I_ROWS 4
#define I_COLS 4
#define P_ROWS 4
#define P_COLS 4
#define Y_ROWS 2
#define Y_COLS 1
#define K_ROWS 4
#define K_COLS 2
#define X_ROWS 4
#define X_COLS 1
#define Z_ROWS 1
#define Z_COLS 2

typedef struct matrix {
    float* mat;
    int rows;
    int cols;
} matrix;

inline void matAdd(matrix* res, matrix* A, matrix* B) {
    int i, j;

    for(i=0; i < res->rows; i++) {
        for(j=0; j < res->cols; j++) {
            res->mat[i*res->rows + j] = A->mat[i*res->rows + j] + B->mat[i*res->rows + j];
            printf("%f \n",res->mat[i*res->rows + j]);
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
                sum += A->mat[i*A->rows + k] * B->mat[j*B->cols + k];
                
            }
            res->mat[i*B->cols + j] = sum;
        }
    }
}

void predictState(matrix* x, matrix* F, matrix *x_old) {
    matMul(x, F, x_old);
    
    printf("%f\n",x_old->mat[0]);
}

void kalmanFilter(
            float x1, 
            float y1, 
            float x2, 
            float y2, 
            float dZ,
            matrix *P_old, 
            matrix *x_old) {
    matrix x;
    float matrixX[4][1] = {{1},{1},{1},{1}};
    x.mat = (float*)&matrixX;
    
    predictState(&x, P_old, x_old);
}


int main() {
    // Declare the matrix
    matrix P, x;
    float matrixP[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}; 
    float matrixX[4][1] = {{1},{1},{1},{1}};
    
    // Initialize the matrix
    P.rows = P_ROWS;
    P.cols = P_COLS;
    x.rows = X_COLS;
    P.mat = (float*)&matrixP;
    x.mat = (float*)&matrixX;
    
    float x1, y1, x2, y2, dZ;
    x1 = y1 = x2 = y2 = 1;
    dZ = 3;
        
    kalmanFilter(x1, y1, x2, y2, dZ, &P, &x);
    
    return 0;
}

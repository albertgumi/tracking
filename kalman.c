#include <stdlib.h>
#include <stdio.h>
#include "kalman.h"

/*
 * Data structure to store the matrix
 */
/*typedef struct matrix {
    float** mat;
    int rows;
    int cols;
};*/

/** Malloc space for the matrices
 *
 * @param M matrix
 * @param rows total number of rows
 * @param cols total number of columns
 */
inline void matAlloc(float** M, int rows, int cols) {
    int i;
    M = malloc(sizeof(float) * rows * cols);

    for(i=0; i < rows; i++) {
        M[i] = malloc(sizeof(float)*cols);
    }
}

/** Matrix transpose
 *
 */
inline void matTrans(float* M, float* T, int rows, int cols) {
    int i, j;

    for(i=0; i < rows; i++) {
        for(j=0;  j < cols; j++) {
            T[j*rows + i] = M[i* cols + j];
        }
    }
}

/** Matrix inversion for the S matrix
 * @param M matrix to use
 */
inline void matInv(float *M[2]) {

    float I[2][2];    // We only invert S which is 2x2
    float det = M[0][0]*M[1][1]-M[0][1]*M[1][0];
    
    I[0][0] = M[1][1]/det;
    I[0][1] = -(M[0][1]/det); 
    I[1][0] = -(M[1][0]/det); 
    I[1][1] = M[0][0]/det;

    //return I;
}

/** Matrix substraction
 *
 * @param A matrix
 * @param B matrix
 * @param rows number of rows
 * @param cols number of cols
 */
inline void matSubs(float* A, float* B, float *C, int rows, int cols) {
    int i, j;

    for(i=0; i < rows; i++) {
        for(j=0; j < cols; j++) {
            C[i*rows + j] = A[i*rows + j] - B[i*rows + j];
        }
    }
}


/** Matrix addition
 *
 * @param A matrix
 * @param B matrix
 */
inline void matAdd(float* A, float* B, float *C, int rows, int cols) {
    int i, j;

    for(i=0; i < rows; i++) {
        for(j=0; j < cols; j++) {
            C[i*rows + j] = A[i*rows + j] + B[i*rows + j];
        }
    }
}

/** Matrix multiplication
 *
 * @param A matrix
 * @param B matrix
 */
inline void matMul(float* A, float* B, float* C, int rowsA, int colsA, int colsB) {
    int i, j, k;
    float sum;

    for(i=0; i < rowsA; i++) {
        for(j=0; j < colsB; j++) {
            sum = 0;
            for(k=0; k < colsA; k++) {
                sum += A[i*rowsA + k] * B[j*colsB + k];
            }
            C[i*colsB + j] = sum;
            #ifdef DEBUG
                printf("%f\n", sum);
            #endif
        }
    }
}


/** Calculate the tracklets using the kalman filter.
 *
 * @param x1 current X position
 * @param y1 current Y position
 * @param x2 next X position
 * @param y2 next Y position
 * @param dZ increment in the Z axis
 * @param P_old matrix that containts the previous covariance
 * @param x_old matrix that contains the previous track
 */
void kalmanFilter(
            float x1, 
            float y1, 
            float x2, 
            float y2, 
            float dZ,
            float P_old[4][4], 
            float x_old[4][1]) 
{

    // Declaration of rows and columns sizes of the matrix
    /*
    const int F_rows = 4;
    const int F_cols = 4;
    const int H_rows = 2;
    const int H_cols = 4;
    const int R_rows = 2;
    const int R_cols = 2;
    const int I_rows = 4;
    const int I_cols = 4;
    const int P_rows = 4;
    const int P_cols = 4;
    const int y_rows = 2;
    const int y_cols = 1;
    const int K_rows = 4;
    const int K_cols = 2;
    const int x_rows = 4;
    const int x_cols = 1;
    const int z_rows = 1;
    const int z_cols = 2;
    */

    float H[2][4] = {{1.,0.,0.,0.},{0.,1.,0.,0.}};  // observation matrix
    float R[2][2] = {{1,0},{0,1}};      // Estimated error covariance
    float I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}; 

    float P[4][4] = { {0 } };      // Estimation of the error

    float y[2][1] = { {0 } };    // Innovation or measurement residual
    float S[2][2] = { {0 } };    // Innovation (or residual) covariance
    float K[4][2] = { {0 } };    // Optimal Kalman gain 
    
    float F[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};      // Propagation matrix
    //track new_t;    // New calculated track
    
    float x[4][1] = { {0 } };          // Newest estimate of the current "true" state
    
    float Pprev[4][4] = { {0 } };
    float z[1][2] = { {0 } };      // Measurement vector. For us is our hit.
    
    int i;
    // Introduce inside the 
    
    
    for(i=0; i < 4; i++) {
        printf("%f %f %f %f\n",F[i][0], F[i][1], F[i][2], F[i][3]);
    }
    printf("\n"); 
    /* 
     * Phase 1: PREDICT
     */

    // Predicted (a priori) state estimate
    // x = F * x
    matMul((float*)F, (float*)x_old, (float*) x, 4, 4, 1);  

    // Predicted (a priori) estimate covariance
    // P = F * Pprev * H^T
    float Ht[4][2];
    matTrans((float*)H,(float*)Ht,2,4);
/*
    for(i=0; i < 2; i++) {
    //    printf("%f %f %f %f\n",H[i][0], H[i][1]), H[i][2], H[i][3];
    }
    for(i=0; i < 4; i++) {
    //    printf("%f %f\n",Ht[i][0], Ht[i][1]);
    }*/


    float tmpFP[4][4];
    matMul((float*)F, (float*)P_old, (float*)tmpFP, 4, 4, 4);
    for(i=0; i < 4; i++) {
        printf("%f %f %f %f\n",Pprev[i][0], Pprev[i][1], Pprev[i][2], Pprev[i][3]);
    }
    printf("\n");
    for(i=0; i < 4; i++) {
        printf("%f %f %f %f\n",tmpFP[i][0], tmpFP[i][1], tmpFP[i][2], tmpFP[i][3]);
    }
    printf("\n");
    matMul((float*)tmpFP, (float*)Ht, (float*)P, 4,4,2);
    

    //P = matMul(matMul(F,Pprev, 4, 4, 4),matTrans(H, 2, 4), 4, 4, 2);     
    for(i=0; i < 4; i++) {
        printf("%f %f %f %f\n",P[i][0], P[i][1], P[i][2], P[i][3]);
    }
    

    /*
     * Phase 2: UPDATE
     */

    // Innovation or measurement residual
    // y = z - K * x
    //y = matSubs(z - matMul(H, x, 2, 4, 1), 1, 2);
   
    // Innovation (or residual) covariance
    // S = H * P * H^T + R
    //S = matAdd(matMul(H, matMul(P,matTrans(H, 2, 4), R), )); 

    // Optimal Kalman gain
    // K = P * H^T * S^-1
    //K = matMul(P, matMul(matTrans(H),matInv(S)));
    
    // Updated (a posteriori) state estimate
    // x_new = x + K * y
    //x_new = matAdd(x, matMul(K, y));  
    
    // Updated (a posteriori) estimate covariance
    
    
    
    
    
    // Check if its near the minimum
    
}



int main(int argc, char **argv) {

    // points[10];
    int i;
    float P[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}; 
    float x[4][1] = {{1},{1},{1},{1}};

    kalmanFilter(1,1,1,1,3,P,x);

    for(i=0;i<10;i++) {
        //kalmanFilter(t[i]);
    }
    
    return 0;
}

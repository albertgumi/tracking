#include <stdio.h>

#include "algebra.h"
#include "kalman.h"
#include "definitions.h"


void predictState(matrix* x, matrix* F, matrix *x_old) {

    matMul(x, F, x_old);
}

void predictCovariance(matrix* P, matrix* F, matrix* P_old, matrix* H) {

    matMul3(P, F, P_old, H);
}

void innovationResidual(matrix* y, matrix* z, matrix* H, matrix* x) {
    // y = z - K * x
    matMul(y, H, x);
    matSub(y, z, y);
    
}

void innvationCovariance(matrix* S, matrix* H, matrix* P, matrix* Ht, matrix* R) {

    // S = H * P * H^T + R
    
    matMul3(S, H, P, Ht);
    
    matAdd(S, R, S); 
    
}

void optimalGain(matrix* K, matrix* P, matrix* Ht, matrix* S) {
    // K = P * H^T * S^-1

    matrix S_1;
    float matrixS_1[2][2];
    S_1.mat = (float*)&matrixS_1;
    S_1.rows = S_ROWS;
    S_1.cols = S_COLS;

    matInv(&S_1, S);
    matMul3(K, P, Ht, &S_1);

}

void updatedState(matrix* x_new, matrix* x, matrix* K, matrix* y) {

    // x_new = x - K * y
    matMul(x_new, K, y);
    matAdd(x_new, x, x_new);
    
}

void updatedEstimate(matrix* P_new, matrix* K, matrix* H, matrix* P) {

    // P_new = (I - K * H) * P          
    matrix I;
    float matrixI[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    I.mat = (float*)&matrixI;
    I.rows = I_ROWS;
    I.cols = I_COLS;

    matrix tmpIKH;
    float matrixIKH[4][4];
    tmpIKH.mat = (float*)&matrixIKH;
    tmpIKH.rows = I_ROWS;
    tmpIKH.cols = I_COLS;

    matMul(&tmpIKH, K, H);
    matSub(&tmpIKH, &I, &tmpIKH);
    matMul(P_new, &tmpIKH, P); 

}


void kalmanFilter(
            float x1, 
            float y1, 
            float x2, 
            float y2, 
            float dZ,
            matrix *P_old, 
            matrix *x_old) {

    // Declare track vector X
    matrix x;
    float matrixX[4][1] = {{1},{1},{1},{1}};
    x.mat = (float*)&matrixX;
    x.rows = X_ROWS;
    x.cols = X_COLS;

    // Declare Covariance for time k
    matrix P;
    float matrixP[4][4];
    P.mat = (float*)&matrixP;
    P.rows = P_ROWS;
    P.cols = P_COLS;

    // Declare F
    matrix F;
    float matrixF[4][4] = {{1,0,dZ,0},{0,1,0,dZ},{0,0,1,0},{0,0,0,1}};
    F.mat = (float*)&matrixF;
    F.rows = F_ROWS;
    F.cols = F_COLS;
    
    // Declare Ft
    matrix Ft;
    float matrixFt[4][4] = {{1,0,0,0},{0,1,0,0},{dZ,0,1,0},{0,dZ,0,1}};
    Ft.mat = (float*)&matrixFt;
    Ft.rows = F_COLS;   // Cols inverted because is the transpose matrix
    Ft.cols = F_ROWS;   // Rows inverted because is the transpose matrix

    // Declare H
    matrix H;
    float matrixH[2][4] = {{1,0,0,0},{0,1,0,0}};
    H.mat = (float*)&matrixH;
    H.rows = H_ROWS;
    H.cols = H_COLS;

    // Declare Ht
    matrix Ht;
    float matrixHt[4][2] = {{1,0},{0,1},{0,0},{0,0}};
    Ht.mat = (float*)&matrixHt;
    Ht.rows = H_COLS;   // Cols inverted because is the transpose matrix
    Ht.cols = H_ROWS;   // Rows inverted because is the transpose matrix

    // Declare y
    matrix y;
    float matrixY[2][1] =  {{x2},{y2}};
    y.mat = (float*)&matrixY;
    y.rows = Y_ROWS;
    y.cols = Y_COLS;

    // Declare S
    matrix S;
    float matrixS[2][2] = {{1,0},{0,0}};
    S.mat = (float*)&matrixS;
    S.rows = S_ROWS;
    S.cols = S_COLS;

    // Declare K
    matrix K;
    float matrixK[4][2] = {{1,0},{0,1},{0,0},{0,0}};
    K.mat = (float*)&matrixK;
    K.rows = K_ROWS;
    K.cols = K_COLS;

    // Declare z
    matrix z;
    float matrixZ[1][2] = {{x2,y2}};
    z.mat = (float*)&matrixZ;
    z.rows = Z_ROWS;
    z.cols = Z_COLS;

    // Declare R
    matrix R;
    float matrixR[2][2] = {{1,0},{0,1}};
    R.mat = (float*)&matrixR;
    R.rows = R_ROWS;
    R.cols = R_COLS;


    /*
     * Phase 1: PREDICT
     */

    // Predicted (a priori state estimate
    // x = F * x_old
    predictState(&x, &F, x_old);

    // P = F * P_old * H^T
    predictCovariance(&P, &F, P_old, &Ft);
    
    /*
     * Phase 2: UPDATE
     */

    // Innovation (or measurament) residual
    // y = z - K * x
    innovationResidual(&y, &z, &H, &x);

    // Innovation (or residual) covariance
    // S = H * P * H^T + R
    innvationCovariance(&S, &H, &P, &Ht, &R);

    // Optimal Kalman gain
    // K = P * H^T * S^-1
    optimalGain(&K, &P, &Ht, &S);

    // Updated (a posteriori) state estimate
    // x_new = x - K * y
    updatedState(x_old, &x, &K, &y);    // we put the new X in the argument old

    // Updated (a posteriori) estimate covariance
    // P_new = (I - K * H) * P          
    updatedEstimate(P_old, &K, &H, &P);  // we put the new P in the argument old
    
}


int main() {

    int i, j; 

    // Declare the matrix
    matrix P, x;
    float matrixP[4][4] = {{3,0,0,0},{0,3,0,0},{0,0,3,0},{0,0,0,3}}; 
    float matrixX[4][1] = {{1},{1},{0.03},{0.03}};
    
    // Initialize the matrix
    P.rows = P_ROWS;
    P.cols = P_COLS;
    x.rows = X_ROWS;
    x.cols = X_COLS;
    P.mat = (float*)&matrixP;
    x.mat = (float*)&matrixX;
    
    float dZ = 3;   // Asumed as the distances between planes
    
    for(i=1; i < 10; i++) {
        kalmanFilter(i, i, i+1, i+1, dZ, &P, &x);
        
        printf("i =  %d\n",i);
        printf("x =\n\n");
        printMat(&x);
        printf("P = \n\n");
        printMat(&P);
        
    }
    printf("\n");
    
    return 0;
}

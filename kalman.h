#ifndef KALMAN_H
#define KALMAN_H 1


void predictState(matrix* x, matrix* F, matrix *x_old);

void predictCovariance(matrix* P, matrix* F, matrix* P_old, matrix* H);

void innovationResidual(matrix* y, matrix* z, matrix* H, matrix* x);

void innvationCovariance(matrix* S, matrix* H, matrix* P, matrix* Ht, matrix* R);

void optimalGain(matrix* K, matrix* P, matrix* Ht, matrix* S);

void updatedState(matrix* x_new, matrix* x, matrix* K, matrix* y);

void updatedEstimate(matrix* P_new, matrix* K, matrix* H, matrix* P) ;


void kalmanFilter(float x1, float y1, float x2, float y2, float dZ, matrix *P_old, matrix *x_old);

#endif

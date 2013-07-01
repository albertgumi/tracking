#include <stdlib.h>
#include <stdio.h>
#include "kalman.h"


typedef struct track {
    int x;
    int y;
    int dx;
    int dy;
};


inline int** matTrans(int** M, int x, int y) {

}

inline int** matInv(int** M, int x, int y) {

}

/** Matrix multiplication algorithm
 *
 */
inline int** matMul(int** A, int** B) {

}

/** Matrix substraction
 *
 */
inline int** matSubs(int** A, int** B) {

}

/** Matrix substraction
 *
 */
inline int** matAdd(int** A, int** B) {

}


/** Calculate the tracklets using the kalman filter.
 *
 * @param coords 
 */
void kalmanFilter(track t) {
    float F[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};      // Propagation matrix
    float P[4][4];      // Estimation of the error
    float H[2][4] = {{1,0,0,0},{0,1,0,0}};  // observation matrix
    
    int y[2][1];    // Innovation or measurement residual
    int S[2][2];    // Innovation (or residual) covariance
    int K[4][2];    // Optimal Kalman gain 
    
    track new_t;    // New calculated track
    
    int x[4][1];          // Newest estimate of the current "true" state
    
    int Pprev[][];
    int z[][];      // Measurement vector. For us is our hit.
    int R[][];      // Estimated error covariance
    
    // Phase 1: Predict
    
    x = matMul(F, track); // Predicted (a priori) state estimate
    
    P = matMul(matMul(F,Pprev),matTrans(H)); // Predicted (a priori) estimate covariance
    
    
    // Phase 2: Update
    
    y = matSubs(z - matMul(H, x));// Innovation or measurement residual
    
    S = matAdd(matMul(H, matMul(P,matTrans(H), R); // Innovation (or residual) covariance
    
    K = matMul(P, matMul(matTrans(H),matInv(S)));// Optimal Kalman gain
    
    x_new = matAdd(x, matMul(K, y));// Updated (a posteriori) state estimate
    
    // Updated (a posteriori) estimate covariance
    
    
    
    
    
    // Check if its near the minimum
    
}



int main(int argc, char **argv) {

    track points[10];
    int i;
    
    for(i=0;i<10;i++) {
        kalmanFilter(t[i]);
    }
    
    return 0;
}

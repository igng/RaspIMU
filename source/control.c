/*
#include <stdlib.h>
#include <math.h>
#include "debug.h"
#include "linalg.h"
#include "control.h"

// system variables
float x[3] = {0};
float u[2] = {0};

// Kalman filter variables
float x_hat[3] = {0};   // state estimation
float z[3] = {0};       // state measurement
float P_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // x's variance
float F_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // df/dx
float H_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // dh/dx
float R_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // estimation error covariance
float Q_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // system covariance (should be 2x2)
float W_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // df/dw (should be 3x2)
float L_init[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};  // dh/dw
float Y_init[3] = {0, 0, 0};                    // measurement error
Matrix *P, *F, *H, *R, *Q, *W, *L, *K, *X, *Y;

void unicycle(float *x, float *u)
{
    x[0] += u[0]*cos(x[2])*dt;
    x[1] += u[0]*sin(x[2])*dt;
    x[2] += u[1]*dt;
}

void F_fun(void)
{
    F->elem[0][2] = u[0]*sin(x[2])*dt;
    F->elem[1][2] = -u[0]*cos(x[2])*dt;
}

void ekf(void)
{
    int i;

    // Predict
    unicycle(x_hat, u);
    F_fun();
    Matrix *P1 = mat_prod(mat_prod(F, P), mat_tran(F));
    Matrix *P2 = mat_prod(mat_prod(W, Q), mat_tran(W));
    P = mat_sum(P1, P2);

    // Update
    Matrix *K1 = mat_prod(P, mat_tran(H));
    Matrix *K2 = mat_prod(mat_prod(H, P), mat_tran(H));
    Matrix *K3 = mat_prod(mat_prod(L, R), mat_tran(L));
    K = mat_prod(K1, mat_sum(K2, K3));
    
    for (i = 0; i < 3; i++)
        Y->elem[i][0] = z[i] - x[i];

    X = mat_prod(K, Y);
}

void control_init(void)
{
    P = mat_fill(P_init, 3, 3);
    F = mat_fill(F_init, 3, 3);
    H = mat_fill(H_init, 3, 3);
    R = mat_fill(R_init, 3, 3);
    Q = mat_fill(Q_init, 3, 3);
    W = mat_fill(W_init, 3, 3);
    L = mat_fill(L_init, 3, 3);
    Y = mat_fill(Y_init, 3, 1);

    int i;

    for (i = 0; ; i++)
        ekf();
}
*/

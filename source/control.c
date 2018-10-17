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
Matrix *P, *P_m, *F, *H, *R, *Q, *W, *L, *K; // main matrices
Matrix *P11, *P12, *P21, *P22, *K1, *K2;
Matrix *F_t, *W_t, *X, *Y; // support matrices

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

    /* Predict */
    unicycle(x_hat, u);
    F_fun();
    // P1
    mat_prod(F, P, &P11);
    mat_tran(F, &F_t);
    mat_prod(P11, F_t, &P12);
    // P2
    mat_prod(W, Q, &P21);
    mat_tran(W, &W_t);
    mat_prod(P21, W_t, &P22);
    // P
    mat_add(P11, P22, &P_m);

    /* Update */
    //Matrix *K1 = mat_prod(P, mat_tran(H));
    //Matrix *K2 = mat_prod(mat_prod(H, P), mat_tran(H));
    //Matrix *K3 = mat_prod(mat_prod(L, R), mat_tran(L));
    // Asadding that H and L are the identity matrix and won't change
    mat_add(P_m, R, &K1);
    mat_inv(K1, &K2);
    mat_prod(P_m, K2, &K);
    
    for (i = 0; i < 3; i++)
        Y->elem[i][0] = z[i] - x[i];

    mat_prod(K, Y, &X);

    for (i = 0; i < 3; i++)
        x[i] = X->elem[i][0];

    mat_sub(H, K, &P11);
}

void control_init(void)
{
    mat_fill(&P, P_init, 3, 3);
    mat_fill(&H, H_init, 3, 3);
    mat_fill(&R, R_init, 3, 3);
    mat_fill(&Q, Q_init, 3, 3);
    mat_fill(&W, W_init, 3, 3);
    mat_fill(&L, L_init, 3, 3);

    mat_alloc(&P_m, 3, 3);
    mat_alloc(&F, 3, 3);
    mat_alloc(&P11, 3, 3);
    mat_alloc(&P12, 3, 3);
    mat_alloc(&P21, 3, 3);
    mat_alloc(&P22, 3, 3);
    mat_alloc(&F_t, 3, 3);
    mat_alloc(&W_t, 3, 3);
    mat_alloc(&K1, 3, 3);
    mat_alloc(&K2, 3, 3);
    mat_alloc(&K, 3, 3);
    mat_alloc(&Y, 3, 1);
    mat_alloc(&X, 3, 1);
}

void control_destroy(void)
{
    mat_delete(P);
    mat_delete(P_m);
    mat_delete(F);
    mat_delete(H);
    mat_delete(R);
    mat_delete(Q);
    mat_delete(W);
    mat_delete(L);
    mat_delete(P11);
    mat_delete(P12);
    mat_delete(P21);
    mat_delete(P22);
    mat_delete(F_t);
    mat_delete(W_t);
    mat_delete(K1);
    mat_delete(K2);
    mat_delete(K);
    mat_delete(X);
    mat_delete(Y);
}

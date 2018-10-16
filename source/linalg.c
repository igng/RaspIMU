#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "debug.h"
#include "linalg.h"

Matrix *P;

void test(Matrix *P)
{
    Matrix *A = P;
    Matrix *B = P;

    INFO("Matrix");
    mat_print(P);

    INFO("Sum");
    mat_print(mat_sum(A, B));

    INFO("Product");
    mat_print(mat_prod(A, B));

    INFO("Transpose");
    mat_print(mat_tran(P));

    INFO("Determinant");
    printf("%.2f\n\n", mat_det(P));

    INFO("Minor 2,0");
    mat_print(mat_min(P, 2, 0));

    INFO("Inverse");
    mat_print(mat_inv(P));
}

Matrix *mat_alloc(int rows, int cols)
{
    Matrix *M;
    int r;

    if ((M = malloc(sizeof(Matrix))) == NULL)
        error_handler("mat_alloc | malloc on struct");

    M->rows = rows;
    M->cols = cols;
    M->det = 0;
    
    if ((M->elem = malloc(sizeof(float *) * M->rows)) == NULL)
        error_handler("mat_alloc | malloc");
    for (r = 0; r < M->rows; r++)
        if ((M->elem[r] = calloc(M->cols, sizeof(float))) == NULL)
            error_handler("mat_alloc | calloc");
    
    return M;
}

Matrix *mat_fill(float *elements, int rows, int cols)
{
    int r, c;
    Matrix *M = mat_alloc(rows, cols);

    for (r = 0; r < M->rows; r++)
        for (c = 0; c < M->cols; c++)
        {
            int index = r*(M->cols) + c;
            M->elem[r][c] = elements[index];
        }

    return M;
}

Matrix *mat_sum(Matrix *A, Matrix *B)
{
    int r, c;
    Matrix *C = mat_alloc(A->rows, A->cols);

    for (r = 0; r < C->rows; r++)
        for (c = 0; c < C->cols; c++)
            C->elem[r][c] = A->elem[r][c] + B->elem[r][c];

    return C;
}

Matrix *mat_prod(Matrix *A, Matrix *B)
{
    int r, c, k;
    Matrix *C = mat_alloc(A->rows, B->cols);

    for (r = 0; r < C->rows; r++)
        for (c = 0; c < C->cols; c++)
            for (k = 0; k < C->cols; k++)
                C->elem[r][c] += A->elem[r][k]*B->elem[k][c];

    return C;
}

Matrix *mat_tran(Matrix *M)
{
    int r, c;
    Matrix *M_t = mat_alloc(M->cols, M->rows);

    for (r = 0; r < M->rows; r++)
        for (c = 0; c < M->cols; c++)
            M_t->elem[r][c] = M->elem[c][r];

    return M_t;
}

Matrix *mat_min(Matrix *M, int i, int j)
{
    int r, c;
    Matrix *M_min = mat_alloc(M->rows - 1, M->cols - 1);

    for (r = 0; r < i; r++)
    {
        for (c = 0; c < j; c++)
            M_min->elem[r][c] = M->elem[r][c];
        for (c = j; c < M->cols-1; c++)
            M_min->elem[r][c] = M->elem[r][c+1];
    }
    for (r = i; r < M->rows-1; r++)
    {
        for (c = 0; c < j; c++)
            M_min->elem[r][c] = M->elem[r+1][c];
        for (c = j; c < M->cols-1; c++)
            M_min->elem[r][c] = M->elem[r+1][c+1];
    }

    return M_min;
}

float mat_det(Matrix *M)
{
    float det = 0;
    int i;
    Matrix *M_min = mat_alloc(M->rows - 1, M->cols - 1);

    if (M->rows == 1)
        det = M->elem[0][0];
    else
    {
        for (i = 0; i < M->rows; i++)
        {
            M_min = mat_min(M, 0, i);
            M_min->det = mat_det(M_min);
            det = det + M->elem[0][i]*pow(-1, i)*M_min->det;
        }
    }

    return det;
} 

Matrix *mat_inv(Matrix *M)
{
    int r, c;
    Matrix *M_min = mat_alloc(M->rows - 1, M->cols - 1);
    Matrix *M_inv = mat_alloc(M->rows, M->cols);

    M->det = mat_det(M);
    if (M->det == 0)
    {
        ERROR("Matrix is singular!");
        exit(EXIT_FAILURE);
    }
    
    if (M->rows == 1)
        M_inv->elem[0][0] = M->elem[0][0]/M->det;
    else
    {
        for (r = 0; r < M->rows; r++)
            for (c = 0; c < M->cols; c++)
            {
                M_min = mat_min(M, r, c);
                M_min->det = mat_det(M_min);
                M_inv->elem[c][r] = pow(-1, r+c)*M_min->det/M->det;
            }
    }
    
    return M_inv;
}

void mats_init(void)
{
    int Pr = 3;
    int Pc = 3;
    int r, c;
    float *P_init = malloc(sizeof(float)*Pr*Pc);
    float mat[] = {1, 2, 3, 3, 1, 5, 1, 2, 1};

    for (r = 0; r < Pr; r++)
        for (c = 0; c < Pc; c++)
            P_init[r*Pc + c] = mat[r*Pc + c];

    P = mat_fill(P_init, Pr, Pc);
    test(P);
}

void mat_print(Matrix *M)
{
    int r, c;

    for (r = 0; r < M->rows; r++)
    {
        printf("\n");
        for (c = 0; c < M->cols; c++)
            printf ("%.2f ", M->elem[r][c]);
    }
    printf("\n\n");
}

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "debug.h"
#include "linalg.h"

Matrix *P;
Matrix *C;
Matrix *A;
Matrix *B;

void test(Matrix *P)
{
    A = P;
    B = P;

    mat_print(P, "P");
    
    mat_add(A, B, &C);
    mat_print(C, "Sum");

    mat_prod(A, B, &C);
    mat_print(C, "Prod");

    mat_tran(P, &C);
    mat_print(C, "Transpose");

    mat_inv(P, &C);
    mat_print(C, "Inverse");
}

void mat_alloc(Matrix **M, int rows, int cols)
{
    int r;

    if (((*M) = malloc(sizeof(Matrix))) == NULL)
        error_handler("mat_alloc | malloc on struct");

    (*M)->rows = rows;
    (*M)->cols = cols;
    (*M)->det = 0;
    
    if (((*M)->elem = malloc(sizeof(float *) * (*M)->rows)) == NULL)
        error_handler("mat_alloc | malloc");
    for (r = 0; r < (*M)->rows; r++)
        if (((*M)->elem[r] = calloc((*M)->cols, sizeof(float))) == NULL)
            error_handler("mat_alloc | calloc");
}

void mat_delete(Matrix *M)
{
    int r;

    for (r = 0; r < M->rows; r++)
        free(M->elem[r]);

    free(M->elem);
    free(M);
}

void mat_fill(Matrix **M, float *elements, int rows, int cols)
{
    int r, c;
    mat_alloc(M, rows, cols);

    for (r = 0; r < (*M)->rows; r++)
        for (c = 0; c < (*M)->cols; c++)
        {
            int index = r*((*M)->cols) + c;
            (*M)->elem[r][c] = elements[index];
        }
}

void mat_add(Matrix *A, Matrix *B, Matrix **C)
{
    int r, c;

    for (r = 0; r < A->rows; r++)
        for (c = 0; c < A->cols; c++)
            (*C)->elem[r][c] = A->elem[r][c] + B->elem[r][c];
}

void mat_sub(Matrix *A, Matrix *B, Matrix **C)
{
    int r, c;

    for (r = 0; r < A->rows; r++)
        for (c = 0; c < A->cols; c++)
            (*C)->elem[r][c] = A->elem[r][c] - B->elem[r][c];
}

void mat_prod(Matrix *A, Matrix *B, Matrix **C)
{
    int r, c, k;

    for (r = 0; r < A->rows; r++)
        for (c = 0; c < B->cols; c++)
        {
            (*C)->elem[r][c] = 0;
            for (k = 0; k < A->cols; k++)
                (*C)->elem[r][c] += A->elem[r][k]*B->elem[k][c];
        }
}

void mat_tran(Matrix *M, Matrix **M_t)
{
    int r, c;

    for (r = 0; r < M->rows; r++)
        for (c = 0; c < M->cols; c++)
            (*M_t)->elem[r][c] = M->elem[c][r];
}

void mat_min(Matrix *M, int i, int j, Matrix **M_min)
{
    int r, c;

    for (r = 0; r < i; r++)
    {
        for (c = 0; c < j; c++)
            (*M_min)->elem[r][c] = M->elem[r][c];
        for (c = j; c < M->cols-1; c++)
            (*M_min)->elem[r][c] = M->elem[r][c+1];
    }
    for (r = i; r < M->rows-1; r++)
    {
        for (c = 0; c < j; c++)
            (*M_min)->elem[r][c] = M->elem[r+1][c];
        for (c = j; c < M->cols-1; c++)
            (*M_min)->elem[r][c] = M->elem[r+1][c+1];
    }
}

float mat_det(Matrix **M)
{
    float det = 0;
    int i;
    Matrix *M_min;
    mat_alloc(&M_min, (*M)->rows - 1, (*M)->cols - 1);

    if ((*M)->rows == 1)
        det = (*M)->elem[0][0];
    else
    {
        for (i = 0; i < (*M)->rows; i++)
        {
            mat_min(*(M), 0, i, &M_min);
            mat_det(&M_min);
            det += (*M)->elem[0][i]*pow(-1, i)*M_min->det;
        }
    }

    mat_delete(M_min);
    (*M)->det = det;
    return det;
} 

void mat_inv(Matrix *M, Matrix **M_inv)
{
    int r, c;
    Matrix *M_min;
    mat_alloc(&M_min, M->rows - 1, M->cols - 1);

    M->det = mat_det(&M);
    if (M->det == 0)
    {
        ERROR("Matrix is singular!");
        exit(EXIT_FAILURE);
    }
    
    if (M->rows == 1)
        (*M_inv)->elem[0][0] = M->elem[0][0]/M->det;
    else
    {
        for (r = 0; r < M->rows; r++)
            for (c = 0; c < M->cols; c++)
            {
                mat_min(M, r, c, &M_min);
                M_min->det = mat_det(&M_min);
                (*M_inv)->elem[c][r] = pow(-1, r+c)*M_min->det/M->det;
            }
    }

    mat_delete(M_min);
}

void mats_init(void)
{
    int Pr = 3;
    int Pc = 3;
    int r, c;
    float *P_init = malloc(sizeof(float)*Pr*Pc);
    float mat[] = {1, 2, 3, 3, 2, 1, 1, 1, 5};

    for (r = 0; r < Pr; r++)
        for (c = 0; c < Pc; c++)
            P_init[r*Pc + c] = mat[r*Pc + c];

    mat_fill(&P, P_init, Pr, Pc);
    mat_alloc(&C, 3, 3);

    test(P);

    free(P_init);
    mat_delete(P);
    mat_delete(C);
}

void mat_print(Matrix *M, char *name)
{
    int r, c;

    INFO("%s [%dx%d]", name, M->rows, M->cols);
    for (r = 0; r < M->rows; r++)
    {
        printf("\n");
        for (c = 0; c < M->cols; c++)
            printf ("%.2f ", M->elem[r][c]);
    }
    printf("\n\n");
}

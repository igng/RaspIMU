#ifndef LINALG_H_
#define LINALG_H_

typedef struct
{
    int rows;     /* number of rows */
    int cols;     /* number of columns */
    float **elem; /* matrix's elements */
    float det;    /* matrix's determinant */
} Matrix;

Matrix *mat_alloc(int, int);
Matrix *mat_fill(float *, int, int);
Matrix *mat_sum(Matrix *, Matrix *);
Matrix *mat_prod(Matrix *, Matrix *);
Matrix *mat_tran(Matrix *);
Matrix *mat_inv(Matrix *);
Matrix *mat_min(Matrix *, int, int);
float mat_det(Matrix *);
void mats_init(void);
void mat_print(Matrix *);

#endif

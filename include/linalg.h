#ifndef LINALG_H_
#define LINALG_H_

typedef struct
{
    int rows;     /* number of rows */
    int cols;     /* number of columns */
    float **elem; /* matrix's elements */
    float det;    /* matrix's determinant */
} Matrix;

void mat_alloc(Matrix **, int, int);
void mat_fill(Matrix **, float *, int, int);
void mat_add(Matrix *, Matrix *, Matrix **);
void mat_sub(Matrix *, Matrix *, Matrix **);
void mat_prod(Matrix *, Matrix *, Matrix **);
void mat_tran(Matrix *, Matrix **);
void mat_inv(Matrix *, Matrix **);
void mat_min(Matrix *, int, int, Matrix **);
float mat_det(Matrix **);
void mats_init(void);
void mat_print(Matrix *, char *);
void mat_delete(Matrix *);

#endif

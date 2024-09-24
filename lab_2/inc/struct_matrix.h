#ifndef __STRUCT_MATRIX_H__
#define __STRUCT_MATRIX_H__

#include <stdlib.h>

typedef struct _matrix
{
    size_t rows, cols;
    float **data;
} matrix_t;

typedef enum
{
    M_OK = 0,
    M_MEM,
    M_INPUT,
    M_ERR,
    M_RANGE
} matrix_error_t;

//ALLOC
matrix_t *matrix_alloc(size_t rows, size_t cols);
void matrix_free(matrix_t **m);
//ENDALLOC

//INIT
matrix_error_t matrix_init(matrix_t *m);
//ENDINIT

//MANIP
matrix_error_t matrix_set(matrix_t *m, size_t i, size_t j, float val);
matrix_error_t matrix_get(const matrix_t *m, size_t i, size_t j, float *res);
matrix_error_t matrix_randomize(matrix_t *m, float min, float max);
//ENDMANIP

//OPERATIONS
matrix_t *matrix_mult(const matrix_t *A, const matrix_t *B);
matrix_t *matrix_mult_vinograd(const matrix_t *A, const matrix_t *B);
//ENDOPERATIONS

//OUTPUt FOR DEBUG
void matrix_print(const matrix_t *m);
//ENDOUTPUT

#endif

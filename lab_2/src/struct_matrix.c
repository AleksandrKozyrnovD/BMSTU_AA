#include <stdio.h>
#include <time.h>

#include "struct_matrix.h"


matrix_t *matrix_alloc(size_t rows, size_t cols)
{
    matrix_t *m = (matrix_t *)malloc(sizeof(matrix_t));
    if (!m)
    {
        return NULL;
    }
    float **data = (float **)malloc(sizeof(float *) * rows); //float pointers
    if (!data)
    {
        free(m);
        return NULL;
    }

    m->data = data;
    m->rows = rows;
    m->cols = cols;
    for (size_t i = 0; i < rows; i++)
    {
        // data[i] = (float *)malloc(sizeof(float) * cols);
        data[i] = (float *)calloc(cols, sizeof(float)); //autoinit
        if (!data[i])
        {
            matrix_free(&m);
            return NULL;
        }
    }

    // matrix_init(m);

    return m;
}


void matrix_free(matrix_t **m)
{
    if (!m || !(*m))
    {
        return;
    }
    float **data = (*m)->data;
    for (size_t i = 0; i < (*m)->rows; i++)
    {
        free(data[i]);
    }
    free(data);
    free(*m);
    *m = NULL;
}


matrix_error_t matrix_init(matrix_t *m)
{
    if (!m || m->rows == 0 || m->cols == 0)
    {
        return M_INPUT;
    }

    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            m->data[i][j] = 0.0f;
        }
    }

    return M_OK;
}

matrix_error_t matrix_randomize(matrix_t *m, float min, float max)
{
    if (!m || min >= max)
    {
        return M_INPUT;
    }

    srand(time(NULL));
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            m->data[i][j] = min + ((float)rand() / RAND_MAX) * (max - min);
        }
    }

    return M_OK;
}

matrix_error_t matrix_set(matrix_t *m, size_t i, size_t j, float val)
{
    if (!m || i >= m->rows || j >= m->cols)
    {
        return M_INPUT;
    }
    m->data[i][j] = val;

    return M_OK;
}


matrix_error_t matrix_get(const matrix_t *m, size_t i, size_t j, float *res)
{
    if (!res ||!m || i >= m->rows || j >= m->cols)
    {
        return M_INPUT;
    }
    *res = m->data[i][j];

    return M_OK;
}

matrix_t *matrix_mult(const matrix_t *A, const matrix_t *B)
{
    if (!A || !B || A->cols != B->rows)
    {
        return NULL;
    }
    matrix_t *C = matrix_alloc(A->rows, B->cols);
    if (!C)
    {
        return NULL;
    }
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < B->cols; j++)
        {
            for (size_t k = 0; k < A->cols; k++)
            {
                C->data[i][j] = C->data[i][j] + A->data[i][k] * B->data[k][j];
            }
        }
    }
    return C;
}


matrix_t *matrix_mult_vinograd(const matrix_t *A, const matrix_t *B)
{
    if (!A || !B || A->cols != B->rows)
    {
        return NULL;
    }
    size_t N = A->rows, M = A->cols, K = B->cols;
    float *MulH = (float *)calloc(N, sizeof(float));
    if (!MulH)
    {
        return NULL;
    }
    float *MulV = (float *)calloc(K, sizeof(float));
    if (!MulV)
    {
        free(MulH);
        return NULL;
    }
    matrix_t *C = matrix_alloc(A->rows, B->cols);
    if (!C)
    {
        free(MulH);
        free(MulV);
        return NULL;
    }
    for (size_t i = 0; i < N; i++)
    {
        for (size_t k = 0; k < M / 2; k++)
        {
            MulH[i] = MulH[i] + A->data[i][2 * k] * A->data[i][2 * k + 1];
        }
    }
    for (size_t i = 0; i < K; i++)
    {
        for (size_t k = 0; k < M / 2; k++)
        {
            MulV[i] = MulV[i] + B->data[2 * k][i] * B->data[2 * k + 1][i];
        }
    }
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < K; j++)
        {
            C->data[i][j] = -MulH[i] - MulV[j];
            for (size_t k = 0; k < M / 2; k++)
            {
                C->data[i][j] = C->data[i][j] +
                                              (A->data[i][2 * k + 1] + B->data[2 * k][j]) //*
                                            * (A->data[i][2 * k] + B->data[2 * k + 1][j]);
            }
        }
    }
    if (M % 2) //if M is odd
    {
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < K; j++)
            {
                C->data[i][j] = C->data[i][j] + A->data[i][M - 1] * B->data[M - 1][j];
            }
        }
    }
    free(MulH);
    free(MulV);
    return C;
}

matrix_t *matrix_mult_vinograd_opt(const matrix_t *A, const matrix_t *B)
{
    if (!A || !B || A->cols != B->rows)
    {
        return NULL;
    }
    size_t N = A->rows, M = A->cols, K = B->cols;
    size_t M2 = (M >> 1);
    float *MulH = (float *)calloc(N, sizeof(float));
    if (!MulH)
    {
        return NULL;
    }
    float *MulV = (float *)calloc(K, sizeof(float));
    if (!MulV)
    {
        free(MulH);
        return NULL;
    }
    matrix_t *C = matrix_alloc(A->rows, B->cols);
    if (!C)
    {
        free(MulH);
        free(MulV);
        return NULL;
    }
    float **data_C = C->data;
    float **data_A = A->data;
    float **data_B = B->data;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t k = 0; k < M2; k++)
        {
            MulH[i] += data_A[i][k << 1] * data_A[i][(k << 1) | 0x1];
        }
    }
    for (size_t i = 0; i < K; i++)
    {
        for (size_t k = 0; k < M2; k++)
        {
            MulV[i] += data_B[(k << 1)][i] * data_B[(k << 1) | 0x1][i];
        }
    }
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < K; j++)
        {
            data_C[i][j] -= (MulH[i] + MulV[j]);
            for (size_t k = 0; k < M2; k++)
            {
                data_C[i][j] += 
                                              (data_A[i][(k << 1) | 0x1] + data_B[(k << 1)][j]) //*
                                            * (data_A[i][(k << 1)] + data_B[(k << 1) | 0x1][j]);
            }
            if (M & 1) //if M is odd
            {
                data_C[i][j] += data_A[i][M - 1] * data_B[M - 1][j];
            }
        }
    }
    free(MulH);
    free(MulV);
    return C;
}

void matrix_print(const matrix_t *m)
{
    if (!m)
    {
        return;
    }
    float **data = m->data;
    size_t rows = m->rows, cols = m->cols;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            printf("%f ", data[i][j]);
        }
        printf("\n");
    }
}
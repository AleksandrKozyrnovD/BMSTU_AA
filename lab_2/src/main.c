#include "struct_matrix.h"
#include "experiment.h"


int main(void)
{
    get_time_interval(2, 20, 2, 1000);
    // printf("Hello, World!\n");

    // matrix_t *A = matrix_alloc(1, 4);
    // if (!A)
    // {
    //     return M_MEM;
    // }

    // matrix_t *B = matrix_alloc(4, 1);
    // if (!B)
    // {
    //     matrix_free(&A);
    //     return M_MEM;
    // }


    // matrix_set(A, 0, 0, 1.0f);
    // matrix_set(A, 0, 1, 2.0f);
    // matrix_set(A, 0, 2, 3.0f);
    // matrix_set(A, 0, 3, 4.0f);

    // matrix_set(B, 0, 0, 1.0f);
    // matrix_set(B, 1, 0, 2.0f);
    // matrix_set(B, 2, 0, 3.0f);
    // matrix_set(B, 3, 0, 4.0f);

    // matrix_t *C = matrix_mult_vinograd(A, B);

    // matrix_free(&C);
    // matrix_free(&A);
    // matrix_free(&B);

    return 0;
}
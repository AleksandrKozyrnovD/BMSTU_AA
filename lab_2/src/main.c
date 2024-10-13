#include "struct_matrix.h"
#include "experiment.h"
#include <stdio.h>

int main(void)
{
    // get_time_interval(2, 10, 1, 1000);
    // get_time_interval(10, 111, 11, 1000);

    get_time_interval(25, 300, 25, 100);

    // printf("Hello, World!\n");

    // matrix_t *A = matrix_alloc(5, 5);
    // if (!A)
    // {
    //     return M_MEM;
    // }

    // matrix_t *B = matrix_alloc(5, 5);
    // if (!B)
    // {
    //     matrix_free(&A);
    //     return M_MEM;
    // }

    // for (size_t i = 0; i < 5; i++)
    // {
    //     for (size_t j = 0; j < 5; j++)
    //     {
    //         matrix_set(A, i, j, i * 3 + j);
    //         matrix_set(B, i, j, i * 3 + j);
    //     }
    // }

    // matrix_t *C = matrix_mult_vinograd_opt(A, B);
    // printf("A:\n");
    // matrix_print(A);
    // printf("B:\n");
    // matrix_print(B);
    // printf("C:\n");
    // matrix_print(C);

    // matrix_free(&C);
    // matrix_free(&A);
    // matrix_free(&B);

    return 0;
}
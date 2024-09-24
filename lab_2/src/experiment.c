#include "experiment.h"
#include "struct_matrix.h"

#include <stdio.h>

void calculate_matrix(void)
{
    return;
}


static int get_time_matrix(size_t matrix_size, size_t repeat, double *res)
{
    if (!res) //null pointer
    {
        // printf("No res!\n");
        return 121; //arbitrary number
    }

    clock_t all_clocks = 0, start, end;
    matrix_t *C = NULL;

    matrix_t *A = matrix_alloc(matrix_size, matrix_size);
    if (!A)
    {
        // printf("Cant allocate to A\n");
        return M_MEM;
    }
    matrix_t *B = matrix_alloc(matrix_size, matrix_size);
    if (!B)
    {
        // printf("Cant allocate to B\n");
        matrix_free(&A);
        return M_MEM;
    }

    for (size_t i = 0; i < repeat; i++)
    {
        matrix_randomize(A, -100, 100);
        matrix_randomize(B, -100, 100);

        start = clock();
        C = matrix_mult(A, B);
        end = clock();

        all_clocks += end - start;
        matrix_free(&C);
    }

    *res = (double)(all_clocks) / repeat;

    matrix_free(&A);
    matrix_free(&B);

    return M_OK;
}

int get_time_interval(size_t size_start, size_t size_end, size_t size_step, size_t repeat)
{
    double res;
    int error = 0;
    for (size_t size = size_start; !error && size <= size_end; size += size_step)
    {
        if (!(error = get_time_matrix(size, repeat, &res)))
        {
            printf("size = %zu and time = %f\n", size, res);
        }
        else
        {
            printf("Error at size = %zu, error = %d\n", size, error);
        }
    }

    return error;
}
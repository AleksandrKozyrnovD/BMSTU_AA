#ifndef __EXPIRIMENT_H__
#define __EXPERIMENT_H__

#include <time.h>
#include "struct_matrix.h"


void calculate_matrix(void);

int get_time_interval(size_t size_start, size_t size_end,
                         size_t size_step, size_t repeat);


#endif

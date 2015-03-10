#ifndef CILK_H_
#define CILK_H_

#include "cilk_queue.h"

struct CilkGlobalConsts
{
    int num_threads;
    CilkDeque *thread_deques;
};

CilkGlobalConsts *cilk_global_const = {0, NULL};

void cilk_init();
void cilk_spawn(void *(*start_routine), void *args);
void cilk_join();

#endif

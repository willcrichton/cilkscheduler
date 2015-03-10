#include "cilk.h"

void cilk_spawn(void *(*start_routine), void *args)
{
    pid_t my_tid = gettid();
}

void cilk_join(void *(*start_routine), void *args)
{

}

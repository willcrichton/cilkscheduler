#ifndef CILK_WORK_QUEUE_H_
#define CILK_WORK_QUEUE_H_

#include "cilk_queue_util.h"
#define CILK_QUEUE_INIT_SIZE 16

/* CilkDeque
 * A double ended work-stealing queue based on circular buffers
 * The push and pop operations are guaranteed to be carried out
 * by only 1 thread at a time while the take operation is safe
 * for multiple threads
 */
class CilkDeque
{
public:
    CilkDeque();
    ~CilkDeque();

    /* push: push a cilk_task to the top of the deque
     */
    void push(cilk_task *);

    /* pop: pop a cilk_task from the top of the deque
     * @return: NULL if task queue has <= 0 task, else returns a
     *          pointer to the top task in the deque
     */
    cilk_task *pop();

    /* take: take a cilk_task from the bottom of the deque
     * @return: NULL if task queue has <= 1 task, else returns a
     *          pointer to the bottom task in the deque
     */
    cilk_task *take();

private:
    CilkCircularArray *_queue;
    volatile uint64_t _top;
    volatile uint64_t _bottom;
};

#endif

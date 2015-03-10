#include "cilk_queue.h"

CilkDeque::CilkDeque() :
    _bottom(0), _top(0)
{
    _queue = new CilkCircularArray(CILK_QUEUE_INIT_SIZE);
}

CilkDeque::~CilkDeque()
{
    delete _queue;
}

void CilkDeque::push(cilk_task *task)
{
    uint64_t top = _top;
    uint64_t bottom = _bottom;
    uint64_t num_tasks = top - bottom + 1;

    if (num_tasks >= _queue->size())
    {
        CilkCircularArray *temp = _queue;
        _queue = _queue->copy_double_size();
        delete temp;
    }

    _queue->set(_top + 1, task);
    _top++;

    //Make sure that every processor sees that the top is updated
    __sync_synchronize();
}

cilk_task *CilkDeque::pop()
{
    uint64_t top,bottom;
    uint64_t new_bottom;

    size_t num_tasks;

    // Only a pop from the local processor can update top
    // so we dont need to guarantee that the top is consistent
    // using a CAS
    _top--;
    __sync_synchronize();

    // However, another processor's take can modify the bottom
    bottom = _bottom;
    top = _top + 1;
    num_tasks = top - bottom;

    if (num_tasks < 1)
    {
        //Deque is empty
        _top = _bottom;
        return NULL;
    }
    // In this case, a take and pop can operate on the same element
    // at the same time
    else if(num_tasks == 1) {
        cilk_task *task = _queue->get(top);
        // If take has already taken the takesk, then _bottom == top
        if (!__sync_bool_compare_and_swap(&_bottom, bottom, _top))
        {
            //Take already took the last element
            return NULL;
        }
        else {
            return task;
        }
    }
    else {
        return _queue->get(top);
    }
}

cilk_task *CilkDeque::take()
{
    uint64_t top, bottom;
    uint64_t new_bottom;
    size_t num_tasks;

    bottom = _bottom;

    new_bottom = bottom + 1;
    num_tasks = _top - bottom;

    if (num_tasks <= 0)
    {
        return NULL;
    }

    if (!__sync_bool_compare_and_swap(&_bottom, bottom, new_bottom))
    {
        return NULL;
    }
    else {
        return _queue->get(new_bottom);
    }
}

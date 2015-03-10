#include "cilk_queue.h"

#include <cstdlib>
#include <ctime>
#include <pthread.h>
/* -------------------     RESIZE    -------------------- *
 * Test Resizing capabilities of the deque
 * -------------------------------------------------------*/
void resize_test()
{
    printf("Testing ST Deque Resize\n");
    CilkDeque *deque = new CilkDeque();
    cilk_task *task;
    for (int i = 0; i < CILK_QUEUE_INIT_SIZE; i++)
    {
        task = new cilk_task;
        task->id = i;
        deque->push(task);
    }
    delete deque;
    printf("Passed ST Deque Resize\n");
}


/* -------------------     ST Battery    ---------------- *
 * Test basic sequences of all push followed by all pop or
 * all take in a single threaded setting
 * -------------------------------------------------------*/
bool battery_iter(uint64_t count, bool push)
{
    CilkDeque *deque = new CilkDeque();
    cilk_task *task;
    for (int i = 0; i < count; i++)
    {
        task = new cilk_task;
        task->id = i;
        deque->push(task);
    }

    if (push)
    {
        for (int i = 0; i < count; i++)
        {
            task = deque->pop();
            if (task == NULL)
            {
                printf("ERROR: PP_ST[%u] - Expected %u, got NULL\n", count, count - i - 1);
                return false;
            }
            if (task->id != count-i-1)
            {
                printf("ERROR: PP_ST[%u] - Expected %u, got %d\n", count, count - i - 1, task->id);
                return false;
            }
            delete task;
        }
    } else {
        for (int i = 0; i < count; i++)
        {
            task = deque->take();
            if (task == NULL)
            {
                printf("ERROR: PP_ST[%u] - Expected %u, got NULL\n", count, i);
                return false;
            }
            if (task->id != i)
            {
                printf("ERROR: PP_ST[%u] - Expected %u, got %d\n", count, i, task->id);
                return false;
            }
            delete task;
        }
    }

    delete deque;
    return true;
}

void battery_test()
{
    uint64_t min_count = 2;
    uint64_t max_count = 128;
    printf("Testing Battery ST Push-Pop\n");
    for (uint64_t count = min_count; count < max_count; count *= 2)
    {
        if (!battery_iter(count, true))
        {
            printf("Failed PP_ST[%u]\n", count);
            return;
        }
    }
    printf("Passed ST Battery Push-Pop\n");
    printf("Testing Battery ST Push-Take\n");
    for (uint64_t count = min_count; count < max_count; count *= 2)
    {
        if (!battery_iter(count, false))
        {
            printf("Failed PP_ST[%u]\n", count);
            return;
        }
    }
    printf("Passed ST Battery Push-Take\n");
}

/* -------------------     ST Random    ----------------- *
 * Test random sequences of push, take, and pop in a single
 * threaded setting
 * -------------------------------------------------------*/
void random_operation_iter()
{
    CilkDeque *deque = new CilkDeque();
    cilk_task *task;

    unsigned int iterations = CILK_QUEUE_INIT_SIZE;
    srand(time(0));

    for (unsigned int i = 0; i < iterations; i++)
    {
        int command = rand() % 4;
        if (command == 0)
        {
            task = deque->pop();
            if (task != NULL)
                delete task;
        } else if (command == 1)
        {
            task = deque->take();
            if (task != NULL)
                delete task;
        } else {
            task = new cilk_task;
            deque->push(task);
        }
    }
}

void random_operation_test()
{
    printf("Testing ST Random Operation Sequence\n");
    for (int i = 0; i < 1024; i++)
    {
        random_operation_iter();
    }
    printf("Passed ST Random Operation Sequence\n");
}

/* -------------------     MT Push/Take    -------------- *
 * Runs multiple threads and tests functionality of push with
 * multiple take requests
 * -------------------------------------------------------*/

void *mt_push_take_thread_fun(void *args)
{
    printf("MT Push/Take Thread Active\n");
    CilkDeque *deque = (CilkDeque *) args;
    int count = 0;
    int tries = 0;
    cilk_task *task;
    while (count < 256)
    {
        if ((task = deque->take()) != NULL)
        {
            count++;
            delete task;
        }
    }
    return NULL;
}

void mt_push_take_test()
{
    int t_count = 4;

    printf("Testing MT take after push\n");
    CilkDeque *deque = new CilkDeque();
    for (int i = 0; i < 256 * t_count; i++)
    {
        cilk_task *temp = new cilk_task;
        deque->push(temp);
    }

    pthread_t t_id[t_count];
    for (int i= 0; i < t_count-1; i++)
    {
        pthread_create(&t_id[i], NULL, mt_push_take_thread_fun, (void *) deque);
    }
    mt_push_take_thread_fun((void *) deque);

    for (int i = 0; i < t_count - 1; i++)
    {
        pthread_join(t_id[i], NULL);
    }

    printf("Finished Testing MT take after push\n");
    std::cout << std::endl;

    delete deque;

    printf("Testing MT take while push\n");
    deque = new CilkDeque();
    for (int i= 0; i < t_count; i++)
    {
        pthread_create(&t_id[i], NULL, mt_push_take_thread_fun, (void *) deque);
    }

    for (int i = 0; i < 256 * t_count; i++)
    {
        cilk_task *temp = new cilk_task;
        deque->push(temp);
    }

    for (int i = 0; i < t_count; i++)
    {
        pthread_join(t_id[i], NULL);
    }

    printf("Finished Testing MT take while push\n");
}

int main()
{
    //Single Threaded Tests
    resize_test(); std::cout << std::endl;
    battery_test(); std::cout << std::endl;
    random_operation_test(); std::cout << std::endl;

    //Multi Threaded Tests
    mt_push_take_test(); std::cout << std::endl;
    mt_real_usage_test(); std::cout << std::endl;

    return 1;
}

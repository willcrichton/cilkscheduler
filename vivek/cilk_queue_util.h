#ifndef CILK_QUEUE_UTIL_H_
#define CILK_QUEUE_UTIL_H_

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

typedef unsigned long long uint64_t;

struct cilk_task
{
    int id;
    void *result;
    cilk_task *parent;
    cilk_task **children;
};

class CilkCircularArray
{
public:
    CilkCircularArray(uint64_t capacity) :
        capacity(capacity) {
        buffer = new cilk_task*[capacity];
        memset(buffer, 0, sizeof(cilk_task *) * capacity);
    }

    ~CilkCircularArray() {
        delete buffer;
    }

    inline cilk_task *get(uint64_t idx) {
        return buffer[idx % capacity];
    }

    inline void set(uint64_t idx, cilk_task *task) {
        buffer[idx % capacity] = task;
    }

    inline CilkCircularArray *copy_double_size()
    {
        if (capacity * 2 >= (1 << 31))
        {
            std::cerr << "Cilk Queue cannot deal with more than 2^31 tasks" << std::endl;
            exit(1);
        }

        CilkCircularArray *result = new CilkCircularArray(capacity * 2);
        memcpy(result->buffer, buffer, sizeof(cilk_task *) * capacity);
        return result;
    }

    inline uint64_t size()
    {
        return capacity;
    }

private:
    uint64_t capacity;
    cilk_task **buffer;
};

#endif

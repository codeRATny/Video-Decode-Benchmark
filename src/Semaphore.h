

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
    explicit Semaphore(size_t initial = 0)
    {
        _count = 0;
    }

    ~Semaphore()
    {
    }

    void post(size_t n = 1)
    {
        std::unique_lock<std::recursive_mutex> lock(_mutex);
        _count += n;
        if (n == 1)
        {
            _condition.notify_one();
        }
        else
        {
            _condition.notify_all();
        }
    }

    void wait()
    {
        std::unique_lock<std::recursive_mutex> lock(_mutex);
        while (_count == 0)
        {
            _condition.wait(lock);
        }
        --_count;
    }

private:
    size_t _count;
    std::recursive_mutex _mutex;
    std::condition_variable_any _condition;
};

#endif

#include <chrono>

#include "Semaphore.h"
#include "ffmpeg-cpp.hpp"

extern Semaphore sync_sem;

namespace benchmark
{
    void video_decode(std::string filename, int cores_count, int thread_num = -1);
}
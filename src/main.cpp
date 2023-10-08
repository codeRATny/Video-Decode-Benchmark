#include "Bench.hpp"

std::array<std::string, 5> videofile_names
{
    "4k.ts",
    "quadhd.ts",
    "fullhd.ts",
    "hd.ts",
    "sd.ts"
};

std::array<std::string, 5> resolution
{
    "3840x2160",
    "2560x1440",
    "1920x1080",
    "1280x720",
    "720x480"
};

std::vector<uint64_t> core_usec;

int main(int argc, char **argv)
{
    av_log_set_level(AV_LOG_QUIET);

    if (argc < 2)
    {
        std::cout << "Specify the directory where the video files are located" << std::endl;
        std::cout << "4k.ts     - 3840x2160" << std::endl;
        std::cout << "quadhd.ts - 2560x1440" << std::endl;
        std::cout << "fullhd.ts - 1920x1080" << std::endl;
        std::cout << "hd.ts     - 1280x720" << std::endl;
        std::cout << "sd.ts     - 720x480" << std::endl;
        exit(0);
    }

    std::vector<std::thread> threads;

    for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        core_usec.emplace_back(0);
        threads.emplace_back();
    }

    for (size_t i = 0; i < videofile_names.size(); i++)
    {
        std::cout << "Resolution: " << resolution[i] << std::endl;
        std::cout << "Max time: 2s" << std::endl;

        std::cout << "\tDecoding by single core" << std::endl;

        for (int j = 0; j < std::thread::hardware_concurrency(); j++)
        {
            threads[j] = std::thread(benchmark::video_decode, std::string(argv[1]) + videofile_names[i], 1, j);
        }

        sync_sem.post(std::thread::hardware_concurrency());

        for (int j = 0; j < std::thread::hardware_concurrency(); j++)
        {
            threads[j].join();
        }

        std::cout << "\tDecoding by quarter cores" << std::endl;
        benchmark::video_decode(std::string(argv[1]) + videofile_names[i], std::thread::hardware_concurrency() / 4);

        std::cout << "\tDecoding by half cores" << std::endl;
        benchmark::video_decode(std::string(argv[1]) + videofile_names[i], std::thread::hardware_concurrency() / 2);

        std::cout << "\tDecoding by all cores" << std::endl;
        benchmark::video_decode(std::string(argv[1]) + videofile_names[i], std::thread::hardware_concurrency() / 1);

        std::cout << "\tDecoding by x2 cores" << std::endl;
        benchmark::video_decode(std::string(argv[1]) + videofile_names[i], std::thread::hardware_concurrency() * 2);
    }

    return 0;
}
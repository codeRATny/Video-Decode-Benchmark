#include "Bench.hpp"

Semaphore sync_sem;
std::mutex cout_mtx;

void benchmark::video_decode(std::string filename, int cores_count, int thread_num)
{
    if (thread_num != -1)
        sync_sem.wait();

    auto start = std::chrono::high_resolution_clock::now();

    FFmpegFormatCtx::Ptr format_ctx = std::make_shared<FFmpegFormatCtx>();
    FFmpegCodecCtx::Ptr codec_ctx = std::make_shared<FFmpegCodecCtx>();
    FFmpegPacket::Ptr packet;

    if (format_ctx->Open(filename) == false)
    {
        std::cout << "File open error" << std::endl;
        exit(-1);
    }

    auto video_streams_idx = format_ctx->FindVideoStreamIDX();

    if (video_streams_idx.empty())
    {
        std::cout << "No video stream in file" << std::endl;
        exit(-1);
    }

    std::string all_core = std::to_string(cores_count);
    auto codec_id = format_ctx->GetCodecID(video_streams_idx[0]);
    codec_ctx->Open(codec_id, all_core);

    int frame_counter = 0;
    while (true)
    {
        if (format_ctx->ReadPacket(packet) != 0)
        {
            break;
        }

        if (packet->GetStreamIdx() == video_streams_idx[0])
        {
            auto frame = codec_ctx->Decode(packet);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (thread_num == -1)
        std::cout << "\t\tResult: " << (double)duration.count() / 1000 << "s" << std::endl;
    else
    {
        std::lock_guard<std::mutex> lk(cout_mtx);
        std::cout << "\t\tThread(" << thread_num << ") result: " << (double)duration.count() / 1000 << "s" << std::endl;
    }
}
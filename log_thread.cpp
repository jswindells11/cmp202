#include "log_thread.h"
#include "mandelbrot.h"

#include <iostream>

typedef std::chrono::steady_clock Clock;

void log_thread(const std::string& filepath, const int& thread_count, std::binary_semaphore& finish_sem)
{
    std::fstream fs;
    fs.open(filepath, std::fstream::out | std::fstream::app);
    std::cout << "Logging to " << filepath << std::endl;

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto start = Clock::now();

    cpuinfo_initialize();

    fs << "Mandelbrot Generation started on ";
    fs << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << std::endl;
    fs << "CPU: " << cpuinfo_get_package(0)->name << std::endl;
    fs << "Max thread count: " << cpuinfo_get_cores_count() << std::endl;
    fs << "Thread count: " << thread_count << std::endl;
    fs << "Image size: " << WIDTH << "x" << HEIGHT << std::endl;

    finish_sem.acquire();
    auto end = Clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    fs << "Mandelbrot Generation finished in " << duration.count() << "ms" << std::endl;
    fs.close();
}
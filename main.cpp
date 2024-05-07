#include <condition_variable>
#include <cpuinfo.h>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <semaphore>
#include <sstream>
#include <thread>
#include <vector>

#include "mandelbrot.h"
#include "log_thread.h"
#include "tga.h"

// Function to create a filepath dependent on the platform and current time
std::string create_filepath()
{
    std::ostringstream oss;
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

#ifdef _WIN32
    oss << "output\\";
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H-%M-%S") << "\\";
#else
    oss << "output/";
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H-%M-%S") << "/";
#endif

    std::filesystem::create_directories(oss.str());

    return oss.str();
}

int main() {
    // Gather CPU information
    cpuinfo_initialize();
    std::cout << "CPU: " << cpuinfo_get_package(0)->name << std::endl;
    auto max_threads = cpuinfo_get_cores_count();
    std::cout << "Thread count: " << max_threads << std::endl;

    // Ask how many threads to use
    std::cout << "Enter how many threads to use: ";
    int use_threads = 0;
    while (true)
    {
        std::cin >> use_threads;
        if (use_threads <= 0 || use_threads > max_threads)
        {
            std::cout << "Invalid input." << std::endl;
        }
        else
        {
            break;
        }
    }
    auto now = std::chrono::system_clock::now();
    std::cout << "Starting Mandelbrot generation" << std::endl;

    std::string output_path = create_filepath();
    std::binary_semaphore finish_sem(0);

    // Start the logging thread
    std::thread log(log_thread, output_path + "log.txt", use_threads, std::ref(finish_sem));

    std::mutex count_lock; // lock for thread_count
    std::atomic<int> thread_count(0); // number of threads that have finished
    std::condition_variable cv; // condition variable to wait for threads to finish

    uint32_t image[HEIGHT][WIDTH];
    std::vector<std::thread> threads;
    threads.reserve(use_threads);
    int chunk_size = HEIGHT / use_threads;
    for (int i = 0; i < use_threads; i++)
    {
        threads.emplace_back(compute_mandelbrot, std::ref(image), std::ref(count_lock),
                             std::ref(thread_count), std::ref(cv), -2, 1, 1.125, -1.125, i * chunk_size,
                             (i + 1) * chunk_size);
    }

    std::unique_lock<std::mutex> lock(count_lock);
    while (thread_count != use_threads)
    {
        cv.wait(lock);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    // Write the image to a file
    write_tga(output_path + "mandelbrot.tga", image);

    // Tell the logging thread we're done
    finish_sem.release();
    log.join();

    std::cout << "All done" << std::endl;
    return 0;
}

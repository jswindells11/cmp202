#include <chrono>
#include <cpuinfo.h>
#include <fstream>
#include <iomanip>
#include <semaphore>
#include <string>

#ifndef MANDELBROT_LOG_THREAD_H
#define MANDELBROT_LOG_THREAD_H

void log_thread(const std::string& filepath, const int& thread_count, std::binary_semaphore& finish_sem);

#endif //MANDELBROT_LOG_THREAD_H

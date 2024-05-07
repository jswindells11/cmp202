#include <atomic>
#include <complex>
#include <condition_variable>
#include <cstdint>
#include <mutex>

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#define WIDTH 1920
#define HEIGHT 1200
#define MAX_ITERATIONS 500

void compute_mandelbrot(uint32_t (&image)[HEIGHT][WIDTH], std::mutex& count_lock, std::atomic<int>& thread_count,
                        std::condition_variable& cv, double left, double right, double top, double bottom, int y_start,
                        int y_end);

#endif //MANDELBROT_MANDELBROT_H

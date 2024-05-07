#include "mandelbrot.h"

using std::complex;

void compute_mandelbrot(uint32_t (&image)[HEIGHT][WIDTH], std::mutex& count_lock, std::atomic<int>& thread_count,
                        std::condition_variable& cv, double left, double right, double top, double bottom, int y_start,
                        int y_end)
{
    for (int y = y_start; y < y_end; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            // Work out the point in the complex plane that
            // corresponds to this pixel in the output image.
            complex<double> c(left + (x * (right - left) / WIDTH),
                              top + (y * (bottom - top) / HEIGHT));

            // Start off z at (0, 0).
            complex<double> z(0.0, 0.0);

            // Iterate z = z^2 + c until z moves more than 2 units
            // away from (0, 0), or we've iterated too many times.
            int iterations = 0;
            while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
            {
                z = (z * z) + c;

                ++iterations;
            }

            if (iterations == MAX_ITERATIONS)
            {
                // z didn't escape from the circle.
                // This point is in the Mandelbrot set.
                image[y][x] = 0x000000; // black
            }
            else
            {
                // z escaped within less than MAX_ITERATIONS
                // iterations. This point isn't in the set.
                image[y][x] = 0xFFFFFF; // white
            }
        }
    }

    count_lock.lock();
    thread_count++;
    cv.notify_one();
    count_lock.unlock();
}
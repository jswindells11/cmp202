#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include "mandelbrot.h"

#ifndef MANDELBROT_TGA_H
#define MANDELBROT_TGA_H

void write_tga(std::string filename, uint32_t (&image)[HEIGHT][WIDTH]);

#endif //MANDELBROT_TGA_H

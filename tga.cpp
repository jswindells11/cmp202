#include "tga.h"

using std::ofstream;

void write_tga(std::string filename, uint32_t (&image)[HEIGHT][WIDTH])
{
    ofstream outfile(filename, ofstream::binary);

    uint8_t header[18] = {
            0, // no image ID
            0, // no colour map
            2, // uncompressed 24-bit image
            0, 0, 0, 0, 0, // empty colour map specification
            0, 0, // X origin
            0, 0, // Y origin
            WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
            HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
            24, // bits per pixel
            0, // image descriptor
    };
    outfile.write((const char *)header, 18);

    // Clang-Tidy: Use range-based for loop instead
    for (auto & y : image)
    {
        for (unsigned int x : y)
        {
            uint8_t pixel[3] = {
                    static_cast<uint8_t>(x & 0xFF), // blue channel
                    static_cast<uint8_t>((x >> 8) & 0xFF), // green channel
                    static_cast<uint8_t>((x >> 16) & 0xFF), // red channel
            };
            outfile.write((const char *)pixel, 3);
        }
    }

    outfile.close();
    if (!outfile)
    {
        // An error has occurred at some point since we opened the file.
        std::cout << "Error writing to " << filename << std::endl;
        exit(1);
    }
}
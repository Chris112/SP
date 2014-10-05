#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Image.hpp"
#include <fstream>
#include <string.h>
#include <stdlib.h>


Image* flip(Image *image, char *direction);
Image* copy (Image *image);
Image* resize(Image *image, char* factor);
Image* tile(int argc, char* image[]);




#endif


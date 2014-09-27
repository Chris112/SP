#ifndef FILEIO
#define FILEIO_H

#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Image.hpp"


std::string readFormat(std::ifstream &file);
void readSize(std::ifstream &file, int* rows, int* cols);
int readColour(std::ifstream &file);
void writeFile(Image *image, std::string location);


#endif
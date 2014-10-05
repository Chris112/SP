#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Image.hpp"

void appendCharToCharArray(char a);
std::string readFormat(std::ifstream &file);
void readSize(std::ifstream &file, int* rows, int* cols);
int readColour(std::ifstream &file);
void writeFile(Image *image);
Image* loadImage(char* location);
void getImage(std::ifstream& f, int *buffer,int rows, int cols);
void setOutputFileLocation(char *outLocation);


#endif


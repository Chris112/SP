/*
 * FILE: Image.cpp
 * AUTHOR: Christopher Webb
 * DATE CREATED: 5/10/14
 * DESCRIPTION:
 * Image.cpp is a class file that holds the blueprints of an Image object.
 */


#include "Image.hpp"
#include <iostream>

 #include <stdio.h>




Image::Image(int inRows, int inCols, int inMaxColours){
	rows = inRows;
	cols = inCols;
	maxColours = inMaxColours;

	red = NULL;//new int[rows*cols];
	green = NULL;//new int[rows*cols];
	blue = NULL;//new int[rows*cols]; 

}

Image::Image(){

}

Image::Image(Image* inImage){
 rows = inImage->getRows();
 cols = inImage->getCols();
 maxColours = inImage->getColours();
}

Image::~Image(){
	printf("Freeing an image.\n");
	delete[] red;
	delete[] green;
	delete[] blue;
}

int Image::getRows(){
	return rows;
}

int Image::getCols(){
	return cols;
}

int* Image::getRed(){
	return red; 
}

int* Image::getBlue(){
	return blue; 
}

int* Image::getGreen(){
	return green; 
}

void Image::setRed(int *inRed){
	red = inRed; 
}

void Image::setBlue(int *inBlue){
	blue = inBlue; 
}

void Image::setGreen(int *inGreen){
	green = inGreen; 
}

void Image::setColours(int inColours){
	maxColours = inColours;
}

int Image::getColours(){
	return maxColours;
}

void Image::setRows(int inRows){
	rows = inRows;
}

void Image::setCols(int inCols){
	cols = inCols;
}


#include "Image.hpp"




Image::Image(int inRows, int inCols, int inMaxColours){
	rows = inRows;
	cols = inCols;
	maxColours = inMaxColours;

	red = new int[rows*cols];
	green = new int[rows*cols];
	blue = new int[rows*cols];

}

Image::~Image(){
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

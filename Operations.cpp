#include "Operations.hpp"
#include "Image.hpp"
#include <iostream>

//copy flip resize tile
//./CCSEPAssignment copy examples/computing.ppm output/computing-copy.ppm
//./CCSEPAssignment flip h examples/computing.ppm output/computing-vflip.ppm

//./CCSEPAssignment resize 2 examples/pavilion.ppm output/pavilion-double.ppm
//./CCSEPAssignment resize 0.5 examples/pavilion.ppm output/pavilion-half.ppm



/* FUNCTION: flip
 * IMPORT: image to flip, direction to flip ('v' or 'h'), number of rows in image, number of columns in image.
 * DESCRIPTION: flip an image through the middle either horizontally or vertical.
 * TODO: probably make a new image rather than flip the original.
 */
Image* flip(Image *image, char *direction){
	int rows = image->getRows();
	int cols = image->getCols();
	int maxColours = image->getColours();
	Image *newImage = new Image(rows, cols, maxColours);

	int *red = new int[rows*cols];
	int *green = new int[rows*cols];
	int *blue = new int[rows*cols];

	memcpy(&image->getRed()[0], &red[0], sizeof(int)*rows*cols);
	memcpy(&image->getGreen()[0], &green[0], sizeof(int)*rows*cols);
	memcpy(&image->getBlue()[0], &blue[0], sizeof(int)*rows*cols);



	int tempVar;
	 if (strcmp(direction, "h") == 0){
	 	std::cout << "Flipping image horizontally" << std::endl; 
	 	// user wants to horizontally flip imported image
	 	/*for(int row = 0; row < rows; ++row) {
		    for(int column = 0; column < cols/2; ++column) {
		    	tempVar = image[row*cols + column];
		        image[row*cols + column] = image[row*cols + (cols-1 - column)];
		        image[row*cols + (cols-1 - column)] = tempVar;
		    }
		}*/


		    for (int i = 0; i < rows * (cols*3); i++) {

		    }





	} else if (strcmp(direction, "v") == 0) {

	 	std::cout << "Flipping image vertically" << std::endl;
		// user wants to vertically flip imported image
		int *tempBuffer = new int[cols];
		for (int i=0;i<rows/2;i++) {

			// copy first row to buffer
			//std::cout << "Copying image[" << (i*cols) << "] to buffer." << std::endl;
			memcpy(tempBuffer, &image[i*cols], cols*sizeof(int));
			
			// set first row to last row
			//std::cout << "Setting image[" << (i*cols) << "] to image[" << (rows-1-i)*cols << "]." << std::endl;
			memcpy(&image[i*cols], &image[(rows-1-i)*cols], cols*sizeof(int));
			
			// set last row to buffer
			//std::cout << "Setting image[" << (rows-1-i)*cols << "] to what was in the buffer." << std::endl;
			memcpy(&image[(rows-1-i)*cols], tempBuffer, cols*sizeof(int));
		}
		delete tempBuffer;

	} else {
		std::cout << "NOT FLIPPING IMAGE CUZ INVALID ARG: " << direction << std::endl;
	}

		//for each row
			// set ith element of each row to max - ith element and vice versa
			//int temp = [cols-i-1]
			// image[i] = [cols-i-1]
			// image[cols-i-1] = temp
		// end for
	// else if direction is h
		// for each row


}

Image* copy (Image *image) {
	std::cout << "Copying image." << std::endl;
	int rows = image->getRows();
	int cols = image->getCols();
	Image *newImage = new Image(rows, cols, image->getColours());

	int *newRed = new int[rows*cols];
	int *newBlue = new int[rows*cols];
	int *newGreen = new int[rows*cols];

	memcpy(image->getRed(), newRed, rows*cols);
	memcpy(image->getBlue(), newBlue, rows*cols);
	memcpy(image->getGreen(), newGreen, rows*cols);
	
	newImage->setRed(newRed);
	newImage->setBlue(newBlue);
	newImage->setGreen(newGreen);
	return newImage;
}
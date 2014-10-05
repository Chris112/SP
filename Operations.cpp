/*
 * FILE: Operations.cpp
 * AUTHOR: Christopher Webb
 * DATE CREATED: 5/10/14
 * DESCRIPTION:
 * Operations.cpp is responsible for performing image operations. Possible operations incluide
 * flip, resize, copy and tile. Each operation creates a new image as to not modify the original.
 */

#include <iostream>
#include <math.h>

#include "Operations.hpp"
#include "Image.hpp"
#include "FileIO.hpp"




/* FUNCTION: flip
 * IMPORT: image to flip, direction to flip ('v' or 'h')
 * DESCRIPTION: 
 * flip an image through the middle either horizontally or vertical.
 * EXAMPLE: ./CCSEPAssignment flip h examples/computing.ppm output/computing-vflip.ppm
 */
 Image* flip(Image *image, char *direction){

 	int rows = image->getRows();
 	int cols = image->getCols();
 	int maxColours = image->getColours();
 	Image *newImage = new Image(rows, cols, maxColours);

 	int *newRed = new int[rows*cols];
 	int *newGreen = new int[rows*cols];
 	int *newBlue = new int[rows*cols];

 	int *oldRed = image->getRed();
 	int *oldGreen = image->getGreen();
 	int *oldBlue = image->getBlue();


 	if (strcmp(direction, "h") == 0){
 		printf("Flipping image horizontally.\n");

 		for (int i=0;i<rows;i++) {
 			memcpy(&newRed[i*cols], &oldRed[cols*rows-((i+1)*cols)], sizeof(int)*cols);
 			memcpy(&newGreen[i*cols], &oldGreen[cols*rows-((i+1)*cols)], sizeof(int)*cols);
 			memcpy(&newBlue[i*cols], &oldBlue[cols*rows-((i+1)*cols)], sizeof(int)*cols);

 		}

 		newImage->setRed(newRed);
 		newImage->setGreen(newGreen);
 		newImage->setBlue(newBlue);

 	} else if (strcmp(direction, "v") == 0) {
 		printf("Flipping image vertically.\n");

 		for (int i=0;i<rows;i++) {
 			for (int j=0;j<cols;j++) {
 				newRed[i*cols+j] = oldRed[i*cols+cols-1-j];
 				newGreen[i*cols+j] = oldGreen[i*cols+cols-1-j];
 				newBlue[i*cols+j] = oldBlue[i*cols+cols-1-j];

 			}
 		}

 		newImage->setRed(newRed);
 		newImage->setGreen(newGreen);
 		newImage->setBlue(newBlue);


 	} else {
	/* 
	 * EXPLOIT ID  : 4a
	 * EXPLOIT KIND: Memory leak
	 * DESCRIPTION : newImage, newRed, newBlue, newGreen are all set to NULL before memory is returned to OS.
	 */
	 printf("Unable to perform flip operation as '%s' is an invalid argument.\n", direction);
	 newImage = NULL;
	}


	newRed = oldRed = newGreen = oldGreen = newBlue = oldBlue = NULL;

	return newImage;

}



/* FUNCTION: copy
 * IMPORT: image to copy
 * DESCRIPTION: 
 * Create a copy of an image.
 * EXAMPLE: ./CCSEPAssignment copy examples/computing.ppm output/computing-copy.ppm
 */
Image* copy (Image *image) {
	printf("Copying image.\n");
	int rows = image->getRows();
	int cols = image->getCols();
	Image *newImage = new Image(rows, cols, image->getColours());

	int *newRed   = new int[rows*cols];
	int *newBlue  = new int[rows*cols];
	int *newGreen = new int[rows*cols];

	memcpy(&newRed[0],   &image->getRed()[0],   sizeof(int)*rows*cols);
	memcpy(&newBlue[0],  &image->getBlue()[0],  sizeof(int)*rows*cols);
	memcpy(&newGreen[0], &image->getGreen()[0], sizeof(int)*rows*cols);
	
	newImage->setRed(newRed);
	newImage->setBlue(newBlue);
	newImage->setGreen(newGreen);

	newRed = NULL;
	newGreen = NULL;
	newBlue = NULL;
	return newImage;
}


// 
/* FUNCTION: resize
 * IMPORT: image to copy and a factor to resize it by
 * DESCRIPTION: 
 * Resize an image using nearest neighbour interpolation.
 * EXAMPLE: ./CCSEPAssignment resize 0.5 examples/pavilion.ppm output/pavilion-half.ppm
 * Implementation guided by example at http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/
 */
Image* resize(Image *image, char* inFactor){
	printf("Resizing image by a factor of %s.\n", inFactor);
	Image *newImage = new Image();

	// convert factor to float
	float factor = atof(inFactor);	 

	int w1 = image->getCols();
	int h1 = image->getRows();
	int w2 = factor * image->getCols();
	int h2 = factor * image->getRows();
	//newImage = new Image(h2, w2, image->getColours());
	newImage->setRows(h2);
	newImage->setCols(w2);
	newImage->setColours(image->getColours());

	int *newRed   = new int[h2*w2];
	int *newBlue  = new int[h2*w2];
	int *newGreen = new int[h2*w2];

	int x_ratio = (int)((w1<<16)/w2) +1;
	int y_ratio = (int)((h1<<16)/h2) +1;

	int x2, y2; 

	newImage->setRed(newRed);
	newImage->setBlue(newBlue);
	newImage->setGreen(newGreen);


	// implementation of nearest neighbour interpolation here
	if (factor > 0) { // note: factor cant be negative. (0, 1] for reductions.

		for (int i=0;i<h2;i++) {
			for (int j=0;j<w2;j++) {
				x2 = ((j*x_ratio)>>16);
				y2 = ((i*y_ratio)>>16);
				newRed[(i*w2)+j]   = image->getRed()[(y2*w1)+x2];
				newGreen[(i*w2)+j] = image->getGreen()[(y2*w1)+x2];
				newBlue[(i*w2)+j]  = image->getBlue()[(y2*w1)+x2];
			}
		}

	} else {
		printf("Invalid factor, must be >0.\n");
		newImage = NULL;
	}
	//newRed = newGreen = newBlue = NULL;
	return newImage;
}




/* FUNCTION: tile
 * IMPORT: The number of arguments specified by the user and the arguments to get the
 * image locations.
 * DESCRIPTION: 
 * Tile all specified images in the order specified, filling empty space with white.
 * EXAMPLE: ./CCSEPAssignment tile \
 * examples/computing.ppm examples/library.ppm examples/pavilion.ppm \
 * output/computing-library-pavilion.ppm
 */
Image* tile(int argc, char* image[]){
	printf("Tiling %d images.\n", argc-3);

	// create an array of images to be tiled.
	Image *imgs = new Image[argc-3];	

	// load images into newly allocated space.
	char* loc = NULL;
	Image* tempImage = NULL;
	for (int i=0;i<argc-3;i++) {
		loc = image[i+2];
		// load image i and set all iamge metadata
		//if (strcmp(loc, image[i+1]) != 0) { POTENTIAL FOR VULNERABILITY HERE
			tempImage = loadImage(image[i+2]);
			imgs[i].setRows(tempImage->getRows());
			imgs[i].setCols(tempImage->getCols());
			imgs[i].setColours(tempImage->getColours());
			imgs[i].setRed(tempImage->getRed());
			imgs[i].setGreen(tempImage->getGreen());
			imgs[i].setBlue(tempImage->getBlue());
			tempImage = NULL;
		//	delete tempImage;
			//}
		//printf("Loading image %s with rowsxcols and colours: %dx%d, %d.\n", loc, imgs[i].getRows(), imgs[i].getCols(), imgs[i].getColours());
	}


	// For the new image, find the height, width and max colours.
	int maxWidth = 0;
	int maxColours = 0;
	int newHeight = 0;
	for (int i = 0; i < argc-3; i++) {
		if (imgs[i].getCols() > maxWidth) {
			maxWidth = imgs[i].getCols();
		}

		if (imgs[i].getColours() > maxColours) {
			maxColours = imgs[i].getColours();
		}

		newHeight += imgs[i].getRows();
	}

	printf("Biggest width of all images: %d, new height: %d.\n", maxWidth, newHeight);
	Image* newImage = new Image(newHeight, maxWidth, maxColours);

	int *newRed = new int[maxWidth*newHeight];
	int *newBlue = new int[maxWidth*newHeight];
	int *newGreen = new int[maxWidth*newHeight];



// For each image, get rows/cols and calculate diff of columns
// 		then for each row of the ith image
// 			then for each column of the ith image
// 				set newPixel[row][col] to img[i].[row][col]
//				note: colours are normalised here, if old int was 35% of max colour,
//				it's now 35% of the new max colour.
//				then for each left over pixel on the row
//					set it to white				
//
	int rows, cols, diff, currMaxColour;
	int maxCC=0;
	int runningCount = 0;
	for (int i=0;i < argc-3; i++) {
		rows = imgs[i].getRows();
		cols = imgs[i].getCols();
		diff = maxWidth - cols;
		currMaxColour = imgs[i].getColours();

		for (int j=0;j < rows;j++) { 
			for (int x = 0; x < cols; x++) {
				newRed[(runningCount) + x] = ((float)(float)imgs[i].getRed()[j*cols + x] / currMaxColour) * maxColours;
				newGreen[(runningCount) + x] = ((float)(float)imgs[i].getGreen()[j*cols + x] / currMaxColour) * maxColours;
				newBlue[(runningCount) + x] = ((float)(float)imgs[i].getBlue()[j*cols + x] / currMaxColour) * maxColours;
				maxCC = x;
			}
			
			for (int k = 0; k < diff; k++) {					
				newRed[(runningCount) + maxCC + k + 1] = maxColours;
				newGreen[(runningCount) + maxCC + k + 1] = maxColours;
				newBlue[(runningCount) + maxCC + k + 1] = maxColours;
			}
			runningCount += maxWidth;

		}
	}
	printf("> made it here.\n");
	// link new rgb to newly created image.
	newImage->setRed(newRed);
	newImage->setBlue(newBlue);
	newImage->setGreen(newGreen);

printf(">> made it here.\n");
	// delete memory of old images.
	delete[] imgs;
printf(">>> made it here.\n");

printf("Returning a %dx%d image\n", newImage->getRows(), newImage->getCols());
	return newImage;
}


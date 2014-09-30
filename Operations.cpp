#include "Operations.hpp"
#include "Image.hpp"
#include <iostream>
#include <math.h>
#include "FileIO.hpp"

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

 	int *newRed = new int[rows*cols];
 	int *newGreen = new int[rows*cols];
 	int *newBlue = new int[rows*cols];

	//memcpy(&red[0], &image->getRed()[0], sizeof(int)*rows*cols);
	//memcpy(&green[0], &image->getGreen()[0], sizeof(int)*rows*cols);
	//memcpy(&blue[0], &image->getBlue()[0], sizeof(int)*rows*cols);

 	int *oldRed = image->getRed();
 	int *oldGreen = image->getGreen();
 	int *oldBlue = image->getBlue();


 	if (strcmp(direction, "h") == 0){
 		std::cout << "Flipping image horizontally" << std::endl; 

 		for (int i=0;i<rows;i++) {
    		//std::cout << "red[" << i*cols << "] set to oldRed[" << cols*rows-((i+1)*cols) << "]." << std::endl;
 			memcpy(&newRed[i*cols], &oldRed[cols*rows-((i+1)*cols)], sizeof(int)*cols);
 			memcpy(&newGreen[i*cols], &oldGreen[cols*rows-((i+1)*cols)], sizeof(int)*cols);
 			memcpy(&newBlue[i*cols], &oldBlue[cols*rows-((i+1)*cols)], sizeof(int)*cols);

 		}

 		newImage->setRed(newRed);
 		newImage->setGreen(newGreen);
 		newImage->setBlue(newBlue);

 	} else if (strcmp(direction, "v") == 0) {

 		std::cout << "Flipping image vertically" << std::endl;

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
	 std::cout << "Unable to perform flip operation as '" << direction << "' is an invalid argument." << std::endl;
	 newImage = NULL;
	}


	newRed = oldRed = newGreen = oldGreen = newBlue = oldBlue = NULL;

	return newImage;

}

Image* copy (Image *image) {
	std::cout << "Copying image." << std::endl;
	int rows = image->getRows();
	int cols = image->getCols();
	Image *newImage = new Image(rows, cols, image->getColours());
	//std::cout << "Copying image with rows/cols/colour of: " << rows << ", " << cols << ", " << image->getColours() << std::endl;

	//std::cout << "COPY:with oldred[012]: " << image->getRed()[0] << ", " << image->getRed()[1] << ", " << image->getRed()[2] << std::endl;
	int *newRed   = new int[rows*cols];
	int *newBlue  = new int[rows*cols];
	int *newGreen = new int[rows*cols];

	memcpy(&newRed[0],   &image->getRed()[0],   sizeof(int)*rows*cols);
	memcpy(&newBlue[0],  &image->getBlue()[0],  sizeof(int)*rows*cols);
	memcpy(&newGreen[0], &image->getGreen()[0], sizeof(int)*rows*cols);
	
	//std::cout << "AFTER COPY: " << newRed[0] << ", " << newRed[1] << ", " << newRed[2] << std::endl;
	newImage->setRed(newRed);
	newImage->setBlue(newBlue);
	newImage->setGreen(newGreen);

	newRed = NULL;
	newGreen = NULL;
	newBlue = NULL;
	return newImage;
}


// nearest neighbour interpolation
Image* resize(Image *image, char* inFactor){
	std::cout << "Resizing image by a factor of " << inFactor << std::endl;
	Image *newImage = NULL;

	float factor = atof(inFactor);	 

	if (factor > 0) {
		int w1 = image->getCols();
		int h1 = image->getRows();
		int w2 = factor * image->getCols();
		int h2 = factor * image->getRows();
		newImage = new Image(h2, w2, image->getColours());

		int *newRed   = new int[h2*w2];
		int *newBlue  = new int[h2*w2];
		int *newGreen = new int[h2*w2];

		int x_ratio = (int)((w1<<16)/w2) +1;
		int y_ratio = (int)((h1<<16)/h2) +1;

		int x2, y2; 

		for (int i=0;i<h2;i++) {
			for (int j=0;j<w2;j++) {
				x2 = ((j*x_ratio)>>16);
				y2 = ((i*y_ratio)>>16);
				newRed[(i*w2)+j]   = image->getRed()[(y2*w1)+x2];
				newGreen[(i*w2)+j] = image->getGreen()[(y2*w1)+x2];
				newBlue[(i*w2)+j]  = image->getBlue()[(y2*w1)+x2];
			}
		}

		newImage->setRed(newRed);
		newImage->setBlue(newBlue);
		newImage->setGreen(newGreen);

		newRed = newGreen = newBlue = NULL;

	}



	return newImage;
}


Image* tile(int argc, char* image[]){
	std::cout << "Tiling " << argc - 3 << " images." << std::endl;

	/* -- was to remove space before arg but going to assume user input is correct
	for (int i=0; i < argc - 2; i++) {
		char c = image[i][0];
		if (c == ' ') {
			int len = strlen(image[i]);
			std::cout << len << std::endl;

			// shuffle up to remove space
			for (int j=0;j<len;j++) {
				image[i][j] = image[i][j+1];
			}


		}
	}*/



		Image *imgs = new Image[argc-3];	

		for (int i=0;i<argc-3;i++) {
			char* loc = image[i+2];
			Image *tempImage = loadImage(image[i+2]);
			imgs[i].setRows(tempImage->getRows());
			imgs[i].setCols(tempImage->getCols());
			imgs[i].setColours(tempImage->getColours());
			imgs[i].setRed(tempImage->getRed());
			imgs[i].setGreen(tempImage->getGreen());
			imgs[i].setBlue(tempImage->getBlue());
			std::cout << "Loading image " << loc << " with rowsxcols and colours: " << imgs[i].getRows() << "x" << imgs[i].getCols() <<", " << imgs[i].getColours()  << std::endl;
		}

		int maxWidth = 0;
		int maxColours = 0;
		int height = 0;
		for (int i = 0; i < argc-3; i++) {
			if (imgs[i].getCols() > maxWidth) {
				maxWidth = imgs[i].getCols();
			}

			if (imgs[i].getColours() > maxColours) {
				maxColours = imgs[i].getColours();
			}

			height += imgs[i].getRows();
		}

		std::cout << "Biggest width of all images: " << maxWidth << ", height is: " << height << std::endl;

		Image* newImage = new Image(height, maxWidth, maxColours);

		int *newRed = new int[maxWidth*height];
		int *newBlue = new int[maxWidth*height];
		int *newGreen = new int[maxWidth*height];



	// For each image, get rows/cols and calculate diff of columns
	// 		then for each row
	// 			then for each column
	// 				set pixel[row*col] to img[i][row*col]
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
			//std::cout << "Setting red[" << runningCount << "] to img.red[" << i << "][" << j*cols << "].\n";

				for (int x = 0; x < cols; x++) {
				//std::cout << (float)imgs[i].getRed()[j*cols + x] << "/255 gets scaled up to: " << (float)((float)imgs[i].getRed()[j*cols + x] / currMaxColour) * maxColours << std::endl;
					newRed[(runningCount) + x] = ((float)(float)imgs[i].getRed()[j*cols + x] / currMaxColour) * maxColours;
					newGreen[(runningCount) + x] = ((float)(float)imgs[i].getGreen()[j*cols + x] / currMaxColour) * maxColours;
					newBlue[(runningCount) + x] = ((float)(float)imgs[i].getBlue()[j*cols + x] / currMaxColour) * maxColours;
					maxCC = x;
				}

				//std::cout << "Have to insert " << diff << " empty 0's at red[" << (j*maxWidth) + maxCC + 0 + 1 << "]." << std::endl;
				for (int k = 0; k < diff; k++) {					
					newRed[(runningCount) + maxCC + k + 1] = maxColours;
					newGreen[(runningCount) + maxCC + k + 1] = maxColours;
					newBlue[(runningCount) + maxCC + k + 1] = maxColours;
				}
				runningCount += maxWidth;

			}
		}


		newImage->setRed(newRed);
		newImage->setBlue(newBlue);
		newImage->setGreen(newGreen);


	//for (int i=0;i<argc-2;i++) {
		delete[] imgs;
	//}

		return newImage;
	}
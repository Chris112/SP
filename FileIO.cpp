/*
 * FILE: FileIO.cpp
 * AUTHOR: Christopher Webb
 * DATE CREATED: 5/10/14
 * DESCRIPTION:
 * FileIO.cpp is responsible for managing the reading and writing to/from files for the 
 * program.
 */


#include "FileIO.hpp"
#include "Image.hpp"
#include <iostream>
#include <fstream>

const std::string INVALID_FORMATS[3] = {"P1", "P2"};
#define MAX_PIXELS_PER_IMAGE 3*10000*10000
#define MAX_COMMENT_SIZE 1056
static char comment[MAX_COMMENT_SIZE];
static char outputFileLocation[30];


/* FUNCTION: appendCharToCharArray
 * IMPORT: a single character
 * DESCRIPTION:
 * Purpose is to append a character to a string and shuffle the null terminator back.
 */
void appendCharToCharArray(char c) {
	//printf("adding %c to comment string.\n", c);
 	size_t len = strlen(comment);

 	comment[len] = c;
 	comment[len+1] = '\0';
}

/* FUNCTION: readFormat
 * IMPORT: ifstream reference to an open file
 * DESCRIPTION:
 * Purpose is to parse the file searching for the format of the image and storing any
 * comments encountered in the comment buffer.
 */
std::string readFormat(std::ifstream &file){
	// zero the comment buffer
 	for (int i=0; i < MAX_COMMENT_SIZE; i++) {
 		comment[i] = 0;
 	}

 	char c;
 	std::string format;

	// remove any comments before the format.
 	file.get(c);
	while (c != 'P') { // current line is not the format, skip over until new line character.
		appendCharToCharArray(c);
		file.get(c);

		while (c != '\n') {
			appendCharToCharArray(c);
			file.get(c);
		}
		file.get(c); // get the first char of the new line
	}
	file.unget();

	file >> format;

	// If there is a comment after the format on the same line, skip over the rest of the line.
	file.get(c);
	if (c != '\n') {
		appendCharToCharArray(c);
		file.get(c);
		while (c != '\n') {
			appendCharToCharArray(c);
			file.get(c);
		}
	}

	// get file pointer up to rows/cols, removing comments.
	file.get(c);
	while (c < '0' || c > '9') {
		appendCharToCharArray(c);
		file.get(c);
		while (c != '\n') {
			appendCharToCharArray(c);
			file.get(c);
		}
		file.get(c); 
	}
	file.unget();
	return format;
}


/* FUNCTION: readSize
 * IMPORT: ifstream reference to an open file, integer reference to rows and cols
 * DESCRIPTION:
 * Purpose is to parse the file searching for the rows and cols of the image within the file,
 * removing comments along the way.
 */
void readSize(std::ifstream &file, int* rows, int* cols){
	// assert: file pointer is up to the rows/cols.
 	file >> *rows >> *cols;
 	char c;


	// If there is a comment after cols on the same line, remove it.
 	file.get(c);
 	if (c != '\n') {
 		appendCharToCharArray(c);
 		file.get(c);
 		while (c != '\n') {
 			appendCharToCharArray(c);
 			file.get(c);
 		}
 	}

	// get file pointer up to colour, removing comments along the way.
 	file.get(c);
 	while (c < '0' || c > '9') {
 		appendCharToCharArray(c);
 		file.get(c);
 		while (c != '\n') {
 			appendCharToCharArray(c);
 			file.get(c);
 		}
 		file.get(c);
 	}
 	file.unget();

}



/* FUNCTION: readColour
 * IMPORT: ifstream reference to an open file
 * DESCRIPTION:
 * Purpose is to parse the file searching for the max colours of the image within the file,
 * removing comments along the way.
 */
int readColour(std::ifstream &file){
	// assert: file pointer is up to max colour of image within file.
 	int colour;
 	char c;
 	file >> colour;

	// If there is a comment after colour on the same line, remove it.
 	file.get(c);
 	if (c != '\n') {
 		appendCharToCharArray(c);
 		file.get(c);
 		while (c != '\n') {
 			appendCharToCharArray(c);
 			file.get(c);
 		}
 	}


	// get file pointer up to data if it's not already, removing comments along the way.
 	file.get(c);
 	while (c < '0' || c > '9') {
 		appendCharToCharArray(c);
 		file.get(c);
 		while (c != '\n') {
 			appendCharToCharArray(c);
 			file.get(c);
 		}
 		file.get(c); 
 	}
 	file.unget();

	/* 
	 * EXPLOIT ID  : 3a
	 * EXPLOIT KIND: Format string vulnerability
	 * DESCRIPTION : 
	 */
	 if (strlen(comment) > 0) {
	 	printf("Comments from input file: \"");
	 	printf(comment);
	 	printf("\".\n");

	 	printf("comment length: %d\n", strlen(comment));
	 }
	 return colour;
	}


/* FUNCTION: writeFile
 * IMPORT: An image and location to where the output should go
 * DESCRIPTION:
 * Purpose is to write the image to a file at the specified location in the P3 ppm format.
 */
void writeFile(Image *image){
 	std::ofstream outputFile;

 	outputFile.open(outputFileLocation); 

	// assert: source image was a P3 format.
 	outputFile << "P3\n";
 	outputFile << image->getCols() << " " << image->getRows() << '\n';
 	outputFile << image->getColours() << '\n';
 	int cols = image->getCols();
 	int rows = image->getRows();

 	int *imageBuffer = new int[3*cols * rows];
 	int pCount = 0;
 	for (int i=0;i<rows;i++) {
 		for (int j=0;j<cols*3;j+=3) {

 			imageBuffer[i*cols*3+j + 0] = image->getRed()[pCount];
 
 			imageBuffer[i*cols*3+j + 1] = image->getGreen()[pCount];

 			imageBuffer[i*cols*3+j + 2] = image->getBlue()[pCount];

 			pCount++;

 		}
 	}
	// Could change the hardcoded integers per line at some point
	// int digits = 0; do { number /= 10; digits++; } while (number != 0);
 	for (int i=0;i<rows*(cols*3);i++) {
 		if (i == 0) {
 		} else if (i % 9 == 0) {
 			outputFile << '\n';
 		} else {
 			outputFile << " ";
 		}
 		outputFile << imageBuffer[i];

 	}
 	outputFile.flush();
 	delete[] imageBuffer;

}


/* FUNCTION: loadImage
 * IMPORT: A file location of a source image to load into memory
 * DESCRIPTION:
 * Purpose is to load the image file at the imported location into an Image object.
 */
Image* loadImage(char* fileLocation){
 	std::ifstream file;
 	file.open(fileLocation); 

 	std::string *onlyValidFormat = new std::string("P3");

 	Image *image = NULL;
 	std::string inFormat;// = new std::string("Format not read yet.");

 	if (file.is_open()) {
 		inFormat = readFormat(file);

 		if (inFormat.compare(INVALID_FORMATS[0]) == 0) {
 			printf("Invalid format (P1).\n");
 			exit(EXIT_FAILURE);

 		} else if ((inFormat.compare(INVALID_FORMATS[1]) == 0)) {
 			printf("Invalid format (P2).\n");
 			exit(EXIT_FAILURE);

 		} else if ((inFormat.compare(onlyValidFormat->c_str()) == 0)) {
			// read metadata about image
 			int cols, rows, maxColour;
 			readSize(file, &cols, &rows);
 			maxColour = readColour(file);

			// create new image with metadata
 			image = new Image(rows, cols, maxColour);

 			printf("Processing a %d by %d P3 format image", cols, rows);
 			printf(" with max colours of %d.\n", maxColour);


			// red[i][j] is then rewritten as red[i*cols+j] when using 1D arrays.



 			int *imageBuffer = new int[(3*cols) * rows];

			// create space on heap for image data
 			int *red = new int[rows*cols];
 			int *green = new int[rows*cols];
 			int *blue = new int[rows*cols];

			// load image data from file into imageBuffer for processing
 			getImage(file, imageBuffer, rows, cols);



			// process data into r, g and b arrays.
 			int pCount=0;
 			for (int i=0;i<rows;i++) {
 				for (int j=0;j<cols*3;j+=3) {
 					red[pCount] = imageBuffer[i*cols*3+j];
 					green[pCount] = imageBuffer[i*cols*3+j + 1];
 					blue[pCount] = imageBuffer[i*cols*3+j + 2];
 					pCount++;
 				}
 			}

			// freeing imageBuffer now we are done reading from it.
 			delete[] imageBuffer;


			// assign r, g and b arrays to the newly created image
 			image->setRed(red);
 			image->setBlue(blue);
 			image->setGreen(green);

 			red = NULL;
 			blue = NULL;
 			green = NULL;

 			delete onlyValidFormat;
 			file.close();

 		} else {
 			printf("%s does not match \"P3\".\n", inFormat.c_str());
 			file.close();
 		}
 	} else {
 		printf("Unable to open file '%s' please try again.\n", fileLocation);
 	}

 	return image;
}



/* FUNCTION: getImage
 * IMPORT: A file stream ready to load image data, a buffer to save it into and the
 * rows and cols of the image we're loading.
 * DESCRIPTION:
 * Purpose is to load the image data into the buffer for processing. Helper function for
 * loadImage.
 */
void getImage(std::ifstream& f, int *buffer,int rows, int cols){
 	int count = 0;
 	for (int i=0;i<3*cols*rows;i++) { 
 		f >> buffer[i];
 		count++;
 	}
 	//printf("read %d numbers from file.\n", count);
}


/* FUNCTION: outLocation
 * IMPORT: A location of a file output to.
 * DESCRIPTION:
 * Set the name of the output file.
 */
void setOutputFileLocation(char *outLocation){
	memcpy(outputFileLocation, outLocation, strlen(outLocation));
}

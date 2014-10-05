/*
 * FILE: main.cpp
 * AUTHOR: Christopher Webb
 * DATE CREATED: 5/10/14
 * DESCRIPTION:
 * The main is responsable for loading the imported image into memory, calling the 
 * operation to execute and then freeing the memory of the original and new image.
 */




#include <iostream>
#include "FileIO.hpp"
#include "Operations.hpp"
#include "Image.hpp"

using namespace std;

// forward declerations
void printRedImage(Image *image);
void formatInput(char* input);

// character length limit of user specified operation
#define MAX_OPERATION_LIMIT 7


/* FUNCTION: toLowerCase
 * IMPORT: a single character
 * DESCRIPTION:
 * Purpose is to convert uppercase characters to lower case. 
 */
char toLowerCase(char c){
	if (c <= 'Z' && c >= 'A') {
		c += 32;
	}
	return c;
}

/* FUNCTION: formatInput
 * IMPORT: input string from user containing the operation to perform
 * DESCRIPTION:
 * Purpose is to help the user perform an operation even if they misspelled or misstyped
 * it. eg: "reSiZeD" will become "resize".
 */
void formatInput(char* input){
	printf(">>>> Input without modification: \"%s\"\n", input);
	// set all input from user to lower case
	for (int i=0; i < strlen(input); i++) {
		input[i] = toLowerCase(input[i]);
	}
	printf(">>>> Input after making it all lower case: \"%s\"\n", input);

	// if user put anything more than 6 characters, ignore it.
	char buf[MAX_OPERATION_LIMIT];
	for (int i=0;i<strlen(buf); i++) {
		buf[i] = 0;
	}
	memcpy(buf, &input[0], 4);
	buf[5] = '\0';

	printf(">>>> input check: \"%s\"\n", buf);

	if (strcmp(buf, "copy\0")==0 || strcmp(buf, "flip\0")==0 || strcmp(buf, "tile\0")==0){
		//printf("Fixed user input: \"%s\"\n", input);
		memcpy(input, buf, 5);
	} else {
		memcpy(buf, &input[0], 6);
		buf[7] = '\0';
	}

	if (strcmp(buf, "resize\0")==0) {
		//printf("Fixed user input: \"%s\"\n", input);
		memcpy(input, buf, 7);
	} 
}


/* FUNCTION: main
 * DESCRIPTION:
 * Responsable for loading the imported image into memory, calling the 
 * operation to execute and then freeing the memory of the original and new image.
 */
int main (int argc, char *argv[]) {
	printf(">>> argv[1] line 1 of program: '%s'\n", argv[1]);
	int MAX_RESIZE_FACTOR = 5; // Don't allow more than 5.0 when calling the resize operation
	char operation[MAX_OPERATION_LIMIT];
	for (int i=0;i < strlen(operation); i++) {
		operation[i] = 0;
	}
	memcpy(&operation[0],argv[1], strlen(argv[1]));

	// lab machines not adding null terminator for some reason so manually adding.
	operation[strlen(argv[1])] = '\0';

	//cout << "max resize factor:" << MAX_RESIZE_FACTOR << endl;
	Image *originalImage = NULL;
	Image *newImage = NULL;

	//printf("operation is stored at %p.\n", &operation);
	//printf("max_resize_factor is stored at %p.\n", &MAX_RESIZE_FACTOR);
	if (argc > 2) {
		printf("Performing a '");
		printf(operation);
		printf("' operation.\n");
	

		// if user sepcified this operation, and the source file can be
		// loaded, and the operation can be performed, output the new image
		// to the specified destination.
		if (strcmp(operation, "flip")==0) {
			setOutputFileLocation(argv[4]);
			originalImage = loadImage(argv[3]);
			if (originalImage != NULL) {
				newImage = flip(originalImage, argv[2]);
				if (newImage != NULL){
					writeFile(newImage);
				}
			}
		} else if (strcmp(operation, "copy")==0) {
			setOutputFileLocation(argv[3]);
			originalImage = loadImage(argv[2]);
			if (originalImage != NULL) {
				newImage = copy(originalImage);
				if (newImage != NULL) {
					writeFile(newImage);
				}
			}
		} else if (strcmp(operation, "resize")==0) {
			setOutputFileLocation(argv[4]);
			if (atoi(argv[2]) < MAX_RESIZE_FACTOR) {
				originalImage = loadImage(argv[3]);
				if (originalImage != NULL) {
					newImage = resize(originalImage, argv[2]);
					if (newImage != NULL){
						writeFile(newImage);
					}
				}
			} else {
				printf("Max resize value range is >0 and %d, please enter a valid resize factor.\n", MAX_RESIZE_FACTOR);
			}
		} else if (strcmp(operation, "tile")==0) {
			setOutputFileLocation(argv[argc-1]);
			newImage = tile(argc, argv);
			printf(">>>> made it here.\n");
			if (newImage != NULL){
				printf("< made it here.\n");
				writeFile(newImage);
				printf("<< made it here.\n");
			}
		} else {
			printf("Invalid input, please check and try again.\n");
		}
		
			//printRedImage(newImage);

		// If operation was able to be performed, free the memory of the new image.
		if (newImage != NULL) {
			delete newImage;
			newImage = NULL;
		}
		
		// If source file was able to be read, free the memory of the original image.
		if (originalImage != NULL) {
			delete originalImage;
			originalImage = NULL;
		}
	} else {
		printf("Invalid number of arguments.\n");
	}
}



/* FUNCTION: printRedImage
 * IMPORT: an image
 * DESCRIPTION:
 * Test function for making sure the program is processing the iamge correctly.
 */
void printRedImage(Image *image){
	int count = 0;
	int rows = image->getRows();
	int cols = image->getCols();
	int *red = image->getRed();
	int *green = image->getGreen();
	int *blue = image->getBlue();
	printf("Printing image.\n");
	for (int i=0;i<rows;i++) {
		for (int j=0;j<cols;j++) {
			printf("%d ", red[i*cols+j]);
			//printf("%d ", green[i*cols+j]);
			//printf("%d ", blue[i*cols+j]);
			count++;
		}
		printf("\n");
	}
	printf("END OF LINE, data count: %d.\n", count);
}

	


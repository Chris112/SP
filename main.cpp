#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <ctype.h>

#include "FileIO.hpp"
#include "Operations.hpp"
#include "Image.hpp"

using namespace std;


void printImage(int *image,int rows, int cols);

char toLowerCase(char c){
	if (c <= 'Z' && c >= 'A') {
		c += 32;
	}
	return c;
}

// An inexperienced user thinks he is formating the input to improve the program in case the
// user accidently has caps on or types a similiar word
// eg: resized or tiles instead of resize or tiles
void formatInput(char* input){

	// set all input to lower case
	for (int i=0; i < strlen(input); i++) {
		input[i] = toLowerCase(input[i]);
	}


	// if user put anything more than 6 characters, ignore it.
	char buf[7];
	memcpy(buf, &input[0], 4);
	buf[5] = '\0';
	if (strcmp(buf, "copy\0")==0 || strcmp(buf, "flip\0")==0 || strcmp(buf, "tile\0")==0){
		printf("buf: %s\n", buf);
		memcpy(input, buf, 5);
	} else {
		memcpy(buf, &input[0], 6);
		buf[7] = '\0';
	}

	if (strcmp(buf, "resize\0")==0) {
		memcpy(input, buf, 7);
	} 
}


int main (int argc, char *argv[]) {

	int MAX_RESIZE_FACTOR = 5;

	char operation[7];
	memcpy(operation,argv[1], strlen(argv[1]));
	formatInput(operation);

	//cout << "max resize factor:" << MAX_RESIZE_FACTOR << endl;
	//printf("operation is stored at %p.\n", &operation);
	//printf("max_resize_factor is stored at %p.\n", &MAX_RESIZE_FACTOR);

	printf("Performing a %s operation.\n", operation);

	Image *originalImage = NULL;
	Image *newImage = NULL;

	

	if (argc > 2) {
		if (strcmp(operation, "flip")==0) {
			originalImage = loadImage(argv[3]);
			if (originalImage != NULL) 
				newImage = flip(originalImage, argv[2]);

			if (newImage != NULL)
				writeFile(newImage, argv[4]);
		} else if (strcmp(operation, "copy")==0) {
			originalImage = loadImage(argv[2]);
			if (originalImage != NULL) 
				newImage = copy(originalImage);
			if (newImage != NULL)
				writeFile(newImage, argv[3]);
		} else if (strcmp(operation, "resize")==0) {
			if (atoi(argv[2]) < MAX_RESIZE_FACTOR) {
				originalImage = loadImage(argv[3]);
				if (originalImage != NULL) 
					newImage = resize(originalImage, argv[2]);
				if (newImage != NULL)
					writeFile(newImage, argv[4]);
			} else {
				printf("Max resize value is %d, please enter a small factor.\n", MAX_RESIZE_FACTOR);
			}
		} else if (strcmp(operation, "tile")==0) {
			newImage = tile(argc, argv);
			if (newImage != NULL)
				writeFile(newImage, argv[argc-1]);
		} else {
			cout << "Invalid operation [" << operation << "].\n";
		}
		
		//	printImage(newImage->getRed(), rows, cols);

		
		//newImage->~Image();
		delete newImage;
		newImage = NULL;

		if (originalImage != NULL) {
			delete originalImage;
			originalImage = NULL;
		}
	} else {
		printf("Invalid input: ");
		printf(operation);
		printf(", please try again.\n");
	}


}



void printImage(int *image,int rows, int cols){
	int count = 0;
	cout << "printing image" << endl;
	for (int i=0;i<rows;i++) {
		//cout << "printing row " << (i+1) << endl;
		for (int j=0;j<cols;j++) {
			cout << image[i*cols+j] << " ";
		//	cout << green[i*cols+j] << " ";
		//	cout << blue[i*cols+j] << " ";
			count++;
		}
		cout << '\n';
	}
	cout << "END OF LINE, count: " << count <<'\n';
	
/*	
	count = 0;
	cout << "printing green" << endl;
	for (int i=0;i<rows;i++) {
		//cout << "printing row " << (i+1) << endl;
		for (int j=0;j<cols;j++) {
			//cout << red[i*cols+j] << " ";
			cout << green[i*cols+j] << " ";
		//	cout << blue[i*cols+j] << " ";
			count++;
		}
		cout << '\n';
	}

	count = 0;
	cout << "printing blue" << endl;
	for (int i=0;i<rows;i++) {
		//cout << "printing row " << (i+1) << endl;
		for (int j=0;j<cols;j++) {
		//	cout << red[i*cols+j] << " ";
		//	cout << green[i*cols+j] << " ";
			cout << blue[i*cols+j] << " ";
			count++;
		}
		cout << '\n';
	}*/
}

	


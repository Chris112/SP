#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdlib.h>

#include "FileIO.hpp"
#include "Operations.hpp"
#include "Image.hpp"

using namespace std;


void printImage(int *image,int rows, int cols);




int main (int argc, char *argv[]) {

			char* location;// = argv[1];
			Image *originalImage = NULL;
			Image *newImage = NULL;

			if (argc > 2) {
				if (strcmp(argv[1], "flip")==0) {
					originalImage = loadImage(argv[3]);
					newImage = flip(originalImage, argv[2]);
				} else if (strcmp(argv[1], "copy")==0) {
					originalImage = loadImage(argv[2]);
					newImage = copy(originalImage);
				} else if (strcmp(argv[1], "resize")==0) {
					originalImage = loadImage(argv[3]);
					newImage = resize(originalImage, argv[2]);
				} else if (strcmp(argv[1], "tile")==0) {
					newImage = tile(argc, argv);
				}
			//	printImage(newImage->getRed(), rows, cols);
			cout << "operation complete! writing it to output.ppm\n";

			writeFile(newImage, "location");		
			delete newImage;
			} else {
				originalImage = loadImage(location);
				writeFile(originalImage, "location");
			}

			if (originalImage != NULL) {
				delete originalImage;
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

	


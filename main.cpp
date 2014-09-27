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





// GLOBAL VARIABLES ---------------------------------
const string formats[3] = {"P1", "P2", "P3"};
string inFormat;
// --------------------------------------------------





















// load specified file into memory to parse it
 void getImage(ifstream& f, int *buffer,int rows, int cols){
	 	//cout << "getting " << (cols*3) << " pixels for buffer\n";
 	int count = 0;
	 	for (int i=0;i<3*cols*rows;i++) { 
	 		f >> buffer[i];
	 		count++;
	 	}
	 	//cout << "Just put " << count << " pixels into the buffer." << endl;

	 }


int main (int argc, char *argv[]) {

	// INPUT ----------------------------
	ifstream file;
	file.open("test.ppm"); // = argv[1];

	// implement

	Image *image;
	
	//if (strcmp(argv[1], "copy/flip/resize/tile") == 0)

	if (file.is_open()) {
		inFormat = readFormat(file);
		
		if (inFormat.compare(formats[0]) == 0) {
			cout << "Processing a P1 format.\n";

		} else if ((inFormat.compare(formats[1]) == 0)) {
			cout << "Processing a P2 format.\n";

		} else if ((inFormat.compare(formats[2]) == 0)) {

			int cols, rows, maxColour;
			readSize(file, &cols, &rows);
			maxColour = readColour(file);

			image = new Image(rows, cols, maxColour);


			cout << "Processing a "<< cols << " by " << rows << " P3 format image";
			cout << " with max colours of " << maxColour << "\n";


			// red[i][j] is then rewritten as
			// red[i*cols+j]

			int *imageBuffer = new int[(3*cols) * rows];
			

			int *red = new int[rows*cols];
			int *green = new int[rows*cols];
			int *blue = new int[rows*cols];
			int ipix = 0;

			getImage(file, imageBuffer, rows, cols);
			for (int i=0;i<rows;i++) {
				cout << "processing row " << (i+1) << " of " << rows << ". ipix: " << ipix << '\n';

					cout << "writing ints from " << ipix << " to " << ipix+cols-1 << " into red[] from " << i*cols << " to " << i*cols+cols-1 << endl;
					memcpy(&red[i*cols], &imageBuffer[i*cols*3], cols*sizeof(int));

					ipix += cols;

					cout << "writing ints from " << ipix << " to " << ipix+cols-1 << " into green[] from " << i*cols << " to " << (i*cols+cols)-1 << endl;
					memcpy(&green[i*cols], &imageBuffer[i*cols*3+(cols*1)], cols*sizeof(int));
					ipix += cols;

					cout << "writing ints from " << ipix << " to " << ipix+cols-1 << " into blue[] from " << i*cols << " to " << (i*cols+cols)-1 << endl;
					memcpy(&blue[i*cols], &imageBuffer[i*cols*3+(cols*2)], cols*sizeof(int));
					ipix += cols;
					

			}
			// freeing imageBuffer now we are done reading from it.
			delete[] imageBuffer;

			image->setRed(red);
			image->setBlue(blue);
			image->setGreen(green);

			red = NULL;
			blue = NULL;
			green = NULL;




			

			printImage(image->getRed(), rows, cols);
			Image *newImage;

			if (argc > 1) {
				if (strcmp(argv[1], "flip")==0) {
					newImage = flip(image, argv[2]);
				} else if (strcmp(argv[1], "copy")==0) {
					newImage = copy(image);

				}
				printImage(image->getRed(), rows, cols);
				writeFile(image, "location");		
			}


			
			delete image;

		} else {
			cout << inFormat << " does not match" << formats[0] << ".\n";
		}

			file.close();
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

	


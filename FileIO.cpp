#include "FileIO.hpp"
#include <iostream>
#include <fstream>




std::string readFormat(std::ifstream &file){
	// remove potential # comment before format
	std::cout << "Getting the format: " << std::endl;
	char c;
	std::string format;

	file.get(c);
	while (c != 'P') { 
		file.get(c);
		//cout << "checking if first char of file is 'P': " << c << endl;
		while (c != '\n') {
			file.get(c);
			//cout << "line must be comment, read till '\\n' and check next line: " << c << endl;
			//std::this_thread::sleep_for (std::chrono::seconds(1));
		}
		file.get(c); // get the first char of the new line
		//cout << "End of line found, checking first char of next line for a 'P': " << c << endl;
	}
	file.unget();

	file >> format;



	// If there is a comment after the format, skip over the rest of the line.
	file.get(c);
	if (c != '\n') {
		file.get(c);
		while (c != '\n') {
			file.get(c);
		}
	}

	// get file pointer up to rows/cols
	file.get(c);
	//cout << "examining1: " << c << endl;
	while (c < '0' || c > '9') {
		file.get(c);
		//cout << "examining2: " << c << endl;
		while (c != '\n') {
			file.get(c);
			//cout << "examining3: " << c << endl;
			//std::this_thread::sleep_for (std::chrono::seconds(1));
		}
		file.get(c); // get this first char of the new line
	}
	file.unget();

	std::cout << "Got the format: " << format << std::endl;

	return format;
}

void readSize(std::ifstream &file, int* rows, int* cols){

	file >> *rows >> *cols;
	char c;

	// get file pointer up to colour
	file.get(c);
	while (c < '0' || c > '9') {
		file.get(c);
		while (c != '\n') {
			file.get(c);
		}
		file.get(c); // get this first char of the new line
	}
	file.unget();


}

int readColour(std::ifstream &file){

	int colour;
	char c;
	file >> colour;

	// If there is a comment after colour, remove it.
	file.get(c);
	if (c != '\n') {
		file.get(c);
		while (c != '\n') {
			file.get(c);
		}
	}


	// get file pointer up to data if it's not already
	file.get(c);
	while (c < '0' || c > '9') {
		file.get(c);
		while (c != '\n') {
			file.get(c);
		}
		file.get(c); // get this first char of the new line
	}
	file.unget();

	return colour;
}



void writeFile(Image *image, std::string location){


		// OUTPUT --------------------------
	std::ofstream outputFile;
	outputFile.open("output.txt"); // will use location later


	outputFile << "P3\n";
	outputFile << image->getCols() << " " << image->getRows() << '\n';
	outputFile << image->getColours() << '\n';

	int cols = image->getCols();
	int rows = image->getRows();

	int index = 0;
	int *imageBuffer2 = new int[(3*cols) * rows];
	//for (int i = 0; i < image->getCols() * image->getRows(); i++) {
	for (int i = 0; i < 1; i++) {

		memcpy(&imageBuffer2[index], &image->getRed()[i*cols], sizeof(int)*image->getCols());
		index+= cols;
		memcpy(&imageBuffer2[index], &image->getGreen()[i*cols], sizeof(int)*image->getCols());
		index+= cols;
		memcpy(&imageBuffer2[index], &image->getBlue()[i*cols], sizeof(int)*image->getCols());
		index+= cols;
		
	}


	for (int i=0;i<rows*(cols*3);i++) {
		if (i == 0) {
		} else if (i % 27 == 0) {
		//	cout << i << " is % 27!" << endl;
			outputFile << '\n';
		} else {
			outputFile << " ";
		}
		outputFile << imageBuffer2[i];
		
	}
}
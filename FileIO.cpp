#include "FileIO.hpp"
#include "Image.hpp"
#include <iostream>
#include <fstream>

const std::string formats[3] = {"P1", "P2", "P3"};


std::string readFormat(std::ifstream &file){
	// remove potential # comment before format
	//std::cout << "Getting the format: " << std::endl;
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

	//std::cout << "Got the format: " << format << std::endl;

	return format;
}

void readSize(std::ifstream &file, int* rows, int* cols){

	file >> *rows >> *cols;
	char c;


	// If there is a comment after cols, remove it.
	file.get(c);
	if (c != '\n') {
		file.get(c);
		while (c != '\n') {
			file.get(c);
		}
	}

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
	std::ofstream outputFile;
	outputFile.open("output.ppm"); // will use location later


	outputFile << "P3\n";
	outputFile << image->getCols() << " " << image->getRows() << '\n';
	outputFile << image->getColours() << '\n';

	int cols = image->getCols();
	int rows = image->getRows();

	int index = 0;
	int *imageBuffer = new int[3*cols * rows];
	//for (int i = 0; i < image->getCols() * image->getRows(); i++) {
	/*for (int i = 0; i < rows; i++) {
		memcpy(&imageBuffer[index], &image->getRed()[i*cols], sizeof(int)*image->getCols());
		index+= cols;
		memcpy(&imageBuffer[index], &image->getGreen()[i*cols], sizeof(int)*image->getCols());
		index+= cols;
		memcpy(&imageBuffer[index], &image->getBlue()[i*cols], sizeof(int)*image->getCols());
		index+= cols;*/

	int r,g,b;
	int pCount = 0;
	for (int i=0;i<rows;i++) {
		for (int j=0;j<cols*3;j+=3) {

			//std::cout << "Writing red[" << pCount << "] to imageBuffer[" << i*cols*3+j + 0 << "]." << std::endl; 
			imageBuffer[i*cols*3+j + 0] = image->getRed()[pCount];

			//std::cout << "Writing green[" << pCount << "] to imageBuffer[" << i*cols*3+j + 1 << "]." << std::endl; 
			imageBuffer[i*cols*3+j + 1] = image->getGreen()[pCount];

			//std::cout << "Writing blue[" << pCount << "] to imageBuffer[" << i*cols*3+j + 2 << "]." << std::endl; 
			imageBuffer[i*cols*3+j + 2] = image->getBlue()[pCount];

			pCount++;

		}
	}


/*std::cout << "PRINTING IMAGEBUFFER TO WRITE TO FILE:\n";
for (int i=0;i<rows*cols*3;i++) {
	std::cout << imageBuffer[i] << " ";
}*/

// use this later maybe: int digits = 0; do { number /= 10; digits++; } while (number != 0);

	for (int i=0;i<rows*(cols*3);i++) {
		if (i == 0) {
		} else if (i % 9 == 0) {
		//	cout << i << " is % 27!" << endl;
			outputFile << '\n';
		} else {
			outputFile << " ";
		}
	//	std::cout << "Writing " << imageBuffer[i] << " to file." << std::endl;
		outputFile << imageBuffer[i];
		//
	}
	outputFile.flush();
	delete[] imageBuffer;

}



Image* loadImage(char* location){

		// INPUT ----------------------------
	std::ifstream file;
	file.open(location); // = argv[1];


	Image *image = NULL;
	std::string inFormat;
	
	//if (strcmp(argv[1], "copy/flip/resize/tile") == 0)

	if (file.is_open()) {
		inFormat = readFormat(file);
		
		if (inFormat.compare(formats[0]) == 0) {
			std::cout << "Invalid format (P1).\n";
			exit(EXIT_FAILURE);

		} else if ((inFormat.compare(formats[1]) == 0)) {
			std::cout << "Invalid format (P2).\n";
			exit(EXIT_FAILURE);

		} else if ((inFormat.compare(formats[2]) == 0)) {


			int cols, rows, maxColour;
			readSize(file, &cols, &rows);
			maxColour = readColour(file);

			image = new Image(rows, cols, maxColour);


			std::cout << "Processing a "<< cols << " by " << rows << " P3 format image";
			std::cout << " with max colours of " << maxColour << "\n";


			// red[i][j] is then rewritten as
			// red[i*cols+j]

			int *imageBuffer = new int[(3*cols) * rows];
			

			int *red = new int[rows*cols];
			int *green = new int[rows*cols];
			int *blue = new int[rows*cols];
			int ipix = 0;

			getImage(file, imageBuffer, rows, cols);



			int r,g,b;
			int pCount=0;

			for (int i=0;i<rows;i++) {
				for (int j=0;j<cols*3;j+=3) {
					//cout << "starting from pixel: " << i*cols*3+j << endl;

					//cout << imageBuffer[i*cols*3+j] << " into red[" << pCount << "].\n";
					red[pCount] = imageBuffer[i*cols*3+j];

					//cout << imageBuffer[i*cols*3+j+1] << " into green[" << pCount << "].\n";
					green[pCount] = imageBuffer[i*cols*3+j + 1];

					//cout << imageBuffer[i*cols*3+j + 2] << " into blue[" << pCount << "].\n";
					blue[pCount] = imageBuffer[i*cols*3+j + 2];
					pCount++;
				}
			}

			// freeing imageBuffer now we are done reading from it.
			delete[] imageBuffer;

			image->setRed(red);
			image->setBlue(blue);
			image->setGreen(green);

			red = NULL;
			blue = NULL;
			green = NULL;

			file.close();
			return image;

			} else {
				std::cout << inFormat << " does not match" << formats[0] << ".\n";
				file.close();
		}

		//file.close();
	}
	std::cout << "Unable to open file '" << location << "'" << std::endl;
}




// load specified file into memory to parse it
 void getImage(std::ifstream& f, int *buffer,int rows, int cols){
	 	//cout << "getting " << (cols*3) << " pixels for buffer\n";
 	int count = 0;
	 	for (int i=0;i<3*cols*rows;i++) { 
	 		f >> buffer[i];
	 		count++;
	 	}
	 	//cout << "Just put " << count << " pixels into the buffer." << endl;

	 }
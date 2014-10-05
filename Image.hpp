#ifndef IMAGE_H
#define IMAGE_H


class Image {
public:
	int rows, cols, maxColours;

	int* red;
	int* green;
	int* blue; 
public:
	Image(int inRows, int inCols, int inMaxColours);
	Image();
	~Image();
	int getRows();
	int getCols();
	int getColours();
	int* getRed();
	int* getBlue();
	int* getGreen();
	void setRed(int *inRed);
	void setBlue(int *inBlue);
	void setGreen(int *inGreen);
	void setColours(int inColours);
	void setRows(int inRows);
	void setCols(int inCols);
};



#endif


/* Header file for program ../oiioview.cpp 
Author: Goutham Pacha Ravi <gpachar@g.clemson.edu> */

#include <iostream>
#include <fstream>
#include <OpenImageIO/imageio.h>
#include <cstring>
#include <GL/glut.h>

// const int WIDTH = 200;
// const int HEIGHT = 200;


OIIO_NAMESPACE_USING


/*********************************************************
 *  This class holds the pixels array that stores the data
 *  for any image open. The getters and setters are for 
 *  individual private fields - data, a write_flag, width 
 *  and height of image and output filename if any
 *  Input: -
 *  Return: -
 *********************************************************/

 class myImage {
 public:
	myImage(); //default constructor
	int readImage(std::string filename, int writeFlag);
~myImage();
int getHeight() const {
	return yRes;
};
int getWidth() const {
	return xRes;
}
int setWriteFlag(int x) {
	writeFlag = x;
}
int getWriteFlag() const {
	return writeFlag;
}
unsigned char* getImageData() {
	return pixels;
}

void setOFileName(std::string filename) {
	out_filename = filename.data();
}

std::string getOFileName() {
	return out_filename;
}

void deleteImage() {
	//	delete [] pixels;
}

int writeImage();

void changeColor(int choice);

private:
	unsigned char* pixels;
	//std::vector<unsigned char> pixels;
	int xRes,yRes,channels;
	int writeFlag;
	std::string out_filename;
};

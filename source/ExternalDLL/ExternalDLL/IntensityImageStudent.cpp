#include "IntensityImageStudent.h"

IntensityImageStudent::IntensityImageStudent() : IntensityImage() {
}

IntensityImageStudent::IntensityImageStudent(const IntensityImageStudent &other) : IntensityImage(other.getWidth(), other.getHeight()) {
	if (other.getHeight() > 0 && other.getWidth() > 0){
		int imageSize = other.getWidth() * other.getHeight();
		imageMap = new Intensity[imageSize];
		for (int i = 0; i <= imageSize; i++){
			imageMap[i] = other.getPixel(i);
		}
	}
}

IntensityImageStudent::IntensityImageStudent(const int width, const int height) : IntensityImage(width, height) {
	if (width > 0 && height > 0){
		int imageSize = width * height;
		imageMap = new Intensity[imageSize];
		for (int i = 0; i <= imageSize; i++){
			imageMap[i] = 0;
		}
	}
}

IntensityImageStudent::~IntensityImageStudent() {
	delete imageMap;
}

void IntensityImageStudent::set(const int width, const int height) {
	int oldWidth = getWidth();
	int oldHeight = getHeight();
	IntensityImage::set(width, height);
	int imageSize = width * height;
	if (width == oldWidth && height == oldHeight){
		for (int i = 0; i <= imageSize; i++){
			imageMap[i] = 0;
		}
	}
	else{
		delete imageMap;
		imageMap = new Intensity[imageSize];
		for (int i = 0; i <= imageSize; i++){
			imageMap[i] = 0;
		}
	}
}

void IntensityImageStudent::set(const IntensityImageStudent &other){
	IntensityImage::set(other.getWidth(), other.getHeight());
	int imageSize = other.getWidth() * other.getHeight();
	delete imageMap;
	imageMap = new Intensity[imageSize];
	for (int i = 0; i <= imageSize; i++){
		imageMap[i] = other.getPixel(i);
	}
}


void IntensityImageStudent::setPixel(int x, int y, Intensity pixel) {
	int pix = getWidth() * y + x;
	imageMap[pix] = pixel;
}

void IntensityImageStudent::setPixel(int i, Intensity pixel) {
	imageMap[i] = pixel;
}

Intensity IntensityImageStudent::getPixel(int x, int y) const {
	if (x <= getWidth()){
		if (y <= getHeight()){
			int pix = getWidth() * y + x;
			return imageMap[pix];
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

Intensity IntensityImageStudent::getPixel(int i) const {
	return imageMap[i];
}
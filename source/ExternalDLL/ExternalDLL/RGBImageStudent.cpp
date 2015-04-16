#include "RGBImageStudent.h"

RGBImageStudent::RGBImageStudent() : RGBImage() {
}

RGBImageStudent::RGBImageStudent(const RGBImageStudent &other) : RGBImage(other.getWidth(), other.getHeight()) {
	int imageSize =other.getHeight() * other.getWidth();
	imageStorage = new RGB [imageSize];
	for (int i = 0; i <= imageSize; i++){
		imageStorage[i] = other.getPixel(i);
	}

}

RGBImageStudent::RGBImageStudent(const int width, const int height) : RGBImage(width, height) {

	if (width > 0 && height > 0){
		int imageSize = width * height;
		imageStorage = new RGB[imageSize];
		for (int i = 0; i <= imageSize; i++){
			imageStorage[i] = 0;
		}
	}
}

RGBImageStudent::~RGBImageStudent() {
	delete imageStorage;
}

void RGBImageStudent::set(const int width, const int height) {
	int oldWidth = getWidth();
	int oldHeight = getHeight();
	RGBImage::set(width, height);
	if (oldHeight == height && oldWidth == width){
		int imageSize = height * width;
		for (int i = 0; i <= imageSize; i++){
			imageStorage[i] = 0;
		}
	}
	else{
		delete imageStorage;
		int imageSize = height * width;
		imageStorage = new RGB[imageSize];
		for (int i = 0; i <= imageSize; i++){
			imageStorage[i] = 0;
		}
	}
}

void RGBImageStudent::set(const RGBImageStudent &other) {
	int oldWidth = getWidth();
	int oldHeight = getHeight();
	RGBImage::set(other.getWidth(), other.getHeight());
	int imageSize = getHeight() * getWidth();
	if (oldWidth == getHeight() && oldHeight == getHeight()){
		for (int i = 0; i <= imageSize; i++){
			imageStorage[i] = other.getPixel(i);
		}
	}
	else{
		delete imageStorage;
		imageStorage = new RGB[imageSize];
		for (int i = 0; i <= imageSize; i++){
			imageStorage[i] = other.getPixel(i);
		}
	}

}

void RGBImageStudent::setPixel(int x, int y, RGB pixel) {
	
	int pix = getWidth() * y + x;
	imageStorage[pix] = pixel;

}

void RGBImageStudent::setPixel(int i, RGB pixel) {
	imageStorage[i] = pixel;
}

RGB RGBImageStudent::getPixel(int x, int y) const {
	int pix = getWidth() * y + x;
	return imageStorage[pix];
}

RGB RGBImageStudent::getPixel(int i) const {
	return imageStorage[i];
}
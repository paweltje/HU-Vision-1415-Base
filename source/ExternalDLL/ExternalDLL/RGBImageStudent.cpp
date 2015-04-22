#include "RGBImageStudent.h"
#include <cstring>

RGBImageStudent::RGBImageStudent() : RGBImage(), pixelMap(nullptr) {
	
}

RGBImageStudent::RGBImageStudent(const RGBImageStudent &other) : RGBImage(other.getWidth(), other.getHeight()), pixelMap(nullptr) {
	const int size = other.getWidth() * other.getHeight();
	if(size > 0) {
		pixelMap = new RGB[size];
		std::memcpy(pixelMap, other.pixelMap, sizeof(RGB) * size);
	}
}

RGBImageStudent::RGBImageStudent(const int width, const int height) : RGBImage(width, height), pixelMap(nullptr) {
	const int size = width * height;
	if(size > 0) {
		pixelMap = new RGB[size](); //With () will init heap on 0
	}
}

RGBImageStudent::~RGBImageStudent() {
	delete pixelMap;
}

void RGBImageStudent::set(const int width, const int height) {
	const int oldSize = getWidth() * getHeight(), newSize = width * height;
	RGBImage::set(width, height);
	
	if(oldSize == newSize && pixelMap != nullptr) {
		std::memset(pixelMap, 0, sizeof(RGB) * newSize);
	} else {
		delete pixelMap;
		if(newSize > 0) {
			pixelMap = new RGB[newSize]();
		} else {
			pixelMap = nullptr;
		}
	}
}

void RGBImageStudent::set(const RGBImageStudent &other) {
	const int oldSize = getWidth() * getHeight(), newSize = other.getWidth() * other.getHeight();
	RGBImage::set(other.getWidth(), other.getHeight());
	
	if(oldSize == newSize && pixelMap != nullptr) {
		std::memcpy(pixelMap, other.pixelMap, sizeof(RGB) * newSize);
	} else {
		delete pixelMap;
		if(newSize > 0) {
			pixelMap = new RGB[newSize];
			std::memcpy(pixelMap, other.pixelMap, sizeof(RGB) * newSize);
		} else {
			pixelMap = nullptr;
		}
	}
}

void RGBImageStudent::setPixel(int x, int y, RGB pixel) {
	int i = x + y * getWidth();
#ifdef _DEBUG
	if(i >= 0 && i < getWidth() * getHeight() && pixelMap != nullptr) {
		pixelMap[i] = pixel;
	}
#else
	pixelMap[i] = pixel;
#endif
}

void RGBImageStudent::setPixel(int i, RGB pixel) {
#ifdef _DEBUG
	if(i >= 0 && i < getWidth() * getHeight() && pixelMap != nullptr) {
		pixelMap[i] = pixel;
	}
#else
	pixelMap[i] = pixel;
#endif
}

RGB RGBImageStudent::getPixel(int x, int y) const {
	int i = x + y * getWidth();
#ifdef _DEBUG
	if(i >= 0 && i < getWidth() * getHeight() && pixelMap != nullptr) {
		return pixelMap[i];
	} else {
		return 0;
	}
#else
	return pixelMap[i];
#endif
}

RGB RGBImageStudent::getPixel(int i) const {
#ifdef _DEBUG
	if(i >= 0 && i < getWidth() * getHeight() && pixelMap != nullptr) {
		return pixelMap[i];
	} else {
		return 0;
	}
#else
	return pixelMap[i];
#endif
}
#include "IntensityImageStudent.h"
#include <cstring>

IntensityImageStudent::IntensityImageStudent() : IntensityImage(), pixelMap(nullptr) {
	
}

IntensityImageStudent::IntensityImageStudent(const IntensityImageStudent &other) : IntensityImage(other.getWidth(), other.getHeight()), pixelMap(nullptr) {
	const int size = other.getWidth() * other.getHeight();
	if(other.getWidth() > 0 && other.getHeight() > 0) {
		pixelMap = new Intensity[size];
		std::memcpy(pixelMap, other.pixelMap, sizeof(Intensity) * size);
	}
}

IntensityImageStudent::IntensityImageStudent(const int width, const int height) : IntensityImage(width, height), pixelMap(nullptr) {
	const int size = width * height;
	if(width > 0 && height > 0) {
		pixelMap = new Intensity[size](); //With () will init heap on 0
	}
}

IntensityImageStudent::~IntensityImageStudent() {
	delete pixelMap;
}

void IntensityImageStudent::set(const int width, const int height) {
	const int oldSize = getWidth() * getHeight(), newSize = width * height;
	IntensityImage::set(width, height);

	if(oldSize == newSize && pixelMap != nullptr) {
		std::memset(pixelMap, 0, sizeof(Intensity) * newSize);
	} else {
		delete pixelMap;
		if(newSize > 0) {
			pixelMap = new Intensity[newSize]();
		} else {
			pixelMap = nullptr;
		}
	}
}


void IntensityImageStudent::set(const IntensityImageStudent &other) {
	const int oldSize = getWidth() * getHeight(), newSize = other.getWidth() * other.getHeight();
	IntensityImage::set(other.getWidth(), other.getHeight());

	if(oldSize == newSize && pixelMap != nullptr) {
		std::memcpy(pixelMap, other.pixelMap, sizeof(Intensity) * newSize);
	} else {
		delete pixelMap;
		if(newSize > 0) {
			pixelMap = new Intensity[newSize];
			std::memcpy(pixelMap, other.pixelMap, sizeof(Intensity) * newSize);
		} else {
			pixelMap = nullptr;
		}
	}
}

void IntensityImageStudent::setPixel(int x, int y, Intensity pixel) {
	int i = x + y * getWidth();
#ifdef _DEBUG
	if(i >= 0 && i < getWidth() * getHeight() && pixelMap != nullptr) {
		pixelMap[i] = pixel;
	}
#else
	pixelMap[i] = pixel;
#endif
}

void IntensityImageStudent::setPixel(int i, Intensity pixel) {
#ifdef _DEBUG
	if(i >= 0 && i < getWidth() * getHeight() && pixelMap != nullptr) {
		pixelMap[i] = pixel;
	}
#else
	pixelMap[i] = pixel;
#endif
}

Intensity IntensityImageStudent::getPixel(int x, int y) const {
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

Intensity IntensityImageStudent::getPixel(int i) const {
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
#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	IntensityImage * returner = new IntensityImageStudent(image.getWidth(), image.getHeight());
	const int size = image.getWidth() * image.getHeight();
	if(image.getWidth() > 0 && image.getHeight() > 0) {
		for(int i = 0; i < size; i++) {
			RGB pixel = image.getPixel(i);
			returner->setPixel(i, pixel.r / 3 + pixel.g / 3 + pixel.b / 3);
		}
	}
	return returner;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}
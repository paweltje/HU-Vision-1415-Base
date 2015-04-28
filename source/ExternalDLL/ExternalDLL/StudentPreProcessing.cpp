#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	IntensityImage * returner = new IntensityImageStudent(image.getWidth(), image.getHeight());
	const int size = image.getWidth() * image.getHeight();
	if(image.getWidth() > 0 && image.getHeight() > 0) {
		for(int i = 0; i < size; i++) {
			RGB pixel = image.getPixel(i);
			returner->setPixel(i, (0.0722 * pixel.b) + (0.7152 * pixel.g) + (0.2126 * pixel.r));
		}
	}
	return returner;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

Intensity calc(const int matrix[7][7], int ** data) {
	Intensity total = 0;
	for(int x = 0; x < 7; x++) {
		for(int y = 0; y < 7; y++) {
			total += matrix[x][y] * data[x][y];
		}
	}
	return total;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	const int matrix[7][7] = {
		{0 ,  0, -1, -2, -1,  0,  0},
		{0 , -2, -3, -4, -3, -2,  0},
		{-1, -3,  1,  9,  1, -3, -1},
		{-2, -2,  9, 22,  9, -4, -2},
		{-1, -3,  1,  9,  1, -3, -1},
		{ 0, -2, -3, -4, -3, -2,  0},
		{ 0,  0, -1, -2, -1,  0,  0}
	};

	IntensityImage * returner = new IntensityImageStudent(image.getWidth(), image.getHeight());
	if(image.getWidth() <= 0 && image.getHeight() <= 0) {
		return returner;
	}
	int* data[7];
	for(int i = 0; i < 7; i++) {
		data[i] = new int[7]();
	}
	for(int x = 0; x < image.getWidth(); x++) {
		if(x < 3 || x > image.getWidth() - 3) {
			for(int y = 0; y < image.getHeight(); y++) {
				returner->setPixel(x, y, 0);
			}
		} else {
			for(int y = 0; y < image.getHeight(); y++) {
				if(y == 0) {
					for(int i = 0; i < 7; i++) {
						data[4][i] = image.getPixel(x - (i - 3), y + 1);
						data[5][i] = image.getPixel(x - (i - 3), y + 2);
						data[6][i] = image.getPixel(x - (i - 3), y + 3);
					}
				} else {
					for(int i = 0; i < 7; i++) {
						data[6][i] = image.getPixel(x - (i - 3), y + 3);
					}
				}

				if(y < 3 || y > image.getHeight() - 3) {
					returner->setPixel(x, y, 0);
				} else {
					returner->setPixel(x, y, calc(matrix, data));
				}

				int * temp = data[0];
				for(int i = 0; i < 6; i++) {
					data[i] = data[i + 1];
				}
				data[6] = temp;
			}		
		}
	}

	for(int i = 0; i < 7; i++) {
		delete data[i];
	}
	return returner;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	const int T = 220;

	IntensityImage * returner = new IntensityImageStudent(image.getWidth(), image.getHeight());
	const int size = image.getWidth() * image.getHeight();
	if(image.getWidth() > 0 && image.getHeight() > 0) {
		for(int i = 0; i < size; i++) {
			//returner->setPixel(i, image.getPixel(i) > T ? 0 : 255);
			returner->setPixel(i, image.getPixel(i));
		}
	}
	return returner;
}
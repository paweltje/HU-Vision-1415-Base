#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"

#include <iostream>

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

Intensity calc7(const int matrix[7][7], int ** data) {
	const int size = 7;
	int total = 127;
	for(int x = 0; x < size; x++) {
		for(int y = 0; y < size; y++) {
			total += matrix[x][y] * data[x][y];
		}
	}

	if(total > 127) {
		return 255;
	} else if(total < -127) {
		return 0;
	} else {
		return static_cast<Intensity>(total + 127);
	}
	return total;
}

Intensity calc9(const int matrix[9][9], int ** data) {
	const int size = 9;
	int total = 127;
	for(int x = 0; x < size; x++) {
		for(int y = 0; y < size; y++) {
			total += matrix[x][y] * data[x][y];
		}
	}

	if(total > 127) {
		return 255;
	} else if(total < -127) {
		return 0;
	} else {
		return static_cast<Intensity>(total + 127);
	}
	return total;
}

#define MATRIX7

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
#ifdef MATRIX7
	const int matrixSize = 7;
	const int halfMatrixSize = 3;

	const int matrix[matrixSize][matrixSize] = {
		{0 ,  0, -1, -2, -1,  0,  0},
		{0 , -2, -3, -4, -3, -2,  0},
		{-1, -3,  1,  9,  1, -3, -1},
		{-2, -2,  9, 22,  9, -4, -2},
		{-1, -3,  1,  9,  1, -3, -1},
		{ 0, -2, -3, -4, -3, -2,  0},
		{ 0,  0, -1, -2, -1,  0,  0}
	};
#else
	const int matrixSize = 9;
	const int halfMatrixSize = 4;
	const int matrix[matrixSize][matrixSize] = {
		{ 0, -1, -1, -2, -2, -2, -1, -1,  0},
		{-1, -2, -4, -5, -5, -5, -4, -2, -1},
		{-1, -4, -5, -3,  0, -3, -5, -4, -1},
		{-2, -5, -3, 12, 24, 12, -3, -5, -2},
		{-2, -5,  0, 24, 40, 24,  0, -5, -2},
		{-2, -5, -3, 12, 24, 12, -3, -5, -2},
		{-1, -4, -5, -3,  0, -3, -5, -4, -1},
		{-1, -2, -4, -5, -5, -5, -4, -2, -1},
		{ 0, -1, -1, -2, -2, -2, -1, -1,  0}
	};
#endif

	IntensityImage * returner = new IntensityImageStudent(image.getWidth(), image.getHeight());
	if(image.getWidth() <= 0 && image.getHeight() <= 0) {
		return returner;
	}
	int* data[matrixSize];
	for(int i = 0; i < matrixSize; i++) {
		data[i] = new int[matrixSize]();
	}
	for(int x = 0; x < image.getWidth(); x++) {
		if(x < halfMatrixSize || x > image.getWidth() - halfMatrixSize) {
			for(int y = 0; y < image.getHeight(); y++) {
				returner->setPixel(x, y, 0);
			}
		} else {
			for(int y = 0; y < image.getHeight(); y++) {
				if(y == 0) {
					for(int i = 0; i < matrixSize; i++) {
						for(int j = halfMatrixSize + 1; j < matrixSize; j++) {
							data[j][i] = image.getPixel(x - (i - halfMatrixSize), y + j - halfMatrixSize);
						}
					}
				} else {
					for(int i = 0; i < matrixSize; i++) {
						data[matrixSize - 1][i] = image.getPixel(x - (i - halfMatrixSize), y + halfMatrixSize);
					}
				}

				if(y < halfMatrixSize || y > image.getHeight() - halfMatrixSize) {
					returner->setPixel(x, y, 0);
				} else {
#ifdef MATRIX7
					returner->setPixel(x, y, ~calc7(matrix, data));
#else
					returner->setPixel(x, y, ~calc9(matrix, data));
#endif
				}

				int * temp = data[0];
				for(int i = 0; i < matrixSize - 1; i++) {
					data[i] = data[i + 1];
				}
				data[matrixSize - 1] = temp;
			}		
		}
	}

	for(int i = 0; i < matrixSize; i++) {
		delete data[i];
	}
	return returner;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	const int T = 245;

#ifdef MATRIX7
	const int minBlackNeigboursCount = 5; //7x7 matrix
#else
	const int minBlackNeigboursCount = 9; //9x9 matrix
#endif

	IntensityImage * step1 = new IntensityImageStudent(image.getWidth(), image.getHeight());
	const int size = image.getWidth() * image.getHeight();
	if(image.getWidth() > 0 && image.getHeight() > 0) {
		for(int i = 0; i < size; i++) {
#ifndef STOPTHRESHOLDING
			step1->setPixel(i, image.getPixel(i) > T ? 0 : 255);
#else
			step1->setPixel(i, image.getPixel(i));
#endif
		}
	}

	IntensityImage * step2 = new IntensityImageStudent(*step1);
#ifndef STOPTHRESHOLDING
	for(int x = 2; x < image.getWidth() - 2; x++) {
		for(int y = 2; y < image.getHeight() - 2; y++) {
			if(step1->getPixel(x, y) == 0) {
				int blackNeighboursCount = 0;
				for(int x2 = x - 2; x2 < x + 2; x2++) {
					for(int y2 = y - 2; y2 < y + 2; y2++) {
						if(step1->getPixel(x2, y2) == 0) {
							blackNeighboursCount++;
						}
					}
				}

				if(blackNeighboursCount < minBlackNeigboursCount) {
					step2->setPixel(x, y, 255);
				}
			}
		}
	}
#endif

	return step2;
}
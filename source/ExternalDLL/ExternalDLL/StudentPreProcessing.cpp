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

Intensity calc(const int matrix[7][7], int ** data) {
	int total = 127;
	for(int x = 0; x < 7; x++) {
		for(int y = 0; y < 7; y++) {
			total += matrix[x][y] * data[x][y];
		}
		//std::cout << (int)total << " ";
	}

	if(total > 127) {
		return 255;
	} else if(total < -127) {
		return 0;
	} else {
		return static_cast<Intensity>(total + 127);
	}

	//std::cout << std::endl << (int)total << std::endl;
	return total;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	std::cout << std::endl << std::endl;
	std::cout << "=========Step Edge Detection=========" << std::endl;
	std::cout << "===============Wibren================" << std::endl;

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
				returner->setPixel(x, y, 255);
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
					returner->setPixel(x, y, 255);
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
	const int T = 25;

	IntensityImage * returner = new IntensityImageStudent(image.getWidth(), image.getHeight());
	const int size = image.getWidth() * image.getHeight();
	if(image.getWidth() > 0 && image.getHeight() > 0) {
		for(int i = 0; i < size; i++) {
			returner->setPixel(i, image.getPixel(i) > T ? 255 : 0);
			//returner->setPixel(i, image.getPixel(i));
		}
	}

	for(int x = 2; x < image.getWidth() - 2; x++) {
		for(int y = 2; y < image.getHeight() - 2; y++) {
			if(returner->getPixel(x, y) == 0) {
				int blackNeighboursCount = 0;
				for(int x2 = x - 2; x2 < x + 2; x2++) {
					for(int y2 = y - 2; y2 < y + 2; y2++) {
						if(returner->getPixel(x2, y2) == 0) {
							blackNeighboursCount++;
						}
					}
				}
			}
		}
	}

	return returner;
}
#include "StudentExtraction.h"
#include "IntensityImageStudent.h"

#include "ImageIO.h"

bool StudentExtraction::stepExtractEyes(const IntensityImage &image, FeatureMap &features) const {
	std::cout << "Step Pre!\n";

	const int LeftLeftX = std::fmin(features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[0].getX(), features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[1].getX());
	const int LeftRightX = std::fmax(features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[0].getX(), features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[1].getX());
	const int LeftUpY = std::fmin(features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[0].getY(), features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[1].getY());
	const int LeftDownY = std::fmax(features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[0].getY(), features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[1].getY());

	const int RightLeftX = std::fmin(features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[0].getX(), features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[1].getX());
	const int RightRightX = std::fmax(features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[0].getX(), features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[1].getX());
	const int RightUpY = std::fmin(features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[0].getY(), features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[1].getY());
	const int RightDownY = std::fmax(features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[0].getY(), features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[1].getY());

	const int LeftWidth = LeftRightX - LeftLeftX;
	const int LeftHeight = LeftDownY - LeftUpY;
	const int LeftSize = LeftWidth * LeftHeight;
	if(LeftSize == 0) {
		return false;
	}

	const int RightWidth = RightRightX - RightLeftX;
	const int RightHeight = RightDownY - RightUpY;
	const int RightSize = RightWidth * RightHeight;
	if(RightSize == 0) {
		return false;
	}

	IntensityImage * LeftEyeCopy = new IntensityImageStudent(LeftWidth, LeftHeight);
	IntensityImage * RightEyeCopy = new IntensityImageStudent(RightWidth, RightHeight);

	for(int x = LeftLeftX; x < LeftRightX; x++) {
		for(int y = LeftUpY; y < LeftDownY; y++) {
			LeftEyeCopy->setPixel(x - LeftLeftX, y - LeftUpY, image.getPixel(x, y));
		}
	}

	for(int x = RightLeftX; x < RightRightX; x++) {
		for(int y = RightUpY; y < RightDownY; y++) {
			RightEyeCopy->setPixel(x - RightLeftX, y - RightUpY, image.getPixel(x, y));
		}
	}

	IntensityImageStudent * LeftEyeSmall = new IntensityImageStudent(LeftWidth, LeftHeight);
	IntensityImageStudent * RightEyeSmall = new IntensityImageStudent(RightWidth, RightHeight);

	LeftEyeSmall->set(*LeftEyeCopy);
	RightEyeSmall->set(*RightEyeCopy);

	for(int x = 0; x < LeftWidth; x++) {
		for(int y = 0; y < LeftHeight; y++) {
			if(LeftEyeCopy->getPixel(x, y) == 255) {
				if(x > 0 && y > 0 && x < LeftWidth - 1 && y < LeftHeight - 1) {
					LeftEyeSmall->setPixel(x - 1, y, 255);
					LeftEyeSmall->setPixel(x, y - 1, 255);
					LeftEyeSmall->setPixel(x + 1, y, 255);
					LeftEyeSmall->setPixel(x, y + 1, 255);
				} else {
					if(x > 0) {
						LeftEyeSmall->setPixel(x - 1, y, 255);
					}
					if(y > 0) {
						LeftEyeSmall->setPixel(x, y - 1, 255);
					}
					if(x < LeftWidth - 1) {
						LeftEyeSmall->setPixel(x + 1, y, 255);
					}
					if(y < LeftHeight - 1) {
						LeftEyeSmall->setPixel(x, y + 1, 255);
					}
				}
			}
		}
	}

	for(int x = 0; x < RightWidth; x++) {
		for(int y = 0; y < RightHeight; y++) {
			if(RightEyeCopy->getPixel(x, y) == 255) {
				if(x > 0 && y > 0 && x < RightWidth - 1 && y < RightHeight - 1) {
					RightEyeSmall->setPixel(x - 1, y, 255);
					RightEyeSmall->setPixel(x, y - 1, 255);
					RightEyeSmall->setPixel(x + 1, y, 255);
					RightEyeSmall->setPixel(x, y + 1, 255);
				} else {
					if(x > 0) {
						RightEyeSmall->setPixel(x - 1, y, 255);
					}
					if(y > 0) {
						RightEyeSmall->setPixel(x, y - 1, 255);
					}
					if(x < RightWidth - 1) {
						RightEyeSmall->setPixel(x + 1, y, 255);
					}
					if(y < RightHeight - 1) {
						RightEyeSmall->setPixel(x, y + 1, 255);
					}
				}
			}
		}
	}

	delete LeftEyeCopy;
	delete RightEyeCopy;
	LeftEyeCopy = nullptr;
	RightEyeCopy = nullptr;

	IntensityImageStudent * LeftEyeFinal = new IntensityImageStudent(LeftWidth, LeftHeight);
	IntensityImageStudent * RightEyeFinal = new IntensityImageStudent(RightWidth, RightHeight);

	LeftEyeFinal->set(*LeftEyeSmall);
	RightEyeFinal->set(*RightEyeSmall);

	for(int x = 0; x < LeftWidth; x++) {
		for(int y = 0; y < LeftHeight; y++) {
			if(LeftEyeSmall->getPixel(x, y) == 0) {
				if(x > 0 && y > 0 && x < LeftWidth -1 && y < LeftHeight - 1) {
					LeftEyeFinal->setPixel(x - 1, y, 0);
					LeftEyeFinal->setPixel(x, y - 1, 0);
					LeftEyeFinal->setPixel(x + 1, y, 0);
					LeftEyeFinal->setPixel(x, y + 1, 0);
				} else {
					if(x > 0) {
						LeftEyeFinal->setPixel(x - 1, y, 0);
					}
					if(y > 0) {
						LeftEyeFinal->setPixel(x, y - 1, 0);
					}
					if(x < LeftWidth - 1) {
						LeftEyeFinal->setPixel(x + 1, y, 0);
					}
					if(y < LeftHeight - 1) {
						LeftEyeFinal->setPixel(x, y + 1, 0);
					}
				}
			}
		}
	}

	for(int x = 0; x < RightWidth; x++) {
		for(int y = 0; y < RightHeight; y++) {
			if(RightEyeSmall->getPixel(x, y) == 0) {
				if(x > 0 && y > 0 && x < RightWidth - 1 && y < RightHeight - 1) {
					RightEyeFinal->setPixel(x - 1, y, 0);
					RightEyeFinal->setPixel(x, y - 1, 0);
					RightEyeFinal->setPixel(x + 1, y, 0);
					RightEyeFinal->setPixel(x, y + 1, 0);
				} else {
					if(x > 0) {
						RightEyeFinal->setPixel(x - 1, y, 0);
					}
					if(y > 0) {
						RightEyeFinal->setPixel(x, y - 1, 0);
					}
					if(x < RightWidth - 1) {
						RightEyeFinal->setPixel(x + 1, y, 0);
					}
					if(y < RightHeight - 1) {
						RightEyeFinal->setPixel(x, y + 1, 0);
					}
				}
			}
		}
	}

	delete LeftEyeSmall;
	delete RightEyeSmall;
	LeftEyeSmall = nullptr;
	RightEyeSmall = nullptr;

	unsigned long int LeftTotalX = 0, LeftTotalY = 0, RightTotalX = 0, RightTotalY = 0, LeftBlackCount = 0, RightBlackCount = 0;

	for(int x = 0; x < LeftWidth; x++) {
		for(int y = 0; y < LeftHeight; y++) {
			if(LeftEyeFinal->getPixel(x, y) == 0) {
				LeftTotalX += x;
				LeftTotalY += y;
				LeftBlackCount++;
			}
		}
	}

	for(int x = 0; x < RightWidth; x++) {
		for(int y = 0; y < RightHeight; y++) {
			if(RightEyeFinal->getPixel(x, y) == 0) {
				RightTotalX += x;
				RightTotalY += y;
				RightBlackCount++;
			}
		}
	}

	int LeftCenterX, LeftCenterY, RightCenterX, RightCenterY;
	LeftCenterX = LeftTotalX / LeftBlackCount;
	LeftCenterY = LeftTotalY / LeftBlackCount;
	RightCenterX = RightTotalX / RightBlackCount;
	RightCenterY = RightTotalY / RightBlackCount;

	int LeftEyeStartX = LeftCenterX,  LeftEyeEndX = LeftCenterX, LeftEyeStartY = LeftCenterY, LeftEyeEndY = LeftCenterY, RightEyeStartX = RightCenterX, RightEyeEndX = RightCenterX, RightEyeStartY = RightCenterY, RightEyeEndY = RightCenterY;
	bool LeftWait = LeftEyeFinal->getPixel(LeftCenterX, LeftCenterY) == 255;
	bool RightWait = RightEyeFinal->getPixel(RightCenterX, RightCenterY) == 255;
	bool Wait = LeftWait;

	for (int x = LeftCenterX; x >= 0; x--){
		if(LeftEyeFinal->getPixel(x, LeftCenterY) == 0) {
			LeftEyeStartX = x;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}
	Wait = LeftWait;
	for (int x = LeftCenterX; x < LeftWidth; x++){
		if(LeftEyeFinal->getPixel(x, LeftCenterY) == 0) {
			LeftEyeEndX = x;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}
	Wait = LeftWait;
	for (int y = LeftCenterY; y >= 0; y--){
		if(LeftEyeFinal->getPixel(LeftCenterX, y) == 0) {
			LeftEyeStartY = y;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}
	Wait = LeftWait;
	for (int y = LeftCenterY; y < LeftHeight; y++){
		if(LeftEyeFinal->getPixel(LeftCenterX, y) == 0) {
			LeftEyeEndY = y;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}

	Wait = RightWait;
	for (int x = RightCenterX; x >= 0; x--){
		if(RightEyeFinal->getPixel(x, RightCenterY) == 0) {
			RightEyeStartX = x;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}
	Wait = RightWait;
	for (int x = RightCenterX; x < RightWidth; x++){
		if(RightEyeFinal->getPixel(x, RightCenterY) == 0) {
			RightEyeEndX = x;
			Wait = false;
		} else if(!Wait){
			break;
		}
	}
	Wait = RightWait;
	for (int y = RightCenterY; y >= 0; y--){
		if(RightEyeFinal->getPixel(RightCenterX, y) == 0) {
			RightEyeStartY = y;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}
	Wait = RightWait;
	for (int y = RightCenterY; y < RightHeight; y++){
		if(RightEyeFinal->getPixel(RightCenterX, y) == 0) {
			RightEyeEndY = y;
			Wait = false;
		} else if(!Wait) {
			break;
		}
	}

	delete LeftEyeFinal;
	delete RightEyeFinal;

	features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[0].setX(LeftEyeStartX + LeftLeftX);
	features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[0].setY(LeftEyeStartY + LeftUpY);
	features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[1].setX(LeftEyeEndX + LeftLeftX);
	features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints()[1].setY(LeftEyeEndY + LeftUpY);

	features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[0].setX(RightEyeStartX + RightLeftX);
	features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[0].setY(RightEyeStartY + RightUpY);
	features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[1].setX(RightEyeEndX + RightLeftX );
	features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints()[1].setY(RightEyeEndY + RightUpY);

	Feature leftCenter(Feature::FEATURE_NOSTRIL_LEFT);
	leftCenter.addPoint(Point2D<double>(LeftCenterX + LeftLeftX, LeftCenterY + LeftUpY));

	Feature rightCenter(Feature::FEATURE_NOSTRIL_RIGHT);
	rightCenter.addPoint(Point2D<double>(RightCenterX + RightLeftX, RightCenterY + RightUpY));

	return true;
}

bool StudentExtraction::stepExtractNose(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentExtraction::stepExtractMouth(const IntensityImage &image, FeatureMap &features) const {
	return false;
}
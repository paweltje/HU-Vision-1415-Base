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

	ImageIO::isInDebugMode = true;
	ImageIO::saveIntensityImage(image, "C:\\Users\\Wibren\\Desktop\\temp.png");
	ImageIO::isInDebugMode = false;

	std::cout << "Step 1!\n";

	IntensityImage * LeftEyeGrayCopy = new IntensityImageStudent(LeftWidth, LeftHeight);
	IntensityImage * RightEyeGrayCopy = new IntensityImageStudent(RightWidth, RightHeight);

	for(int x = LeftLeftX; x < LeftRightX; x++) {
		for(int y = LeftUpY; y < LeftDownY; y++) {
			LeftEyeGrayCopy->setPixel(x - LeftLeftX, y - LeftUpY, image.getPixel(x, y));
		}
	}

	for(int x = RightLeftX; x < RightRightX; x++) {
		for(int y = RightUpY; y < RightDownY; y++) {
			RightEyeGrayCopy->setPixel(x - RightLeftX, y - RightUpY, image.getPixel(x, y));
		}
	}

	std::cout << "Step 2!\n";

	int LeftEyeHistogram[256] = {0};

	for(int i = 0; i < 255; i++) {
		LeftEyeHistogram[i] = 0;
	}

	int RightEyeHistogram[256] = {0};
	
	for (int i = 0; i < LeftSize; i++){
		LeftEyeHistogram[LeftEyeGrayCopy->getPixel(i)]++;
	}
	for (int i = 0; i < RightSize; i++){
		RightEyeHistogram[RightEyeGrayCopy->getPixel(i)] ++;
	}

	int check = LeftSize;
	for(int i = 0; i < 256; i++) {
		std::cout << "Check: " << check << "\n";
		check -= LeftEyeHistogram[i];
	}
	if(check != 0) {
		std::cout << "Problems " << check << "\n";
	}

	std::cout << "Step 3!\n";

	const float percBlackestPixels = 0.10f;
	int LeftCenterSizeThreshold = 0;
	int RightCenterSizeThreshold = 0;

	int LeftCenterThreshold = percBlackestPixels * LeftSize;
	int RightCenterThreshold = percBlackestPixels * RightSize;

	std::cout << "LCT: " << LeftCenterThreshold << "\n";

	for(int i = 255; i >= 0; i--) {
		LeftCenterThreshold -= LeftEyeHistogram[i];
		std::cout << "LCT: " << LeftCenterThreshold << "\n";
		LeftCenterSizeThreshold += LeftEyeHistogram[i];
		std::cout << "LCS: " << LeftCenterSizeThreshold << "\n";
		if(LeftCenterThreshold < 0) {
			LeftCenterThreshold = i;
			std::cout << "Left Threshold" << i << "\n";
			break;
		}	
	}

	for(int i = 255; i >= 0; i--) {
		RightCenterThreshold -= RightEyeHistogram[i];
		RightCenterSizeThreshold += RightEyeHistogram[i];
		if(RightCenterThreshold < 0) {
			RightCenterThreshold = i;
			std::cout << "Right Threshold" << i << "\n";
			break;
		}
	}

	std::cout << "Step 4!\n";

	unsigned long int LeftTotalX = 0, LeftTotalY = 0, RightTotalX = 0, RightTotalY = 0;

	for(int x = 0; x < LeftWidth; x++) {
		for(int y = 0; y < LeftHeight; y++) {
			if(LeftEyeGrayCopy->getPixel(x, y) >= LeftCenterThreshold) {
				LeftTotalX += x;
				LeftTotalY += y;
			}
		}
	}

	for(int x = 0; x < RightWidth; x++) {
		for(int y = 0; y < RightHeight; y++) {
			if(RightEyeGrayCopy->getPixel(x, y) >= RightCenterThreshold) {
				RightTotalX += x;
				RightTotalY += y;
			}
		}
	}

	int LeftCenterX, LeftCenterY, RightCenterX, RightCenterY;
	LeftCenterX = LeftTotalX / LeftCenterSizeThreshold;
	LeftCenterY = LeftTotalY / LeftCenterSizeThreshold;
	RightCenterX = RightTotalX / RightCenterSizeThreshold;
	RightCenterY = RightTotalY / RightCenterSizeThreshold;

	std::cout << "Left Center Point (" << LeftCenterX << ", " << LeftCenterY << ")\nRight Center Point (" << RightCenterX << ", " << RightCenterY << ")\n";

	std::cout << "Step 5!\n";

	int LeftEyeStartX, LeftEyeEndX, LeftEyeStartY, LeftEyeEndY, RightEyeStartX, RightEyeEndX, RightEyeStartY, RightEyeEndY;
	Intensity LeftCenterValue = LeftEyeGrayCopy->getPixel(LeftCenterX, LeftCenterY);
	Intensity RightCenterValue = RightEyeGrayCopy->getPixel(RightCenterX, RightCenterY);
	for (int x = LeftCenterX; x >= 0; x--){
		if (LeftEyeGrayCopy->getPixel(x, LeftCenterY) < LeftCenterValue + 64){
			LeftEyeStartX = x;
			break;
		}
	}
	for (int x = LeftCenterX; x < LeftWidth; x++){
		if (LeftEyeGrayCopy->getPixel(x, LeftCenterY) < LeftCenterValue + 64){
			LeftEyeEndX = x;
			break;
		}
	}
	for (int y = LeftCenterY; y >= 0; y--){
		if (LeftEyeGrayCopy->getPixel(LeftCenterX, y) < LeftCenterValue + 64){
			LeftEyeStartY = y;
			break;
		}
	}
	for (int y = LeftCenterY; y < LeftHeight; y++){
		if (LeftEyeGrayCopy->getPixel(LeftCenterX, y) < LeftCenterValue + 64){
			LeftEyeEndY = y;
			break;
		}
	}

	for (int x = RightCenterX; x >= 0; x--){
		if (RightEyeGrayCopy->getPixel(x, RightCenterY) < RightCenterValue + 64){
			RightEyeStartX = x;
			break;
		}
	}
	for (int x = RightCenterX; x < RightWidth; x++){
		if (RightEyeGrayCopy->getPixel(x, RightCenterY) < RightCenterValue + 64){
			RightEyeEndX = x;
			break;
		}
	}
	for (int y = RightCenterY; y >= 0; y--){
		if (RightEyeGrayCopy->getPixel(RightCenterX, y) < RightCenterValue + 64){
			RightEyeStartY = y;
			break;
		}
	}
	for (int y = RightCenterY; y < RightHeight; y++){
		if (RightEyeGrayCopy->getPixel(RightCenterX, y) < RightCenterValue + 64){
			RightEyeEndY = y;
			break;
		}
	}

	std::cout << "Step Final!\n";

	delete LeftEyeGrayCopy;
	delete RightEyeGrayCopy;

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

	features.putFeature(leftCenter);
	features.putFeature(rightCenter);

	std::cout << "Doei!\n";

	return true;
}

bool StudentExtraction::stepExtractNose(const IntensityImage &image, FeatureMap &features) const {
	return true;
}

bool StudentExtraction::stepExtractMouth(const IntensityImage &image, FeatureMap &features) const {
	return false;
}
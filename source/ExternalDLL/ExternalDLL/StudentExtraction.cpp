#include "StudentExtraction.h"
#include "IntensityImageStudent.h"

bool StudentExtraction::stepExtractEyes(const IntensityImage &image, FeatureMap &features) const {
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

	IntensityImage * LeftEyeGrayCopy = new IntensityImageStudent(LeftWidth, LeftHeight);
	IntensityImage * RightEyeGrayCopy = new IntensityImageStudent(RightWidth, RightHeight);

	for(int x = LeftLeftX; x < LeftRightX; x++) {
		for(int y = LeftUpY; y < LeftDownY; y++) {
			LeftEyeGrayCopy->setPixel(x, y, image.getPixel(x, y));
		}
	}

	for(int x = RightLeftX; x < RightRightX; x++) {
		for(int y = RightUpY; y < RightDownY; y++) {
			RightEyeGrayCopy->setPixel(x, y, image.getPixel(x, y));
		}
	}
	int LeftEyeHistogram[255] = {0};
	int RightEyeHistogram[255] = {0};
	
	for (int i = 0; i < LeftSize; i++){
		LeftEyeHistogram[LeftEyeGrayCopy->getPixel(i)] ++;
	}
	for (int i = 0; i < RightSize; i++){
		RightEyeHistogram[RightEyeGrayCopy->getPixel(i)] ++;
	}


	return false;
}

bool StudentExtraction::stepExtractNose(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentExtraction::stepExtractMouth(const IntensityImage &image, FeatureMap &features) const {
	return false;
}
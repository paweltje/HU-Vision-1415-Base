#include "StudentLocalization.h"
#include "IntensityImageStudent.h"

bool StudentLocalization::stepFindHead(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseMouthAndChin(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindChinContours(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseEndsAndEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const {
	const int minimalEyeHeight = 7, minimalSearchErea = 10, minimalBlackCountEye = 20, maximalBlackCountSkin = 15;

	std::cout << std::endl << std::endl;
	std::cout << "=========Localization step 5=========" << std::endl;
	std::cout << "Searching for: Eye's" << std::endl;
	std::cout << "===============Wibren================" << std::endl;

	Point2D<double> headMostLeftPoint = features.getFeature(Feature::FEATURE_HEAD_LEFT_NOSE_BOTTOM).getPoints()[0];
	Point2D<double> headMostRightPoint = features.getFeature(Feature::FEATURE_HEAD_RIGHT_NOSE_BOTTOM).getPoints()[0];
	Point2D<double> headTopPoint = features.getFeature(Feature::FEATURE_HEAD_TOP).getPoints()[0];
	Point2D<double> headBottomPoint = features.getFeature(Feature::FEATURE_CHIN).getPoints()[0];

	Point2D<double> HeadCenter((headMostLeftPoint.x + headMostRightPoint.x) / 2, (headTopPoint.y + headBottomPoint.y) / 2);

	Point2D<double> noseMostLeftPoint = features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0];
	Point2D<double> noseMostRightPoint = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0];

	int ystart = 0; //eye start hight
	int yend = 0;   //eye end	hight

	for(int y = HeadCenter.y - minimalSearchErea; y < HeadCenter.y + minimalSearchErea || yend == 0; y++) { //each row in minimal search area or if no eye found extended
		int blackCount = 0;
		for(int x = headMostLeftPoint.x; x < headMostRightPoint.x; x++) { //search row
			if(image.getPixel(x, y) < 128) blackCount++;				  //if black increment counter
		}
		std::cout << y << ": " << blackCount << ", ";
		if(blackCount > minimalBlackCountEye && ystart == 0)		      //More black in row then minimalBlackCountEye and no start setted yet
			ystart = y;
		if(blackCount < maximalBlackCountSkin && ystart != 0 && yend == 0)//Less black in row then maximalBlackCountSkin and started and not ended yet
			yend = y;
		if(blackCount > minimalBlackCountEye && yend != 0 && yend - ystart < minimalEyeHeight) { //More black in row then minimalBlackCountEye and the previeuwes known "eye" is properly a eyebrow
			ystart = y;
			yend = 0;
		}
	}

	std::cout << std::endl << ystart << ' ' << yend << std::endl;

	Feature leftEyeRect(Feature::FEATURE_EYE_LEFT_RECT);
	leftEyeRect.addPoint(Point2D<double>(noseMostLeftPoint.x, ystart));
	leftEyeRect.addPoint(Point2D<double>(noseMostLeftPoint.x - 10, yend));

	Feature rightEyeRect(Feature::FEATURE_EYE_RIGHT_RECT);
	rightEyeRect.addPoint(Point2D<double>(noseMostRightPoint.x, ystart));
	rightEyeRect.addPoint(Point2D<double>(noseMostRightPoint.x + 10, yend));

	features.putFeature(leftEyeRect);
	features.putFeature(rightEyeRect);

	return true;
}
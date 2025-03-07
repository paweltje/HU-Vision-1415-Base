#include "StudentLocalization.h"
#include "IntensityImageStudent.h"
#include <math.h>

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

	std::cout << "Finding center X: " << headMostLeftPoint.x << " <> " << headMostRightPoint.x << " = " << HeadCenter.x << " Y: " << headTopPoint.y << " <> " << headBottomPoint.y << " = " << HeadCenter.y << std::endl;

	Point2D<double> noseMostLeftPoint = features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0];
	Point2D<double> noseMostRightPoint = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0];

	double highestNoseHight = noseMostLeftPoint.y < noseMostRightPoint.y ? noseMostLeftPoint.y : noseMostRightPoint.y;

	int ystart = 0; //eye start hight
	int yend = 0;   //eye end	hight

	for(int y = HeadCenter.y - minimalSearchErea; y < highestNoseHight; y++) { //each row in minimal search area or if no eye found extended
		int blackCount = 0;
		for(int x = headMostLeftPoint.x; x < headMostRightPoint.x; x++) { //search row
			if(image.getPixel(x, y) < 128) blackCount++;				  //if black increment counter
		}
		//::cout << y << ": " << blackCount << ", ";
		if(blackCount > minimalBlackCountEye && ystart == 0)		      //More black in row then minimalBlackCountEye and no start setted yet
			ystart = y;
		if(blackCount < maximalBlackCountSkin && ystart != 0 && yend == 0)//Less black in row then maximalBlackCountSkin and started and not ended yet
			yend = y;
		if(blackCount > minimalBlackCountEye && yend != 0 /*&& yend - ystart < minimalEyeHeight*/) { //More black in row then minimalBlackCountEye and the previeuwes known "eye" is properly a eyebrow
			ystart = y;
			yend = 0;
		}
	}
	int headmiddle = (headMostRightPoint.x + headMostLeftPoint.x) / 2;
	std::cout <<"This is head middle: " <<  headmiddle << std::endl;
	int blackCount = 0;
	int xstartleft = 0;
	int xendleft = 0;
	int xstartright = 0;
	int xendright = 0;
	for (int x = noseMostLeftPoint.getX() - 5; x < headmiddle; x++){
		blackCount = 0;
		for (int y = ystart; y < yend; y++){
			if (image.getPixel(x, y) == 0){
				blackCount++;
			}
		}
		if (blackCount == 0){
			xendleft = x;
			break;
		}
	}
	for (int x = noseMostLeftPoint.getX() - 5; x >= headMostLeftPoint.x; x--){
		blackCount = 0;
		for (int y = ystart; y < yend; y++){
			if (image.getPixel(x, y) == 0){
				blackCount++;
			}
		}	
		if (blackCount == 0){
			xstartleft = x;
			break;
		}
	}
	for (int x = noseMostRightPoint.getX() + 5; x < headMostRightPoint.x; x++){
		blackCount = 0;
		for (int y = ystart; y < yend; y++){
			if (image.getPixel(x,y) == 0){
				blackCount++;
			}
		}
		if (blackCount == 0){
			xendright = x;
			break;
		}
	}
	for (int x = noseMostRightPoint.getX() + 5; x > headmiddle; x--){
		blackCount = 0;
		for (int y = ystart; y < yend; y++){
			if (image.getPixel(x, y) == 0){
				blackCount++;
			}
		}
		if (blackCount == 0){
			xstartright = x;
			break;
		}
	}

	
	
	IntensityImageStudent * returner = new IntensityImageStudent();
	returner->set(image);
	for (int x = noseMostLeftPoint.x - 10; x < noseMostLeftPoint.x; x++){
		for (int y = ystart; y < yend; y++){
			if (image.getPixel(x, y) == 1){
				returner->setPixel(x - 1, y, 1);
				returner->setPixel(x + 1, y, 1);
				returner->setPixel(x, y + 1, 1);
				returner->setPixel(x, y - 1, 1);
			}
		}
	}
	for (int x = noseMostRightPoint.x; x < noseMostRightPoint.x + 10; x++){
		for (int y = ystart; y < yend; y++){
			if (image.getPixel(x, y) == 1){
				returner->setPixel(x - 1, y, 1);
				returner->setPixel(x + 1, y, 1);
				returner->setPixel(x, y + 1, 1);
				returner->setPixel(x, y - 1, 1);
			}
		}
	}

	std::cout << std::endl << ystart << ' ' << yend << std::endl;

	Feature leftEyeRect(Feature::FEATURE_EYE_LEFT_RECT);
	leftEyeRect.addPoint(Point2D<double>(xendleft, ystart));
	leftEyeRect.addPoint(Point2D<double>(xstartleft, yend));

	Feature rightEyeRect(Feature::FEATURE_EYE_RIGHT_RECT);
	rightEyeRect.addPoint(Point2D<double>(xstartright, ystart));
	rightEyeRect.addPoint(Point2D<double>(xendright, yend));


	features.putFeature(leftEyeRect);
	features.putFeature(rightEyeRect);

	return true;
}
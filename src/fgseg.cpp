#include <opencv2/opencv.hpp>
#include "fgseg.hpp"
#include <random>

using namespace fgseg;

bgs::bgs(int bgsMode, int morphologicalOpsMode)
{
	_bgsMode = bgsMode;
	_morphOpsMode = morphologicalOpsMode;

	if (_bgsMode == 0) {
		_pMOG2 = cv::createBackgroundSubtractorMOG2();
	}
}

cv::Mat bgs::getFGmask(cv::Mat frame, double learningrate) {

	if (_bgsMode == 0){
		// Learning Rate (learningrate):
		// The value between 0 and 1 that indicates how fast the background model is
		// learnt. Negative parameter (default -1) value makes the algorithm to use some automatically chosen learning
		// rate. 0 means that the background model is not updated at all, 1 means that the background model
		// is completely reinitialized from the last frame.
		_pMOG2->apply(frame, _fgmask, learningrate);
		RunMorphologicalOps(_morphOpsMode);
	}

	return _fgmask;
}


//Performs Morphological operations to get rid of small dots (OPENING) and to remove small holes (CLOSING)
void bgs::RunMorphologicalOps(int iMode) {
	try {

		/* Operation
		   ---------
		   Opening: MORPH_OPEN: 2
		   Closing : MORPH_CLOSE : 3
		   Gradient : MORPH_GRADIENT : 4
		   Top Hat : MORPH_TOPHAT: 5
		   Black Hat : MORPH_BLACKHAT: 6
		*/
		int operation = MORPH_OPEN;

		/* Shape
		   -----
		   MORPH_RECT
		   MORPH_CROSS
		   MORPH_ELLIPSE
		*/
		int morph_elem = MORPH_RECT;
		int morph_size = 1;

		Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		//OPENING
		if (iMode > 0) morphologyEx(_fgmask, _fgmask, operation, element);
		if (iMode == 3) {
			morphologyEx(_fgmask, _fgmask, operation, element);
			morphologyEx(_fgmask, _fgmask, operation, element);
			morphologyEx(_fgmask, _fgmask, operation, element);
		}
		//Second Morphological operation
		operation = MORPH_CLOSE;
		morph_elem = MORPH_RECT;
		morph_size = 1;

		element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		//CLOSE
		if (iMode == 2) morphologyEx(_fgmask, _fgmask, operation, element);
	}
	catch (const std::exception& e) {
		cout << "MorphologicalOps ERROR";
	}
}

cv::Mat fgseg::bgs::removeShadows_127(cv::Mat fgmask)
{
	try
	{
		fgmask.setTo(0, fgmask == 127);
	}
	catch (const std::exception&){
		cout << "ERROR removeShadows_127";
	}

	return fgmask;
}
#include <opencv2/opencv.hpp>

#ifndef FGSEG_H_INCLUDE
#define FGSEG_H_INCLUDE

using namespace cv;
using namespace std;

namespace fgseg {

	class bgs {

		public:
			bgs(int BKsubtractionMode, int morphologicalOpsMode);
			cv::Mat getFGmask(cv::Mat frame, double learningrate);
			void RunMorphologicalOps(int iMode);
			cv::Mat removeShadows_127(cv::Mat fgmask) ;

		private:
			cv::Mat _fgmask;
			Ptr<BackgroundSubtractor> _pMOG2;

			//Background Subtraction Mode
			//0 = MOG2
			//1 = ...
			int _bgsMode;

			//Morphological Operations Mode
			//0 = No morphological ops performed
			//1 = OPENING
			//2 = OPENING and CLOSING
			int _morphOpsMode;
	};
}

#endif
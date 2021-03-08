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

			//bgs(double threshold, bool rgb);
			//~bgs(void);
			//void init_bkg(cv::Mat Frame);
			//void bkgSubtraction(cv::Mat Frame);
			//void removeShadows(double alpha, double beta, double tau, cv::Mat Frame);
			//cv::Mat getBG() { return _bkg; };
			//cv::Mat getDiff() { return _diff; };
			//cv::Mat getBGSmask() { return _bgsmask; };
			//cv::Mat getShadowMask() { return _shadowmask; };
			//
			//void BkgRunningAverage(double tau, double alpha, bool selective_update, cv::Mat Frame);
			//void GhostSuppression(double tau, int threshold_ghosts2, cv::Mat Frame);
			//bool IsForeground(int x, int y, double tau, cv::Mat Frame);
			//void SingleGaussian(double alpha, cv::Mat Frame);
			//void GMG(cv::Mat Frame);
			//void DoForeground(cv::Mat Frame);
			//void DoShadow();
			//void Test(cv::Mat Img);

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

			//cv::Mat _bkg; //Background model
			//cv::Mat	_frame; //current frame
			//cv::Mat _diff; //abs diff frame
			//cv::Mat _bgsmask; //binary image for bgssub (FG)
			//cv::Mat _shadowmask; //binary mask for detected shadows
			//bool _rgb;
			//double _threshold;
			////ADD ADITIONAL VARIABLES HERE
			////...
			//cv::Mat _bkg_GREY;
			//cv::Mat _bkg_RGB;
			//cv::Mat _SG_Mean;
			//cv::Mat _SG_Var;
			//bool _SG_Mean_Initialized; //Apparently use of pointers to check if NULL is not supported, so we use a boolean
			//bool _SG_Var_Initialized; //Apparently use of pointers to check if NULL is not supported, so we use a boolean
			////int _3Channels = CV_8UC3;
			//int _3Channel = CV_8UC3;
			//int _1Channel = CV_8UC1;
			////cv::Mat _ghostDetectionCounter;
			//int** _ghostDetectionCounter;
	};
}

#endif
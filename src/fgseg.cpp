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

////default constructor
//bgs::bgs(double threshold, bool rgb)
//{
//	_rgb = rgb;
//	_threshold = threshold;
//	_SG_Mean_Initialized = false;
//	_SG_Var_Initialized = false;
//}
//
////default destructor
//bgs::~bgs(void)
//{
//	////Deleting _ghostDetectionCounter Matrix
//	//if (_ghostDetectionCounter != NULL) {
//	//	int rows = _frame.rows;
//	//	for (int i = 0; i < rows; ++i)
//	//		delete[] _ghostDetectionCounter[i];
//	//	delete[] _ghostDetectionCounter;
//	//}
//}
////method to initialize bkg (first frame - hot start)
//void bgs::init_bkg(cv::Mat Frame)
//{
//	try {
//		int rows = _frame.rows;
//		int cols = _frame.cols;
//
//		//We store both versions to facilitate computations of some parts of the code that do not depend on the flag _rgb.
//		_bkg_GREY = Mat::zeros(Size(cols, rows), _1Channel);
//		_bkg_RGB = Mat::zeros(Size(cols, rows), _3Channel);
//
//		Frame.copyTo(_bkg_RGB);
//		cvtColor(_bkg_RGB, _bkg_GREY, COLOR_BGR2GRAY);
//
//		//We set the main variable _bkg with the flag selection _rgb
//		if (!_rgb) {
//			_bkg = Mat::zeros(Size(cols, rows), _1Channel);
//			_bkg_GREY.copyTo(_bkg);
//		}
//		else {
//			_bkg = Mat::zeros(Size(cols, rows), _3Channel);
//			_bkg_RGB.copyTo(_bkg);
//		}
//	}
//	catch (const std::exception& e) {
//		cout << "init_bkg ERROR";
//	}
//}
//
////method to perform BackGroundSubtraction
//void bgs::bkgSubtraction(cv::Mat Frame)
//{
//	try {
//		if (!_rgb) {
//			_frame = Mat::zeros(Size(Frame.cols, Frame.rows), _1Channel);
//			_diff = Mat::zeros(Size(Frame.cols, Frame.rows), _1Channel);
//			_bgsmask = Mat::zeros(Size(Frame.cols, Frame.rows), _1Channel);
//
//			Frame.copyTo(_frame);
//			cvtColor(_frame, _frame, COLOR_BGR2GRAY);
//
//			absdiff(_frame, _bkg, _diff);
//			//_diff = abs(_frame - _bkg);
//
//			_bgsmask = _diff > _threshold;
//			//threshold(_diff, _bgsmask, _threshold, 255, 3);
//		}
//		else {
//			_frame = Mat::zeros(Size(Frame.cols, Frame.rows), _3Channel);
//			_diff = Mat::zeros(Size(Frame.cols, Frame.rows), _3Channel);
//			_bgsmask = Mat::zeros(Size(Frame.cols, Frame.rows), _3Channel);
//
//			Frame.copyTo(_frame);
//
//			absdiff(_frame, _bkg, _diff);
//			//_diff = abs(_frame - _bkg);
//
//			_bgsmask = _diff > _threshold;
//			//threshold(_diff, _bgsmask, _threshold, 255, 3);
//		}
//	}
//	catch (const std::exception& e) {
//		cout << "bkgSubstracion ERROR";
//	}
//}
//
//void bgs::DoForeground(cv::Mat Frame) {
//
//	try {
//
//		if (!_rgb) {
//			_fgmask = Mat::zeros(Size(Frame.cols, Frame.rows), _1Channel);
//			_bgsmask.copyTo(_fgmask);
//		}
//		else {
//			_fgmask = Mat::zeros(Size(Frame.cols, Frame.rows), _3Channel);
//			cvtColor(_bgsmask, _fgmask, COLOR_BGR2GRAY);
//		}
//		//absdiff(_fgmask, _shadowmask, _fgmask);
//		_fgmask = _fgmask - _shadowmask;
//	}
//	catch (const std::exception& e) {
//		cout << "DoForeground ERROR";
//	}
//}
//
//void bgs::DoShadow() {
//
//	try {
//		_shadowmask = Mat::zeros(Size(_frame.cols, _frame.rows), _1Channel);
//	}
//	catch (const std::exception& e) {
//		cout << "DoForeground ERROR";
//	}
//}
//
////method to detect and remove shadows in the BGS mask to create FG mask
//void bgs::removeShadows(double alpha, double beta, double tau, cv::Mat Frame)
//{
//	try {
//
//		if (!_rgb) {
//			_shadowmask = Mat::zeros(Size(_bgsmask.cols, _bgsmask.rows), _1Channel);
//			_fgmask = Mat::zeros(Size(_bgsmask.cols, _bgsmask.rows), _1Channel);
//
//			Mat srcImg = Frame;
//			Mat Img_GREY;
//			Mat bkgImg = _bkg_RGB;
//			Mat hsvImg;
//			Mat hsvBkg;
//			Mat Shadow;
//
//			cvtColor(srcImg, Img_GREY, COLOR_BGR2GRAY);
//
//			double Dh = 0;
//			double IH = 0;
//			double IS = 0;
//			double IV = 0;
//			double BH = 0;
//			double BS = 0;
//			double BV = 0;
//			double a = alpha;
//			double b = beta;
//
//			vector<Mat> Bkgchannels;
//			vector<Mat> Imgchannels;
//
//			cvtColor(srcImg, hsvImg, CV_BGR2HSV);
//			cvtColor(bkgImg, hsvBkg, CV_BGR2HSV);
//
//			split(hsvImg, Imgchannels);
//			split(hsvBkg, Bkgchannels);
//
//			int rows = _frame.rows;
//			int cols = _frame.cols;
//
//			for (int x = 0; x < rows; x++) {
//				for (int y = 0; y < cols; y++) {
//
//					if (Bkgchannels[2].at<uchar>(x, y) > 0) //To avoid division by zero
//					{
//						IH = Imgchannels[0].at<uchar>(x, y);
//						IS = Imgchannels[1].at<uchar>(x, y);
//						IV = Imgchannels[2].at<uchar>(x, y);
//						BH = Bkgchannels[0].at<uchar>(x, y);
//						BS = Bkgchannels[1].at<uchar>(x, y);
//						BV = Bkgchannels[2].at<uchar>(x, y);
//
//						Dh = min(abs(IH - BH), 360 - abs(IH - BH));
//
//						if ((a <= IV / BV) && (IV / BV <= b) && (abs(IS - BS) <= tau) && (Dh <= tau))
//						{
//							_shadowmask.at<uchar>(x, y) = 255;
//						}
//					}
//				}
//			}
//		}
//		else {
//			_shadowmask = Mat::zeros(Size(_bgsmask.cols, _bgsmask.rows), _1Channel);
//			_fgmask = Mat::zeros(Size(_bgsmask.cols, _bgsmask.rows), _3Channel);
//
//			Mat Img_3C;
//			Mat Bkg_3C;
//			Mat Img_HSV;
//			Mat Bkg_HSV;
//
//			Frame.copyTo(Img_3C);
//			_bkg.copyTo(Bkg_3C);
//
//			double Dh = 0;
//			double IH = 0;
//			double IS = 0;
//			double IV = 0;
//			double BH = 0;
//			double BS = 0;
//			double BV = 0;
//			double a = alpha;
//			double b = beta;
//
//			vector<Mat> BkgChannels;
//			vector<Mat> ImgChannels;
//
//			cvtColor(Img_3C, Img_HSV, CV_BGR2HSV);
//			cvtColor(Bkg_3C, Bkg_HSV, CV_BGR2HSV);
//
//			split(Img_HSV, ImgChannels);
//			split(Bkg_HSV, BkgChannels);
//
//			int rows = _frame.rows;
//			int cols = _frame.cols;
//
//			for (int x = 0; x < rows; x++) {
//				for (int y = 0; y < cols; y++) {
//
//					if (BkgChannels[2].at<uchar>(x, y) > 0) //To avoid division by zero
//					{
//						IH = ImgChannels[0].at<uchar>(x, y);
//						IS = ImgChannels[1].at<uchar>(x, y);
//						IV = ImgChannels[2].at<uchar>(x, y);
//						BH = BkgChannels[0].at<uchar>(x, y);
//						BS = BkgChannels[1].at<uchar>(x, y);
//						BV = BkgChannels[2].at<uchar>(x, y);
//
//						Dh = min(abs(IH - BH), 360 - abs(IH - BH));
//
//						if ((a <= IV / BV) && (IV / BV <= b) && (abs(IS - BS) <= tau) && (Dh <= tau))
//						{
//							_shadowmask.at<uchar>(x, y) = 255;
//						}
//					}
//				}
//			}
//		}
//
//		//imshow("_shadowmask", _shadowmask);
//		//imshow("_fgmask", _fgmask);
//	}
//	catch (const std::exception& e) {
//		cout << "removeShadows ERROR";
//	}
//}
//
//void bgs::BkgRunningAverage(double tau, double alpha, bool selective_update, cv::Mat Frame)
//{
//	try {
//		_frame = Mat::zeros(Size(Frame.cols, Frame.rows), _1Channel);
//		Frame.copyTo(_frame);
//
//		if (!_rgb) {
//
//			cvtColor(_frame, _frame, COLOR_BGR2GRAY);
//
//			if (selective_update) {
//				int rows = _frame.rows;
//				int cols = _frame.cols;
//
//				for (int x = 0; x < rows; x++) {
//					for (int y = 0; y < cols; y++) {
//						if (_bgsmask.at<uchar>(x, y) == 0) {
//							_bkg.at<uchar>(x, y) = alpha * _frame.at<uchar>(x, y) + (1 - alpha)* _bkg.at<uchar>(x, y);
//						}
//					}
//				}
//			}
//			else {
//				int rows = _frame.rows;
//				int cols = _frame.cols;
//
//				for (int x = 0; x < rows; x++) {
//					for (int y = 0; y < cols; y++) {
//						if (_bgsmask.at<uchar>(x, y) == 0) {
//							_bkg.at<uchar>(x, y) = _frame.at<uchar>(x, y);
//						}
//					}
//				}
//			}
//		}
//		else {
//
//		}
//	}
//	catch (const std::exception& e) {
//		cout << "bkgRunningAverage ERROR";
//	}
//}
//
//void bgs::GhostSuppression(double tau, int threshold_ghosts2, cv::Mat Frame) {
//
//	int rows = _frame.rows;
//	int cols = _frame.cols;
//
//	//We initialize _ghostDetectionCounter Matrix if first time.
//	if (_ghostDetectionCounter == NULL) {
//		_ghostDetectionCounter = new int*[rows];
//		for (int i = 0; i < rows; ++i)
//			_ghostDetectionCounter[i] = new int[cols];
//
//		//Setting values to 0
//		for (int x = 0; x < rows; x++) {
//			for (int y = 0; y < cols; y++) {
//				_ghostDetectionCounter[x][y] = 0;
//			}
//		}
//	}
//
//	try {
//
//		if (!_rgb) {
//
//			_frame = Mat::zeros(Size(cols, rows), _1Channel);
//			Frame.copyTo(_frame);
//			cvtColor(_frame, _frame, COLOR_BGR2GRAY);
//
//			for (int x = 0; x < rows; x++) {
//				for (int y = 0; y < cols; y++) {
//
//					//Is pixel foreground?
//					if (IsForeground(x, y, tau, _frame)) {
//						_ghostDetectionCounter[x][y] += 1;
//					}
//					else {
//						_ghostDetectionCounter[x][y] = 0;
//					}
//
//					//Background incorporation if threshold riched
//					if (_ghostDetectionCounter[x][y] > threshold_ghosts2) {
//						_bkg.at<uchar>(x, y) = _frame.at<uchar>(x, y);
//					}
//				}
//			}
//		}
//		else {
//
//			for (int x = 0; x < rows; x++) {
//				for (int y = 0; y < cols; y++) {
//
//					//Is pixel foreground?
//					if (IsForeground(x, y, tau, _frame)) {
//						_ghostDetectionCounter[x][y] += 1;
//					}
//					else {
//						_ghostDetectionCounter[x][y] = 0;
//					}
//
//					//Background incorporation if threshold riched
//					if (_ghostDetectionCounter[x][y] > threshold_ghosts2) {
//						_bkg.at<Vec3b>(x, y) = _frame.at<Vec3b>(x, y);
//					}
//				}
//			}
//		}
//
//	}
//	catch (const std::exception& e) {
//		cout << "bkgRunningAverage ERROR";
//	}
//
//}
//
////Returns a boolean indicating if the pixel is Foreground or belongs to Background
//bool bgs::IsForeground(int x, int y, double tau, cv::Mat Frame) {
//	bool retorno = false;
//
//	try {
//
//		if (!_rgb) {
//			double diff;
//
//			diff = abs(Frame.at<uchar>(x, y) - _bkg.at<uchar>(x, y));
//
//			if (diff > tau) retorno = true;
//			//if (diff > 0) retorno = true;
//		}
//		else {
//			int count = 0;
//
//			Vec3b int_Frame = Frame.at<Vec3b>(x, y);
//			Vec3b int_bkg = _bkg.at<Vec3b>(x, y);
//
//			if (abs(int_Frame.val[0] - int_bkg[0]) > tau) count += 1;
//			if (abs(int_Frame.val[1] - int_bkg[1]) > tau) count += 1;
//			if (abs(int_Frame.val[2] - int_bkg[2]) > tau) count += 1;
//
//			if (count > 2) retorno = true;
//		}
//		return retorno;
//	}
//	catch (const std::exception& e) {
//		cout << "IsForeground ERROR";
//	}
//}
//
//void bgs::SingleGaussian(double alpha, cv::Mat Frame) {
//	try {
//
//		if (!_rgb) {
//
//			Frame.copyTo(_frame);
//			cvtColor(_frame, _frame, COLOR_BGR2GRAY);
//
//			int rows = _frame.rows;
//			int cols = _frame.cols;
//
//			_fgmask = Mat::zeros(Size(cols, rows), _1Channel);
//			_bgsmask = Mat::zeros(Size(cols, rows), _1Channel);
//			_diff = Mat::zeros(Size(cols, rows), _1Channel);
//
//			//Initialization of Mat objects for Single Gaussian
//			if (!_SG_Mean_Initialized) {
//				//SingleGausian init
//				_SG_Mean = Mat::zeros(Size(cols, rows), _1Channel);
//				_SG_Var = Mat::zeros(Size(cols, rows), _1Channel);
//				//We use values of Background
//				_bkg.copyTo(_SG_Mean);
//			}
//
//			for (int x = 0; x < rows; x++) {
//				for (int y = 0; y < cols; y++) {
//					//If pixel is foreground we update Mean and Std of it
//					if (_fgmask.at<uchar>(x, y) == 0) {
//						//Update Mean and Std of pixel
//						double img = _frame.at<uchar>(x, y);
//						double mean = _SG_Mean.at<uchar>(x, y);
//						double std = (_SG_Var.at<uchar>(x, y)) ^ 2;
//						double a = alpha;
//
//						_SG_Mean.at<uchar>(x, y) = a * img + (1 - a) * mean;
//						_SG_Var.at<uchar>(x, y) = sqrt(a * (pow(img - mean, 2)) + (1 - a) * std);
//
//						//Foreground evaluation
//						mean = _SG_Mean.at<uchar>(x, y);
//						double var = _SG_Var.at<uchar>(x, y);
//						if ((img < mean + 3 * var) || (img > mean - 3 * var)) {
//							//It is Background
//							_bgsmask.at<uchar>(x, y) = 255;
//						}
//						//else {
//						//	//It is Foreground
//						//	_fgmask.at<uchar>(x, y) = 255;
//						//}
//					}
//				}
//			}
//
//			absdiff(_frame, _bkg, _diff);
//			_bgsmask = _diff > _threshold;
//
//			//imshow("_bgsmask", _bgsmask);
//			//imshow("_fgmask", _fgmask);
//		}
//		else {
//			Frame.copyTo(_frame);
//			//cvtColor(_frame, _frame, COLOR_BGR2GRAY);
//
//			int rows = _frame.rows;
//			int cols = _frame.cols;
//
//			_fgmask = Mat::zeros(Size(cols, rows), _1Channel);
//			_bgsmask = Mat::zeros(Size(cols, rows), _3Channel);
//			_diff = Mat::zeros(Size(cols, rows), _3Channel);
//
//			//Initialization of Mat objects for Single Gaussian
//			if (!_SG_Mean_Initialized) {
//				//SingleGausian init
//				_SG_Mean = Mat::zeros(Size(cols, rows), _3Channel);
//				_SG_Var = Mat::zeros(Size(cols, rows), _3Channel);
//				//We use values of Background
//				//cvtColor(_bkg, _SG_Mean, COLOR_BGR2GRAY);
//				_bkg.copyTo(_SG_Mean);
//			}
//
//			for (int x = 0; x < rows; x++) {
//				for (int y = 0; y < cols; y++) {
//					//If pixel is foreground we update Mean and Std of it
//					if (_fgmask.at<uchar>(x, y) == 0) {
//						//Update Mean and Std of pixel
//
//						Vec3b intensity_frame = _frame.at<Vec3b>(x, y);
//						Vec3b intensity_SG_Mean = _SG_Mean.at<Vec3b>(x, y);
//						Vec3b intensity_SG_Var = _SG_Var.at<Vec3b>(x, y);
//
//						double a = alpha;
//
//						double B_img = intensity_frame.val[0];
//						double B_mean = intensity_SG_Mean.val[0];
//						double B_std = (intensity_SG_Var.val[0]) ^ 2;
//
//						double G_img = intensity_frame.val[1];
//						double G_mean = intensity_SG_Mean.val[1];
//						double G_std = (intensity_SG_Var.val[1]) ^ 2;
//
//						double R_img = intensity_frame.val[2];
//						double R_mean = intensity_SG_Mean.val[2];
//						double R_std = (intensity_SG_Var.val[2]) ^ 2;
//
//						//Blue Channel
//						_SG_Mean.at<Vec3b>(x, y)[0] = a * B_img + (1 - a) * B_mean;
//						_SG_Var.at<Vec3b>(x, y)[0] = sqrt(a * (pow(B_img - B_mean, 2)) + (1 - a) * B_std);
//
//						//Green Channel
//						_SG_Mean.at<Vec3b>(x, y)[1] = a * G_img + (1 - a) * G_mean;
//						_SG_Var.at<Vec3b>(x, y)[1] = sqrt(a * (pow(G_img - G_mean, 2)) + (1 - a) * G_std);
//
//						//Red Channel
//						_SG_Mean.at<Vec3b>(x, y)[2] = a * R_img + (1 - a) * R_mean;
//						_SG_Var.at<Vec3b>(x, y)[2] = sqrt(a * (pow(R_img - R_mean, 2)) + (1 - a) * R_std);
//
//						//Foreground evaluation
//						double B_Var;
//						double G_Var;
//						double R_Var;
//
//						//Blue Channel
//						B_mean = _SG_Mean.at<Vec3b>(x, y)[0];
//						B_Var = _SG_Var.at<Vec3b>(x, y)[0];
//						if ((B_img < B_mean + 3 * B_Var) || (B_img > B_mean - 3 * B_Var)) {
//							//It is Background
//							_bgsmask.at<Vec3b>(x, y)[0] = B_img;
//						}
//						//Green Channel
//						G_mean = _SG_Mean.at<Vec3b>(x, y)[1];
//						G_Var = _SG_Var.at<Vec3b>(x, y)[1];
//						if ((G_img < G_mean + 3 * G_Var) || (G_img > G_mean - 3 * G_Var)) {
//							//It is Background
//							_bgsmask.at<Vec3b>(x, y)[1] = G_img;
//						}
//						//Red Channel
//						R_mean = _SG_Mean.at<Vec3b>(x, y)[2];
//						R_Var = _SG_Var.at<Vec3b>(x, y)[2];
//						if ((R_img < R_mean + 3 * R_Var) || (R_img > R_mean - 3 * R_Var)) {
//							//It is Background
//							_bgsmask.at<Vec3b>(x, y)[2] = R_img;
//						}
//					}
//				}
//			}
//			absdiff(_frame, _bkg, _diff);
//			_bgsmask = _diff > _threshold;
//		}
//	}
//	catch (const std::exception& e) {
//		cout << "SingleGaussian ERROR";
//	}
//}
//
//
//void bgs::GMG(cv::Mat Frame) {
//	try {
//
//	}
//	catch (const std::exception& e) {
//		cout << "GMG ERROR";
//	}
//}
//
//void bgs::Test(cv::Mat Frame) {
//	try {
//
//	}
//	catch (const std::exception& e) {
//		cout << "Test ERROR";
//	}
//}
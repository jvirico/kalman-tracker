#include <opencv2/opencv.hpp>

#ifndef KALMAN_H_INCLUDE
#define KALMAN_H_INCLUDE

using namespace cv;
using namespace std;

namespace kalmantracking {

	class kalman {

	public:
		kalman();
		kalman(int stateTransitionSize, int measurementSize, cv::Mat A, cv::Mat H, cv::Mat P, cv::Mat Q, cv::Mat R, bool bFullDebudOn);
		kalman(unsigned int iMode, bool bFullDebugOn);

		void Restart();

		void SetTransitionMatrix(cv::Mat A);
		void SetMeasurementMatrix(cv::Mat H);
		void SetUncertaintyMatrix(cv::Mat R);
		void SetStateNoiseCovMatrix(cv::Mat Q);
		void SetMeasuresNoiseCovMatrix(cv::Mat R);

		Point Predict(Point ball);


		//  getters for trajectory
		std::vector<cv::Point> getPredictedList() const {
			return _predicted_list;
		}

		std::vector<cv::Point> getCorrectedList() const {
			return _corrected_list;
		}

		std::vector<cv::Point> getGroundTruthList() const {
			return _GroundTruth_list;
		}

		const string& getStatus() const {
			return _status;
		}

		void setStatus(const string &status) {
			this->_status = status;
		}

	private:
		cv::KalmanFilter _k;
		cv::Mat _state;
		cv::Mat _measurement;
		int _stateTransitionSize = 4;
		int _measurementsSize = 2;
		int _controlSize = 0;
		int _type = CV_32F;
		bool _initialized = false;
		bool _bFullDebugOn = false;
		std::vector<cv::Point> _predicted_list;
		std::vector<cv::Point> _corrected_list;
		std::vector<cv::Point> _GroundTruth_list;
		string _status;
	};
}

#endif

#include <opencv2/opencv.hpp>
#include "kalman.hpp"

using namespace kalmantracking;

kalman::kalman()
{
	_k.init(_stateTransitionSize, _measurementsSize, _controlSize, _type);

	cv::Mat S(_stateTransitionSize, 1, _type);
	cv::Mat M(_measurementsSize, 1, _type);
	S.copyTo(_state);
	M.copyTo(_measurement);

	//Default Transition Matrix
	cv::setIdentity(_k.transitionMatrix);
	//Default State Noise Covariance Matrix
	cv::setIdentity(_k.processNoiseCov, cv::Scalar(1e-2));
	//Default Measures Noise Covariance Matrix
	cv::setIdentity(_k.measurementNoiseCov, cv::Scalar(1e-1));

	//Default Measurement Matrix
	_k.measurementMatrix = cv::Mat::zeros(_measurementsSize, _stateTransitionSize, _type);
	_k.measurementMatrix.at<float>(0) = 1.0f;
	_k.measurementMatrix.at<float>(7) = 1.0f;
	_k.measurementMatrix.at<float>(16) = 1.0f;
	_k.measurementMatrix.at<float>(23) = 1.0f;
}

kalmantracking::kalman::kalman(int stateTransitionSize, int measurementSize, cv::Mat A, cv::Mat H, cv::Mat P, cv::Mat Q, cv::Mat R, bool bFullDebudOn)
{
	_bFullDebugOn = bFullDebudOn;
	_stateTransitionSize = stateTransitionSize;
	_measurementsSize = measurementSize;
	_k.init(_stateTransitionSize, _measurementsSize, _controlSize, _type);

	_state = Mat::zeros(_stateTransitionSize, 1, _type);
	_measurement = Mat::zeros(_measurementsSize, 1, _type);

	if (_bFullDebugOn) cout << "_state = " << endl << " " << _state << endl << endl;
	if (_bFullDebugOn) cout << "_measurement = " << endl << " " << _measurement << endl << endl;

	//State Transition Matrix
	A.copyTo(_k.transitionMatrix);
	//Measurement Matrix
	H.copyTo(_k.measurementMatrix);
	//Uncertainty
	P.copyTo(_k.errorCovPre);
	//State Noise Covariance Matrix
	Q.copyTo(_k.processNoiseCov);
	//Measures Noise Covariance Matrix
	R.copyTo(_k.measurementNoiseCov);
}

kalmantracking::kalman::kalman(unsigned int iMode, bool bFullDebugOn)
{
	//**********************************************************
	// A -> State Transition Matrix
	// H -> Measurement Matrix
	// P -> Uncertainty Matrix
	// Q -> Noise of process, Covariance Matrix
	// R -> Noise of measurements, Covariance Matrix
	//**********************************************************
	cv::Mat A;
	cv::Mat H;
	cv::Mat P;
	cv::Mat Q;
	cv::Mat R;
	unsigned int uiType = CV_32F;
	int stateSize;
	int measurementSize;

	switch (iMode)
	{
	case 1:
		stateSize = 4;
		measurementSize = 2;

		//Xk = x Vx y Vy
		A = Mat::zeros(Size(stateSize, stateSize), uiType);
		cv::setIdentity(A);
		A.at<float>(0) = 1.0f;
		A.at<float>(1) = 1.0f;
		A.at<float>(5) = 1.0f;
		A.at<float>(10) = 1.0f;
		A.at<float>(11) = 1.0f;
		A.at<float>(15) = 1.0f;
		if (bFullDebugOn) cout << "A = " << endl << " " << A << endl << endl;

		H = Mat::zeros(Size(stateSize, measurementSize), uiType);
		//cv::setIdentity(H);
		H.at<float>(0) = 1.0f;
		H.at<float>(6) = 1.0f;
		if (bFullDebugOn) cout << "H = " << endl << " " << H << endl << endl;

		P = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(P);
		P.at<float>(0) = 10e5f;
		P.at<float>(5) = 10e5f;
		P.at<float>(10) = 10e5f;
		P.at<float>(15) = 10e5f;
		if (bFullDebugOn) cout << "P = " << endl << " " << P << endl << endl;

		Q = Mat::zeros(Size(stateSize, stateSize), uiType);
		cv::setIdentity(Q);
		Q.at<float>(0) = 25.0f;
		Q.at<float>(5) = 10.0f;
		Q.at<float>(10) = 25.0f;
		Q.at<float>(15) = 10.0f;
		if (bFullDebugOn) cout << "Q = " << endl << " " << Q << endl << endl;

		R = Mat::zeros(Size(measurementSize, measurementSize), uiType);
		cv::setIdentity(R);
		R.at<float>(0) = 25.0f;
		R.at<float>(3) = 25.0f;
		if (bFullDebugOn) cout << "R = " << endl << " " << R << endl << endl;

		break;

	case 2:
		stateSize = 6;
		measurementSize = 2;

		//Xk = x Vx Ax y Vy Ay
		A = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(A);
		A.at<float>(0) = 1.0f;
		A.at<float>(1) = 1.0f;
		A.at<float>(2) = 0.5f;
		A.at<float>(7) = 1.0f;
		A.at<float>(8) = 1.0f;
		A.at<float>(14) = 1.0f;
		A.at<float>(21) = 1.0f;
		A.at<float>(22) = 1.0f;
		A.at<float>(23) = 0.5f;
		A.at<float>(28) = 1.0f;
		A.at<float>(29) = 1.0f;
		A.at<float>(35) = 1.0f;
		if (bFullDebugOn) cout << "A = " << endl << " " << A << endl << endl;

		H = Mat::zeros(Size(stateSize, measurementSize), uiType);
		//cv::setIdentity(H);
		H.at<float>(0) = 1.0f;
		H.at<float>(9) = 1.0f;
		if (bFullDebugOn) cout << "H = " << endl << " " << H << endl << endl;

		P = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(P);
		P.at<float>(0) = 10e5f;
		P.at<float>(7) = 10e5f;
		P.at<float>(14) = 10e5f;
		P.at<float>(21) = 10e5f;
		P.at<float>(28) = 10e5f;
		P.at<float>(35) = 10e5f;
		if (bFullDebugOn) cout << "P = " << endl << " " << P << endl << endl;

		Q = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(Q);
		Q.at<float>(0) = 25.0f;
		Q.at<float>(7) = 10.0f;
		Q.at<float>(14) = 1.0f;
		Q.at<float>(21) = 25.0f;
		Q.at<float>(28) = 10.0f;
		Q.at<float>(35) = 1.0f;
		if (bFullDebugOn) cout << "Q = " << endl << " " << Q << endl << endl;

		R = Mat::zeros(Size(measurementSize, measurementSize), uiType);
		//cv::setIdentity(R);
		R.at<float>(0) = 25.0f;
		R.at<float>(3) = 25.0f;
		if (bFullDebugOn) cout << "R = " << endl << " " << R << endl << endl;

		break;
	default:
		stateSize = 6;
		measurementSize = 2;

		//Xk = x Vx Ax y Vy Ay
		A = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(A);
		A.at<float>(0) = 1.0f;
		A.at<float>(1) = 1.0f;
		A.at<float>(2) = 0.5f;
		A.at<float>(7) = 1.0f;
		A.at<float>(8) = 1.0f;
		A.at<float>(14) = 1.0f;
		A.at<float>(21) = 1.0f;
		A.at<float>(22) = 1.0f;
		A.at<float>(23) = 0.5f;
		A.at<float>(28) = 1.0f;
		A.at<float>(29) = 1.0f;
		A.at<float>(35) = 1.0f;
		if (bFullDebugOn) cout << "A = " << endl << " " << A << endl << endl;

		H = Mat::zeros(Size(stateSize, measurementSize), uiType);
		//cv::setIdentity(H);
		H.at<float>(0) = 1.0f;
		H.at<float>(9) = 1.0f;
		if (bFullDebugOn) cout << "H = " << endl << " " << H << endl << endl;

		P = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(P);
		P.at<float>(0) = 10e5f;
		P.at<float>(7) = 10e5f;
		P.at<float>(14) = 10e5f;
		P.at<float>(21) = 10e5f;
		P.at<float>(28) = 10e5f;
		P.at<float>(35) = 10e5f;
		if (bFullDebugOn) cout << "P = " << endl << " " << P << endl << endl;

		Q = Mat::zeros(Size(stateSize, stateSize), uiType);
		//cv::setIdentity(Q);
		Q.at<float>(0) = 25.0f;
		Q.at<float>(7) = 10.0f;
		Q.at<float>(14) = 1.0f;
		Q.at<float>(21) = 25.0f;
		Q.at<float>(28) = 10.0f;
		Q.at<float>(35) = 1.0f;
		if (bFullDebugOn) cout << "Q = " << endl << " " << Q << endl << endl;

		R = Mat::zeros(Size(measurementSize, measurementSize), uiType);
		//cv::setIdentity(R);
		R.at<float>(0) = 25.0f;
		R.at<float>(3) = 25.0f;
		if (bFullDebugOn) cout << "R = " << endl << " " << R << endl << endl;
	}

	_bFullDebugOn = bFullDebugOn;
	_stateTransitionSize = stateSize;
	_measurementsSize = measurementSize;
	_k.init(_stateTransitionSize, _measurementsSize, _controlSize, _type);

	_state = Mat::zeros(_stateTransitionSize, 1, _type);
	_measurement = Mat::zeros(_measurementsSize, 1, _type);

	if (_bFullDebugOn) cout << "_state = " << endl << " " << _state << endl << endl;
	if (_bFullDebugOn) cout << "_measurement = " << endl << " " << _measurement << endl << endl;

	//State Transition Matrix
	A.copyTo(_k.transitionMatrix);
	//Measurement Matrix
	H.copyTo(_k.measurementMatrix);
	//Uncertainty
	P.copyTo(_k.errorCovPre);
	//State Noise Covariance Matrix
	Q.copyTo(_k.processNoiseCov);
	//Measures Noise Covariance Matrix
	R.copyTo(_k.measurementNoiseCov);
}


void kalmantracking::kalman::SetTransitionMatrix(cv::Mat A)
{
	//Default State Transition Matrix
	A.copyTo(_k.transitionMatrix);
}

void kalmantracking::kalman::SetMeasurementMatrix(cv::Mat H)
{
	//Measurement Matrix
	H.copyTo(_k.measurementMatrix);
}

void kalmantracking::kalman::SetUncertaintyMatrix(cv::Mat R)
{
	R.copyTo(_k.errorCovPre);
}

void kalmantracking::kalman::SetStateNoiseCovMatrix(cv::Mat Q)
{
	//State Noise Covariance Matrix
	Q.copyTo(_k.processNoiseCov);
}

void kalmantracking::kalman::SetMeasuresNoiseCovMatrix(cv::Mat R)
{
	//Measures Noise Covariance Matrix
	R.copyTo(_k.measurementNoiseCov);
}

Point kalmantracking::kalman::Predict(Point ball)
{
	Point KBall;

	try {

		KBall.x = 0;
		KBall.y = 0;

		if (_initialized) {

			// 1. We have measuerment..
			// 2. we have not measurment...



		//If initialized
			//Predict
			_state = _k.predict();
			if (_bFullDebugOn) cout << "KF prediction: _state = " << endl << " " << _state << endl << endl;

			if (ball.x != 0 || ball.y != 0) {
				//If measured
				_measurement.at<float>(0) = ball.x;
				_measurement.at<float>(1) = ball.y;
				//Update
				if (_bFullDebugOn) cout << "KF correction: _measurement = " << endl << " " << _measurement << endl << endl;
				_k.correct(_measurement);
				setStatus("Corrected");

			}
			else
			{

				// update the measuerment to the state
				_measurement.at<float>(0) = ball.x;
				_measurement.at<float>(1) = ball.y;
				setStatus("Predicted");

			}

			//Return values
			switch (_stateTransitionSize)
			{
			case 4:
				KBall.x = _state.at<float>(0);
				KBall.y = _state.at<float>(2);
				break;
			case 6:
				KBall.x = _state.at<float>(0);
				KBall.y = _state.at<float>(3);
				break;
			default:
				KBall.x = _state.at<float>(0);
				KBall.y = _state.at<float>(2);
			}

		}


		else {


			//If not initialized
			if (ball.x != 0 || ball.y != 0) {
				//If measured
				_measurement.at<float>(0) = ball.x;
				_measurement.at<float>(1) = ball.y;

				//Initilize
				switch (_stateTransitionSize)
				{
				case 4:
					_state.at<float>(0) = _measurement.at<float>(0);
					_state.at<float>(1) = 0;
					_state.at<float>(2) = _measurement.at<float>(1);
					_state.at<float>(3) = 0;
					break;
				case 6:
					_state.at<float>(0) = _measurement.at<float>(0);
					_state.at<float>(1) = 0;
					_state.at<float>(2) = 0;
					_state.at<float>(3) = _measurement.at<float>(1);
					_state.at<float>(4) = 0;
					_state.at<float>(5) = 0;
					break;
				default:
					_state.at<float>(0) = _measurement.at<float>(0);
					_state.at<float>(1) = 0;
					_state.at<float>(2) = _measurement.at<float>(1);
					_state.at<float>(3) = 0;
				}


				if (_bFullDebugOn) cout << "KF statePost: _state = " << endl << " " << _state << endl << endl;
				_k.statePost = _state;
				_initialized = true;
				setStatus("Initialized");

				//_k.correct(_measurement);
				//Return values
				switch (_stateTransitionSize)
				{
				case 4:
					KBall.x = _state.at<float>(0);
					KBall.y = _state.at<float>(2);
					break;
				case 6:
					KBall.x = _state.at<float>(0);
					KBall.y = _state.at<float>(3);
					break;
				default:
					KBall.x = _state.at<float>(0);
					KBall.y = _state.at<float>(2);
				}
			}

		}

		// add points to trajectory

		_GroundTruth_list.push_back(ball);

		if (_status == "Predicted") {
			_predicted_list.push_back(KBall);
		}
		else {
			_corrected_list.push_back(KBall);
		}
		

		return KBall;

	}
	catch (const std::exception& e) {
		cout << "ERROR kalmantracking";
		return Point(0, 0);
	}
}

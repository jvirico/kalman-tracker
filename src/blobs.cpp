#include "blobs.hpp"


using namespace std;
using namespace cv;

/**
  *	Draws blobs with different rectangles on the image 'frame'. All the input arguments must be
  *  initialized when using this function.
  *
  * \param frame Input image
  * \param pBlobList List to store the blobs found
  * \param labelled - true write label and color bb, false does not wirite label nor color bb
  *
  * \return Image containing the draw blobs. If no blobs have to be painted
  *  or arguments are wrong, the function returns a copy of the original "frame".
  *
  */
Mat paintBlobImage(cv::Mat frame, std::vector<cvBlob> bloblist, bool labelled)
{
	cv::Mat blobImage;
	//check input conditions and return original if any is not satisfied
	//...
	frame.copyTo(blobImage);

	//required variables to paint
	//...

	//paint each blob of the list
	for (int i = 0; i < bloblist.size(); i++)
	{
		cvBlob blob = bloblist[i]; //get ith blob
		//...
		Scalar color;
		std::string label = "";
		switch (blob.label) {
		case PERSON:
			color = Scalar(255, 0, 0);
			label = "PERSON";
			break;
		case CAR:
			color = Scalar(0, 255, 0);
			label = "CAR";
			break;
		case OBJECT:
			color = Scalar(0, 0, 255);
			label = "OBJECT";
			break;
		default:
			color = Scalar(255, 255, 255);
			label = "BALL";
		}

		Point p1 = Point(blob.x, blob.y);
		Point p2 = Point(blob.x + blob.w, blob.y + blob.h);

		rectangle(blobImage, p1, p2, color, 1, 8, 0);
		if (labelled)
		{
			rectangle(blobImage, p1, p2, color, 1, 8, 0);
			putText(blobImage, label, p1, FONT_HERSHEY_SIMPLEX, 0.5, color);
		}
		else
			rectangle(blobImage, p1, p2, Scalar(255, 255, 255), 2, 8, 0);
	}

	//destroy all resources (if required)
	//...

	//return the image to show
	return blobImage;
}


/**
 *	Blob extraction from 1-channel image (binary). The extraction is performed based
 *	on the analysis of the connected components. All the input arguments must be
 *  initialized when using this function.
 *
 * \param fgmask Foreground/Background segmentation mask (1-channel binary image)
 * \param bloblist List with found blobs
 *
 * \return Operation code (negative if not succesfull operation)
 */
int extractBlobs(cv::Mat fgmask, std::vector<cvBlob> &bloblist, int connectivity)
{
	try {

		Mat aux; // image to be updated each time a blob is detected (blob cleared)
		fgmask.convertTo(aux, CV_32SC1);

		//clear blob list (to fill with this function)
		bloblist.clear();

		//Connected component analysis
		Mat stats;
		Mat centroids;

		int blob_count = connectedComponentsWithStats(fgmask, aux, stats, centroids, connectivity, CV_16U);

		for (int i = 0; i < blob_count; i++)
		{
			int x = stats.at<int>(i, cv::CC_STAT_LEFT);
			int y = stats.at<int>(i, cv::CC_STAT_TOP);
			int w = stats.at<int>(i, cv::CC_STAT_WIDTH);
			int h = stats.at<int>(i, cv::CC_STAT_HEIGHT);
			int area = stats.at<int>(i, cv::CC_STAT_AREA);
			double cx = centroids.at<double>(i, 0);
			double cy = centroids.at<double>(i, 1);

			if (x == 0 && y == 0 && w == fgmask.cols && h == fgmask.rows) {
				//It is the whole image
			}
			else {
				cvBlob blob = initBlob(i, x, y, w, h, cx, cy);
				bloblist.push_back(blob);
			}
		}

		return 1;
	}
	catch (const std::exception& e) {
		cout << "extractBlobs ERROR";
		return -1;
	}
}


int removeSmallBlobs(std::vector<cvBlob> bloblist_in, std::vector<cvBlob> &bloblist_out, int min_width, int min_height)
{
	try {
		//clear blob list (to fill with this function)
		bloblist_out.clear();

		for (int i = 0; i < bloblist_in.size(); i++)
		{
			cvBlob blob_in = bloblist_in[i]; //get ith blob
			if (blob_in.w >= min_width && blob_in.h >= min_height) {
				bloblist_out.push_back(blob_in);
			}
		}

		return 1;
	}
	catch (const std::exception& e) {
		cout << "removeSmallBlobs ERROR";
		return -1;
	}
}

int removeBigBlobs(std::vector<cvBlob> bloblist_in, std::vector<cvBlob> &bloblist_out, int max_width, int max_height)
{
	try {
		//clear blob list (to fill with this function)
		bloblist_out.clear();

		for (int i = 0; i < bloblist_in.size(); i++)
		{
			cvBlob blob_in = bloblist_in[i]; //get ith blob
			if (blob_in.w <= max_width && blob_in.h <= max_height) {
				bloblist_out.push_back(blob_in);
			}
		}

		return 1;
	}
	catch (const std::exception& e) {
		cout << "removeSmallBlobs ERROR";
		return -1;
	}
}

Point GetBallCoordinates(std::vector<cvBlob> vBlobs)
{
	Point retorno;
	double max_area = 0;

	try {

		for (int i = 0; i < vBlobs.size(); i++)
		{
			double blob_area = vBlobs[i].w * vBlobs[i].h;
			
			if (blob_area > max_area) {
				retorno.x = vBlobs[i].cx;
				retorno.y = vBlobs[i].cy;
				max_area = blob_area;
			}
		}

	}
	catch (const std::exception& e) {
		cout << "removeSmallBlobs ERROR";
		retorno.x = 0;
		retorno.y = 0;
	}

	return retorno;
}



//int extractBlobs_floodFill(cv::Mat fgmask, std::vector<cvBlob> &bloblist, int connectivity)
//{
//	//check input conditions and return -1 if any is not satisfied
//	//...		
//
//	//required variables for connected component analysis 
//	//...
//	Mat aux; // image to be updated each time a blob is detected (blob cleared)
//	fgmask.convertTo(aux, CV_32SC1);
//
//	//clear blob list (to fill with this function)
//	bloblist.clear();
//
//	//Connected component analysis
//	Rect rec;
//	Scalar newVal = Scalar(0);
//	Mat mask; // for the burn stage .. obviously i am setting it to zero zoo
//	int blob_id = 1;
//	for (int i = 0; i < aux.cols; i++) {
//		for (int j = 0; j < aux.rows; j++) {
//
//			if (aux.at<uchar>(Point(i, j)) == 255) {
//
//				floodFill(aux, Point(i, j), newVal, &rec, Scalar(0), Scalar(0), connectivity);
//				cvBlob blob = initBlob(blob_id, rec.x, rec.y, rec.width, rec.height);
//
//				blob_id = blob_id + 1;
//				// add to the blob list
//				bloblist.push_back(blob);
//			}
//
//		}
//
//	}
//
//	// void creation of a unqie blob in the center
//	//destroy all resources
//	//...
//
//	//return OK code
//	return 1;
//}
//
//
///**
// *	Blob extraction from 1-channel image (binary). The extraction is performed based
// *	on the analysis of the connected components. All the input arguments must be
// *  initialized when using this function.
// *
// * \param fgmask Foreground/Background segmentation mask (1-channel binary image)
// * \param bloblist List with found blobs
// *
// * \return Operation code (negative if not succesfull operation)
// */
//int extractBlobs_Optional1(cv::Mat fgmask, std::vector<cvBlob> &bloblist, int connectivity)
//{
//	try {
//		//check input conditions and return -1 if any is not satisfied
//		//...
//		//imshow("fgmask", fgmask);
//		//required variables for connected component analysis
//		//...
//		Mat aux; // image to be updated each time a blob is detected (blob cleared)
//		Mat fgmask_aux;
//		fgmask.convertTo(aux, CV_32SC1);
//		fgmask.copyTo(fgmask_aux);
//
//		//imshow("fgmask", fgmask);
//
//		//clear blob list (to fill with this function)
//		bloblist.clear();
//
//		//Connected component analysis
//		//Grass-Fire implementation
//		int iID = 0;
//		vector<Point> vPointsList;
//		//vector<pair<int,int>> vPointsList;
//
//		int rows = aux.rows;
//		int cols = aux.cols;
//
//		for (int x = 0; x < rows; x++) {
//			for (int y = 0; y < cols; y++) {
//
//				if (fgmask_aux.at<uchar>(x, y) == 255)//We find a new Blob
//				{
//					int iUpX = x;
//					int iLeftY = y;
//					int iDownX = x;
//					int iRightY = y;
//					Point oCurrPoint;
//					//std::pair<int,int> oCurrPoint;
//					//vPointsList.reserve(10000000);
//					iID = iID + 1;
//
//					vPointsList.push_back(Point(x, y));
//					//vPointsList.push_back(make_pair(x, y));
//
//					do {
//
//						oCurrPoint = vPointsList[0];
//
//						//aux.at<int>(oCurrPoint.x, oCurrPoint.y) = iID;
//						fgmask_aux.at<uchar>(oCurrPoint.x, oCurrPoint.y) = 0;
//						//fgmask_aux.at<uchar>(oCurrPoint.first, oCurrPoint.second) = 0;
//
//						vPointsList.erase(vPointsList.begin()); //We erase the pixel we just checked
//
//						try {
//							//We add neighbours to list
//							if (connectivity == 4) {
//
//								//right
//								if (oCurrPoint.y < cols - 2) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x, oCurrPoint.y + 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x, oCurrPoint.y + 1));
//										if (iRightY < oCurrPoint.y + 1) iRightY = oCurrPoint.y + 1;
//									}
//								}
//								//down
//								if (oCurrPoint.x < rows - 2) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x + 1, oCurrPoint.y) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x + 1, oCurrPoint.y));
//										if (iDownX < oCurrPoint.x + 1) iDownX = oCurrPoint.x + 1;
//									}
//								}
//								//left
//								if (oCurrPoint.y > 0) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x, oCurrPoint.y - 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x, oCurrPoint.y - 1));
//										if (iLeftY < oCurrPoint.y - 1) iLeftY = oCurrPoint.y - 1;
//									}
//								}
//								//up
//								if (oCurrPoint.x > 0) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x - 1, oCurrPoint.y) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x - 1, oCurrPoint.y));
//										if (iUpX < oCurrPoint.x - 1) iUpX = oCurrPoint.x - 1;
//									}
//								}
//
//								////right
//								//if (oCurrPoint.second < cols - 2) {
//								//	if (fgmask_aux.at<uchar>(oCurrPoint.first, oCurrPoint.second + 1) == 255) {
//								//		vPointsList.push_back(make_pair(oCurrPoint.first, oCurrPoint.second + 1));
//								//	}
//								//}
//								////bottom
//								//if (oCurrPoint.first < rows - 2) {
//								//	if (fgmask_aux.at<uchar>(oCurrPoint.first + 1, oCurrPoint.second) == 255) {
//								//		vPointsList.push_back(make_pair(oCurrPoint.first + 1, oCurrPoint.second));
//								//	}
//								//}
//								////left
//								//if (oCurrPoint.second > 0) {
//								//	if (fgmask_aux.at<uchar>(oCurrPoint.first, oCurrPoint.second - 1) == 255) {
//								//		vPointsList.push_back(make_pair(oCurrPoint.first, oCurrPoint.second - 1));
//								//	}
//								//}
//								////up
//								//if (oCurrPoint.first > 0) {
//								//	if (fgmask_aux.at<uchar>(oCurrPoint.first - 1, oCurrPoint.second) == 255) {
//								//		vPointsList.push_back(make_pair(oCurrPoint.first - 1, oCurrPoint.second));
//								//	}
//								//}
//							}
//							else {
//								//For 8 neighbours
//								//right
//								if (oCurrPoint.y < cols - 2) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x, oCurrPoint.y + 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x, oCurrPoint.y + 1));
//										if (iRightY < oCurrPoint.y + 1) iRightY = oCurrPoint.y + 1;
//									}
//								}
//								//down
//								if (oCurrPoint.x < rows - 2) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x + 1, oCurrPoint.y) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x + 1, oCurrPoint.y));
//										if (iDownX < oCurrPoint.x + 1) iDownX = oCurrPoint.x + 1;
//									}
//								}
//								//left
//								if (oCurrPoint.y > 0) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x, oCurrPoint.y - 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x, oCurrPoint.y - 1));
//										if (iLeftY < oCurrPoint.y - 1) iLeftY = oCurrPoint.y - 1;
//									}
//								}
//								//up
//								if (oCurrPoint.x > 0) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x - 1, oCurrPoint.y) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x - 1, oCurrPoint.y));
//										if (iUpX < oCurrPoint.x - 1) iUpX = oCurrPoint.x - 1;
//									}
//								}
//
//								//right up
//								if (oCurrPoint.y < cols - 2 && oCurrPoint.x > 0) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x - 1, oCurrPoint.y + 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x - 1, oCurrPoint.y + 1));
//										if (iRightY < oCurrPoint.y + 1) iRightY = oCurrPoint.y + 1;
//										if (iUpX < oCurrPoint.x - 1) iUpX = oCurrPoint.x - 1;
//									}
//								}
//								//right down
//								if (oCurrPoint.y < cols - 2 && oCurrPoint.x < rows - 2) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x + 1, oCurrPoint.y + 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x + 1, oCurrPoint.y + 1));
//										if (iRightY < oCurrPoint.y + 1) iRightY = oCurrPoint.y + 1;
//										if (iDownX < oCurrPoint.x + 1) iDownX = oCurrPoint.x + 1;
//									}
//								}
//								//left up
//								if (oCurrPoint.y > 0 && oCurrPoint.x > 0) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x - 1, oCurrPoint.y - 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x - 1, oCurrPoint.y - 1));
//										if (iLeftY < oCurrPoint.y - 1) iLeftY = oCurrPoint.y - 1;
//										if (iUpX < oCurrPoint.x - 1) iUpX = oCurrPoint.x - 1;
//									}
//								}
//								//left down
//								if (oCurrPoint.y > 0 && oCurrPoint.x < rows - 2) {
//									if (fgmask_aux.at<uchar>(oCurrPoint.x + 1, oCurrPoint.y - 1) == 255) {
//										vPointsList.push_back(Point(oCurrPoint.x + 1, oCurrPoint.y - 1));
//										if (iLeftY < oCurrPoint.y - 1) iLeftY = oCurrPoint.y - 1;
//										if (iDownX < oCurrPoint.x + 1) iDownX = oCurrPoint.x + 1;
//									}
//								}
//
//							}
//						}
//						catch (const std::exception& e) {
//							cout << "extractBlobs_Custom_IF ERROR";
//						}
//					} while (!vPointsList.empty());
//
//					//Blob creation
//					cvBlob blob = initBlob(iID, iUpX, iLeftY, abs(iRightY - iLeftY) + 1, abs(iDownX - iUpX) + 1);
//					bloblist.push_back(blob);
//					//vPointsList.clear();
//					//imshow("aux", fgmask_aux);
//				}
//			}
//		}
//
//		return 1;
//	}
//	catch (const std::exception& e) {
//		cout << "extractBlobs_Custom ERROR";
//		return -1;
//	}
//}
//
///**
// *	Blob classification between the available classes in 'Blob.hpp' (see CLASS typedef). All the input arguments must be
// *  initialized when using this function.
// *
// * \param frame Input image
// * \param fgmask Foreground/Background segmentation mask (1-channel binary image)
// * \param bloblist List with found blobs
// *
// * \return Operation code (negative if not succesfull operation)
// */
//
// // ASPECT RATIO MODELS
//#define MEAN_PERSON 0.3950
//#define STD_PERSON 0.1887
////#define ASPECT_RATIO_PERSON 0.2755
//
//#define MEAN_CAR 1.4736
//#define STD_CAR 0.2329
////#define ASPECT_RATIO_CAR 1.4666
//
//#define MEAN_OBJECT 1.2111
//#define STD_OBJECT 0.4470
//
//// end ASPECT RATIO MODELS
//
//// distances
//float ED(float val1, float val2)
//{
//	return sqrt(pow(val1 - val2, 2));
//}
//
//float WED(float val1, float val2, float std)
//{
//	return sqrt(pow(val1 - val2, 2) / pow(std, 2));
//}
////end distances
//int classifyBlobs_Optional2(cv::Mat frame, cv::Mat fgmask, std::vector<cvBlob> &bloblist)
//{
//	try {
//		//check input conditions and return -1 if any is not satisfied
//		//...
//
//		//required variables for classification
//		//...
//
//		//classify each blob of the list
//		for (int i = 1; i < bloblist.size(); i++)
//		{
//			cvBlob blob = bloblist[i]; //get i-th blob
//			//...
//			float fAspectRatio = float(blob.w) / blob.h;
//			//Mean calculation of the blob
//			Rect crop(blob.x, blob.y, blob.w, blob.h);
//			Mat oBlob = frame(crop);
//			//Mat oBlob = fgmask(crop);
//			Mat oBlob_HSV;
//
//			cvtColor(oBlob, oBlob_HSV, CV_BGR2HSV);
//
//			int histSize = 256;
//			int hist_w = 512, hist_h = 400;
//			float range[] = { 0, 256 }; //the upper boundary is exclusive
//			const float* histRange = { range };
//			bool uniform = true, accumulate = false;
//			Mat b_hist, g_hist, r_hist, h_hist, v_hist;
//
//			vector<Mat> bgr_channels;
//			vector<Mat> hsv_channels;
//			split(oBlob, bgr_channels);
//			split(oBlob_HSV, hsv_channels);
//
//			/// Compute the histograms:
//			calcHist(&bgr_channels[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
//			calcHist(&bgr_channels[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
//			calcHist(&bgr_channels[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
//
//			calcHist(&hsv_channels[0], 1, 0, Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate);
//			calcHist(&hsv_channels[1], 1, 0, Mat(), v_hist, 1, &histSize, &histRange, uniform, accumulate);
//
//
//			Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
//
//			normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
//			normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
//			normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
//
//			normalize(h_hist, h_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
//			normalize(v_hist, v_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
//
//			//***********************
//			//Histogram visualization
//			//***********************
//			/*
//			int bin_w = cvRound((double)hist_w / histSize);
//
//			for (int i = 1; i < histSize; i++)
//			{
//				line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
//					Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
//					Scalar(255, 0, 0), 2, 8, 0);
//				line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
//					Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
//					Scalar(0, 255, 0), 2, 8, 0);
//				line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
//					Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
//					Scalar(0, 0, 255), 2, 8, 0);
//			}
//
//			imshow("Blob Image", oBlob);
//			imshow("Blob Histogram", histImage);
//			waitKey();
//			*/
//			//***********************
//
//
//
//			//***********************
//			//Code to build a Model
//			//***********************
//			//We serialize a PERSON histogram to use as a model to compare with.
//			//This code only executes once, for the creation of the model
//			//The model must be deserialized at the beginning of the application so
//			//we can compute euclidian distance of blobs to the model and classify.
//			/*
//			SerializeMat("PERSON_b_hist.bin", b_hist);
//			SerializeMat("PERSON_g_hist.bin", g_hist);
//			SerializeMat("PERSON_r_hist.bin", r_hist);
//
//			SerializeMat("CAR_b_hist.bin", b_hist);
//			SerializeMat("CAR_g_hist.bin", g_hist);
//			SerializeMat("CAR_r_hist.bin", r_hist);
//
//
//			//This should be done at blobs.initBlob()
//			Mat PERSON_b_hist = matread("PERSON_b_hist.bin");
//			Mat PERSON_g_hist = matread("PERSON_g_hist.bin");
//			Mat PERSON_r_hist = matread("PERSON_r_hist.bin");
//			*/
//
//			//***********************
//			// Code to Classify based 
//			// on Euclidian Distance 
//			// of Histograms
//			//***********************
//			// Example of how to calculate Euclidian Distance of Blob to PERSON model histogram.
//			/*
//			double dist_to_person_b = norm(PERSON_b_hist, oBlob.channels[0], NORM_L2);
//			double dist_to_person_g = norm(PERSON_g_hist, oBlob.channels[1], NORM_L2);
//			double dist_to_person_r = norm(PERSON_r_hist, oBlob.channels[2], NORM_L2);
//
//			double total_distance_to_PERSON = dist_to_person_b + dist_to_person_g + dist_to_person_r;
//			*/
//
//
//
//			//*************************
//			//Color channels evaluation
//			//*************************
//			//We have been playing with color channels information to see
//			//if we could get any discriminatory features based on just statistics
//			//of color values, but the Histogram approach is more robust.
//			/*
//
//			//cvtColor(oBlob, oBlob, COLOR_BGR2GRAY);
//
//			Scalar oMean;
//			Scalar oStd;
//
//			//meanStdDev(oBlob, oMean, oStd);
//			meanStdDev(oBlob, oMean, oStd);
//
//			float fMean = (oMean(0) + oMean(1) + oMean(2)) / 3;
//			float fStd = (oStd(0) + oStd(1) + oStd(2)) / 3;
//			//float fMean = oMean(0);
//			//float fStd = oStd(0);
//
//			//Distance calculation to each class, min distance will be the class asigned to the blob
//			float fPersonDistance_Mean = ED(fMean, float(MEAN_PERSON));
//			float fCarDistance_Mean = ED(fMean, float(MEAN_CAR));
//			float fObjectDistance_Mean = ED(fMean, float(MEAN_OBJECT));
//
//			if (fPersonDistance_Mean < fCarDistance_Mean && fPersonDistance_Mean < fObjectDistance_Mean) bloblist[i].label = CLASS::PERSON;
//			if (fCarDistance_Mean < fPersonDistance_Mean && fCarDistance_Mean < fObjectDistance_Mean) bloblist[i].label = CLASS::CAR;
//			if (fObjectDistance_Mean < fPersonDistance_Mean && fObjectDistance_Mean < fCarDistance_Mean) bloblist[i].label = CLASS::OBJECT;
//
//			//float fPersonDistance_Std = ED(fStd, float(STD_PERSON));
//			//float fCarDistance_Std = ED(fStd, float(STD_CAR));
//			//float fObjectDistance_Std = ED(fStd, float(STD_OBJECT));
//
//			//if (fPersonDistance_Std < fCarDistance_Std && fPersonDistance_Std < fObjectDistance_Std) bloblist[i].label = CLASS::PERSON;
//			//if (fCarDistance_Std < fPersonDistance_Std && fCarDistance_Std < fObjectDistance_Std) bloblist[i].label = CLASS::CAR;
//			//if (fObjectDistance_Std < fPersonDistance_Std && fObjectDistance_Std < fCarDistance_Std) bloblist[i].label = CLASS::OBJECT;
//
//			//float fPersonDistance_AR = ED(fAspectRatio, float(MEAN_PERSON));
//			//float fCarDistance_AR = ED(fAspectRatio, float(MEAN_CAR));
//
//			//if (fPersonDistance_AR < fCarDistance_AR) {
//			//	bloblist[i].label = CLASS::PERSON;
//			//}else bloblist[i].label = CLASS::CAR;
//		}
//			*/
//
//			return 1;
//		}
//	}
//	catch (const std::exception& e) {
//		cout << "ERROR classifyBlobs_Optional2";
//		return -1;
//	}
//}
//
//
//void SerializeMat(const string& filename, const Mat& mat)
//{
//	ofstream fs(filename, fstream::binary);
//
//	// Header
//	int type = mat.type();
//	int channels = mat.channels();
//	fs.write((char*)&mat.rows, sizeof(int));    // rows
//	fs.write((char*)&mat.cols, sizeof(int));    // cols
//	fs.write((char*)&type, sizeof(int));        // type
//	fs.write((char*)&channels, sizeof(int));    // channels
//
//	// Data
//	if (mat.isContinuous())
//	{
//		fs.write(mat.ptr<char>(0), (mat.dataend - mat.datastart));
//	}
//	else
//	{
//		int rowsz = CV_ELEM_SIZE(type) * mat.cols;
//		for (int r = 0; r < mat.rows; ++r)
//		{
//			fs.write(mat.ptr<char>(r), rowsz);
//		}
//	}
//}
//
//Mat DeserializeMat(const string& filename)
//{
//	ifstream fs(filename, fstream::binary);
//
//	// Header
//	int rows, cols, type, channels;
//	fs.read((char*)&rows, sizeof(int));         // rows
//	fs.read((char*)&cols, sizeof(int));         // cols
//	fs.read((char*)&type, sizeof(int));         // type
//	fs.read((char*)&channels, sizeof(int));     // channels
//
//	// Data
//	Mat mat(rows, cols, type);
//	fs.read((char*)mat.data, CV_ELEM_SIZE(type) * rows * cols);
//
//	return mat;
//}
//
//
//int classifyBlobs(std::vector<cvBlob> &bloblist)
//{
//	try {
//		//check input conditions and return -1 if any is not satisfied
//		//...
//
//		//required variables for classification
//		//...
//
//		//classify each blob of the list
//		for (int i = 1; i < bloblist.size(); i++)
//		{
//			cvBlob blob = bloblist[i]; //get i-th blob
//			//...
//			float fAspectRatio = float(blob.w) / blob.h;
//
//			float fPersonDistance_AR = ED(fAspectRatio, float(MEAN_PERSON));
//			float fCarDistance_AR = ED(fAspectRatio, float(MEAN_CAR));
//			float fObjDistance_AR = ED(fAspectRatio, float(MEAN_OBJECT));
//
//			bool bCouldBePER = false;
//			bool bCouldBeCAR = false;
//			bool bCouldBeOBJ = false;
//
//			if ((fAspectRatio >= MEAN_PERSON - STD_PERSON) && (fAspectRatio <= MEAN_PERSON + STD_PERSON)) {
//				bCouldBePER = true;
//			}
//			if ((fAspectRatio >= MEAN_CAR - STD_CAR) && (fAspectRatio <= MEAN_CAR + STD_CAR)) {
//				bCouldBeCAR = true;
//			}
//			if ((fAspectRatio >= MEAN_OBJECT - STD_OBJECT) && (fAspectRatio <= MEAN_OBJECT + STD_OBJECT)) {
//				bCouldBeOBJ = true;
//			}
//			if ((fPersonDistance_AR < fCarDistance_AR) && bCouldBePER) {
//				bloblist[i].label = CLASS::PERSON;
//			}
//			else if ((fPersonDistance_AR > fCarDistance_AR) && bCouldBeCAR) {
//				bloblist[i].label = CLASS::CAR;
//			}
//			else if (bCouldBeCAR) {
//				bloblist[i].label = CLASS::OBJECT;
//			}
//			else bloblist[i].label = CLASS::UNKNOWN;
//		}
//
//		//destroy all resources
//		//...
//
//		//return OK code
//		return 1;
//	}
//	catch (const std::exception& e) {
//		cout << "ERROR classifyBlobs_Mine";
//		return -1;
//	}
//}
//
////stationary blob extraction function
// /**
//  *	Stationary FG detection
//  *
//  * \param fgmask Foreground/Background segmentation mask (1-channel binary image)
//  * \param fgmask_history Foreground history counter image (1-channel integer image)
//  * \param sfgmask Foreground/Background segmentation mask (1-channel binary image)
//  *
//  * \return Operation code (negative if not succesfull operation)
//  *
//  *
//  * Based on: Stationary foreground detection for video-surveillance based on foreground and motion history images, D.Ortego, J.C.SanMiguel, AVSS2013
//  *
//  */
//
//#define FPS 25 //check in video - not really critical
//#define SECS_STATIONARY 5 // to set
//#define I_COST 2 // to set // increment cost for stationarity detection
//#define D_COST 4 // to set // decrement cost for stationarity detection
//#define STAT_TH 0.5 // to set
//
//int extractStationaryFG(Mat fgmask, Mat &fgmask_history, Mat &sfgmask)
//{
//	float numframes4static = (FPS*SECS_STATIONARY);
//
//	//convert to floats
//
//	fgmask.convertTo(fgmask, CV_32F);
//	fgmask_history.convertTo(fgmask_history, CV_32F);
//
//	//removing shadowss when pixel values =127
//	threshold(fgmask, fgmask, 200, 1, THRESH_BINARY);
//
//	//increase or decrease the history according to equations 2 and 3
//	fgmask_history = fgmask_history + (I_COST * fgmask);
//	fgmask_history = fgmask_history - (D_COST * (1 - fgmask));
//	//TO avoid negative values in fgmask_history
//	threshold(fgmask_history, fgmask_history, 0, 1024, THRESH_TOZERO);
//
//	//update sfgmask
//
//	cv::Mat mini;
//	mini = min(1, fgmask_history / (numframes4static));
//
//	//threshold
//	threshold(mini, sfgmask, STAT_TH, 255, THRESH_BINARY);
//
//	//blobs format
//	sfgmask.convertTo(sfgmask, CV_8UC1);
//
//	return 1;
//}
//
////Performs two Morphological operations to get rid of small dots (OPENING) and to remove small holes (CLOSING)
//void MorphologicalOps(int iMode, Mat &fgmask) {
//	try {
//
//		/* Operation
//		   ---------
//		   Opening: MORPH_OPEN: 2
//		   Closing : MORPH_CLOSE : 3
//		   Gradient : MORPH_GRADIENT : 4
//		   Top Hat : MORPH_TOPHAT: 5
//		   Black Hat : MORPH_BLACKHAT: 6
//		*/
//		int operation = MORPH_OPEN;
//
//		/* Shape
//		   -----
//		   MORPH_RECT
//		   MORPH_CROSS
//		   MORPH_ELLIPSE
//		*/
//		int morph_elem = MORPH_ELLIPSE;
//		int morph_size = 1;
//
//		Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
//		//OPENING
//		if (iMode > 0) morphologyEx(fgmask, fgmask, operation, element);
//
//
//		//Second Morphological operation
//		operation = MORPH_CLOSE;
//		morph_elem = MORPH_RECT;
//		morph_size = 1;
//
//		element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
//		//CLOSE
//		if (iMode == 2) morphologyEx(fgmask, fgmask, operation, element);
//	}
//	catch (const std::exception& e) {
//		cout << "MorphologicalOps ERROR";
//	}
//}
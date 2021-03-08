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
#ifndef BLOBS_H_INCLUDE
#define BLOBS_H_INCLUDE

#include "opencv2/opencv.hpp"
using namespace cv;

// Maximun number of char in the blob's format
const int MAX_FORMAT = 1024;

/// Type of labels for blobs
typedef enum {
	UNKNOWN = 0,
	PERSON = 1,
	GROUP = 2,
	CAR = 3,
	OBJECT = 4
} CLASS;

struct cvBlob {
	int     ID;  /* blob ID        */
	int   x, y;  /* blob position  */
	int   w, h;  /* blob sizes     */
	int cx, cy;
	CLASS label; /* type of blob   */
	char format[MAX_FORMAT];
};

inline cvBlob initBlob(int id, int x, int y, int w, int h, int cx, int cy)
{
	cvBlob B = { id,x,y,w,h,cx,cy,UNKNOWN };
	return B;
}

Mat paintBlobImage(Mat frame, std::vector<cvBlob> bloblist, bool labelled);
int extractBlobs(Mat fgmask, std::vector<cvBlob> &bloblist, int connectivity);
int removeSmallBlobs(std::vector<cvBlob> bloblist_in, std::vector<cvBlob> &bloblist_out, int min_width, int min_height);
int removeBigBlobs(std::vector<cvBlob> bloblist_in, std::vector<cvBlob> &bloblist_out, int min_width, int min_height);
Point GetBallCoordinates(std::vector<cvBlob>);

#endif


#ifndef Movement_h
#define Movement_h

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Codes.h"

using namespace cv;
using namespace std;

class Movement
{
public:
	Movement();
	~Movement();
	bool SetImage(Mat image);
	bool IsThereMovement();

private:
	vector<Mat> images;
	Mat GetDifferenceBetweenImages(Mat img1, Mat img2, Mat img3);
	bool IsThereDifference(Mat imgDiff);

};


#endif // !Movement_h


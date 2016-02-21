#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//Global Variables
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;

void MatchingMethod(int, void*);

int main(int argc, char** argv)
{
	//Load image and template
	img = imread("C:\\Users\\shane_000\\Desktop\\College\\FinalYearProject\\FindWally\\waldo2original.png");
	templ = imread("C:\\Users\\shane_000\\Desktop\\College\\FinalYearProject\\FindWally\\beachWally.png");

	//Create windows
	namedWindow(result_window, CV_WINDOW_AUTOSIZE);
	namedWindow(image_window, CV_WINDOW_AUTOSIZE);

	MatchingMethod(0, 0);

	waitKey(0);
	return 0;
}

/**
* @function MatchingMethod
*/
void MatchingMethod(int, void*)
{
	//Source image to display
	Mat img_display;
	img.copyTo(img_display);

	//Creating the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	//Perfom Matching and Normalize
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	//Selecting the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	//For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	//These lines show the result
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	imshow(result_window, result);
	imshow(image_window, img_display);

	return;
}
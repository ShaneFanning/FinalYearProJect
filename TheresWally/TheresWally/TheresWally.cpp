// TheresWally.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
# include "opencv2/core/core.hpp"
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//cv::Mat wally = cv::imread("C:/Users/Shane/Desktop/College/FYP/FinalYearProJect/TheresWally/Pictures/carnivalWalda.jpg");
	//cv::Mat puzzle = cv::imread("C:/Users/Shane/Desktop/College/FYP/FinalYearProJect/TheresWally/Pictures/carnivalPuzzle.jpg");

	//cv::Mat wally = cv::imread("C:/Users/Shane/Desktop/College/FYP/FinalYearProJect/TheresWally/Pictures/fareWally.jpg");
	//cv::Mat puzzle = cv::imread("C:/Users/Shane/Desktop/College/FYP/FinalYearProJect/TheresWally/Pictures/farePuzzle.jpg");

	cv::Mat wally = cv::imread("C:/Users/Shane/Desktop/College/FYP/FinalYearProJect/TheresWally/Pictures/skiWally.jpg");
	cv::Mat puzzle = cv::imread("C:/Users/Shane/Desktop/College/FYP/FinalYearProJect/TheresWally/Pictures/skiPuzzle.jpg");

	if (wally.empty()) {
		cout << "image not loaded";
	}
	else if (puzzle.empty()) {
		cout << "puzzle not loaded";
	}
	else {
		int minHes = 400;

		cv::SurfFeatureDetector detector(minHes);
		std::vector<KeyPoint> WKeys, PKeys;

		detector.detect(wally, WKeys);
		detector.detect(puzzle, PKeys);

		cv::SurfDescriptorExtractor extractor;

		cv::Mat WDesc, PDesc;

		extractor.compute(wally, WKeys, WDesc);
		extractor.compute(puzzle, PKeys, PDesc);

		cv::FlannBasedMatcher matcher;
		std::vector< DMatch > matches;
		matcher.match(WDesc, PDesc, matches);

		double max_dist = 0; double min_dist = 100;

		//-- Quick calculation of max and min distances between keypoints
		for (int i = 0; i < WDesc.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < min_dist) min_dist = dist;
			if (dist > max_dist) max_dist = dist;
		}

		std::vector< DMatch > good_matches;

		for (int i = 0; i < WDesc.rows; i++)
		{
			if (matches[i].distance <= max(2 * min_dist, 0.02))
			{
				good_matches.push_back(matches[i]);
			}
		}

		cv::Mat img_matches;
		cv::drawMatches(wally, WKeys, puzzle, PKeys,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		for (int i = 0; i < (int)good_matches.size(); i++)
		{
			printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
		}

		cv::imshow("Found", img_matches);
		waitKey(0);
	}
}
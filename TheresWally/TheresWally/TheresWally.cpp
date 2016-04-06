/*
	Shane Fanning - C12373451
	Final Year Project - There's Wally
	This project allows the user to select from several different Where's Wally puzzles and then tries to automatically solve them for the user to see the solution to the problem.
	The output will automatically be adjusted to fit screen size so that it is easily visible.
	Once the first puzzle is solved the user will be able to choose another puzzle or close the program
*/

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include "wtypes.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void selectPuzzle();

// This function is used to get the size of the screen so that the final display will fit the screen on smaller laptops
void getScreenSize(int& width, int& height)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	width = desktop.right;
	height = desktop.bottom;
}

/*
This is the main part of the program it uses SurfFeatureDetector, SurfDescriptorExtractor and FlannbasedMatcher to take the keypoints from a template and puzzle and try to match them.
This should be able to find Wally's final location on the screen so that the user can see it.
For the final display I have the template beside the puzzle with the matches being shown for the user to see.
*/
void Search(cv::Mat puzzle) {
	int width, height;
	getScreenSize(width, height);

	cv::Mat wally = cv::imread("/../TheresWally/Templates/Wally/Movie.png");

	if (wally.empty()) {
		cout << "image not loaded";
	}

	else {
		int minHes = 400;

		cv::SurfFeatureDetector detector(minHes);
		std::vector<KeyPoint> WKeys, PKeys;

		//Key points for both the template and the puzzle are detected here
		detector.detect(wally, WKeys);
		detector.detect(puzzle, PKeys);

		cv::SurfDescriptorExtractor extractor;

		cv::Mat WDesc, PDesc;

		//We use the original images and the key points to get the descriptors
		//These descriptors will be used for matching between the teeo images.
		extractor.compute(wally, WKeys, WDesc);
		extractor.compute(puzzle, PKeys, PDesc);

		//Finall we use flann based matcher to find all corresponding descriptors between the two images
		cv::FlannBasedMatcher matcher;
		std::vector< DMatch > matches;
		matcher.match(WDesc, PDesc, matches);

		double max_dist = 0; double min_dist = 100;

		//Calculation of max and min distances between keypoints
		for (int i = 0; i < WDesc.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < min_dist) min_dist = dist;
			if (dist > max_dist) max_dist = dist;
		}

		std::vector< DMatch > good_matches;

		//Makess sure all matches are within max distance that was declared above
		for (int i = 0; i < WDesc.rows; i++)
		{
			if (matches[i].distance <= max(2 * min_dist, 0.02))
			{
				good_matches.push_back(matches[i]);
			}
		}

		//Creates the final image which will have both the template and the puzzle side by side
		//with the good matches drawn between them so that the user can see Wally's location
		cv::Mat img_matches;
		cv::drawMatches(wally, WKeys, puzzle, PKeys,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		for (int i = 0; i < (int)good_matches.size(); i++)
		{
			printf("Match Found [%d] Template: %d  \tPuzzle: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
		}

		//Solved puzzle will be displayed in a windopw adjusted to the size of the screen
		namedWindow("Found", CV_WINDOW_NORMAL);
		cv::resizeWindow("Found", width, height);
		cv::imshow("Found", img_matches);
		waitKey(0);
		selectPuzzle();
	}
}

// This function allows he user to select a puzzle to be searched or close the program
void selectPuzzle() {
	cv::Mat Deep_Sea = cv::imread("/../TheresWally/Puzzles/0.jpg");
	cv::Mat Movie = cv::imread("/../TheresWally/Puzzles/2.jpg");
	cv::Mat Giants = cv::imread("/../TheresWally/Puzzles/3.jpg");
	cv::Mat Stage = cv::imread("/../TheresWally/Puzzles/5.jpg");
	cv::Mat Wallys = cv::imread("/../TheresWally/Puzzles/15.jpg");

	int c;
	printf("Please enter the number for the puzzle you want to search or 0 to exit!.\n\t 1. Deep sea \n\t 2. Movie Set \n\t 3. Land of Giants \n\t 4. The Big Stage \n\t 5. Multiple Wally's \n");
	c = getchar();

	switch (c)
	{
	// Depending on what is entered the program start a search for its corresponding puzzle
	case 1:
		Search(Deep_Sea);
		break;
	case 2:
		Search(Movie);
		break;
	case 3:
		Search(Giants);
		break;
	case 4:
		Search(Stage);
		break;
	case 5:
		Search(Wallys);
		break;
	case 0:
		// If zero is entered the program will close
		break;
	}
}

// Main is used to call selectPuzzle() after every search and at the start of the program 
int main(int argc, char** argv)
{
		selectPuzzle();
		return 0;
}
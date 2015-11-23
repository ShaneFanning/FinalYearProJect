#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main(){

Mat im = imread("C:\\Users\\shane_000\\Pictures\\Saved Pictures\\001 - 1.jpg");
if (im.empty()){

cout << "Cannot load image!" << endl;
return -1;

}

imshow("Image", im);
waitKey(0);

}
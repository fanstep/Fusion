// Fusion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <ctime>
#include <time.h>
#include "opencv2/video/tracking.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include<iostream>

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	im1 = imread("image1.jpg", CV_LOAD_GRAYSCALE_IMAGE);
	im2 = imread("image2.jpg", CV_LOAD_GRAYSCALE_IMAGE);
	im1.convertTo(im1, CV_32FC1);
	im2.convertTo(im2, CV_32FC1);
	
		Mat tmp1 = im1;
		Mat tmp2 = im2;
		Mat dst1, dst2;
		Mat up1, up2, Id1, Id2; Mat Idf[4];
		Mat dl, dl1;
		for (int i = 0; i < 4; i++){
			pyrDown(tmp1, dst1, Size(tmp1.cols / 2, tmp1.rows / 2));
			pyrUp(dst1, up1, Size(dst1.cols * 2, dst1.rows * 2));
			Id1 = tmp1 - up1;
			tmp1 = dst1;
			pyrDown(tmp2, dst2, Size(tmp2.cols / 2, tmp2.rows / 2));
			pyrUp(dst2, up2, Size(dst2.cols * 2, dst2.rows * 2));
			Id2 = tmp2 - up2;
			tmp2 = dst2;
			dl = abs(Id1) - abs(Id2);
			dl1.convertTo(dl1, CV_32FC1);
			for (int m = 0; m < dl.rows; m++){
				for (int n = 0; n < dl.cols; n++){
					if (dl.at<float>(m, n) >= 0){
						dl.at<float>(m, n) = 1;
					}
					else{
						dl.at<float>(m, n) = 0;
					}
				}
			}
			dl1 = 1 - dl;
			Idf[i] = dl.mul(Id1) + dl1.mul(Id2);
		}
		Mat imf = 0.5*(tmp1 + tmp2);
		for (int j = 3; j >= 0; j--){
			pyrUp(imf, imf, Size(imf.cols * 2, imf.rows * 2));
			imf = Idf[j] + imf;
		}
		double min, max;
		cv::minMaxIdx(imf, &min, &max);
		cv::Mat adjmap;
		cv::convertScaleAbs(imf, adjmap, 255 / max);
		cv::imshow("Out", adjmap);

		//sprintf_s(s, "frame%d.jpg", k);
		//imwrite(s, adjmap);

	waitKey(0);
	return 0;
}


#ifndef _FUSION_H_
#define _FUSION_H_

#include <opencv2/opencv.hpp>

using namespace cv;

void fusion(Mat& rgb, Mat& mono, Mat& output);
void nonLinearMask(Mat& input, int radius, float sigma, float midtone, Mat& output);


#endif //_FUSION_H_
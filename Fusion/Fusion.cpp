#include <vector>
#include <iostream>
#include "Fusion.h"

using namespace std;

void fusion(Mat& rgb, Mat& mono, Mat& output)
{
	output.create(Size(rgb.cols, rgb.rows), CV_32FC3);

	Mat rgbf, monof;
	rgb.convertTo(rgbf, CV_32FC3, 1.0 / 255);
	mono.convertTo(monof, CV_32FC3, 1.0 / 255);

	float tmpYUV1[3], tmpYUV2[3];
	for (int i = 0; i < rgb.rows; ++i)
	{
		for (int j = 0; j < rgb.cols; ++j)
		{
			//BGR2YUV
			tmpYUV1[0] = 0.299 * rgbf.at<Vec3f>(i, j)[2] + 0.587 * rgbf.at<Vec3f>(i, j)[1] + 0.114 * rgbf.at<Vec3f>(i, j)[0];
			tmpYUV1[1] = -0.147 * rgbf.at<Vec3f>(i, j)[2] - 0.289 * rgbf.at<Vec3f>(i, j)[1] + 0.436 * rgbf.at<Vec3f>(i, j)[0];
			tmpYUV1[2] = 0.615 * rgbf.at<Vec3f>(i, j)[2] - 0.515 * rgbf.at<Vec3f>(i, j)[1] - 0.1000 * rgbf.at<Vec3f>(i, j)[0];

			if (tmpYUV1[0] == 0) {
				tmpYUV1[0] = 1;
			}

			//UV调整
			float ratio = 1 + (monof.at<Vec3f>(i, j)[0] / tmpYUV1[0] - 1) / 2;
			tmpYUV2[1] = tmpYUV1[1] * ratio;
			tmpYUV2[2] = tmpYUV1[2] * ratio;
			//Y替换
			tmpYUV2[0] = monof.at<Vec3f>(i, j)[0];

			//YUV2BGR
			tmpYUV1[0] = tmpYUV2[0] + 2.032 * tmpYUV2[1];
			tmpYUV1[1] = tmpYUV2[0] - 0.394  * tmpYUV2[1] - 0.581 * tmpYUV2[2];
			tmpYUV1[2] = tmpYUV2[0] + 1.140 * tmpYUV2[2];
			for (int k = 0; k < 3; ++k)
			{
				tmpYUV1[k] = tmpYUV1[k] < 0 ? 0 : tmpYUV1[k];
				tmpYUV1[k] = tmpYUV1[k] > 1 ? 1 : tmpYUV1[k];
			}

			output.at<Vec3f>(i, j)[0] = tmpYUV1[0];
			output.at<Vec3f>(i, j)[1] = tmpYUV1[1];
			output.at<Vec3f>(i, j)[2] = tmpYUV1[2];
		}
	}
	output.convertTo(output, CV_8UC3, 255);
}

void nonLinearMask(Mat& input, int radius, float sigma, float midtone, Mat& output)
{
	//原图缩小25倍
	Mat resizeInput;
	resize(input, resizeInput, Size(), 0.2, 0.2, INTER_NEAREST);

	//转化成灰度图像
	Mat grayInput;
	Mat yuv;
	cvtColor(resizeInput, grayInput, COLOR_BGR2GRAY);
	cvtColor(input, yuv, COLOR_BGR2YUV);

	//灰度图取反
	for (int i = 0; i < grayInput.rows; ++i)
	{
		for (int j = 0; j < grayInput.cols; ++j)
		{
			grayInput.at<uchar>(i, j) = 255 - grayInput.at<uchar>(i, j);
		}
	}

	//模糊
	Mat tmpMask, mask;
	GaussianBlur(grayInput, tmpMask, Size(radius, radius), sigma);
	resize(tmpMask, mask, Size(input.cols, input.rows), 0, 0, INTER_NEAREST);

	output.create(Size(input.cols, input.rows), CV_8UC3);
	float exponent = 0;
	for (int i = 0; i < input.rows; ++i)
	{
		for (int j = 0; j < input.cols; ++j)
		{
			exponent = pow(2, (midtone - mask.at<uchar>(i, j)) / midtone);
			yuv.at<Vec3b>(i, j)[0] = 255 * pow(yuv.at<Vec3b>(i, j)[0] / 255.0, exponent);
		}
	}

	cvtColor(yuv, output, COLOR_YUV2BGR);
}

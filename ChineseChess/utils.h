#ifndef _UTILS__CHESS_H_
#define _UTILS__CHESS_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


const int Xdiff = -4;		//棋盘像素点偏移，用于最开始的校准
const int Ydiff = 6;

const int NUM = 10;
int Start_Pos[NUM] = {1,2,3,4,5,6,7,8,9,10};						//红色棋子的开始位置
int White_Pos[NUM] = { 112,113,114,115,116,117,118,119,120,121};   //白色棋子的开始位置

#define RED 0			//识别红色棋子
#define WHITE 1			//识别白色棋子

																   
//单个像素点周围的8个点
int dir_diff[8][2] = {
	{ -1, -1 },
	{ 0,  -1 },
	{ 1,  -1 },
	{ 1,   0 },
	{ 1,   1 },
	{ 0,   1 },
	{ -1, -1 },
	{ -1,  0 },
};

int FormatName(int index,string& str_index)
{
	stringstream ss;
	ss << index;
	ss >> str_index;
	//cout << str_index << endl;
	return 0;
}

//腐蚀和膨胀处理函数
void Erosion()
{
	//int erosion_type;
	//if (erosion_elem == 0){ erosion_type = MORPH_RECT; }
	//else if (erosion_elem == 1){ erosion_type = MORPH_CROSS; }
	//else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(3, 3),
		Point(1, 1));

	/// 腐蚀操作
	//erode(src, erosion_dst, element);
}

//腐蚀和膨胀处理函数
void Dilation()
{
	//int dilation_type;
	//if (dilation_elem == 0){ dilation_type = MORPH_RECT; }
	//else if (dilation_elem == 1){ dilation_type = MORPH_CROSS; }
	//else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(3, 3),
		Point(1, 1));
	///膨胀操作
	//dilate(src, dilation_dst, element);
}

//取周围9个点的平均值作为该点的像素值
void HSVAve(Mat_<Vec3b>_I, Point P, int&h, int&s, int&v)
{
	int _px = 0, _py = 0;
	int h_sum = 0, s_sum = 0, v_sum = 0;


	for (int i = 0; i < 8; i++)
	{
		_px = P.x + dir_diff[i][0];
		_py = P.y + dir_diff[i][1];

		h_sum += _I(Point(_px, _py))[0];
		s_sum += _I(Point(_px, _py))[1];
		v_sum += _I(Point(_px, _py))[2];
	}

	h_sum += _I(P)[0];
	s_sum += _I(P)[1];
	v_sum += _I(P)[2];

	h = (int)h_sum*1.0 / 9;
	s = (int)s_sum*1.0 / 9;
	v = (int)v_sum*1.0 / 9;
}


void find_thresh(Mat src, Point chess_pos[])
{
	//保证单通道图像
	Mat src_gray;
	if (src.channels() == 3)
		cvtColor(src, src_gray, CV_BGR2GRAY);
	else
		src_gray = src.clone();

	int thresh = 150;
	RNG rng(12345);
	Mat canny_output;
	vector<Mat> contours;
	vector<Vec4i> hierarchy;

	// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	/// 计算矩
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	if (contours.size() != 121)
	{
		cout << "detect err : contour not equal 121 !" << endl;
		//return;
	}

	/// 计算中心矩:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i< contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	ofstream os("121pos.txt");

	// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 120; i>=0; i--)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
		//imshow("Contours", drawing);
		//cvWaitKey(0);
		os << (int)mc[i].x << '\t' << (int)mc[i].y << endl;
		chess_pos[120 - i] = mc[i];				//中心点坐标存入数组(改变从下往上的顺序)
	}
	os.close();
}

void GetLabelImg()
{
	Mat src = imread("56.png");

	cvtColor(src, src, CV_BGR2GRAY);
	threshold(src, src, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	Mat erosion_dst, dilate_dst;

	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(3, 3),
		Point(1, 1));

	//腐蚀操作
	erode(src, erosion_dst, element);
	dilate(erosion_dst, dilate_dst, element);
		
	imwrite("morph6_29.bmp", dilate_dst);
	cvWaitKey(0);
	//Point chess_pos[121];
	//find_thresh(dilate_dst, chess_pos);
}


void LabelImg()
{
	Point leftu(54, 180);
	Point rightd(356,448);
	Mat erosion_dst, dilate_dst;

	Mat bin = imread("morph6_29.bmp");
	Rect chess(leftu, rightd);
	Mat src = bin(chess);

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 3));
	cv::erode(src, src, element);
	cv::dilate(src, src, element);

	imwrite("src_crop.bmp", src);
	cvWaitKey(0);
	Point chess_pos[121];
	find_thresh(src, chess_pos);

}

#endif
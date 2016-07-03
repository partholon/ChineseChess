#ifndef _GET_FRAME_H_
#define _GET_FRAME_H_


#include "utils.h"

//read video file
int CapVideo(const char* video_file)
{
	VideoCapture cap(video_file);  // open the default camera
	if (!cap.isOpened())		   // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);

	int count = 0;
	while (true){
		Mat frame;
		cap >> frame;             // get a new frame from camera
		if (frame.empty())
			break;

		count++;
		if (count == 4000)
		{
			imwrite("frame_40.bmp", frame);
			break;
		}

		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		cv::imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}

	return 0;
}

//将视频文件写成图像帧
int  CropVideo2Frames(const char* video_file)
{
	VideoCapture cap(video_file);  // open the default camera
	if (!cap.isOpened())		   // check if we succeeded
		return -1;

	int i = 0;
	string suffix;
	
	//\~
	Mat hsv_img;
	while (true)
	{
		Mat frame;
		cap >> frame;

		if (frame.empty())
			break;
		FormatName(i, suffix);
		suffix = string("D:\\Video\\") + suffix + string(".png");
		cout << "writing img: " << i << '\t' << "img" << endl;

		GaussianBlur(frame, frame, Size(5, 5), 0, 0);
		cvtColor(frame, hsv_img, COLOR_BGR2HSV);
		imshow("img3color", hsv_img);
		imshow("img3src", frame);

		if (i == 100)
		{
			imwrite("img_src.bmp", frame);
			imwrite("img_hsv.bmp", hsv_img);
		}

		cvWaitKey(0);
		suffix.clear();
		i++;
		if (waitKey(2) >= 0) 
			break;
	}

	return 0;
}

//write video frame into file
int RecordVideo()
{
	VideoCapture vcap(0);
	if (!vcap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	int frame_width =  (int)vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("D:\\Video\\out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height), true);

	for (;;){
		Mat frame;
		vcap >> frame;
		video.write(frame);
		imshow("Frame", frame);
		char c = (char)waitKey(30);
		if (c == 27) break;
	}
	return 0;
}

//测试两帧图像直接做差
void TESTabs()
{
	Mat bg_frame =  imread("D:\\Video\\out_frames_null\\1024.png",0);
	Mat cam_frame = imread("D:\\Video\\out0_frames_v1_500\\400.png",0);
	Mat motion;

	//GaussianBlur(cam_frame, cam_frame, Size(7, 7), 1.5, 1.5);
	absdiff(bg_frame, cam_frame, motion);

	threshold(motion, motion, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("bg_frame", bg_frame);
	imshow("cam_frame", cam_frame);

	//threshold(motion, motion, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);
	imshow("motion", motion);
	cvWaitKey(0);
}

#endif
#ifndef _CAM_TEST_H_
#define _CAM_TEST_H_

#include "chess_pos.h"


void BALL_TEST()
{
	Mat src_ = imread("D:\\Video\\6_30\\out3\\25.png");
	Mat I;
	GaussianBlur(src_, I, Size(5, 5), 0, 0);

	Mat_<Vec3b> _I = I;

	int b = _I(303, 274)[0];
	int g = _I(303, 274)[1];
	int r = _I(303, 274)[2];
	cout << "b: " << b << '\t' << "g: " << g << '\t' << "r: " << r << endl;

	Mat src = I.clone();
	cvtColor(src, src, CV_BGR2GRAY);
	threshold(src, src, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("I_bin.bmp", src);
	cvWaitKey(0);
}

//write video frame into file
int CAP_FRAME()
{
	VideoCapture vcap(0);
	if (!vcap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	int frame_width =  (int)vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)vcap.get(CV_CAP_PROP_FRAME_HEIGHT);

	Mat frame;
	int status = InitPosArray(vcap, PosBGR);
	if (status != 0)
	{
		cout << "Initial error ! " << endl;
		return -1;
	}
	else{
		cout << "Initial success !" << endl;
	}

	while(true){
		vcap >> frame;
		GaussianBlur(frame, frame, Size(5, 5), 0, 0);

		//Piece10Status(frame,frame);
	    char c = cvWaitKey(30);
		if (c == 27)
			break;
	}
	return 0;
}


//�����ض����RGBֵ�Ƿ�ı�
void TEST_CERTAINPOINT()
{
	Mat frame;
	VideoCapture vcap(0);
	if (!vcap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return ;
	}

	int frame_width = (int)vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)vcap.get(CV_CAP_PROP_FRAME_HEIGHT);

	while (true){
		vcap >> frame;
		GaussianBlur(frame, frame, Size(5, 5), 0, 0);

		Mat_<Vec3b> _I = frame;

		int b = _I(300, 174)[0];
		int g = _I(300, 174)[1];
		int r = _I(300, 174)[2];
		cout <<"1: "<< "b: " << b << '\t' << "g: " << g << '\t' << "r: " << r << endl;

		b = _I(380, 224)[0];
		g = _I(380, 224)[1];
		r = _I(380, 224)[2];
		cout << "2: " << "b: " << b << '\t' << "g: " << g << '\t' << "r: " << r << endl;

		circle(frame, Point(300, 174), 2, Scalar(0, 255, 0), 1, 8);
		circle(frame, Point(380, 224), 2, Scalar(0, 255, 0), 1, 8);
		imshow("IMG", frame);
		char c = cvWaitKey(30);
		if (c == 27)
			break;
	}

}

//���Խ�ͼ��ת����HSV�ռ�
void TEST_BIN()
{
	Mat img = imread("D:\\Video\\340.png");
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	
	//imwrite("hsv_img.bmp", imgHSV);
	Mat_<Vec3b>_I = imgHSV;

	int h = _I(275, 240)[0];
	int s = _I(267, 256)[0];
	int v = _I(277, 243)[0];
	cout << h << "  " << s << "  " << v << endl;

	h = _I(300, 176)[0];
	s = _I(300, 176)[0];
	v = _I(300, 176)[0];
	cout << h << "  " << s << "  " << v << endl;

	cvWaitKey(0);
}


//����HSV�ռ�ʶ������״̬
int HSV_CAP_FRAME()
{
	VideoCapture vcap(0);
	if (!vcap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	int frame_width = (int)vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)vcap.get(CV_CAP_PROP_FRAME_HEIGHT);

	Mat frame;
	Mat hsv_img;

	int count = 0;

	while (true){
		vcap >> frame;

		GaussianBlur(frame, frame, Size(5, 5), 0, 0);
		
		cvtColor(frame, hsv_img, COLOR_BGR2HSV);

		hsv_img = imread("hsv_.bmp");
		Piece10Status(hsv_img,frame);
		
		char c = cvWaitKey(30);
		if (c == 27)
			break;
	}
	return 0;
}

//mode:white,red
vector<int> GetCurPos(Mat hsv, Mat src, int mode =0)
{
	vector<int>  pos_array;
	int piece = 0;
	Mat_<Vec3b>_I = hsv;
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < COLIDX[i]; j++)
		{
			piece++;
			Point pos = GetPiexlPos(i, j);		//�õ�(i,j)�����������
			pos.y = pos.y + Ydiff;
			pos.x = pos.x + Xdiff;
			//int h = _I(pos)[0];
			//int s = _I(pos)[1];
			//int v = _I(pos)[2];
			int h, s, v;
			HSVAve(_I, pos, h, s, v);

			//��Ӧ��ɫ(��ɫ)��if (h>26 && s<50 && v>100 && v<230)��-̫����Ҫ����v(optiom paraments)
			//��Ӧ��ɫ(��ɫ)��if (h>80 && s>80 && v>60)�����ַ���������ͬoptiom paraments
			//��ɫ(HSV��ɫ)���ȶ�

			//if (h>26 && s<50 && (v>100 && v<230))									//�ò�����Ҫ�����趨
			if (mode == RED)
			{																		//90
				if (h>135 && (s>15 && s<130) && (v>50 && v<180))						//��Ӧ��ɫ(hsv��ɫ)
				{
					circle(hsv, pos, 2, Scalar(0, 255, 255), 1, 8);
					pos_array.push_back(piece);
				}
			}

			if (mode == WHITE)
			{
				if (h>90 && s>160 && v>105)				//��Ӧblue(hsv)
				{
					circle(hsv, pos, 2, Scalar(0, 255, 255), 1, 8);
					pos_array.push_back(piece);
				}
			}
			
			//circle(src, pos, 2, Scalar(0, 255, 255), 1, 8);
		
		}
	}

	imshow("src_label", hsv);
	cvWaitKey(0);

	return pos_array;
}


//���Ե���HSVͼƬ..
void TEST_HSV_IMG()
{
	Mat hsv;
	Mat src = imread("3_cimg_src.bmp");
	cvtColor(src, hsv, COLOR_BGR2HSV);
	imshow("hsv", hsv);

	int piece = 0;
	Mat_<Vec3b>_I = hsv;
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < COLIDX[i]; j++)
		{
			piece++;
			Point pos = GetPiexlPos(i, j);		//�õ�(i,j)�����������
			//pos.y = pos.y + 3;				//һ�������������ӵ������(��Ҫ����ͼ��ȷ��)
			int h = _I(pos)[0];
			int s = _I(pos)[1];
			int v = _I(pos)[2];
			if (h>16 && s<50 && v>100)			//ȡֵ��Χ���ȶ�(���ڰ�ɫ����)
			{
				circle(src, pos, 2, Scalar(0, 255, 0), 1, 8);
			}
		
			if (h>160)							//ȡֵ��Χ���ȶ�(���ں�ɫ����)
			{
				circle(src, pos, 2, Scalar(0, 255, 255), 1, 8);
			}
		
		}
	}
	imshow("_src_", src);
	cvWaitKey(0);
}

//����HSV����
int TEST_HSV()
{
	VideoCapture vcap(0);
	if (!vcap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	int frame_width =  (int)vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)vcap.get(CV_CAP_PROP_FRAME_HEIGHT);

	Mat frame, hsv_img;
	int pre_pos[NUM] = {0};									//�����һ�����ӵ��λ��

	int count = 50;
	while (count>0)
	{
		vcap >> frame;
		count--;
	}
	count = 0;
	int pos_tmp[NUM] = {0};
	while (true){
		vcap >> frame;										//��֤��ȡ��ͼ���ǵ�ǰ���µ�ͼ��
		cvWaitKey(30);
		vcap >> frame;
		cvWaitKey(30);
		vcap >> frame;
		cvWaitKey(30);

		GaussianBlur(frame, frame, Size(5, 5), 0, 0);
		cvtColor(frame, hsv_img, COLOR_BGR2HSV);			//ת����HSV�ռ䴦��Ч������

		vector<int> pieces = GetCurPos(hsv_img, frame);
		cout << "detect size: "<<pieces.size() << endl;
		if (pieces.size() != NUM)
		{
			cout << "detect pieces error !" << endl;
			return -1;
		}

		for (int i = 0; i < pieces.size(); i++)
			pos_tmp[i] = pieces[i];

		if (count == 0)									   //��һ������
		{
			for (int i = 0; i < pieces.size(); i++)
			{
				pre_pos[i] = pieces[i];
				cout << pre_pos[i]<<"  ";
			}
			cout << endl;
			goto WAIT;
		}
				
		if(count==1)                                      //����λ������
		{
			int cur = 0, pre = 0;
			for (int i = 0; i < pieces.size(); i++)
			{
				cout<< pieces[i] << "  ";
			}
			cout<< endl;

			UpdatePos(pos_tmp, pre_pos, pre, cur);
			cout << pre << "--------->" << cur << endl;
		}
WAIT:
		count = 1;
		char c;
		std::cin >> c;
		if (c == 'q')
			break;
	}
	return 0;
}


//ʹ��HSV��ɫ�ռ�ʶ�����ӵ�״̬
int GetPiecesPos(Mat cur_frame, int& pre, int& cur)
{
	Mat frame, hsv_img;
	frame = cur_frame.clone();
	//imshow("frame", frame);
	//cvWaitKey(0);

	int pos_tmp[NUM] = { 0 };

	if (cur_frame.empty())
	{
		cout<<"capture image is empty !" << endl;
		pre = -1, cur = -1;
		return -1;
	}

	GaussianBlur(frame, frame, Size(5, 5), 0, 0);		//��˹ȥ��

	cvtColor(frame, hsv_img, COLOR_BGR2HSV);			//ת����HSV�ռ䴦��Ч������

	vector<int> pieces = GetCurPos(hsv_img, frame);
	cout << "detect size: " << pieces.size() << endl;
	if (pieces.size() != NUM)
	{
		cout << "detect pieces error !" << endl;
		pre = -2, cur = -2;
		return -1;
	}

	for (int i = 0; i < pieces.size(); i++)
		pos_tmp[i] = pieces[i];

	cur = 0, pre = 0;

	UpdatePos(pos_tmp, Start_Pos, pre, cur);				//����λ������
	std::cout << pre << "--------->" << cur << endl;
	
	return 0;
}

#endif
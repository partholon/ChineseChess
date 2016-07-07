#ifndef _CHESS_POS_H_
#define _CHESS_POS_H_

#include "utils.h"

//���������ص�(����յ�)����
class Pos{
public:
	int x ;
	int y ;
	Pos(int x_, int y_):x(x_), y(y_){}
};

//���Բ���λ״̬��ʾ
//�������ӵ�״̬
bool line1[1]   = { 0 };
bool line2[2]   = { 0 };
bool line3[3]   = { 0 };
bool line4[4]   = { 0 };
bool line5[13]  = { 0 };
bool line6[12]  = { 0 };
bool line7[11]  = { 0 };
bool line8[10]  = { 0 };
bool line9[9]   = { 0 };
bool line10[10] = { 0 };
bool line11[11] = { 0 };
bool line12[12] = { 0 };
bool line13[13] = { 0 };
bool line14[4]  = { 0 };
bool line15[3]  = { 0 };
bool line16[2]  = { 0 };
bool line17[1]  = { 0 };

//���ö�ά����
int sline[17][13];

//������λ������
const int COLIDX[17] = { 1, 2, 3, 4, 13, 12, 11, 10, 9, 10, 11, 12, 13, 4, 3, 2, 1 };

//��������λ�õ�RGBֵ
int PosBGR[121][3];

//��Ҫ����ʵ���������
Point  itval[17][2] = {
	{ Point(317, 118), Point(0, 0) },
	{ Point(309, 132), Point(326, 132) },
	{ Point(300, 147), Point(334, 146) },
	{ Point(291, 160), Point(342, 160) },
	{ Point(216, 175), Point(417, 173) },
	{ Point(226, 188), Point(407, 186) },
	{ Point(235, 201), Point(398, 199) },
	{ Point(243, 214), Point(389, 211) },
	{ Point(252, 225), Point(380, 224) },
	{ Point(245, 238), Point(388, 236) },
	{ Point(237, 250), Point(394, 248) },
	{ Point(230, 261), Point(402, 260) },
	{ Point(223, 274), Point(408, 271) },
	{ Point(293, 284), Point(339, 283) },
	{ Point(300, 294), Point(331, 294) },
	{ Point(308, 305), Point(324, 305) },
	{ Point(316, 315), Point(0, 0) },
};

//ά��1-10������.(���������һ��)
int CurHumanPos[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };	//��ǰλ��


//����״̬�仯
class Transfer{
public:
	int pre;	//�ı�֮ǰ
	int cur;	//��ǰֵ
	Transfer(int x = 1, int y = 1) : pre(x), cur(y) 
	{}
};

//�����Ӹı��״̬����pos�ṹ����
//�������ֵΪ1�����ʾ�������ƶ�;Ϊ0������״̬����
//�������Ϊ-1,���������
int GetChangeStatus(int CurPos[],int PrePos[],Transfer& pos)
{
	int index=-1, count=0;

	for (int i = 0; i < 10; i++)
	{
		if (CurPos[i] != PrePos[i])
		{
			index = i;
			count++;
		}
	}
	if (count > 1)		//ͬһʱ���ж�����Ӹı䣬����
		return -1;
	
	if (count == 0)		//û�����Ӹı�
		return 0;
	
	if (count == 1)		//��һ�����Ӹı�
	{
		pos.pre = PrePos[index];
		pos.cur = CurPos[index];
		return 1;
	}
	return 1;
}

void InitStatus(bool status[][17])
{
	memset(status, 0, 17 * 17);
}

//�õ�ĳ���ӵ����������
Point GetPiexlPos(int row,int col)
{
	//��ֹԽ�����
	if ((row < 0 || row>16) || (col<0 || col>COLIDX[row]))
	{
		cout << "error postion !" << endl;
		return Point(-1, -1);
	}

	int num = COLIDX[row];

	if (num == 1)
	{
		//1,17��ֻ��һ������
		if (row == 0)
			return itval[0][0];
		else if (row == 16)
			return itval[16][0];
	}

	//�������Թ�ϵ
	Point pie;

	double x_diff =  itval[row][1].x - itval[row][0].x;
	double y_diff =  itval[row][1].y - itval[row][0].y;
	pie.x = itval[row][0].x + x_diff*col*1.0 / (num - 1)*1.0;
	pie.y = itval[row][0].y + y_diff*col*1.0 / (num - 1)*1.0;

	return pie;
}

//�ʼ��ʼ������λ�õ�rgb����
int InitPosArray(VideoCapture& vcap, int pos_bgr[][3])
{
	Mat frame;
	int count = 100;
	int piece = 0;
	memset(pos_bgr, 0, sizeof(pos_bgr));			//����ȫ����0

	while (count--)									//�ȴ�����ͷ�ȶ�
	{
		vcap >> frame;
		imshow("standard.bmp",frame);
		cvWaitKey(30);
	}
	////\~~
	//frame = imread("D:\\Video\\6_30_\\66.png");

	GaussianBlur(frame, frame, Size(5, 5), 0, 0);

	Mat_<Vec3b> _I = frame;
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < COLIDX[i]; j++)
		{
			Point pos = GetPiexlPos(i, j);				//�õ�(i,j)�����������
			pos_bgr[piece][0] = _I(pos.x,pos.y)[0];		//����ǰ���ص�λ�õ�BGRֵ��ֵ������
			pos_bgr[piece][1] = _I(pos.x,pos.y)[1];		//ͨ���Ƚ����ص�λ�õ�װBGRֵ�仯
			pos_bgr[piece][2] = _I(pos.x,pos.y)[2];		//�ж�״̬�Ƿ�ı�
			piece++;									//1-121��
		}
	}

	return 0;
}

//������ɫ�ж����ӵ�״̬(���֣�-1,0,1)
int ClassPie(int bgr[],int piece)
{
	int err_ = 12;							//����rgbֵ�����仯Ϊ8
	int b = bgr[0];
	int g = bgr[1];
	int r = bgr[2];
	int b_err = abs(b - PosBGR[piece][0]);	//�ͳ�ʼʱ���bgrֵ�Ƚ�
	int g_err = abs(g - PosBGR[piece][1]);
	int r_err = abs(r - PosBGR[piece][2]);

	int ERR = b_err + g_err + r_err;
	if (ERR>err_ )							//�ж�״̬�Ƿ�ı�
		return 1;
	else
		return 0;
}

//get a piexl postion
int GetOneStatus_v1(Mat img,Point piexl,int bgr[])
{
	Mat_<Vec3b> _I = img;
	int i, j;
	int b = 0, g = 0, r = 0;

	b = _I(piexl.x, piexl.y)[0];
	g = _I(piexl.x, piexl.y)[1];
	r = _I(piexl.x, piexl.y)[2];
	for (int d = 0; d < 8; d++)			//�����ص���Χ8���㿼�ǽ�ȥ
	{
		i = piexl.x + dir_diff[d][0];
		j = piexl.y + dir_diff[d][1];

		b += _I(i, j)[0];
		g += _I(i, j)[1];
		r += _I(i, j)[2];
	}
	b = b / 9;				
	g = g / 9;
	r = r / 9;
	bgr[0] = b;
	bgr[1] = g;
	bgr[2] = r;

	return 0;
}

//get a piexl postion:NULL
bool GetOneStatus(Mat img, Point piexl)
{
	int diff = 2;	//ʵ�������̬����,��Χ����
	Rect round(piexl.x-diff,piexl.y-diff,2*diff,2*diff);
	Mat round_bin = img(round);

	int count = 0;
	uchar* p;
	for (int i = 0; i < round_bin.rows; ++i)
	{
		p = round_bin.ptr<uchar>(i);
		for (int j = 0; j < round_bin.cols; j++)
		{
			if (p[j] == 255)
				count++;
		}
	}
	
	//�������������ǰ�ɫ����˵����������״̬�ı�
	double accuracy = count*1.0/(diff*diff*1.0);
	if (accuracy >= 0.9)
		return 1;
	else
		return 0;
}

//����10�����ӵ�λ��
int UpdatePos(int change[],int pre_pos[],int& pre,int& cur)
{
	int num = NUM;
	int pre_index = -1;
	int cur_index = -1;
	int i, j;
	
	for (i = 0; i < num; i++)
		if (change[i] != pre_pos[i])
			break;
	if (i == num)
	{
		pre = -3, cur = -3;
		return 0;
	}


	for ( i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			if (change[i] == pre_pos[j])
				break;	
		}
		if (j >= num)
		{	
			cur_index = i;  //λ�ò���֮ǰ������λ�ü���(˵�����µ�λ��)
		}
	}

	for (j = 0; j < num; j++)
	{
		for (i = 0; i< num; i++)
		{
			if (pre_pos[j]==change[i])
				break;
		}
		if (i >= num)
		{
			pre_index = j;   //�ɵ�λ�ò���֮ǰ������λ�ü���(˵�����ƶ���λ��)
		}
	}
	pre = pre_pos[pre_index];
	cur = change[cur_index];

	pre_pos[pre_index] = cur;

	return 0;
}

//Print all the Grid status
vector<int> Piece10Status(Mat hsv_img,Mat src, int row = 17)
{
	vector<int> change_pos;
	int piece = 0;										//1-121���
	int bgr[3] = {0};
	Mat_<Vec3b>_I = hsv_img;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < COLIDX[i]; j++)
		{
			Point pos = GetPiexlPos(i, j);				//�õ�(i,j)�����������
			//bgr[0] = _I(pos.x, pos.y)[0];				//����ǰ���ص�λ�õ�BGRֵ��ֵ������
			//bgr[1] = _I(pos.x, pos.y)[1];				//ͨ���Ƚ����ص�λ�õ�װBGRֵ�仯
			//bgr[2] = _I(pos.x, pos.y)[2];				//�ж�״̬�Ƿ�ı�
			//int ex = ClassPie(bgr,piece);
			
			//int h = _I(pos)[0];
			//if (h<40)
			//	circle(src, pos, 2, Scalar(255, 0, 255), 1, 8);
			piece++;
		}
	}
	//imshow("hello", src);
	//imshow("hsv", hsv_img);
	return change_pos;
}


//��������ǰ����ͼ������ǰ����ͼ��
//trans:��ʼ��λ��
int GetNewPostion(Mat cur_img, Mat pre_img, Transfer& trans)
{
	Mat cur_gray, pre_gray;
	Mat motion;

	if (cur_img.channels() == 3)
		cvtColor(cur_img, cur_gray, CV_BGR2GRAY);
	else
		cur_gray = cur_img.clone();

	if (cur_img.channels() == 3)
		cvtColor(pre_img, pre_gray, CV_BGR2GRAY);
	else
		pre_gray = pre_img.clone();

	//��ǰ����ͼ�������ǰ��ͼ�������(�������ʼ��״̬����ı�)
	absdiff(cur_gray, pre_gray, motion);
	//imshow("abss", motion);
	//cvWaitKey(0);
	threshold(motion, motion, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	//�������Ӹı��λ��(������ʼ��2��)
	vector<int> change;// = Piece10Status(motion);
	cout << change.size() << endl;

	if (change.size() != 2)	//(������ʼ��2��)
	{
		cout << "detect status error! " << endl;
		return -1;
	}
	if (change[0] == change[1])
	{
		cout << "detect status error! " << endl;
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		if (CurHumanPos[i] == change[0] || CurHumanPos[i]==change[1])
		{
			if (CurHumanPos[i] == change[0] && CurHumanPos[i] != change[1])
			{
				trans.cur = change[1];
				trans.pre = change[0];
				CurHumanPos[i] = change[1];		//�������ӵ�״̬����(1:������λ��)
				return 0;
			}
			else
			{
				trans.cur = change[0];
				trans.pre = change[1];
				CurHumanPos[i] = change[0];		//�������ӵ�״̬����(0:������λ��)
				return 0;
			}
		}
	}

	return 0;
}

//������֡ͼ��ֱ�����
void TESTStatus()
{
	Mat bg_frame  = imread("D:\\Video\\out_frames_null\\1024.png", 0);
	Mat cam_frame = imread("D:\\Video\\out0_frames_v1_3000\\400.png", 0);
	Mat motion;

	absdiff(bg_frame, cam_frame, motion);
	threshold(motion, motion, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	//Piece10Status(motion);
}

#endif
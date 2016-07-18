// ChineseChessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChineseChess.h"
#include "ChineseChessDlg.h"
#include "MotorCalibrationDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <Windows.h>
#include "recognition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChineseChessDlg �Ի���



//ȫ�ֱ���
_thread_com g_Com1;
_thread_com g_Com2;
CString gZero;
vector<CString> gOrder; //�����б�
ofstream logfile,logfile2;

int run_flag = false;

//����ͼ�񲿷ֱ���
Mat cur_frame;
bool Captureing = false;
HANDLE cam_handle;


static DWORD WINAPI camera_thread(LPVOID para)
{
	VideoCapture vcap(0);		//Ĭ������ͷ
	if (!vcap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	int frame_width =  vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	int count_ = 0;
	while(run_flag){
		vcap >> cur_frame;

		if (count_ == 150)
			imwrite("_tmp_.bmp", cur_frame);

		char c = (char)waitKey(30);
		if (Captureing)		//�ȴ�ȫ�ֱ���cur_frame�����߳���ȫ����
		{
			waitKey(30);
			Captureing = false;
		}
		count_++;
	}
	return 0;
}


#include <io.h>

//��ȡ���ӿ�ʼʱ���λ��/*Ĭ�Ϻ�ɫ*/
vector<int>  InitStartPos(int mode)
{
	vector<int> vec_pos;
	vec_pos.push_back(-1);//����쳣���ظ�vector

	Mat hsv_img;
	int pos_tmp[NUM] = { 0 };
	Mat src;

	//_tmp_.bmp does not exists
	if (_access("_tmp_.bmp", 0) == -1)
	{
		src = cur_frame;
	}
	else
		src = imread("_tmp_.bmp");

	//check if frame empty
	if (src.empty())
	{
		return vec_pos;
	}
	Mat frame = src;

	//Sleep(2000);										//��֤����ͷ�ȶ�����
	GaussianBlur(frame, frame, Size(5, 5), 0, 0);		//��˹ȥ��
	cvtColor(frame, hsv_img, COLOR_BGR2HSV);			//ת����HSV�ռ䴦��Ч������

	vector<int> pieces = GetCurPos(hsv_img, frame,mode,1);

	return pieces;
}

CChineseChessDlg::CChineseChessDlg(CWnd* pParent /*=NULL*/)
//: CDialogEx(IDD_CHINESECHESS_DIALOG, pParent)
{
	gChess.m_cwnd = this;
	isCalibration = false;
	cam_thread_handle = NULL;
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChineseChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChineseChessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(ON_COM_RECEIVE, On_Receive)
END_MESSAGE_MAP()


// CChineseChessDlg ��Ϣ�������

BOOL CChineseChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	////  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChineseChessDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	HCURSOR hcur;
	hcur = LoadCursor(

		AfxGetInstanceHandle(),  // ��ȡʵ�����

		MAKEINTRESOURCE(IDC_CURSOR4) // �¹��

		);
	SetCursor(hcur);

	HDC memdc = CreateCompatibleDC(NULL);
	HBITMAP h = (HBITMAP)SelectObject(memdc, chessBoard);
	BitBlt(dc.m_hDC, 0, 0, bmpWidth, bmpHeight, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, h);

	CBitmap bitmapbk, bitmap[6], bitmapb, bitmapc, bitmapblank;
	CDC dcmemb, dcmem[6], dcmembk, dcmemblank;
	bitmap[1].LoadBitmap(IDB_BMP_P1);
	bitmap[2].LoadBitmap(IDB_BMP_P2);
	bitmap[3].LoadBitmap(IDB_BMP_P3);
	bitmap[4].LoadBitmap(IDB_BMP_P4);
	bitmap[5].LoadBitmap(IDB_BMP_P5);
	bitmap[0].LoadBitmap(IDB_BMP_P6);
	bitmapbk.LoadBitmap(IDB_BITMAP12);//��Ӱ
	bitmapb.LoadBitmap(IDB_BITMAP8);//����͸������ɫ��Χ��
	bitmapblank.LoadBitmap(IDB_BMP_BLANK);//����͸������ɫ��Χ��

	dcmem[1].CreateCompatibleDC(&dc);
	dcmem[2].CreateCompatibleDC(&dc);
	dcmem[3].CreateCompatibleDC(&dc);
	dcmem[4].CreateCompatibleDC(&dc);
	dcmem[5].CreateCompatibleDC(&dc);
	dcmem[0].CreateCompatibleDC(&dc);
	dcmemb.CreateCompatibleDC(&dc);
	dcmembk.CreateCompatibleDC(&dc);
	dcmemblank.CreateCompatibleDC(&dc);

	dcmem[1].SelectObject(&bitmap[1]);
	dcmem[2].SelectObject(&bitmap[2]);
	dcmem[3].SelectObject(&bitmap[3]);
	dcmem[4].SelectObject(&bitmap[4]);
	dcmem[5].SelectObject(&bitmap[5]);
	dcmem[0].SelectObject(&bitmap[0]);
	dcmembk.SelectObject(&bitmapbk);
	dcmemb.SelectObject(&bitmapb);
	dcmemblank.SelectObject(&bitmapblank);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			if (0 != gChess.playerType[i]) {
				if (0 != gChess.fullChess[gChess.factionChess[i][j].number].color) {
					if (DOWN == gChess.factionChess[i][j].upOrDown) {
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 17,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 17,
							35, 35, &dcmembk, 0, 0, SRCAND);
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 17,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 17,
							35, 35, &dcmem[i], 0, 0, SRCPAINT);
					}
					else 
					{
						//��Ӱ
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 10,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 8,
							35, 35, &dcmemb, 0, 0, SRCAND);

						//����
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 19,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 19,
							35, 35, &dcmembk, 0, 0, SRCAND);
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 19,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 19,
							35, 35, &dcmem[i], 0, 0, SRCPAINT
							);

					}
				}
			}
		}
	}
	
	//���Ƶ�ǰ��ұ�־
	if (gChess.playerType[gChess.currentPlayer] != 0) {
		//dc.BitBlt(	
		//	bmpWidth*0.0625 - 22,
		//	bmpHeight*0.365 - 22,
		//	44,44,
		//	NULL,0,0, WHITENESS);

		dc.BitBlt(
			bmpWidth*0.0625 - 10,
			bmpHeight*0.361 - 8,
			35, 35, &dcmemb, 0, 0, SRCAND);

		dc.BitBlt(
			bmpWidth*0.0625 - 19+ 5*gChess.currentPlayer,
			bmpHeight*0.361 - 19 + 5*gChess.currentPlayer,
			35, 35, &dcmembk, 0, 0, SRCAND);
		dc.BitBlt(
			bmpWidth*0.0625 - 19 + 5*gChess.currentPlayer,
			bmpHeight*0.361 - 19 + 5*gChess.currentPlayer,
			35, 35, &dcmem[gChess.currentPlayer], 0, 0, SRCPAINT);
	}

	CDialogEx::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChineseChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����������϶����˳�
LRESULT CChineseChessDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//ȫ������Ҫ�϶����ڣ�ע�͵�
	case WM_NCHITTEST:
		RECT yj1;
		::GetWindowRect(this->m_hWnd, &yj1);
		POINTS yj2 = (POINTS)MAKEPOINTS(lParam);

		//����ڱ�������
		if ((yj2.x < yj1.left + bmpWidth*0.947) && (yj2.y > yj1.top) && (yj2.y < yj1.top + bmpHeight*0.0648))
		{
			return HTCAPTION;
		}
		else {
			return HTCLIENT;
		}

	case WM_KEYDOWN:
		if (wParam != VK_ESCAPE)
			break;

	case WM_DESTROY:
		if (this->MessageBox("�����Ҫ�˳���Ϸ��?", "����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
			Destroy();
		}

		break;

	default:
		return CDialogEx::WindowProc(message, wParam, lParam);
	}

	CDialogEx::WindowProc(message, wParam, lParam);
}

void CChineseChessDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//�ж��Ƿ�������
	CRect rgn;
	if (HUMANSTEP == gChess.playerType[gChess.currentPlayer]) {
		//���ѡ������
		if (DOWN == tempplace.upOrDown || tempplace.number == gChess.pChess->number) {
			for (int j = 0; j < 10; j++) {
				rgn = CRect(
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].x - 16,
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].y - 16,
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].x + 16,
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].y + 16
					);

				if (rgn.PtInRect(point)) {
					//���ƾ���������
					if (NULL != gChess.pChess) {
						gChess.pChess->upOrDown = DOWN;
						this->InvalidateRect(
							CRect(
								gChess.fullChess[gChess.pChess->number].x - 19,
								gChess.fullChess[gChess.pChess->number].y - 19,
								gChess.fullChess[gChess.pChess->number].x + 20,
								gChess.fullChess[gChess.pChess->number].y + 22), 0);
					}
					//����������̧��
					gChess.pChess = &gChess.factionChess[gChess.currentPlayer][j];
					tempplace.number = gChess.pChess->number;
					gChess.pChess->upOrDown = UP;
					this->InvalidateRect(
						CRect(
							gChess.fullChess[gChess.pChess->number].x - 19,
							gChess.fullChess[gChess.pChess->number].y - 19,
							gChess.fullChess[gChess.pChess->number].x + 20,
							gChess.fullChess[gChess.pChess->number].y + 22), 0);
				}
			}
			//������򲻶��˳�
			if (NULL == gChess.pChess)
				return;
			tempplace.upOrDown = UP;
		}

		//ȷ������
		if (UP == tempplace.upOrDown) {
			CRect rgn;
			bool isPlayed = false;

			if (NULL == gChess.pChess)
				return;
			for (int i = 0; i < gChess.pChess->availablePlace.size(); i++) {
				int targetPlace = gChess.pChess->availablePlace[i];
				int originPlace = gChess.pChess->number;
				rgn = CRect(
					gChess.fullChess[targetPlace].x - 16,
					gChess.fullChess[targetPlace].y - 16,
					gChess.fullChess[targetPlace].x + 16,
					gChess.fullChess[targetPlace].y + 16);
				if (rgn.PtInRect(point)) {
					#ifndef _DEBUG
					//�쳣�����˳�
					if (!gOrder.empty()) {
						AfxMessageBox("��ȴ���е���������");
						return;
					}
					#endif

					if ((gChess.fullChess[targetPlace].place == gChess.currentPlayer + 1)
						|| (gChess.fullChess[targetPlace].place == 0)
						|| (gChess.fullChess[targetPlace].place == ((gChess.currentPlayer + 3 >= 6) ? (gChess.currentPlayer - 3) : (gChess.currentPlayer + 3)) + 1))
					{
						//·������
						vector<int> routh;
						gChess.SetMoreWalkPoint(originPlace, targetPlace, routh);
						gChess.SetWorkOrder(routh);

						//������ӻ�е���ƶ�
						//CString orderTemp;
						//orderTemp.Format("gspd1500;adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[originPlace].pos1 - 500, gChess.fullChess[originPlace].pos2 - 500, gChess.fullChess[originPlace].pos3 - 500);
						////���͵�һ�������������
						//if (bConnectFlag1) {
						//	g_Com1.write(orderTemp.GetBuffer(255), orderTemp.GetLength());
						//	logfile << "write->" << orderTemp << endl;
						//	bMotorReady_5 = false;
						//	bMotorReady_6 = false;
						//	bMotorReady_7 = false;
						//}
						////��������ָ��
						////orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", gChess.fullChess[originPlace].pos1, gChess.fullChess[originPlace].pos2, gChess.fullChess[originPlace].pos3);
						//orderTemp = "gspd800;gstp500;";
						//gOrder.push_back(orderTemp);
						//gOrder.push_back("x1c");
						////orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[originPlace].pos1 - 500, gChess.fullChess[originPlace].pos2 - 500, gChess.fullChess[originPlace].pos3 - 500);
						//orderTemp = "gstp-500;";
						//gOrder.push_back(orderTemp);
						//orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[targetPlace].pos1 - 500, gChess.fullChess[targetPlace].pos2 - 500, gChess.fullChess[targetPlace].pos3 - 500);
						//gOrder.push_back(orderTemp);
						////orderTemp.Format("gspd400;adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[targetPlace].pos1, gChess.fullChess[targetPlace].pos2, gChess.fullChess[targetPlace].pos3);
						//orderTemp = "gstp500;";
						//gOrder.push_back(orderTemp);
						//gOrder.push_back("x3c");
						////orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[targetPlace].pos1 - 500, gChess.fullChess[targetPlace].pos2 - 500, gChess.fullChess[targetPlace].pos3 - 500);
						//orderTemp = "gstp-500;";
						//gOrder.push_back(orderTemp);

						//������ͼ
						gChess.fullChess[targetPlace].color = gChess.currentPlayer + 1;
						gChess.fullChess[originPlace].color = 0;
						this->InvalidateRect(
							CRect(
								gChess.fullChess[gChess.pChess->number].x - 19,
								gChess.fullChess[gChess.pChess->number].y - 19,
								gChess.fullChess[gChess.pChess->number].x + 20,
								gChess.fullChess[gChess.pChess->number].y + 22), 0);

						gChess.pChess->number = targetPlace;
						gChess.pChess->upOrDown = DOWN;
						this->InvalidateRect(
							CRect(
								gChess.fullChess[gChess.pChess->number].x - 19,
								gChess.fullChess[gChess.pChess->number].y - 19,
								gChess.fullChess[gChess.pChess->number].x + 19,
								gChess.fullChess[gChess.pChess->number].y + 19), 0);
						isPlayed = true;
						break;
					}
				}
			}

			if ((gChess.fullChess[gChess.pChess->number].place) == (gChess.currentPlayer + 1)
				|| (gChess.fullChess[gChess.pChess->number].place == 0)
				|| (gChess.fullChess[gChess.pChess->number].place == ((gChess.currentPlayer + 3 >= 6) ? (gChess.currentPlayer - 3) : (gChess.currentPlayer + 3)) + 1)) {
				if (gChess.pChess->number != tempplace.number) {
					isPlayed = true;
					gChess.pChess->upOrDown = DOWN;
					this->InvalidateRect(
						CRect(
							gChess.fullChess[gChess.pChess->number].x - 19,
							gChess.fullChess[gChess.pChess->number].y - 19,
							gChess.fullChess[gChess.pChess->number].x + 20,
							gChess.fullChess[gChess.pChess->number].y + 22), 0);
					tempplace.number = 0;
				}
			}

			//�Ƿ���ȷ�߹���
			if (isPlayed) {
				//			if (bpbb)
				gChess.pChess = NULL;

				bool flag = true;
				int destination = (gChess.currentPlayer + 3>5) ? (gChess.currentPlayer - 3) : (gChess.currentPlayer + 3);
				for (int j = 0; j < 10; j++) {
					if (gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].place != destination + 1)
						flag = false;
				}
				if (flag) {
					CString notice;
					notice.Format("���%d", gChess.currentPlayer + 1);
					notice.Append("������");
					MessageBox(notice, "����");
					gChess.playerType[gChess.currentPlayer] = BLANK;
				}

				int restPlayer = 0;
				for (int i = 0; i < 6; i++) {
					restPlayer += gChess.playerType[i];
				}
				//��ʣ����ң���Ϸ����
				if (0 == restPlayer)
					return;

				for (gChess.faction = 0; gChess.faction < 6; gChess.faction++) {
					gChess.ChessFull();
				}
				tempplace.upOrDown = DOWN;
				isPlayed = false;
				do {
					gChess.currentPlayer++;
					gChess.currentPlayer %= 6;
				} while (0 == gChess.playerType[gChess.currentPlayer]);
				gChess.NextStep();
				gOrder.push_back(gZero);
				g_Com1.write(gOrder[0].GetBuffer(255), gOrder[0].GetLength());
				logfile << "write->" << gOrder[0] << endl;
				gOrder.erase(gOrder.begin());
				bMotorReady_5 = false;
				bMotorReady_6 = false;
				bMotorReady_7 = false;
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CChineseChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//���Ͻ��˳�
	CRect closeRect(bmpWidth*0.947, 0, bmpWidth, bmpHeight*0.0648);
	if (closeRect.PtInRect(point)) {
		if (this->MessageBox("�����Ҫ�˳���Ϸ��?", "����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
			Destroy();
		}
	}

	//Start��ť
	CRect startRect(bmpWidth*0.0255, bmpHeight*0.0833, bmpWidth*0.102, bmpHeight*0.129);
	if (startRect.PtInRect(point)) {
		//�´��ڲ��񽹵�
		SetCapture();
		NewGame();
		Invalidate(0);
		ReleaseCapture();
	}

	//OK!!!��ť
	CRect okRect(bmpWidth*0.0255, bmpHeight*0.139, bmpWidth*0.102, bmpHeight*0.184);
	if (okRect.PtInRect(point)) {
		//�´��ڲ��񽹵�
		Captureing = true;
		int pre = 0, cur = 0;
		GetPiecesPos(cur_frame, pre, cur);	//��ȡ���ӵ���ʼ��
		pre = 122 - pre;
		cur = 122 - cur;
		/**************/
		PeopleDo(pre,cur);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CChineseChessDlg::NewGame()
{
	tempplace.upOrDown = DOWN;
	NewGameDlg newGameDlg;
	MotorCalibrationDlg calibrationDlg;
	gChess.currentPlayer = 0;
	newGameDlg.m_type_player1 = 1;
	newGameDlg.m_type_player4 = 2;
	newGameDlg.m_PlayBegin = 1;

	//ѯ��
	//int nResponseConfirm = this->MessageBox("�Ƿ���Ե��", "����", MB_ICONQUESTION|MB_YESNO);
	//if (nResponseConfirm == IDYES) {
	//	//У׼���
	//	calibrationDlg.DoModal();
	//}
	//����Ϸ����
	int nResponseNewGame = newGameDlg.DoModal();
	if (nResponseNewGame == IDOK)
	{
		logfile.open("./log.txt");
		logfile2.open("./log2.txt");
		gOrder.reserve(50);

#if !_DEBUG
		int cam = 0;
		//�����̼߳������ͷ
		DWORD id;
		//�����߳�camera_thread,cur_frame!!!
		run_flag = true;
		if(cam_handle==NULL)
			cam_handle = CreateThread(NULL, 0, camera_thread, (LPVOID)cam, 0, &id); //�����߳�
#endif

		//�򿪴���
		int port1, port2;
		port1 = newGameDlg.m_ComList[newGameDlg.port1];
		port2 = newGameDlg.m_ComList[newGameDlg.port2];
		if (!g_Com1.open(port1)) {
			AfxMessageBox(_T("�򿪴���1ʧ��"));
			return;
		}
		else {
			bConnectFlag1 = true;
		}
		if (!g_Com2.open(port2)) {
			AfxMessageBox(_T("�򿪴���2ʧ��"));
			return;
		}
		else {
			bConnectFlag2 = true;
		}
		//������Ϣ����
		g_Com1.set_hwnd(this->m_hWnd);

		//�����̼߳���
		g_Com1.resume();

		//�����ʼ��
		//֮��ӵ�InitChess��
		g_Com1.write("adr5;ena;adr6;ena;adr7;ena;", 27);
		Sleep(100);
		gZero.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd1500;", gChess.fullChess[61].pos1 - 3000, gChess.fullChess[61].pos2 - 3000, gChess.fullChess[61].pos3 - 3000);
		g_Com1.write(gZero.GetBuffer(255), gZero.GetLength());
		logfile << "write->" << gZero << endl;
		bMotorReady_5 = false;
		bMotorReady_6 = false;
		bMotorReady_7 = false;


#if !_DEBUG
		//!!!��ʼ�����ӵĿ�ʼλ��
		Sleep(2500);									//�ȴ�����ͷ�ȶ�
		if (_access("_tmp_.bmp", 0) == 0)
		{
			system(" del _tmp_.bmp ");
			Sleep(50);
			imwrite("_tmp_.bmp", cur_frame);
		}

		//�ұ�
		vector<int> vec_red = InitStartPos(0);		//��ʼ����ɫ���ӵ�λ��
		if (10 == vec_red.size()) {
			for (int i = 0; i < 10; i++)
				Right_Pos[i] = vec_red[i];
		}
		else {
			AfxMessageBox("red error");
			//return;
		}

		//���
		vector<int> vec_white = InitStartPos(1);	//��ʼ����ɫ���ӵ�λ��
		if (10 == vec_white.size()) {
			for (int i = 0; i < 10; i++)
				Left_Pos[i] = vec_white[i];
		}
		else {
			AfxMessageBox("blue error");
			//return;
		}
#endif	

		gChess.NewGame();
		gChess.playerType[0] = newGameDlg.m_type_player1;
		gChess.playerType[1] = newGameDlg.m_type_player2;
		gChess.playerType[2] = newGameDlg.m_type_player3;
		gChess.playerType[3] = newGameDlg.m_type_player4;
		gChess.playerType[4] = newGameDlg.m_type_player5;
		gChess.playerType[5] = newGameDlg.m_type_player6;
#if !_DEBUG
		gChess.SetFactionChess(Right_Pos, 3);//��ͼ��ʶ������ʼ��
		gChess.SetFactionChess(Left_Pos, 0);//��ͼ��ʶ������ʼ��
#endif
		gChess.SetColor();
		this->Invalidate(1);

#if !_DEBUG
		//@@@
		for (int i = 112; i <=121; i++) {
			Right_Pos[i-112] = i;
		}

		//�ָ�����
 		gChess.InitChess();
		if (!gOrder.empty()) {
			gOrder.push_back(gZero);
			CString tempStr;
			tempStr = gOrder[0];
			gOrder.erase(gOrder.begin());
			if (bConnectFlag1) {
				g_Com1.write(tempStr.GetBuffer(255), tempStr.GetLength());
				logfile << "write->" << tempStr << endl;
				bMotorReady_5 = false;
				bMotorReady_6 = false;
				bMotorReady_7 = false;
			}
		}
#endif

		int totalPlayer = 6;
		for (gChess.faction = 0; gChess.faction < 6; gChess.faction++)
			gChess.ChessFull();

		//�ж��������
		while (!gChess.playerType[newGameDlg.m_PlayBegin - 1])
		{
			newGameDlg.m_PlayBegin++;
			if (newGameDlg.m_PlayBegin > 6)
				newGameDlg.m_PlayBegin = 1;
			totalPlayer--;
			if (totalPlayer == 0)
				break;
		}
		gChess.currentPlayer = newGameDlg.m_PlayBegin - 1;

		if (0 <= gChess.currentPlayer && 5 >= gChess.currentPlayer) {
			if (0 != gChess.playerType[gChess.currentPlayer]) {
				gChess.beginGame = TRUE;
				gChess.NextStep();
			}
		}
	}
	else if (nResponseNewGame == IDCANCEL) {

	}
	

	return;
}

bool CChineseChessDlg::PeopleDo(int originPlace, int targetPlace)
{
	bool isPlayed = false;

	//��������Ƿ�Ϸ�
 	if (originPlace < 1 || originPlace>122){ 
		AfxMessageBox("ʶ�����ʧ��");
		return false;
	}
	if (gChess.fullChess[originPlace].color == 0) {
		AfxMessageBox("���Ƿ�");
		return false;
	}

	if (targetPlace < 1 || targetPlace>122){
		AfxMessageBox("ʶ���յ�ʧ��");
		return false;
	}
	if (gChess.fullChess[targetPlace].color != 0) {
		AfxMessageBox("�յ�Ƿ�");
		return false;
	}	

	//���»�ͼ
	gChess.fullChess[originPlace].color = 0;
	gChess.fullChess[targetPlace].color = gChess.currentPlayer + 1;
	int i;
	for (i = 0; i < 10; i++) {
		if (gChess.factionChess[gChess.currentPlayer][i].number == originPlace) {
			break;
		}
	}
	gChess.pChess = &gChess.factionChess[gChess.currentPlayer][i];
	gChess.pChess->number = originPlace;
	this->InvalidateRect(
		CRect(
			gChess.fullChess[gChess.pChess->number].x - 18,
			gChess.fullChess[gChess.pChess->number].y - 18,
			gChess.fullChess[gChess.pChess->number].x + 18,
			gChess.fullChess[gChess.pChess->number].y + 18), 0);

	gChess.pChess->number = targetPlace;
	gChess.pChess->upOrDown = DOWN;
	this->InvalidateRect(
		CRect(
			gChess.fullChess[gChess.pChess->number].x - 18,
			gChess.fullChess[gChess.pChess->number].y - 18,
			gChess.fullChess[gChess.pChess->number].x + 18,
			gChess.fullChess[gChess.pChess->number].y + 18), 0);
	isPlayed = true;

	if (isPlayed) {
		gChess.pChess = NULL;

		bool flag = true;
		int destination = (gChess.currentPlayer + 3 > 5) ? (gChess.currentPlayer - 3) : (gChess.currentPlayer + 3);
		for (int j = 0; j < 10; j++) {
			if (gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].place != destination + 1)
				flag = false;
		}
		if (flag) {
			CString notice;
			notice.Format("���%d", gChess.currentPlayer + 1);
			notice.Append("������");
			MessageBox(notice, "����");
			gChess.playerType[gChess.currentPlayer] = BLANK;
		}

		int restPlayer = 0;
		for (int i = 0; i < 6; i++) {
			restPlayer += gChess.playerType[i];
		}
		//��ʣ����ң���Ϸ����
		if (0 == restPlayer)
			return true;

		for (gChess.faction = 0; gChess.faction < 6; gChess.faction++) {
			gChess.ChessFull();
		}
		tempplace.upOrDown = DOWN;
		isPlayed = false;
		do {
			gChess.currentPlayer++;
			gChess.currentPlayer %= 6;
		} while (0 == gChess.playerType[gChess.currentPlayer]);
		gChess.NextStep();

		CString tempStr;
		tempStr = gOrder[0];
		gOrder.erase(gOrder.begin());
		if (bConnectFlag1) {
			g_Com1.write(tempStr.GetBuffer(255), tempStr.GetLength());
			logfile << "write->" << tempStr << endl;
			bMotorReady_5 = false;
			bMotorReady_6 = false;
			bMotorReady_7 = false;
		}
	}

	return true;
}

void CChineseChessDlg::Destroy()
{
	run_flag = false;
	logfile.close();
	g_Com1.close();
	g_Com2.close();
	gOrder.clear();
	Sleep(500);//�ȴ�����ͷ�ر�
	if (_access("_tmp_.bmp", 0) == 0)
	{
		system("del  _tmp_.bmp");
	}
	PostQuitMessage(0);
}

LRESULT CChineseChessDlg::On_Receive(WPARAM wp, LPARAM lp) 
{
	char* pRcvBuffer = new char[5000];
	int BufferLen = g_Com1.read(pRcvBuffer, 5000);

	logfile << time(NULL) << "read<-";
	for (int i = 0; i < BufferLen - 1; i++) {
		logfile << (pRcvBuffer[i]&0xff) << "  ";
		if (0xCC == (BYTE)(pRcvBuffer[i] & 0xff)) {
			if (0x05 == (BYTE)(pRcvBuffer[i + 1] & 0xff)) {
				bMotorReady_5 = true;
			}
			if (0x06 == (BYTE)(pRcvBuffer[i + 1] & 0xff)) {
				bMotorReady_6 = true;
			}
			if (0x07 == (BYTE)(pRcvBuffer[i + 1] & 0xff)) {
				bMotorReady_7 = true;
			}
		}
	}
	logfile << endl;

	if (!gOrder.empty()) {
		CString tempStr;
		tempStr = gOrder.front();
		gOrder.erase(gOrder.begin());
		if ('1c' == tempStr) {
			//tempStr = tempStr.Mid(1, tempStr.GetLength() - 1);
			//BYTE byte = strtol(tempStr, NULL, 16);
			//if (0x1C == byte) {
			//	//��
			//	Sleep(500);
			//	g_Com2 << byte;
			//	Sleep(1000);
			//	PostMessage(ON_COM_RECEIVE);
			//}
			//else if (0x3C == byte) {
			//	//��
			//	Sleep(1000);
			//	g_Com2 << byte;
			//	Sleep(1000);
			//	PostMessage(ON_COM_RECEIVE);
			//}
			for (int i = 0; i < 4; i++) {
				g_Com2 << up[i];
			}
			Sleep(1000);
			PostMessage(ON_COM_RECEIVE);
			logfile << "write->1c" << endl;
		}
		else if('3c' == tempStr){
			for (int i = 0; i < 4; i++) {
				g_Com2 << down[i];
			}
			Sleep(1500);
			PostMessage(ON_COM_RECEIVE);
			logfile << "write->3c" << endl;
		}
		else if (bMotorReady_5 && bMotorReady_6 && bMotorReady_7) {
			// ����˶�����
			Sleep(200);
			g_Com1.write(tempStr.GetBuffer(255), tempStr.GetLength());
			bMotorReady_5 = false;
			bMotorReady_6 = false;
			bMotorReady_7 = false;
			logfile << time(NULL) << "write->"  << tempStr << endl;
		}

		//for (int i = 0; i < gOrder.size(); i++) {
		//	logfile2 << gOrder[i] << endl;
		//}
		//logfile2 << endl;
	}
	delete[]pRcvBuffer;
	return 0;
}
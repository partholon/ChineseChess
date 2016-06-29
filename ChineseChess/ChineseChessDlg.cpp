// ChineseChessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChineseChess.h"
#include "ChineseChessDlg.h"
#include "MotorCalibrationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HBITMAP hBmp;// hbmmp;
// CChineseChessDlg �Ի���

//ȫ�ֱ���
HBITMAP hBmp, hbmmp;
_thread_com g_Com1;
_thread_com g_Com2;
CString gZero;
vector<CString> gOrder; //�����б�

CChineseChessDlg::CChineseChessDlg(CWnd* pParent /*=NULL*/)
	//: CDialogEx(IDD_CHINESECHESS_DIALOG, pParent)
{
	gChess.m_cwnd = this;
	isCalibration = false;
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

	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);

	HDC memdc = CreateCompatibleDC(NULL);
	HBITMAP h = (HBITMAP)SelectObject(memdc, hBmp);
	BitBlt(dc.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, h);

	CBitmap bitmapbk, bitmap[6], bitmapb, bitmapc;
	CDC dcmemb, dcmem[6], dcmembk, dcmemc;
	bitmap[1].LoadBitmap(IDB_BMP_P1);
	bitmap[2].LoadBitmap(IDB_BMP_P2);
	bitmap[3].LoadBitmap(IDB_BMP_P3);
	bitmap[4].LoadBitmap(IDB_BMP_P4);
	bitmap[5].LoadBitmap(IDB_BMP_P5);
	bitmap[0].LoadBitmap(IDB_BMP_P6);
	bitmapbk.LoadBitmap(IDB_BITMAP12);//��Ӱ
	bitmapb.LoadBitmap(IDB_BITMAP8);//����͸������ɫ��Χ��

	dcmem[1].CreateCompatibleDC(&dc);
	dcmem[2].CreateCompatibleDC(&dc);
	dcmem[3].CreateCompatibleDC(&dc);

	dcmem[4].CreateCompatibleDC(&dc);
	dcmem[5].CreateCompatibleDC(&dc);
	dcmem[0].CreateCompatibleDC(&dc);
	dcmemb.CreateCompatibleDC(&dc);
	dcmembk.CreateCompatibleDC(&dc);

	dcmem[1].SelectObject(&bitmap[1]);
	dcmem[2].SelectObject(&bitmap[2]);
	dcmem[3].SelectObject(&bitmap[3]);
	dcmem[4].SelectObject(&bitmap[4]);
	dcmem[5].SelectObject(&bitmap[5]);
	dcmem[0].SelectObject(&bitmap[0]);
	dcmembk.SelectObject(&bitmapbk);
	dcmemb.SelectObject(&bitmapb);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			if (0 != gChess.playerType[i]) {
				if (0 != gChess.fullChess[gChess.factionChess[i][j].number].color) {
					if (DOWN == gChess.factionChess[i][j].upOrDown) {
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 8,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 8,
							17, 17, &dcmembk, 0, 0, SRCAND
							);
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 8,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 8,
							17, 17, &dcmem[i], 0, 0, SRCPAINT
							);
					}
					else 
					{
						//��Ӱ
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 4,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 3,
							17, 17, &dcmemb, 0, 0, SRCAND
							);

						//����
						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 11,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 11,
							17, 17, &dcmembk, 0, 0, SRCAND
							);

						dc.BitBlt(
							gChess.fullChess[gChess.factionChess[i][j].number].x - 11,
							gChess.fullChess[gChess.factionChess[i][j].number].y - 11,
							17, 17, &dcmem[i], 0, 0, SRCPAINT
							);

					}
				}
			}
		}
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
	case WM_NCHITTEST:
		RECT yj1;
		::GetWindowRect(this->m_hWnd, &yj1);
		POINTS yj2 = (POINTS)MAKEPOINTS(lParam);

		//����ڱ�������
		if ((yj2.x < yj1.left + 573) && (yj2.y > yj1.top) && (yj2.y < yj1.top + 24))
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
		if (this->MessageBox("�����Ҫ�˳���Ϸ��?", "����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			PostQuitMessage(0);
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
		if (DOWN == tempplace.upOrDown || tempplace.number == gChess.pChess->number){
			for (int j = 0; j < 10; j++) {
				rgn = CRect(
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].x - 10,
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].y - 10,
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].x + 10,
					gChess.fullChess[gChess.factionChess[gChess.currentPlayer][j].number].y + 10
					);

				if (rgn.PtInRect(point)){
					//���ƾ���������
					if (NULL != gChess.pChess) {
						gChess.pChess->upOrDown = DOWN;
						this->InvalidateRect(
							CRect(
								gChess.fullChess[gChess.pChess->number].x - 11,
								gChess.fullChess[gChess.pChess->number].y - 11,
								gChess.fullChess[gChess.pChess->number].x + 13,
								gChess.fullChess[gChess.pChess->number].y + 13), 0);
					}
					//����������̧��
					gChess.pChess = &gChess.factionChess[gChess.currentPlayer][j];
					tempplace.number = gChess.pChess->number;
					gChess.pChess->upOrDown = UP;
					this->InvalidateRect(
						CRect(
							gChess.fullChess[gChess.pChess->number].x - 11,
							gChess.fullChess[gChess.pChess->number].y - 11,
							gChess.fullChess[gChess.pChess->number].x + 13,
							gChess.fullChess[gChess.pChess->number].y + 13), 0);
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
					gChess.fullChess[targetPlace].x - 10,
					gChess.fullChess[targetPlace].y - 10,
					gChess.fullChess[targetPlace].x + 10,
					gChess.fullChess[targetPlace].y + 10);
				if (rgn.PtInRect(point)) {
					//����ȡ�����ж�
					//Ӱ������
					//
					if ((gChess.fullChess[targetPlace].place == gChess.currentPlayer + 1)
						|| (gChess.fullChess[targetPlace].place == 0)
						|| (gChess.fullChess[targetPlace].place == ((gChess.currentPlayer + 3 >= 6) ? (gChess.currentPlayer - 3) : (gChess.currentPlayer + 3)) + 1))
					{
						gChess.fullChess[targetPlace].color = gChess.currentPlayer + 1;

						//������ӻ�е���ƶ�
						int origin = gChess.fullChess[originPlace].number;
						int target = gChess.fullChess[targetPlace].number;
						CString orderTemp;
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gpd800;", gChess.fullChess[origin].pos1 - 500, gChess.fullChess[origin].pos2 - 500, gChess.fullChess[origin].pos3 - 500);
						//���͵�һ�������������
						if (bConnectFlag1) {
							g_Com1.write(orderTemp.GetBuffer(255), orderTemp.GetLength());
							bMotorReady_5 = false;
							bMotorReady_6 = false;
							bMotorReady_7 = false;
						}
						//��������ָ��
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[origin].pos1, gChess.fullChess[origin].pos2, gChess.fullChess[origin].pos3);
						gOrder.push_back(orderTemp);
						gOrder.push_back("x1c");
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[origin].pos1 - 500, gChess.fullChess[origin].pos2 - 500, gChess.fullChess[origin].pos3 - 500);
						gOrder.push_back(orderTemp);
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;", gChess.fullChess[target].pos1 - 500, gChess.fullChess[target].pos2 - 500, gChess.fullChess[target].pos3 - 500);
						gOrder.push_back(orderTemp);
						orderTemp.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd400;", gChess.fullChess[target].pos1, gChess.fullChess[target].pos2, gChess.fullChess[target].pos3);
						gOrder.push_back(orderTemp);
						gOrder.push_back("x3c");
						gOrder.push_back(gZero);

						//if (gChess.fullChess[targetPlace].number != gChess.fullChess[originPlace].number) 
						gChess.fullChess[originPlace].color = 0;
						this->InvalidateRect(
							CRect(
								gChess.fullChess[gChess.pChess->number].x - 11,
								gChess.fullChess[gChess.pChess->number].y - 11,
								gChess.fullChess[gChess.pChess->number].x + 13,
								gChess.fullChess[gChess.pChess->number].y + 13), 0);

						gChess.pChess->number = targetPlace;
						gChess.pChess->upOrDown = DOWN;
						this->InvalidateRect(
							CRect(
								gChess.fullChess[gChess.pChess->number].x - 11,
								gChess.fullChess[gChess.pChess->number].y - 11,
								gChess.fullChess[gChess.pChess->number].x + 13,
								gChess.fullChess[gChess.pChess->number].y + 13), 0);
						isPlayed = true;
						break;
					}
				}
			}

			//����ȡ�����ж�
			//Ӱ������
			//
			if ((gChess.fullChess[gChess.pChess->number].place) == (gChess.currentPlayer + 1)
				|| (gChess.fullChess[gChess.pChess->number].place == 0)
				|| (gChess.fullChess[gChess.pChess->number].place == ((gChess.currentPlayer + 3 >= 6) ? (gChess.currentPlayer - 3) : (gChess.currentPlayer + 3)) + 1)) {
				if (gChess.pChess->number != tempplace.number) {
					isPlayed = true;
					gChess.pChess->upOrDown = DOWN;
					this->InvalidateRect(
						CRect(
							gChess.fullChess[gChess.pChess->number].x - 11,
							gChess.fullChess[gChess.pChess->number].y - 11,
							gChess.fullChess[gChess.pChess->number].x + 13,
							gChess.fullChess[gChess.pChess->number].y + 13), 0);
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
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CChineseChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//���Ͻ��˳�
	CRect closeRect(573, 0, 600, 24);
	if (closeRect.PtInRect(point)) {
		if (this->MessageBox("�����Ҫ�˳���Ϸ��?", "����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			PostQuitMessage(0);
	}

	//Start��ť
	CRect startRect(43, 68, 103, 94);
	if (startRect.PtInRect(point)) {
		//�´��ڲ��񽹵�
		SetCapture();
		NewGame();
		Invalidate(0);
		ReleaseCapture();
	}

	//OK��ť
	CRect okRect(43, 135, 103, 161);
	if (okRect.PtInRect(point)) {
		//�´��ڲ��񽹵�
		SetCapture();
		NewGame();
		Invalidate(0);
		ReleaseCapture();
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
	int nResponseConfirm = this->MessageBox("�Ƿ���Ե��", "����", MB_ICONQUESTION|MB_YESNO);
	if (nResponseConfirm == IDYES) {
		//У׼���
		calibrationDlg.DoModal();
	}
	//����Ϸ����
	int nResponseNewGame = newGameDlg.DoModal();
	if (nResponseNewGame == IDOK)
	{
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

		gChess.NewGame();
		gChess.playerType[0] = newGameDlg.m_type_player1;
		gChess.playerType[1] = newGameDlg.m_type_player2;
		gChess.playerType[2] = newGameDlg.m_type_player3;
		gChess.playerType[3] = newGameDlg.m_type_player4;
		gChess.playerType[4] = newGameDlg.m_type_player5;
		gChess.playerType[5] = newGameDlg.m_type_player6;
		gChess.SetColor();
		this->Invalidate(1);

		//�����ʼ��
		g_Com1.write("adr5;ena;adr6;ena;adr7;ena;", 37);
		Sleep(100);
		gZero.Format("adr5;pos%d;adr6;pos%d;adr7;pos%d;gspd800;", gChess.fullChess[61].pos1 - 500, gChess.fullChess[61].pos2 - 500, gChess.fullChess[61].pos3 - 500);
		g_Com1.write(gZero.GetBuffer(255), gZero.GetLength());

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

LRESULT CChineseChessDlg::On_Receive(WPARAM wp, LPARAM lp) 
{
	char* pRcvBuffer = new char[5000];
	int BufferLen = g_Com1.read(pRcvBuffer, 5000);
	//�ȴ��������
	for (int i = 0; i < BufferLen - 1; i++) {
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

	if (!gOrder.empty()) {
		CString tempStr;
		tempStr = gOrder.front();
		if ('x' == tempStr[0]) {
			gOrder.erase(gOrder.begin());
			tempStr = tempStr.Mid(1, tempStr.GetLength() - 1);
			BYTE byte = strtol(tempStr, NULL, 16);
			if (0x1C == byte) {
				//��
				g_Com2 << byte;
				Sleep(2000);
			}
			else if (0x3C == byte) {
				//��
				g_Com2 << byte;
				Sleep(4000);
			}
		}
		else if (bMotorReady_5 && bMotorReady_6 && bMotorReady_7) {
			// ����˶�����
			gOrder.erase(gOrder.begin());
			Sleep(200);
			g_Com1.write(tempStr.GetBuffer(255), tempStr.GetLength());
			bMotorReady_5 = false;
			bMotorReady_6 = false;
			bMotorReady_7 = false;
		}
	}
	delete[]pRcvBuffer;
	return 0;
}
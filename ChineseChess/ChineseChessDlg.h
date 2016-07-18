
// ChineseChessDlg.h : ͷ�ļ�
//

#pragma once
#include "ChessGame.h"
#include "NewGameDlg.h"
#include "com_class.h"
#include <Windows.h>

// CChineseChessDlg �Ի���
class CChineseChessDlg : public CDialogEx
{
// ����
public:
	CChineseChessDlg(CWnd* pParent = NULL);	// ��׼���캯��


// �Ի�������
public:	
	void NewGame();	//����Ϸ�ĶԻ���
	bool PeopleDo(int, int); //�������
	void Destroy();

public:
	HBITMAP chessBoard;
	ChessGame gChess; //������������
	Faction tempplace; //��ʱ����
	HANDLE cam_thread_handle;
	BYTE up[4] = { 0xEB,0x90,0x1C,0xB8 };
	BYTE down[4] = { 0xEB,0x90,0x3C,0xB8 };
	int bmpHeight, bmpWidth;

	bool isCalibration;
	bool bMotorReady_5;
	bool bMotorReady_6;
	bool bMotorReady_7;
	bool bConnectFlag1;
	bool bConnectFlag2;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHINESECHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT On_Receive(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

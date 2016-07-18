
// ChineseChessDlg.h : 头文件
//

#pragma once
#include "ChessGame.h"
#include "NewGameDlg.h"
#include "com_class.h"
#include <Windows.h>

// CChineseChessDlg 对话框
class CChineseChessDlg : public CDialogEx
{
// 构造
public:
	CChineseChessDlg(CWnd* pParent = NULL);	// 标准构造函数


// 对话框数据
public:	
	void NewGame();	//新游戏的对话框
	bool PeopleDo(int, int); //玩家下棋
	void Destroy();

public:
	HBITMAP chessBoard;
	ChessGame gChess; //所有棋子数据
	Faction tempplace; //临时棋子
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
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHINESECHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT On_Receive(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

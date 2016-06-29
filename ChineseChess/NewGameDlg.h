#pragma once
#include "afxwin.h"


// NewGameDlg �Ի���

class NewGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewGameDlg)

public:
	NewGameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NewGameDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEWGAME };
#endif
public:
	CString	m_PlayAdd;
	UINT	m_PlayBegin;
	int m_ComList[20];
	CString	m_PlayName;
	int port1, port2;
	void QueryKey(HKEY hKey);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cb_com1;
	CComboBox m_cb_com2;
	afx_msg void OnBnClickedOk();
	int m_type_player1;
	int m_type_player2;
	int m_type_player3;
	int m_type_player4;
	int m_type_player5;
	int m_type_player6;
};

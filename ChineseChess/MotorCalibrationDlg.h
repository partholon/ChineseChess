#pragma once
#include "afxwin.h"
#include "com_class.h"

// MotorCalibrationDlg 对话框

class MotorCalibrationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MotorCalibrationDlg)

public:
	MotorCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MotorCalibrationDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Calibration };
#endif
public:
	bool bConnectFlag;
	int m_ComList[20];
	_asyn_com m_Com1;
	
	void QueryKey(HKEY hKey);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cb_com;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnBnClickedBtnSend(UINT nID);
	CButton m_btn_start;
};

#pragma once


// ConfirmDlg 对话框

class ConfirmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConfirmDlg)

public:
	ConfirmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ConfirmDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

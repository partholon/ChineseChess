// NewGameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChineseChess.h"
#include "NewGameDlg.h"
#include "afxdialogex.h"


// NewGameDlg 对话框

IMPLEMENT_DYNAMIC(NewGameDlg, CDialogEx)

NewGameDlg::NewGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_NEWGAME, pParent)
	, m_type_player1(0)
	, m_type_player2(0)
	, m_type_player3(0)
	, m_type_player4(0)
	, m_type_player5(0)
	, m_type_player6(0)
{
	m_PlayBegin = 1;
	port1 = -1;
	port2 = -1;
	for (int i = 0; i < 20; i++) {
		m_ComList[i] = -1;
	}
}

NewGameDlg::~NewGameDlg()
{
}

void NewGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM1, m_cb_com1);
	DDX_Control(pDX, IDC_COMBO_COM2, m_cb_com2);
	DDX_Radio(pDX, IDC_RADIO1_NONE, m_type_player1);
	DDX_Radio(pDX, IDC_RADIO1_NONE2, m_type_player2);
	DDX_Radio(pDX, IDC_RADIO1_NONE3, m_type_player3);
	DDX_Radio(pDX, IDC_RADIO1_NONE4, m_type_player4);
	DDX_Radio(pDX, IDC_RADIO1_NONE5, m_type_player5);
	DDX_Radio(pDX, IDC_RADIO1_NONE6, m_type_player6);
}


BEGIN_MESSAGE_MAP(NewGameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewGameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// NewGameDlg 消息处理程序
BOOL NewGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 刷新串口列表
	HKEY hTestKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hTestKey)) {
		QueryKey(hTestKey);
	}
	RegCloseKey(hTestKey);

	if (-1 == m_ComList[0]) {
		AfxMessageBox(_T("本机没有串口！"));
	}
	int i = 0;
	while (i < 20 && -1 != m_ComList[i]) {
		CString sCom;
		sCom.Format(_T("COM%d"), m_ComList[i]);
		m_cb_com1.InsertString(i, sCom.GetBuffer(5));
		m_cb_com2.InsertString(i, sCom.GetBuffer(5));
		i++;
	}
	m_cb_com1.SetCurSel(0);
	m_cb_com2.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void NewGameDlg::QueryKey(HKEY hKey) {
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
	//	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	//	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

								 // Enumerate the subkeys, until RegEnumKeyEx fails.

								 // 	if (cSubKeys)
								 // 	{
								 // 		printf( "\nNumber of subkeys: %d\n", cSubKeys);
								 // 
								 // 		for (i=0; i<cSubKeys; i++) 
								 // 		{ 
								 // 			cbName = MAX_KEY_LENGTH;
								 // 			retCode = RegEnumKeyEx(hKey, i,
								 // 				achKey, 
								 // 				&cbName, 
								 // 				NULL, 
								 // 				NULL, 
								 // 				NULL, 
								 // 				&ftLastWriteTime); 
								 // 			if (retCode == ERROR_SUCCESS) 
								 // 			{
								 // 				_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
								 // 			}
								 // 		}
								 // 	} 

								 // Enumerate the key values. 
	if (cValues > 0) {
		for (i = 0, retCode = ERROR_SUCCESS; i<cValues; i++) {
			cchValue = MAX_VALUE_NAME;  achValue[0] = '\0';
			if (ERROR_SUCCESS == RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL)) {
				CString szName(achValue);
				if (-1 != szName.Find(_T("Serial")) || -1 != szName.Find(_T("VCom")) || -1 != szName.Find(_T("VCP"))) {
					BYTE strDSName[10]; memset(strDSName, 0, 10);
					DWORD nValueType = 0, nBuffLen = 10;
					if (ERROR_SUCCESS == RegQueryValueEx(hKey, (LPCTSTR)achValue, NULL, &nValueType, strDSName, &nBuffLen)) {
						int nIndex = -1;
						while (++nIndex < 20) {
							if (-1 == m_ComList[nIndex]) {
								m_ComList[nIndex] = atoi((char*)(strDSName + 3));
								break;
							}
						}
					}
				}
			}
		}
	}
	else {
		AfxMessageBox(_T("本机没有串口....."));
	}
	return;
}

void NewGameDlg::OnBnClickedOk()
{
	port1 = m_cb_com1.GetCurSel();
	port2 = m_cb_com2.GetCurSel();

	CDialogEx::OnOK();
}

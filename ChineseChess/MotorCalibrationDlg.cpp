// MotorCalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChineseChess.h"
#include "MotorCalibrationDlg.h"
#include "afxdialogex.h"


// MotorCalibrationDlg �Ի���

IMPLEMENT_DYNAMIC(MotorCalibrationDlg, CDialogEx)

MotorCalibrationDlg::MotorCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Calibration, pParent)
{
	bConnectFlag = false;
	for (int i = 0; i < 20; i++) {
		m_ComList[i] = -1;
	}
}

MotorCalibrationDlg::~MotorCalibrationDlg()
{
}

void MotorCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cb_com);
	DDX_Control(pDX, IDC_BTN_START, m_btn_start);
}


BEGIN_MESSAGE_MAP(MotorCalibrationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START, &MotorCalibrationDlg::OnBnClickedBtnStart)
	ON_COMMAND_EX_RANGE(IDC_COMBO1, IDC_BTN_STP_DOWN_SMALL, &MotorCalibrationDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDOK, &MotorCalibrationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MotorCalibrationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL MotorCalibrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ˢ�´����б�
	HKEY hTestKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hTestKey)) {
		QueryKey(hTestKey);
	}
	RegCloseKey(hTestKey);

	if (-1 == m_ComList[0]) {
		AfxMessageBox(_T("����û�д��ڣ�"));
	}
	int i = 0;
	while (i < 20 && -1 != m_ComList[i]) {
		CString sCom;
		sCom.Format(_T("COM%d"), m_ComList[i]);
		m_cb_com.InsertString(i, sCom.GetBuffer(5));
		i++;
	}
	m_cb_com.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// MotorCalibrationDlg ��Ϣ�������
void MotorCalibrationDlg::QueryKey(HKEY hKey) {
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
		AfxMessageBox(_T("����û�д���....."));
	}
	return;
}

void MotorCalibrationDlg::OnBnClickedBtnStart()
{
	int Port = m_cb_com.GetCurSel();
	if (Port == CB_ERR) {
		AfxMessageBox(_T("û��ѡ�д���1"));
		return;
	}
	Port = m_ComList[Port];

	if (!m_Com1.open(Port)) {
		AfxMessageBox(_T("�򿪴���1ʧ��"));
		return;
	}
	else {
		m_btn_start.SetWindowText("У׼ȷ��");
		m_Com1.write("adr5;ena;adr6;ena;adr7;ena;gstp1;gspd800;");
		//Sleep(3000);
		m_Com1.write("gstp-1;");
		bConnectFlag = true;
	}

	return;
}


void MotorCalibrationDlg::OnBnClickedOk()
{
	m_Com1.write("gorg;");
	AfxMessageBox(_T("У׼���"));
	m_Com1.close();
	CDialogEx::OnOK();
}


void MotorCalibrationDlg::OnBnClickedCancel()
{
	if(m_Com1.is_open())
		m_Com1.close();
	CDialogEx::OnCancel();
}

BOOL MotorCalibrationDlg::OnBnClickedBtnSend(UINT nID)
{
	string order = "";
	switch (nID) {
	case IDC_BTN_ADR1:
		order = "adr5;";
		break;
	case IDC_BTN_ADR2:
		order = "adr6;";
		break;
	case IDC_BTN_ADR3:
		order = "adr7;";
		break;
	case IDC_BTN_STP_DOWN_BIG:
		order = "stp50;";
		break;
	case IDC_BTN_STP_DOWN_MID:
		order = "stp5;";
		break;
	case IDC_BTN_STP_UP_BIG:
		order = "stp-50;";
		break;
	case IDC_BTN_STP_UP_MID:
		order = "stp-5;";
		break;
	case IDC_BTN_STP_UP_SMALL:
		order = "stp-1;";
		break;
	case IDC_BTN_STP_DOWN_SMALL:
		order = "stp1;";
		break;
	default:
		break;
	}

	if (bConnectFlag) {
		m_Com1.write((char*)order.c_str());
		return 0;
	}
	else {
		AfxMessageBox(_T("�����Ӵ���"));
		return 1;
	}
	return 0;
}

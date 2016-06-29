
// ChineseChess.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ChineseChess.h"
#include "ChineseChessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HBITMAP hBmp;

// CChineseChessApp

BEGIN_MESSAGE_MAP(CChineseChessApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChineseChessApp ����

CChineseChessApp::CChineseChessApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CChineseChessApp ����

CChineseChessApp theApp;


// CChineseChessApp ��ʼ��

BOOL CChineseChessApp::InitInstance()
{
	CWinApp::InitInstance();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("����"));

	if (!AfxSocketInit())
	{
		AfxMessageBox(2);
		return FALSE;
	}
	CChineseChessDlg dlg;
	MSG msg;
	m_pMainWnd = &dlg;
	BOOL nResponse = dlg.CreateEx( 0,
		AfxRegisterWndClass(0), "����",
		WS_POPUP | WS_SYSMENU,
		CRect(0, 0, 600, 573), NULL, NULL, NULL);

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMainWnd->SetIcon(hIcon, TRUE);
	m_pMainWnd->SetIcon(hIcon, FALSE);

	//���뱳��
	hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_BOARD), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);
	HRGN h = CreateRectRgn(0, 0, 600, 573);
	if (h)
		SetWindowRgn(m_pMainWnd->m_hWnd, h, TRUE);
	dlg.ShowWindow(SW_SHOW);
	dlg.UpdateWindow();

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


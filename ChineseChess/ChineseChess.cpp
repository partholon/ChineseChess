
// ChineseChess.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ChineseChess.h"
#include "ChineseChessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChineseChessApp

BEGIN_MESSAGE_MAP(CChineseChessApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChineseChessApp 构造

CChineseChessApp::CChineseChessApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CChineseChessApp 对象

CChineseChessApp theApp;


// CChineseChessApp 初始化

BOOL CChineseChessApp::InitInstance()
{
	CWinApp::InitInstance();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("跳棋"));

	if (!AfxSocketInit())
	{
		AfxMessageBox(2);
		return FALSE;
	}
	CChineseChessDlg dlg;
	MSG msg;
	m_pMainWnd = &dlg;
#if _DEBUG
	//全屏
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	dlg.chessBoard = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_BOARD), IMAGE_BITMAP, cx, cy, LR_CREATEDIBSECTION);
	BITMAP bmp;
	GetObject(dlg.chessBoard, sizeof(BITMAP), &bmp);//获取位图信息
	dlg.bmpHeight = cy;
	dlg.bmpWidth = cx;
#else
	//载入背景
	dlg.chessBoard = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_BOARD), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	BITMAP bmp;
	GetObject(dlg.chessBoard, sizeof(BITMAP), &bmp);//获取位图信息
	dlg.bmpHeight = bmp.bmHeight;
	dlg.bmpWidth = bmp.bmWidth;
#endif
	dlg.gChess.SetBorder(bmp.bmWidth,bmp.bmHeight);

	BOOL nResponse = dlg.CreateEx( 0,
		AfxRegisterWndClass(0), "跳棋",
		WS_POPUP,
		CRect(0, 0, dlg.bmpWidth, dlg.bmpHeight), NULL, NULL, NULL);

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMainWnd->SetIcon(hIcon, TRUE);
	m_pMainWnd->SetIcon(hIcon, FALSE);
	
	HRGN h = CreateRectRgn(0, 0, dlg.bmpWidth, dlg.bmpHeight);
	if (h)
		SetWindowRgn(m_pMainWnd->m_hWnd, h, TRUE);
	dlg.ShowWindow(SW_SHOW);
	//前端显示
	dlg.SetForegroundWindow();
	//窗口置顶
	dlg.SetWindowPos(m_pMainWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	dlg.UpdateWindow();

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


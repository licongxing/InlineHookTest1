
// InlineHookTest1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InlineHookTest1.h"
#include "InlineHookTest1Dlg.h"
#include "afxdialogex.h"
#include "InlineHook.h"
#include "InlineHook7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 实例化钩子对象
CInlineHook g_inlineHookObj;

CInlineHook7 g_inlineHookObj7; 

// WINAPI 一定要声明，指明函数调用方式，这里和MessageBox保持一致
int WINAPI MyMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	g_inlineHookObj.UnHook(); // 必须先卸载钩子 再才可以再次调用被Hook的函数，不然会进入死循环
	::MessageBoxW(hWnd ,_T("进入MyMessageBox了"),_T("被Hook了,5字节InlineHook"),MB_OK);
	::MessageBoxW(hWnd ,lpText,lpCaption,MB_OK);
	g_inlineHookObj.ReHook();
	return 0;
}

// WINAPI 一定要声明，指明函数调用方式，这里和MessageBox保持一致
int WINAPI MyMessageBox7(HWND   hWnd,LPCSTR lpText,	LPCSTR lpCaption,UINT   uType)
{
	g_inlineHookObj7.UnHook(); // 必须先卸载钩子 再才可以再次调用被Hook的函数，不然会进入死循环
	::MessageBoxA(hWnd ,"进入MyMessageBox7了","被Hook了,7字节InlineHook",MB_OK);
	::MessageBoxA(hWnd ,lpText,lpCaption,MB_OK);
	g_inlineHookObj7.ReHook();
	return 0;
}

// CInlineHookTest1Dlg 对话框

CInlineHookTest1Dlg::CInlineHookTest1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInlineHookTest1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInlineHookTest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInlineHookTest1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CInlineHookTest1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInlineHookTest1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInlineHookTest1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CInlineHookTest1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CInlineHookTest1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CInlineHookTest1Dlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CInlineHookTest1Dlg 消息处理程序

BOOL CInlineHookTest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 下钩子
	g_inlineHookObj.Hook("User32.dll","MessageBoxW",(FARPROC)MyMessageBox);

	// 下钩子 7字节
	g_inlineHookObj7.Hook("User32.dll","MessageBoxA",(FARPROC)MyMessageBox7);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInlineHookTest1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInlineHookTest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 弹出对话框 5字节
void CInlineHookTest1Dlg::OnBnClickedButton1()
{
	::MessageBoxW(AfxGetMainWnd()->m_hWnd,_T("我是MessMessageBoxW，测试5字节InlineHook"),_T("title"),MB_OK);
}

// 下钩子 5字节
void CInlineHookTest1Dlg::OnBnClickedButton2()
{
	g_inlineHookObj.ReHook();
}


// 卸载钩子 5字节
void CInlineHookTest1Dlg::OnBnClickedButton3()
{
	g_inlineHookObj.UnHook();
}

// 弹出对话框 7字节
void CInlineHookTest1Dlg::OnBnClickedButton5()
{
	::MessageBoxA(AfxGetMainWnd()->m_hWnd,"我是MessMessageBoxA，测试7字节InlineHook","title",MB_OK);
}

// 下钩子 7字节
void CInlineHookTest1Dlg::OnBnClickedButton6()
{
	g_inlineHookObj7.ReHook();
}

// 卸载钩子 7字节
void CInlineHookTest1Dlg::OnBnClickedButton7()
{
	g_inlineHookObj7.UnHook();
}

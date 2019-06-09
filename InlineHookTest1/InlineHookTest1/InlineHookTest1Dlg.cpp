
// InlineHookTest1Dlg.cpp : ʵ���ļ�
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

// ʵ�������Ӷ���
CInlineHook g_inlineHookObj;

CInlineHook7 g_inlineHookObj7; 

// WINAPI һ��Ҫ������ָ���������÷�ʽ�������MessageBox����һ��
int WINAPI MyMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	g_inlineHookObj.UnHook(); // ������ж�ع��� �ٲſ����ٴε��ñ�Hook�ĺ�������Ȼ�������ѭ��
	::MessageBoxW(hWnd ,_T("����MyMessageBox��"),_T("��Hook��,5�ֽ�InlineHook"),MB_OK);
	::MessageBoxW(hWnd ,lpText,lpCaption,MB_OK);
	g_inlineHookObj.ReHook();
	return 0;
}

// WINAPI һ��Ҫ������ָ���������÷�ʽ�������MessageBox����һ��
int WINAPI MyMessageBox7(HWND   hWnd,LPCSTR lpText,	LPCSTR lpCaption,UINT   uType)
{
	g_inlineHookObj7.UnHook(); // ������ж�ع��� �ٲſ����ٴε��ñ�Hook�ĺ�������Ȼ�������ѭ��
	::MessageBoxA(hWnd ,"����MyMessageBox7��","��Hook��,7�ֽ�InlineHook",MB_OK);
	::MessageBoxA(hWnd ,lpText,lpCaption,MB_OK);
	g_inlineHookObj7.ReHook();
	return 0;
}

// CInlineHookTest1Dlg �Ի���

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


// CInlineHookTest1Dlg ��Ϣ�������

BOOL CInlineHookTest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// �¹���
	g_inlineHookObj.Hook("User32.dll","MessageBoxW",(FARPROC)MyMessageBox);

	// �¹��� 7�ֽ�
	g_inlineHookObj7.Hook("User32.dll","MessageBoxA",(FARPROC)MyMessageBox7);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInlineHookTest1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CInlineHookTest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// �����Ի��� 5�ֽ�
void CInlineHookTest1Dlg::OnBnClickedButton1()
{
	::MessageBoxW(AfxGetMainWnd()->m_hWnd,_T("����MessMessageBoxW������5�ֽ�InlineHook"),_T("title"),MB_OK);
}

// �¹��� 5�ֽ�
void CInlineHookTest1Dlg::OnBnClickedButton2()
{
	g_inlineHookObj.ReHook();
}


// ж�ع��� 5�ֽ�
void CInlineHookTest1Dlg::OnBnClickedButton3()
{
	g_inlineHookObj.UnHook();
}

// �����Ի��� 7�ֽ�
void CInlineHookTest1Dlg::OnBnClickedButton5()
{
	::MessageBoxA(AfxGetMainWnd()->m_hWnd,"����MessMessageBoxA������7�ֽ�InlineHook","title",MB_OK);
}

// �¹��� 7�ֽ�
void CInlineHookTest1Dlg::OnBnClickedButton6()
{
	g_inlineHookObj7.ReHook();
}

// ж�ع��� 7�ֽ�
void CInlineHookTest1Dlg::OnBnClickedButton7()
{
	g_inlineHookObj7.UnHook();
}

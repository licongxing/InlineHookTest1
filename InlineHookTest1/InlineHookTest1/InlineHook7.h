#pragma once

#include <Windows.h>

class CInlineHook7
{
public:
	CInlineHook7(void);
	~CInlineHook7(void);

	bool Hook(LPSTR strModuleName,LPSTR strHookFnName,FARPROC strHookCallFnName);

	bool UnHook();
	bool ReHook();
private:
	
	FARPROC m_pFnOrign; // ҪHook�ĺ�����ַ
	BYTE m_bOld[7]; // ҪHook�ĺ��� ǰ7���ֽ�
	BYTE m_bNew[7]; // ҪHook�ĺ��� �޸ĺ��7���ֽ�
};


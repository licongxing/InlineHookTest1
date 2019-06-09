#pragma once

#include <Windows.h>

class CInlineHook
{
public:
	CInlineHook(void);
	~CInlineHook(void);

	bool Hook(LPSTR strModuleName,LPSTR strHookFnName,FARPROC strHookCallFnName);

	bool UnHook();
	bool ReHook();
private:
	
	FARPROC m_pFnOrign; // ҪHook�ĺ�����ַ
	BYTE m_bOld[5]; // ҪHook�ĺ��� ǰ5���ֽ�
	BYTE m_bNew[5]; // ҪHook�ĺ��� �޸ĺ��5���ֽ�
};


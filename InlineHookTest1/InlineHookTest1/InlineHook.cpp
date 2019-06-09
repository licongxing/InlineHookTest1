#include "StdAfx.h"
#include "InlineHook.h"


CInlineHook::CInlineHook(void)
{
	m_pFnOrign = NULL;
	memset(m_bOld,0,5);
	memset(m_bNew,0,5);
}


CInlineHook::~CInlineHook(void)
{
	UnHook();
}

bool CInlineHook::Hook(LPSTR strModuleName,LPSTR strHookFnName,FARPROC strTargetFnAddr)
{
	bool ret = false;
	HMODULE hModule = GetModuleHandleA(strModuleName);
	if( hModule == NULL )
		goto end;
	m_pFnOrign = (FARPROC)GetProcAddress(hModule,strHookFnName);
	if( m_pFnOrign == NULL )
		goto end;

	SIZE_T numByte;
	// ���汻Hook������ǰ5���ֽ�
	if( ReadProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bOld,5,&numByte) == 0 )
		goto end;

	// ����jmpָ��:jmp Ŀ���ַ����Ӧ�Ļ�����:e9 4�ֽڵ�ƫ����
	m_bNew[0] = 0xe9;
	// ʣ��4�ֽ� ��ƫ����
	* (DWORD*)(&m_bNew[0]+1) = (DWORD)strTargetFnAddr - (DWORD)m_pFnOrign -5;
	
	// �޸ı�Hook������ǰ5���ֽ� �ı���ִ������
	if( WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bNew,5,&numByte) == 0 ) 
		goto end;

end:
	return ret;
}

bool CInlineHook::UnHook()
{
	bool ret = false;
	// ж�ع��ӣ�ʵ���ϼ��ǽ����ĵ�5���ֽڻ�ԭ
	SIZE_T numByte;
	if( m_pFnOrign != NULL && WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bOld,5,&numByte) != 0)
		ret = true;
	return ret;
}

bool CInlineHook::ReHook()
{
	bool ret = true;
	// �ٴ�װ���ӣ�����ҪHook�ĺ�����ǰ5�ֽ� ������ת��Ŀ�꺯��
	SIZE_T numByte;
	if( m_pFnOrign != NULL && WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bNew,5,&numByte) != 0)
		ret = true;
	return ret;
}

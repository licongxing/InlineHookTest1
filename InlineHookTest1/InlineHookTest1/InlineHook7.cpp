#include "StdAfx.h"
#include "InlineHook7.h"


CInlineHook7::CInlineHook7(void)
{

	m_pFnOrign = NULL;
	memset(m_bOld,0,7);
	memset(m_bNew,0,7);
}


CInlineHook7::~CInlineHook7(void)
{
	UnHook();
}

bool CInlineHook7::Hook(LPSTR strModuleName,LPSTR strHookFnName,FARPROC strTargetFnAddr)
{
	bool ret = false;
	HMODULE hModule = GetModuleHandleA(strModuleName);
	if( hModule == NULL )
		goto end;
	m_pFnOrign = (FARPROC)GetProcAddress(hModule,strHookFnName);
	if( m_pFnOrign == NULL )
		goto end;

	SIZE_T numByte;
	// ���汻Hook������ǰ7���ֽ�
	if( ReadProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bOld,7,&numByte) == 0 )
		goto end;

	// ����jmpָ��:mov eax,jmp Ŀ���ַ����Ӧ�Ļ�����:B8 4�ֽڵ�Ŀ���ַ FFE0
	m_bNew[0] = 0xB8,m_bNew[5] = 0xFF,m_bNew[6] = 0xE0;
	// �м�4�ֽ� ��Ŀ���ַ
	* (DWORD*)(&m_bNew[0]+1) = (DWORD)strTargetFnAddr;
	
	// �޸ı�Hook������ǰ7���ֽ� �ı���ִ������
	if( WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bNew,7,&numByte) == 0 ) 
		goto end;

	ret = true;
end:
	return ret;
}

bool CInlineHook7::UnHook()
{
	bool ret = false;
	// ж�ع��ӣ�ʵ���ϼ��ǽ����ĵ�7���ֽڻ�ԭ
	SIZE_T numByte;
	if( m_pFnOrign != NULL && WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bOld,7,&numByte) != 0)
		ret = true;
	return ret;
}

bool CInlineHook7::ReHook()
{
	bool ret = false;
	// �ٴ�װ���ӣ�����ҪHook�ĺ�����ǰ7�ֽ� ������ת��Ŀ�꺯��
	SIZE_T numByte;
	if( m_pFnOrign != NULL && WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bNew,7,&numByte) != 0)
		ret = true;
	return ret;
}

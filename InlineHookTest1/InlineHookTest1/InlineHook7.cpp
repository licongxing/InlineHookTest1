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
	// 保存被Hook函数的前7个字节
	if( ReadProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bOld,7,&numByte) == 0 )
		goto end;

	// 构造jmp指令:mov eax,jmp 目标地址，对应的机器码:B8 4字节的目标地址 FFE0
	m_bNew[0] = 0xB8,m_bNew[5] = 0xFF,m_bNew[6] = 0xE0;
	// 中间4字节 放目标地址
	* (DWORD*)(&m_bNew[0]+1) = (DWORD)strTargetFnAddr;
	
	// 修改被Hook函数的前7个字节 改变其执行流程
	if( WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bNew,7,&numByte) == 0 ) 
		goto end;

	ret = true;
end:
	return ret;
}

bool CInlineHook7::UnHook()
{
	bool ret = false;
	// 卸载钩子，实际上即是将更改的7个字节还原
	SIZE_T numByte;
	if( m_pFnOrign != NULL && WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bOld,7,&numByte) != 0)
		ret = true;
	return ret;
}

bool CInlineHook7::ReHook()
{
	bool ret = false;
	// 再次装钩子，更改要Hook的函数的前7字节 让其跳转到目标函数
	SIZE_T numByte;
	if( m_pFnOrign != NULL && WriteProcessMemory(GetCurrentProcess(),m_pFnOrign,m_bNew,7,&numByte) != 0)
		ret = true;
	return ret;
}

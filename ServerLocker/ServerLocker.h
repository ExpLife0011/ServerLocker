
// ServerLockToolsPs.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServerLockToolsPsApp: 
// �йش����ʵ�֣������ ServerLockToolsPs.cpp
//

class CServerLockToolsPsApp : public CWinApp
{
public:
	CServerLockToolsPsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerLockToolsPsApp theApp;
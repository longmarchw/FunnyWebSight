
// FunnyWS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFunnyWSApp:
// �йش����ʵ�֣������ FunnyWS.cpp
//

class CFunnyWSApp : public CWinApp
{
public:
	CFunnyWSApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFunnyWSApp theApp;
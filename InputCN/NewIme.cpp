#include "stdafx.h"
#include "resource.h"
#include "NewIme.h"
#include "InputWnd.h"
#include "StatusWnd.h"
#include "Global.h"
#include "HandleDic.h"
#include "Assistance.h"
#include "config.h"
/************************************************************************/
/*
/* ��������
/*                                                                      */
/************************************************************************/
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	StatusWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT	CALLBACK	CompWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK	CandWndProc(HWND,UINT,WPARAM,LPARAM);

//////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	RegisterHotKey(hStatus,GlobalAddAtom("SHIFT"),MOD_SHIFT,NULL);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//�ͷŹ���
	UnInstallHook();
	return msg.wParam;
}

/************************************************************************/
/* 
/*  MyRegisterClass()	: ע�ᴰ����
/*                                                                     */
/************************************************************************/

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//
	// ע��״̬����
	//
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)StatusWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= STATUSCLASS;
	wcex.hIconSm		= NULL;

	if(!RegisterClassEx(&wcex))
		return FALSE;

	//
	// ע�����봰��
	//
	WNDCLASSEX wcex2;
	
	wcex2.cbSize = sizeof(WNDCLASSEX); 
	
	wcex2.style			= CS_HREDRAW | CS_VREDRAW;
	wcex2.lpfnWndProc	= (WNDPROC)InputWndProc;
	wcex2.cbClsExtra		= 0;
	wcex2.cbWndExtra		= 0;
	wcex2.hInstance		= hInstance;
	wcex2.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NEWIME);
	wcex2.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex2.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex2.lpszMenuName	= NULL;
	wcex2.lpszClassName	= INPUTCLASS;
	wcex2.hIconSm		= NULL;

	if(!RegisterClassEx(&wcex2))
		return FALSE;

	return TRUE;
}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;


   //���������ļ�
   LoadConfig();
 
  
   if( !CreateStatus(hInstance))			//����״̬����
   {
	   return FALSE;
   }
   
   if (!CreateInputWnd(hInstance))			//�������봰��
   {
		return FALSE;
   }
   ShowWindow(hInput,SW_HIDE);				//��ʼ���������봰��

   InitConfig();
   //////////////////////////////////////////////////////////////////////////
   //װ��ƴ����Ϣ
   if (!InitPYInfo())
   {
	   return FALSE;
   }

   // ���
   if (!InitWBInfo())
   {
	   return FALSE;
   }

   // ����
   if (!InitCZInfo())
   {
	   return FALSE;
   }

   //Ӣ��
   if (!InitEngInfo())
   {
	   return FALSE;
   }

   //�ʻ�
   if (!InitBHInfo())
   {
	   return FALSE;
   }

   //////////////////////////////////////////////////////////////////////////
   //��װ����
   InstallHook();

   
   
   CurMode=MODE_FULL;

   INPUTWNDSTRUCT.sz.cx=400;
   INPUTWNDSTRUCT.sz.cy=42;

   CANDLIST.nPageSize=7;

   //��ʼ������״̬
   KeyState|=HANDLE_CHAR;
   KeyState|=HANDLE_SMALL_NUM;
   //��ʼ�������б�
   COMPLIST.nCurPos=0;
   COMPLIST.nLen=0;

   AssPrint("��ʼ���ɹ�!");

   ShowWindow(hStatus,SW_SHOWNA);			//��ʾ״̬����	
   UpdateWindow(hStatus);

   return TRUE;
}
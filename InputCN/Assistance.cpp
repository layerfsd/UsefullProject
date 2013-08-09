#include "StdAfx.h"
#include <stdio.h>
#include <winable.h>
/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
void AssPrint(char * pszFormat,...)
{
	va_list args; 
	char buf[512];
	va_start( args, pszFormat );
	_vsnprintf(buf,sizeof(buf)-1, pszFormat,args);
	va_end (args);
	buf[sizeof(buf)-1]=0;
	MessageBox(NULL,buf,"TRACE",MB_OK);
}

/************************************************************************/
/* 
/* GetFrontWnd()	: ��ȡӵ�е�ǰ���Ĵ��ھ��
/*																		*/
/************************************************************************/

BOOL GetFrontWnd(HWND *phWnd)
{
	GUITHREADINFO pg;
	pg.cbSize=48;
	GetGUIThreadInfo(NULL,&pg);
	*phWnd=pg.hwndCaret;
	if (pg.hwndCaret)
	{	
		return TRUE;
	}
	else
	{
		*phWnd=NULL;
		AssPrint("��ȡ��ǰ������!");
		return FALSE;
	}
}

/************************************************************************/
/* 
/* GetCaretPoss()	:��ȡ��ǰ����λ��
/*                                                                     */
/************************************************************************/
BOOL GetCaretPoss(LPPOINT lpPtCaret)
{
	POINT ptCaret;
	GUITHREADINFO pg;
	pg.cbSize=48;
	GetGUIThreadInfo(NULL,&pg);
	
	if (pg.hwndCaret)	
	{
		ptCaret.x=pg.rcCaret.right;
		ptCaret.y=pg.rcCaret.bottom;
		ClientToScreen(pg.hwndCaret,&ptCaret);
		lpPtCaret->x=ptCaret.x;
		lpPtCaret->y=ptCaret.y;
		return TRUE;
	}
	//��ȡʧ��
	else
	{
		return FALSE;
	}	
}
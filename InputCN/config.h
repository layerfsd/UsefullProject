
#pragma once

#define WIN32_LEAN_AND_MEAN		

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
 
#include "resource.h"
#include <commctrl.h>
#include <Commdlg.h>

//////////////////////////////////////////////////////////////////////////
#pragma comment (lib,"comctl32.lib")
#pragma  comment (lib,"Gdi32.lib")
//////////////////////////////////////////////////////////////////////////
 
#define LEFT_VALUE	0x01
#define RIGHT_VALUE	0x02
#define PAGE1		0x01
#define PAGE2		0x02
#define PAGE3		0x04
#define PAGE4		0x08


//�������ҳ
typedef struct _tagSetView
{
	HFONT		hFont;					//������Ϣ
	COLORREF	rgbFontColor;			//������ɫ
	COLORREF	regBackColor;			//������ɫ
	int			nTransparent;			//͸����
	BOOL		bSkin;					//ʹ��Ƥ��
	
	char	bmpStatus[80];
	char	bmpChar[80];
	char	bmpInput[80];
	char	bmpEng[80];
	
}SETVIEWSTRUCT,*LPSETVIEWSTRUCT;

//�ȼ�����ҳ
typedef struct _tagSetHotKey
{
	DWORD	dwControl;
	DWORD	dwShift;
	DWORD	dwPage;

}SETHOTKEYSTRUCT;
//////////////////////////////////////////////////////////////////////////
//	SETTING
extern	SETVIEWSTRUCT	SETVIEW;		//�������	
extern	SETHOTKEYSTRUCT	SETKEY;
//////////////////////////////////////////////////////////////////////////
int DoPropertySheet(HWND hWnd);
BOOL WINAPI Config_setkey(HWND hWnd,UINT uMsg, WPARAM wParam,LPARAM lParam);	// ��̨���ã������ȼ��ĺ���
BOOL WINAPI Config_setusual(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);	//��̨���ã����ó��湦�ܵĺ���
BOOL WINAPI Config_setview(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);	// ��̨���ã�������۵Ĺ��ܺ���
BOOL WINAPI Config_setciku(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);	//��̨���ã� ���ôʿ⹦�ܵĺ���

void LoadConfig();
void InitConfig();

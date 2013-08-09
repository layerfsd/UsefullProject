#include "StdAfx.h"
#include "Global.h"
#include "NewIme.h"
#include "Assistance.h"
#include "config.h"
#include "InputWnd.h"

#pragma comment (lib,"Gdi32.lib")

#define BORDER_WIDTH 4
#define COMPWIDTH	400
#define COMPHEIGHT	42
#define WS_EX_LAYOUTED 0x80000			//͸����������
#define LWA_ALPHA      0x02		
//////////////////////////////////////////////////////////////////////////
// 
// �ֲ���������
//
//////////////////////////////////////////////////////////////////////////
void DrawInputBorder(LPRECT);
void Input_OnSetcursor(HWND hWnd,WPARAM wParam,LPARAM lParam);
void Input_OnLButtonUp(HWND hWnd,WPARAM wParam,LPARAM lParam);
void Input_OnMouseMove(HWND hWnd,WPARAM wParam,LPARAM lParam);

//////////////////////////////////////////////////////////////////////////

static RECT		drc;
static POINT	ptdif;
static SIZE		sz;
static BOOL		fCanMove=FALSE;

static HBITMAP		hbmpInput;		//���봰�ڱ���ͼƬ
static HDC			hMemDC;			//���봰���ڴ�DC

typedef   BOOL   (WINAPI   *TRANSWINDOW)(HWND,COLORREF,BYTE,DWORD); 
TRANSWINDOW			TransparentWnd=NULL;	//ָ��SetLayeredWindowAttributes������ָ��
/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
void Input_OnPaint(HWND hWnd,HDC hdc)
{
	RECT ClientRect;
	static HPEN hpInput=NULL;
	static HBRUSH hbruInput=NULL;


	if (CurMode & MODE_ENGLIS) //Ӣ������ģʽ����
	{
		hpInput=CreatePen(PS_SOLID,1,RGB(0,0,180));
		SelectObject(hdc,hpInput);
	
		hbruInput=CreateSolidBrush(SETVIEW.regBackColor);
		SelectObject(hdc,hbruInput);

		GetClientRect(hWnd,&ClientRect);
		Rectangle(hdc,ClientRect.left,ClientRect.top,ClientRect.right,ClientRect.bottom);
		

		MoveToEx(hdc,5,25,NULL);
		LineTo(hdc,ClientRect.right-5,25);

		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,SETVIEW.hFont);
		SetTextColor(hdc,SETVIEW.rgbFontColor);
		//////////////////////////////
		//�������ͺ�ѡ��
		TextOut(hdc,5,5,COMPLIST.Buffer,COMPLIST.nLen);

		for (int i=0;i<ENGINFO.nListLen;i++)
		{
			TextOut(hdc,5,30+i*20,ENGINFO.strList[i],strlen(ENGINFO.strList[i]));
		}

		DeleteObject(hpInput);
		DeleteObject(hbruInput);
	}
	else  //��������ģʽ����  
	{
		if (SETVIEW.bSkin)
		{
			GetClientRect(hWnd,&ClientRect);

			StretchBlt(hdc,0,0,ClientRect.right-ClientRect.left,ClientRect.bottom-ClientRect.top,
						hMemDC,0,0,300,42,SRCCOPY);

		//	BitBlt(hdc,0,0,COMPWIDTH,COMPHEIGHT,hMemDC,0,0,SRCCOPY);
		}
		else
		{
			if (!hpInput)
			{
				hpInput=CreatePen(PS_SOLID,1,RGB(0,0,180));
			}
			if (!hbruInput)
			{
				hbruInput=CreateSolidBrush(SETVIEW.regBackColor);
			}
			GetClientRect(hWnd,&ClientRect);
		
			FillRect(hdc,&ClientRect,hbruInput);
			Rectangle(hdc,ClientRect.left,ClientRect.top,ClientRect.right,ClientRect.bottom);
			SelectObject(hdc,hpInput);
			MoveToEx(hdc,5,21,NULL);
			LineTo(hdc,ClientRect.right-5,21);
		}
		
		/*  ��������ģʽ */
		SelectObject(hdc,SETVIEW.hFont);
		
		// ����������ɫ
		
		SetTextColor(hdc,SETVIEW.rgbFontColor);
		
		SetBkMode(hdc,TRANSPARENT);
		
		TextOut(hdc,10,5,COMPLIST.Buffer,COMPLIST.nLen);
		TextOut(hdc,10,24,CANDLIST.Buff,CANDLIST.nLen);
	}

}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void Input_OnCreate(HWND hWnd)
{
	HDC hdc=GetDC(hWnd);


	hbmpInput=(HBITMAP)LoadImage(hInst,SETVIEW.bmpInput,IMAGE_BITMAP,300,42,LR_SHARED|LR_LOADFROMFILE);
	if (!hbmpInput)
	{
		hbmpInput	=LoadBitmap(hInst,(LPSTR)IDB_BITCOMP);
	}
	hMemDC		=CreateCompatibleDC(hdc);
	SelectObject(hMemDC,hbmpInput);
}
/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void Input_OnDestroy(HWND hWnd)
{
	DeleteObject(hbmpInput);
	ReleaseDC(hWnd,hMemDC);
}
/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void Input_OnNCPaint(HWND hWnd,HDC hdc)
{
	RECT rect;
	GetClientRect(hWnd,&rect);
	rect.bottom+=5;
	rect.right+=5;
	FillRect(hdc,&rect,CreateSolidBrush(RGB(180,180,180)));
}

/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
void Input_OnSetcursor(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	POINT pt;
	HDC hdc;
    BOOL Lbuttondown=FALSE;
	hdc=GetDC(hWnd);
	if ( HIWORD(lParam) == WM_LBUTTONDOWN )
	{
		BOOL Lbuttondown=TRUE;
		SetCapture(hWnd);	
		GetCursorPos(&pt);
		GetWindowRect(hWnd,&drc);
		
		ptdif.x = pt.x - drc.left;		//�������������������Ͻ�x����ľ���
		ptdif.y = pt.y - drc.top;		//�����������������Ͻ�y����ľ���
								
		sz.cx = drc.right - drc.left;	//���ο�ȴ�С
		sz.cy = drc.bottom - drc.top;	//���θ߶ȴ�С 
		
		RECT	m_drc = drc;
		DrawInputBorder(&m_drc);
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		fCanMove = TRUE;			
	}
	
	else  if (HIWORD(lParam) == WM_RBUTTONDOWN )
	{
		SetCapture(hWnd);		
	}	
}
/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void DrawInputBorder(LPRECT lprc)
{
    HDC hDC;
    int sbx, sby;
    hDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
    SelectObject( hDC, GetStockObject( GRAY_BRUSH ));
    sbx = GetSystemMetrics( SM_CXBORDER );
    sby = GetSystemMetrics( SM_CYBORDER );
    PatBlt( hDC, lprc->left,lprc->top, lprc->right-lprc->left-sbx, sby, PATINVERT );
	PatBlt( hDC, lprc->right - sbx, lprc->top, sbx, lprc->bottom - lprc->top-sby, PATINVERT );
    PatBlt( hDC, lprc->right, lprc->bottom-sby, -(lprc->right - lprc->left-sbx), sby, PATINVERT );
    PatBlt( hDC, lprc->left, lprc->bottom, 	sbx, -(lprc->bottom - lprc->top-sby), PATINVERT );
    DeleteDC( hDC );
}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void Input_OnLButtonUp(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
    hdc = GetDC(hWnd);
	POINT pt;
	ReleaseCapture();
	
	if( fCanMove )
	{
		DrawInputBorder(&drc);
		GetCursorPos( &pt );
		MoveWindow(hWnd,pt.x - ptdif.x, pt.y - ptdif.y, sz.cx, sz.cy,TRUE);	
	}
	fCanMove = FALSE;
}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void Input_OnMouseMove(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	POINT pt;
	if( fCanMove )
	{
		GetCursorPos( &pt );
		DrawInputBorder(&drc);
		drc.left   = pt.x - ptdif.x;
		drc.top    = pt.y - ptdif.y;
		drc.right  = drc.left + sz.cx;
		drc.bottom = drc.top + sz.cy;
		DrawInputBorder(&drc);
	}
}
/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
LRESULT CALLBACK InputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Input_OnPaint(hWnd,ps.hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		{
			Input_OnDestroy(hWnd);
			PostQuitMessage(0);
		}
		break;
	/*
	case WM_NCPAINT:
		{
			hdc=GetDC(NULL);
			Input_OnNCPaint(hWnd,hdc);
			ReleaseDC(hWnd,hdc);
		}
		break;
	*/
	case WM_CREATE:
		{
			Input_OnCreate(hWnd);
		}
		break;
	case WM_SETCURSOR:
		{
			Input_OnSetcursor(hWnd,wParam,lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			Input_OnLButtonUp(hWnd,wParam,lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			Input_OnMouseMove(hWnd,wParam,lParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
BOOL CreateInputWnd(HINSTANCE hInstance)
{
	hInput=CreateWindowEx(
						WS_EX_TOOLWINDOW|WS_EX_TOPMOST,
						INPUTCLASS,
						NULL,
						WS_DISABLED|WS_POPUP,
						10,
						10,
						COMPWIDTH,
						COMPHEIGHT,
						NULL,
						(HMENU)NULL,
						hInstance,
						NULL);

	if (!hInput)
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 
/*	UpdateInputWnd()	: �������봰��
/*                                                                      */
/************************************************************************/

void UpdateInputWnd()
{
	if (INPUTWNDSTRUCT.bShow)
	{
		if ( !(CurMode&MODE_ENGLIS))			//����ģʽ�Ĵ���
		{
			HDC hTmpDC=GetDC(hInput);
			SelectObject(hTmpDC,SETVIEW.hFont);
			GetTextExtentPoint32(hTmpDC,CANDLIST.Buff,CANDLIST.nLen,&INPUTWNDSTRUCT.sz);
			INPUTWNDSTRUCT.sz.cx+=10;
			INPUTWNDSTRUCT.sz.cy=42;
			ReleaseDC(hInput,hTmpDC);
			if (INPUTWNDSTRUCT.sz.cx <=100 )
			{
				INPUTWNDSTRUCT.sz.cx=150;
			}
		}
	
		if(GetCaretPoss(&INPUTWNDSTRUCT.pos))
		{
		/*  ��鳬����Ļ��� */
		SetWindowPos(hInput,NULL,
					INPUTWNDSTRUCT.pos.x,
					INPUTWNDSTRUCT.pos.y,
					INPUTWNDSTRUCT.sz.cx,
					INPUTWNDSTRUCT.sz.cy,
					SWP_NOACTIVATE|SWP_NOZORDER);
		}
		ShowWindow(hInput,SW_SHOWNA);
		//�����������
		HDC hdc=GetDC(hInput);
		Input_OnPaint(hInput,hdc);
		ReleaseDC(hInput,hdc);
		
	}
	else
	{
		ShowWindow(hInput,SW_HIDE);
	}
}


/************************************************************************/
/* 
/* �������봰�ڵ�͸����
/*																		*/
/************************************************************************/
void TranspanentWindow(int nOptVal)
{
	if (!TransparentWnd)
	{
		HINSTANCE hUser32=LoadLibrary("User32.dll");
		SetWindowLong(hInput,GWL_EXSTYLE,GetWindowLong(hInput,GWL_EXSTYLE)|WS_EX_LAYOUTED);
		TransparentWnd=(TRANSWINDOW)GetProcAddress(hUser32,"SetLayeredWindowAttributes");
		if (!TransparentWnd)
		{
			AssPrint("װ��SetLayeredWindowAttributes ��������!");
		}
	}

	TransparentWnd(hInput,0,nOptVal*255/100,LWA_ALPHA);
}

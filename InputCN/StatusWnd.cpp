#include "StdAfx.h"
#include "Global.h"
#include "HandleMsg.h"
#include "Assistance.h"
#include "resource.h"
#include "StatusWnd.h"
#include "NewIme.h"
#include "config.h"
#include "HandleDic.h"
#include "StatusWnd.h"
#include "InputWnd.h"

#include <shellapi.h>
#pragma comment(lib,"shell32.lib")
//////////////////////////////////////////////////////////////////////////
static RECT		drc;
static POINT	ptdif;
static SIZE		sz;
static BOOL		fCanMove=FALSE;
static BOOL		bPopMenu=FALSE;

//////////////////////////////////////////////////////////////////////////
void	Status_OnKeyDown(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

void	DrawUIBorder(LPRECT lprc);
void	Status_OnCaptureMouse(HWND hWnd,WPARAM wParam,LPARAM lParam);
void	Status_OnMouseMove();
void	Status_OnLButtonUp(HWND hWnd,WPARAM wParam,LPARAM lParam);
void	Status_OnRButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK  KeyBoardProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*
/* OnKeyDown()	: ���յ�HOOKDLL ���͵ļ�����Ϣ������ת��Ϊ�ַ����Ҹ��µ�
/*				  ���봰��.
/*                                                                      */
/************************************************************************/
void Status_OnKeyDown(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (!HandleMsg(wParam,lParam))		//�ð����̲���Ҫ����
	{
		return ;
	}
    
	if (wParam == VK_BACK)
	{
		HandleBack();
	}
	else if (wParam == VK_SPACE)
	{
		HandleSpace();
	}
	//С����
	else if (wParam >= VK_NUMPAD4 && wParam <= VK_NUMPAD9 && wParam != VK_NUMPAD6)
	{
		 Handle_Small_Num(wParam);
	}
	// 0 - 9
	else if (wParam >= 0x30 && wParam <= 0x39)
	{
		HandleNum(wParam,lParam);
	}
	// A - Z
	else if (wParam >= 0x41 && wParam <= 0x5A)
	{
		HandleChar(wParam,lParam);
	}

	else if (wParam == VK_CONTROL)
	{
		HandleControl();
	}
	//�Ϸ�ҳ
	else if (wParam == VK_LEFT)
	{
	}
	//�·�
	else if (wParam == VK_RIGHT)
	{
	}
}


/************************************************************************/
/* 
/*	״̬������ʾ����
/*                                                                      */
/************************************************************************/

void Status_OnPaint(HWND hWnd,HDC hdc)
{
	HBITMAP hbmpBackBround,hbmpChar;

	hbmpBackBround=(HBITMAP)LoadImage(hInst,SETVIEW.bmpStatus,IMAGE_BITMAP,160,20,LR_LOADFROMFILE|LR_SHARED);

	hbmpChar=(HBITMAP)LoadImage(hInst,SETVIEW.bmpChar,IMAGE_BITMAP,360,40,LR_LOADFROMFILE|LR_SHARED);
	
	HDC hMemDC=CreateCompatibleDC(hdc);
	HDC hPicDC=CreateCompatibleDC(hdc);
	
	SelectObject(hMemDC,hbmpBackBround);
	SelectObject(hPicDC,hbmpChar);

	if (dwType & TYPE_FULL ) //��ʾȫģʽ
	{
		BitBlt(hMemDC,20,0,40,20,hPicDC,0,0,SRCCOPY);
	}

	else if (dwType & TYPE_ENG) //Ӣ��ģʽ
	{
		BitBlt(hMemDC,20,0,40,20,hPicDC,120,0,SRCCOPY);
	}

	else if (dwType & TYPE_WUBI) //���
	{
		BitBlt(hMemDC,20,0,40,20,hPicDC,80,0,SRCCOPY);
	}
	else if (dwType & TYPE_BIHUA) //�ʻ�
	{
		BitBlt(hMemDC,20,0,40,20,hPicDC,160,0,SRCCOPY);
	}
	else if (dwType & TYPE_PINYIN) //ƴ��
	{
		BitBlt(hMemDC,20,0,40,20,hPicDC,40,0,SRCCOPY);
	}


	if (dwType & TYPE_ZHONGWEN)	//���� 
	{
		BitBlt(hMemDC,60,0,20,20,hPicDC,200,0,SRCCOPY);
	}
	else if (dwType & TYPE_YINGYU) //Ӣ�� 
	{
		BitBlt(hMemDC,60,0,20,20,hPicDC,220,0,SRCCOPY);
	}


	if (dwType & TYPE_CHUANGJIAO) //ȫ��
	{
		BitBlt(hMemDC,80,0,20,20,hPicDC,260,0,SRCCOPY);
	}
	else if(dwType & TYPE_BANJIAO) //���
	{
		BitBlt(hMemDC,80,0,20,20,hPicDC,240,0,SRCCOPY);
	}



	if (dwType & TYPE_BIAO1) //ȫ�Ǳ��
	{
		BitBlt(hMemDC,100,0,20,20,hPicDC,280,0,SRCCOPY);
	}
	else if (dwType & TYPE_BIAO2)//��Ǳ��
	{
		BitBlt(hMemDC,100,0,20,20,hPicDC,300,0,SRCCOPY);
	}

	// �����
	BitBlt(hMemDC,120,0,20,20,hPicDC,320,0,SRCCOPY);
	//����
	BitBlt(hMemDC,140,0,20,20,hPicDC,340,0,SRCCOPY);

	BitBlt(hdc,0,0,160,20,hMemDC,0,0,SRCCOPY);
	ReleaseDC(hWnd,hMemDC);
	ReleaseDC(hWnd,hPicDC);
	DeleteObject(hbmpBackBround);
	DeleteObject(hbmpChar);
}
/************************************************************************/
/* 
/* OnRButtonDown()	:�Ҽ������˵�
/*                                                                     */
/************************************************************************/

void Status_OnRButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HMENU	hMenu=LoadMenu(hInst,(LPCSTR)IDR_MENU);
	HMENU	hSubMenu=GetSubMenu(hMenu,0);
	int		cmd;
	POINT	pt;
	GetCursorPos(&pt);

	cmd=TrackPopupMenu(hSubMenu,TPM_LEFTBUTTON|TPM_RETURNCMD,pt.x,pt.y,0,hWnd,NULL);
	//�ж�ѡ���Ǹ��˵�
	switch(cmd)
	{
	case IDM_SETTING:
		DoPropertySheet(hWnd);
		break;
	case IDM_EXIT:
		{
			AssPrint("Select Exit ");
			UnInstallHook();
			ExitProcess(0);
		}
		break;
	case IDM_HELP:
		{
			ShellExecute(hWnd,"open","help.chm",NULL,NULL,SW_NORMAL);
		}
		break;
	default:
		break;	
	}
}

/************************************************************************/
/* 
/* ״̬���ڳ�ʼ��
/*                                                                     */
/************************************************************************/

void Status_OnCreate()
{
	dwType|=TYPE_FULL;			//���
	dwType|=TYPE_ZHONGWEN;		//���� 
	dwType|=TYPE_CHUANGJIAO;	//ȫ��
	dwType|=TYPE_BIAO1;			//���1
}

/************************************************************************/
/*
/* ����Ļ�ϻ���״̬�����ƶ�ʱ������
/*                                                                      */
/************************************************************************/

void DrawUIBorder(LPRECT lprc)
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
/* ״̬���ڴ�������¼�����
/*                                                                      */
/************************************************************************/
void Status_OnCaptureMouse(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	POINT	pt;
	RECT	rcInner;
	int		Offest;

	if ( HIWORD(lParam) == WM_LBUTTONDOWN )
	{
		
		SetCapture(hWnd);	
		GetCursorPos(&pt);
		GetWindowRect(hWnd,&drc);
		

		rcInner.left = drc.left;
		rcInner.top=drc.top;
		rcInner.bottom=drc.bottom;
		rcInner.right=rcInner.left+60;


		if (PtInRect(&rcInner,pt))			//������ƶ�����
		{
			ptdif.x = pt.x - drc.left;		//�������������������Ͻ�x����ľ���
			ptdif.y = pt.y - drc.top;		//�����������������Ͻ�y����ľ���
								
			sz.cx = drc.right - drc.left;	//���ο�ȴ�С
			sz.cy = drc.bottom - drc.top;	//���θ߶ȴ�С 
		
			RECT	m_drc = drc;
			DrawUIBorder(&m_drc);
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			fCanMove = TRUE;
		
		}

		else								//�����ť 
		{
			Offest=pt.x-drc.left;
			Offest=Offest/20;
			
			switch(Offest)
			{
			case 3:				//Ӣ ��
				{
					if (dwType & TYPE_ZHONGWEN)
					{
						dwType=dwType&~TYPE_ZHONGWEN;
						dwType|=TYPE_YINGYU;
					}
					else if (dwType & TYPE_YINGYU)
					{
						dwType=dwType&~TYPE_YINGYU;
						dwType|=TYPE_ZHONGWEN;
					}
				}
				break;
			case 4:			//ȫ ��
				{
					if (dwType & TYPE_CHUANGJIAO)
					{
						dwType=dwType&~TYPE_CHUANGJIAO;
						dwType|=TYPE_BANJIAO;
					}
					else if (dwType & TYPE_BANJIAO)
					{
						dwType=dwType&~TYPE_BANJIAO;
						dwType|=TYPE_CHUANGJIAO;
					}
				}
				break;
			case 5:			//���
				{
					if (dwType & TYPE_BIAO1)
					{
						dwType=dwType&~TYPE_BIAO1;
						dwType|=TYPE_BIAO2;
					}
					else if (dwType & TYPE_BIAO2)
					{
						dwType=dwType&~TYPE_BIAO2;
						dwType|=TYPE_BIAO1;
					}
				}
				break;
			case 6:		//С����
				{
					HWND hDlg=CreateDialog(hInst,(LPSTR)IDD_SOFTKETBOARD,hStatus,(DLGPROC)KeyBoardProc);
					ShowWindow(hDlg,SW_NORMAL);
				}
				break;
			case 7:		//����
				{
					SendMessage(hWnd,WM_RBUTTONDOWN,wParam,lParam);
				}//case
				break;
			default:
				break;
			}

			HDC hStatusDC=GetDC(hStatus);
			Status_OnPaint(hStatus,hStatusDC);
			ReleaseDC(hStatus,hStatusDC);

		}

	}
	
	else  if (HIWORD(lParam) == WM_RBUTTONDOWN )
	{
		SetCapture(hWnd);		
	}
	else if (HIWORD(lParam) == WM_MOUSEMOVE)
	{
		
		GetCursorPos(&pt);
		GetWindowRect(hWnd,&drc);
				
		rcInner.left = drc.left;
		rcInner.top=drc.top;
		rcInner.bottom=drc.bottom;
		rcInner.right=rcInner.left+60;

		
		if (PtInRect(&rcInner,pt))					//���ƶ���������
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		}
		else										//����������
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			Offest=pt.x-drc.left;
			Offest=Offest/20;
			switch(Offest)
			{
			case 3:
				{
					if (dwType & TYPE_ZHONGWEN)
					{

					}
					else if (dwType & TYPE_YINGYU)
					{

					}
				}
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				break;
			}
			
		}
	}
}

/************************************************************************/
/*
/* ��������������
/*                                                                       */
/************************************************************************/
void Status_OnLButtonUp(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	
    HDC hdc;
    hdc = GetDC(hWnd);
	POINT pt;
	ReleaseCapture();
	
	if( fCanMove )
	{
		DrawUIBorder(&drc);
		GetCursorPos( &pt );
		MoveWindow(hWnd,pt.x - ptdif.x, pt.y - ptdif.y, sz.cx, sz.cy,TRUE);	
	}

	fCanMove = FALSE;
}
/************************************************************************/
/*
/* ��������ƶ�ʱ���¼�
/*                                                                      */
/************************************************************************/
void Status_OnMouseMove()
{
	POINT pt;
	if( fCanMove )
	{
		GetCursorPos( &pt );
		DrawUIBorder(&drc);

		drc.left   = pt.x - ptdif.x;
		drc.top    = pt.y - ptdif.y;
		drc.right  = drc.left + sz.cx;
		drc.bottom = drc.top + sz.cy;
		
		DrawUIBorder(&drc);
	}
 }
/************************************************************************/
/* 
/* StatusWndProc()	: ״̬������Ϣ�������
/*                                                                      */
/************************************************************************/
BOOL bSwitchOn=TRUE;
LRESULT CALLBACK StatusWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) 
	{
	case WM_CREATE:
		{
			Status_OnCreate();
		}
		break;

	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			Status_OnPaint(hWnd,ps.hdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_SETCURSOR:
		{
			Status_OnCaptureMouse(hWnd,wParam,lParam);
		}
		break;

	case WM_LBUTTONUP:
		{
			Status_OnLButtonUp(hWnd,wParam,lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			Status_OnRButtonDown(hWnd,wParam,lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			Status_OnMouseMove();
		}
		break;

	case WM_KEYDOWN:
		{
			Status_OnKeyDown(hWnd,message,wParam,lParam);
		}
		break;

	case WM_HOTKEY:
		{
			bSwitchOn=!bSwitchOn;
			if (bSwitchOn)
			{
				InstallHook();
				dwType=dwType&~TYPE_YINGYU;
				dwType|=TYPE_ZHONGWEN;

			}
			else
			{
				UnInstallHook();
				
				//�ı�״̬���� �� Ӣ ѡ��
				dwType=dwType&~TYPE_ZHONGWEN;
				dwType|=TYPE_YINGYU;

				//�������봰��
				INPUTWNDSTRUCT.bShow=FALSE;
				UpdateInputWnd();
			}

			HDC hdc=GetDC(hStatus);
			Status_OnPaint(hStatus,hdc);
			ReleaseDC(hStatus,hdc);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/************************************************************************/
/* 
/* CreateStatus()	: ����״̬����
/* RETUREN			:TRUE �����ɹ�   :FALSE ����ʧ��
/*                                                                     */
/************************************************************************/

BOOL CreateStatus(HINSTANCE hInst)
{
	RECT rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcScreen,0);

	hStatus = CreateWindowEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW,
							STATUSCLASS,
							"MULTIFUNIME",
							WS_DISABLED|WS_POPUP,
							rcScreen.right-200,
							rcScreen.bottom-25,
							160, 
							20,
							NULL,
							NULL, 
							hInst, 
							NULL);

	if(!hStatus)
	{
		return FALSE;
	}
	ShowWindow(hStatus,SW_NORMAL);
	UpdateWindow(hStatus);
	return TRUE;
}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
INT_PTR CALLBACK  KeyBoardProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
switch(uMsg)
	{	
	case WM_COMMAND:
		switch( LOWORD(wParam))
		{
	             /*0-9*/		
		case IDC_0:
			
			PostMessage(hStatus,WM_KEYDOWN,48,0x30);//����'0'
			break;
	
		case IDC_1:

			PostMessage(hStatus,WM_KEYDOWN,49,0x31);//����'1'
			break;
			
		case IDC_2:
			
			PostMessage(hStatus,WM_KEYDOWN,50,0x32);//����'2'
			break;
		case IDC_3:
			
			PostMessage(hStatus,WM_KEYDOWN,51,0x33);//����'3'
			break;
		case IDC_4:
			
			PostMessage(hStatus,WM_KEYDOWN,52,0x34);//����'4'
			break;
		case IDC_5:
			
			PostMessage(hStatus,WM_KEYDOWN,53,0x35);//����'5'
			break;
		case IDC_6:
			
			PostMessage(hStatus,WM_KEYDOWN,54,0x36);//����'6'
			break;
		case IDC_7:
			
			PostMessage(hStatus,WM_KEYDOWN,55,0x37);//����'7'
			break;
		case IDC_8:
			
			PostMessage(hStatus,WM_KEYDOWN,56,0x38);//����'8'
			break;
		case IDC_9:
			
			PostMessage(hStatus,WM_KEYDOWN,57,0x39);//����'9'
			break;

		case IDC_A:
			PostMessage(hStatus,WM_KEYDOWN,65,0x41);//����'a'
			break;
			
		case IDC_B:
			PostMessage(hStatus,WM_KEYDOWN,66,0x42);//����'b'
			break;
	
		case IDC_C:
			PostMessage(hStatus,WM_KEYDOWN,67,0x43);//����'c'
			break;
	
		case IDC_D:
			PostMessage(hStatus,WM_KEYDOWN,68,0x44);//����'d'
			break;
		
		case IDC_E:
			PostMessage(hStatus,WM_KEYDOWN,69,0x45);//����'e'
			break;
	
		case IDC_F:
			PostMessage(hStatus,WM_KEYDOWN,70,0x46);//����'f'
			break;
		
		case IDC_G:
			PostMessage(hStatus,WM_KEYDOWN,71,0x47);//����'g'
			break;
	
		case IDC_H:
			PostMessage(hStatus,WM_KEYDOWN,72,0x48);//����'h'
			break;
	
		case IDC_I:
			PostMessage(hStatus,WM_KEYDOWN,73,0x49);//����'i'
			break;
		
		case IDC_J:
			PostMessage(hStatus,WM_KEYDOWN,74,0x4A);//����'j'
			break;
	
		case IDC_K:
			PostMessage(hStatus,WM_KEYDOWN,75,0x4B);//����'k'
			break;
		
		case IDC_L:
			PostMessage(hStatus,WM_KEYDOWN,76,0x4C);//����'l'
			break;
	
		case IDC_M:
			PostMessage(hStatus,WM_KEYDOWN,77,0x4D);//����'m'
			break;
	
		case IDC_N:
			PostMessage(hStatus,WM_KEYDOWN,78,0x4E);//����'n'
			break;
	
		case IDC_O:
			PostMessage(hStatus,WM_KEYDOWN,79,0x4F);//����'o'
			break;
		
		case IDC_P:
			PostMessage(hStatus,WM_KEYDOWN,80,0x50);//����'p'
			break;
		
		case IDC_Q:
			PostMessage(hStatus,WM_KEYDOWN,81,0x51);//����'q'
			break;
		
		case IDC_R:
			PostMessage(hStatus,WM_KEYDOWN,82,0x52);//����'r'
			break;
		
		case IDC_S:
			PostMessage(hStatus,WM_KEYDOWN,83,0x53);//����'s'
			break;
		
		case IDC_T:
			PostMessage(hStatus,WM_KEYDOWN,84,0x54);//����'t'
			break;
	
		case IDC_U:
			PostMessage(hStatus,WM_KEYDOWN,85,0x55);//����'u'
			break;
		
		case IDC_V:
			PostMessage(hStatus,WM_KEYDOWN,86,0x56);//����'v'
			break;
	
		case IDC_W:
			PostMessage(hStatus,WM_KEYDOWN,87,0x57);//����w'
			break;
		
		case IDC_X:
			PostMessage(hStatus,WM_KEYDOWN,88,0x58);//����'x'
			break;
		
		case IDC_Y:
			PostMessage(hStatus,WM_KEYDOWN,89,0x59);//����'y'
			break;
		
		case IDC_Z:
			PostMessage(hStatus,WM_KEYDOWN,90,0x5A);//����'z'
			break;
		
	  
		case IDC_SPACE:
		   PostMessage(hStatus,WM_KEYDOWN,VK_SPACE,0x20);//���Ϳո�
		   break;
		   
		case  IDC_BACK:
			PostMessage(hStatus,WM_KEYDOWN,VK_BACK,0xA6);//�˸��
			break;
		
         
		case IDC_LSHIFT:
			PostMessage(hStatus,WM_KEYDOWN,VK_LSHIFT,0xA0);//������ߵ�Shift��
             break;

        case IDC_ENTER:
			PostMessage(hStatus,WM_KEYDOWN,VK_RETURN,0x0D);//�س�
             break;

		case IDC_Tab:
			PostMessage(hStatus,WM_KEYDOWN,VK_TAB,0x09);//tab
			break;

		case IDC_Esc:
			EndDialog(hWnd,0);
			break;
		}//command
	break;

	case WM_RBUTTONDOWN:
		EndDialog(hWnd,0);
		break;

	case WM_NCHITTEST:
		return HTCAPTION;
		break;
	}
	return 0L;
}
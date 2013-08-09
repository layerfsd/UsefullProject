#include "StdAfx.h"
#include "Global.h"
#include "HandleDic.h"
#include "InputWnd.h"
#include "Assistance.h"
#include "HandleMsg.h"
#include "StatusWnd.h"
/************************************************************************
/* 
/* HandleMsg ()	: �жϸ���Ϣ�Ƿ���Ҫ����������Ҫ�������ؼٲ�����
/*                                                                      */
/************************************************************************/
BOOL HandleMsg(WPARAM wParam,LPARAM lParam)
{
	if (wParam == VK_BACK)						//ɾ����
	{
		if(KeyState & HANDLE_BACK)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
				PostMessage(hForeWnd,WM_CHAR,wParam,lParam);
				PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}
	else if(wParam == VK_SPACE)				   	//�ո��
	{
		if (KeyState & HANDLE_SPACE)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_CHAR,wParam,lParam);
			}

			return FALSE;
		}
	}

	else if (wParam >= 0x30 && wParam <= 0x39 )	// 0 - 9
	{
		if (KeyState & HANDLE_NUM)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_CHAR,wParam,lParam);
			}
			return FALSE;
		}
	}

	else if (wParam == VK_LEFT)					//LEFT
	{
		if (KeyState & HANDLE_LEFT)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
				PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}

	else if (wParam == VK_RIGHT)				//RIGHT
	{
		if (KeyState & HANDLE_RIGHT)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
				PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}

	else if (wParam == VK_DOWN)					//DOWN
	{
		if (KeyState & HANDLE_DOWN)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
				PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}

	else if (wParam == VK_UP)					//UP
	{
		if (KeyState & HANDLE_UP)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
				PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}
	else if (wParam >= VK_NUMPAD4 && wParam<= VK_NUMPAD9)
	{
		if (KeyState & HANDLE_SMALL_NUM)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
				PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}
	else if (wParam >= 0x41 && wParam <= 0x5A )	// A - Z
	{
		if (KeyState & HANDLE_CHAR)
		{
			return TRUE;
		}
		else
		{
			if (GetFrontWnd(&hForeWnd))
			{
				PostMessage(hForeWnd,WM_CHAR,wParam,lParam);
				//PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
			}
			return FALSE;
		}
	}
	else if (wParam == 0xBC )//����
	{
		if (GetFrontWnd(&hForeWnd))
		{
			PostMessage(hForeWnd,WM_CHAR,(WPARAM)44,lParam);
		}
		return FALSE;
	}
	else if (wParam == 0xBE)//���
	{
		if (GetFrontWnd(&hForeWnd))
		{
			PostMessage(hForeWnd,WM_CHAR,(WPARAM)46,lParam);
		}
		return FALSE;
	}
	else if (wParam == 0xBF) //  /?
	{
		if (GetFrontWnd(&hForeWnd))
		{
			PostMessage(hForeWnd,WM_KEYDOWN,wParam,lParam);
			PostMessage(hForeWnd,WM_CHAR,(WPARAM)44,lParam);
			PostMessage(hForeWnd,WM_KEYUP,wParam,lParam);
		}
		return FALSE;
	}
	else if (wParam == VK_CONTROL)
		return TRUE;

	return FALSE;
}

/************************************************************************/
/* 
/*	HandleChar()	: ���� A - Z
/*	wParam			: �����
/*	lParam			: ������Ϣ
/*	                                                                    */
/************************************************************************/

void HandleChar(WPARAM wParam,LPARAM lParam)
{

	CHAR		ReadAChar;								//������ַ�
	ReadAChar=(CHAR)(wParam+32);
	COMPLIST.Buffer[COMPLIST.nCurPos++]	=ReadAChar;		//����봮����һ���ַ�
	COMPLIST.Buffer[COMPLIST.nCurPos]='\0';
	COMPLIST.nLen=COMPLIST.nCurPos;						//���ó���


	if (CurMode & MODE_FULL)
	{
		/*  �����������Ӣ��  ��������*/
		ResultMode=0;										//�������״̬
		if(SearchWB())
		{
			ResultMode|=MODE_WUBI;
		}
		if (SearchEng())
		{
			ResultMode|=MODE_ENGLIS;
		}
		if (SearchPY())
		{
			ResultMode|=MODE_PINYIN;
		}
		else if( SearchPYCZ() )
		{
			ResultMode|=MODE_CIZU;
		}

		MakeCandStr();	//���ɺ�ѡ�б�
	}

	else if (CurMode & MODE_ENGLIS)					//��Ӣ��ģʽ
	{
		SearchEnglish();
	}

	else if (CurMode & MODE_PINYIN)					//��ƴ��ģʽ
	{
		ResultMode=0;
		if(SearchPY())
		{
			ResultMode|=MODE_PINYIN;
		}
		else if (SearchPYCZ())
		{
			ResultMode|=MODE_CIZU;
		}
		MakeCandStr();
	}

	else if (CurMode & MODE_WUBI)					//�����ģʽ
	{
		if (SearchSingleWB())
		{
			MakeWBCandStr();
		}
	}

	//����״̬ת��
	KeyState=0;
	KeyState|=HANDLE_NUM;
	KeyState|=HANDLE_BACK;
	KeyState|=HANDLE_SPACE;
	KeyState|=HANDLE_LEFT;
	KeyState|=HANDLE_RIGHT;
	KeyState|=HANDLE_UP;
	KeyState|=HANDLE_CHAR;
	KeyState|=HANDLE_DOWN;


	//�������봰��״̬
	INPUTWNDSTRUCT.bShow=TRUE;
	//������ʾ
	UpdateInputWnd();
}

/************************************************************************/
/* 
/*	HandleBack()	: ����ɾ����
/*                                                                      */
/************************************************************************/
void HandleBack()
{
	if (COMPLIST.nLen > 0 )
	{
		COMPLIST.nLen -- ;
		COMPLIST.nCurPos--;
		COMPLIST.Buffer[COMPLIST.nLen]='\0';
	}

	if (COMPLIST.nLen == 0)
	{
		KeyState=0;						//�������״̬
		KeyState|=HANDLE_CHAR;
		KeyState|=HANDLE_SMALL_NUM;
		
		INPUTWNDSTRUCT.bShow=FALSE;
		UpdateInputWnd();
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	
	KeyState	=0;				//��հ���״̬

	if (CurMode & MODE_FULL)
	{
		ResultMode	=0;
		if (SearchWB())	
		{
			ResultMode|=MODE_WUBI;
		}

		if (SearchPY())
		{
			ResultMode|=MODE_PINYIN;
		}
		
		MakeCandStr();
	}


	else if (CurMode & MODE_BIHUA)
	{
		FILE *fp;

		LONG offest=SearchBH(&fp);
		
		if (offest == -1) //�Ҳ�����Ӧ����
		{
			return ;
		}

		MakeCandBHStr(fp,offest);
	}

	else if (CurMode & MODE_PINYIN)
	{
		ResultMode =0;
		if (SearchPY())
		{
			ResultMode|=MODE_PINYIN;
		}
		else if (SearchPYCZ())
		{
			ResultMode|=MODE_CIZU;
		}

		MakeCandStr();
	}


	//���°�����״̬
	KeyState|=HANDLE_CHAR;
	KeyState|=HANDLE_BACK;
	KeyState|=HANDLE_SPACE;
	KeyState|=HANDLE_NUM;
	KeyState|=HANDLE_LEFT;
	KeyState|=HANDLE_RIGHT;

	//�������봰��״̬
	INPUTWNDSTRUCT.bShow=TRUE;
	//������ʾ
	UpdateInputWnd();
}

/************************************************************************/
/* 
/* HandleSpace()	: ����ո����      
/*                                                                      */
/************************************************************************/
void HandleSpace()
{
	if (CurMode&MODE_ENGLIS)
	{
		TranslateEng(1);
	}
	else
	{
		TranslateResult(1);
	}

	KeyState=0;						//�������״̬
	KeyState|=HANDLE_CHAR;			//������ͨ��ĸ����
	KeyState|=HANDLE_SMALL_NUM;

	INPUTWNDSTRUCT.bShow=FALSE;

	ClearCand();
	ClearComp();

	UpdateInputWnd();				//�������봰��
}

/************************************************************************/
/* 
/* �������������
/*                                                                     */
/************************************************************************/

void HandleNum(WPARAM wParam,LPARAM lParam)
{
	int SelectNum=wParam-0x30;

	if (SelectNum <=0 || SelectNum > CANDLIST.nPageSize )
	{
		return;
	}

	if (CurMode&MODE_ENGLIS)
	{
		TranslateEng(SelectNum);
	}
	else
	{
		TranslateResult(SelectNum);
	}


	KeyState=0;						//�������״̬
	KeyState|=HANDLE_CHAR;			//������ͨ��ĸ����
	KeyState|=HANDLE_SMALL_NUM;		//���ñʻ�����

	INPUTWNDSTRUCT.bShow=FALSE;
	ClearCand();
	ClearComp();
	UpdateInputWnd();				//�������봰��
}

/************************************************************************/
/* 
/* �������뷨���л�
/*                                                                     */
/************************************************************************/

void HandleControl()
{
	ClearComp();
	ShowWindow(hInput,SW_HIDE);

	if (CurMode>= MODE_FULL)
	{
		CurMode=MODE_PINYIN;

	}
	else
	{
		CurMode<<=1;
		if (CurMode == MODE_CIZU)
		{
			CurMode<<=1;
		}
	}

	dwType=0;
	dwType|=TYPE_ZHONGWEN;		//���� 
	dwType|=TYPE_CHUANGJIAO;	//ȫ��
	dwType|=TYPE_BIAO1;			//���1
	
	switch(CurMode)			//ת��ģʽ
	{
	case MODE_FULL:
		{
			dwType|=TYPE_FULL;
			KeyState=0;
			KeyState|=HANDLE_CHAR;
			KeyState|=HANDLE_SMALL_NUM;

		}
		break;
	case MODE_ENGLIS:
		{
			dwType|=TYPE_ENG;
			KeyState=0;
			KeyState|=HANDLE_CHAR;
		}
		break;
	case MODE_PINYIN:
		{
			dwType|=TYPE_PINYIN;
			KeyState=0;
			KeyState|=HANDLE_CHAR;
		}
		break;
	case MODE_WUBI:
		{
			dwType|=TYPE_WUBI;
			KeyState=0;
			KeyState|=HANDLE_CHAR;
		}
		break;
	case MODE_BIHUA:
		{
			dwType|=TYPE_BIHUA;
			KeyState=0;
			KeyState|=HANDLE_SMALL_NUM;
		}
		break;
	default:
		break;
	}
	if (CurMode & MODE_ENGLIS)
	{
		INPUTWNDSTRUCT.sz.cx=150;
		INPUTWNDSTRUCT.sz.cy=200;
	}
	else
	{
		INPUTWNDSTRUCT.sz.cx=400;
		INPUTWNDSTRUCT.sz.cy=42;
	}

	HDC hdc=GetDC(hStatus);
	Status_OnPaint(hStatus,hdc);
	ReleaseDC(hStatus,hdc);
}


/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/

void HandleRight()
{
}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void HandLeft()
{
}

/************************************************************************/
/* 
/*
/*                                                                     */
/************************************************************************/
void Handle_Small_Num(WPARAM wParam)
{
	FILE *fp;
	
	int num=wParam - VK_NUMPAD0;
	
	if (num >=7 && num <=9)
	{
		num -= 6;
	}
	
	COMPLIST.Buffer[COMPLIST.nCurPos++]=(char)(num+48);
	COMPLIST.nLen=COMPLIST.nCurPos;
	COMPLIST.Buffer[COMPLIST.nCurPos]='\0';

	
	LONG offest=SearchBH(&fp);
	
	if (offest == -1) //�Ҳ�����Ӧ����
	{
		return ;
	}
	
	MakeCandBHStr(fp,offest);
	
	//���°�����״̬
	
	KeyState|=HANDLE_BACK;
	KeyState|=HANDLE_SPACE;
	KeyState|=HANDLE_NUM;
	KeyState|=HANDLE_LEFT;
	KeyState|=HANDLE_RIGHT;
	
	INPUTWNDSTRUCT.bShow=TRUE;
	
	UpdateInputWnd();
}

/************************************************************************/
/* 
/* AddBHtoComp() : ���һ�ʵ������
/*                                                                     */
/************************************************************************/
void AddBHtoComp(char ch)
{
	COMPLIST.Buffer[COMPLIST.nCurPos++]=ch;
}

/************************************************************************/
/* 
/* ���ͽ������Ŀ�����
/*                                                                      */
/************************************************************************/

void TranslateResult(int num)
{
	
	if (!GetFrontWnd(&hForeWnd))
	{
		return;
	}
	
	int pos=0;

	if (num == 1)				//�������һ��
	{
		pos+=2;
	}
	else if(num > 1 )			//������漸��
	{
		int FindNum=num-1;

		//�ҵ���Ӧ�Ŀ�ʼλ��
		while (TRUE)
		{
			if (CANDLIST.Buff[pos] == ' ')
			{
				pos+=2;
				FindNum--;
				if(!FindNum)
					break;
			}
			else
			{
				pos++;
			}
		}
		pos+=2;				//ָ�������ֿ�ʼλ��g
	}
	
	DWORD dwTransMsg;		//�����ֵı���

	LPCSTR lpCandStr=CANDLIST.Buff;

	while( *(lpCandStr+pos) != ' ')
	{
		dwTransMsg=(unsigned char)*(lpCandStr+pos);
		dwTransMsg<<=8;
		dwTransMsg+=(unsigned char)*(lpCandStr+pos+1);
		PostMessage(hForeWnd,WM_IME_CHAR,dwTransMsg,1);		//���������ֵ�����
		pos+=2;
	}
}


/************************************************************************/
/* 
/* ����Ӣ�Ĵ���Ŀ�����
/*                                                                     */
/************************************************************************/

void TranslateEng(int num)
{

	if (!GetFrontWnd(&hForeWnd))
	{
		return;
	}

	LPSTR lpResult=&ENGINFO.strList[num-1][2];

	while (*lpResult != ' ')
	{
		PostMessage(hForeWnd,WM_CHAR,(WPARAM)*lpResult,1);
		lpResult++;
	}
}



/************************************************************************/
/* 
/*  ƴ��ģʽ�����Ϸ�ҳ
/*                                                                      */
/************************************************************************/
void PYPageUp()
{
}

/************************************************************************/
/* 
/* ƴ��ģʽ�����·�ҳ
/*                                                                     */
/************************************************************************/

void PYPageDown()
{
}

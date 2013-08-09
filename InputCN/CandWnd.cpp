#include "StdAfx.h"
#include "NewIme.h"
#include "Global.h"
#include "Assistance.h"
#include <stdio.h>
#include "InputWnd.h"
/************************************************************************/
/* 
/* CandWndProc()	:	��ѡ������Ϣ�������
/*                                                                      */
/************************************************************************/

LRESULT	CALLBACK	CandWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RECT rt;
		GetClientRect(hWnd, &rt);
		DrawText(hdc, "CompWnd", strlen("CompWndPro"), &rt, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,uMsg, wParam, lParam);
	}
	return 0;
}

/************************************************************************/
/* 
/* CreateCandWnd() :	������ѡ����
/*                                                                     */
/************************************************************************/

BOOL CreateCandWnd(HINSTANCE hInstance)
{

	RECT	rcInputWnd;
	int		nInputHeight;
	int		nInputWidth;

	GetClientRect(hInput,&rcInputWnd);				//��ȡ���봰�ڵĴ�С

	nInputHeight	=rcInputWnd.bottom-rcInputWnd.top;
	nInputWidth		=rcInputWnd.right-rcInputWnd.left;
	nInputHeight	=nInputHeight/2;

	hCand=CreateWindowEx(
						WS_EX_TOPMOST|WS_EX_TOOLWINDOW,
						COMPCLASS,
						NULL,
						WS_POPUP|WS_DISABLED|WS_CHILD,
						0,
						nInputHeight,
						nInputWidth,
						nInputHeight,
						hInput,
						(HMENU)NULL,
						hInstance,
						NULL
						);
	if (!hCand)
	{
		return FALSE;
	}

	return TRUE;

}



/************************************************************************/
/* 
/* MakeCandStr()	: ���ɺ�ѡ�б���
/*                                                                     */
/************************************************************************/

void MakeCandStr()
{
	int nPageSize=CANDLIST.nPageSize;
	int nCandPos=0;
	int num=1;									//��ѡ�б��ѡ������

	// 1. �����ʺ�ѡ�б�.

	if (ResultMode & MODE_WUBI)
	{
		LPSTR	lpWB=WBINFO.strResult;			//ָ����ʵĺ�ѡ�б�

		CANDLIST.Buff[nCandPos++]	=(char)(48+num);
		CANDLIST.Buff[nCandPos++]	='.';

		while (*lpWB != '\0')
		{
			CANDLIST.Buff[nCandPos++]=*lpWB++;
		}

		CANDLIST.Buff[nCandPos++]	=' ';
		CANDLIST.Buff[nCandPos++]	=' ';

		num++;
	} //if

	// 2. ���Ӣ���ѡ�б�.
	// �����ƴû�о��������Ӣ�һ�����ʹ������2����˭
		
	if (ResultMode & MODE_ENGLIS )
	{
		LPSTR	lpEng=ENGINFO.strRestult;
		if (ResultMode & MODE_PINYIN)
		{
			CANDLIST.Buff[nCandPos++]=(char)(48+num);
			CANDLIST.Buff[nCandPos++]='.';
			while (*lpEng != ' ' && *lpEng !='\0')
			{
				CANDLIST.Buff[nCandPos++]=*lpEng++;
			}
			CANDLIST.Buff[nCandPos++]=' ';
			CANDLIST.Buff[nCandPos++]=' ';
			num++;
	
			if (*lpEng !='\0')
			{
				CANDLIST.Buff[nCandPos++]=(char)(48+num);
				CANDLIST.Buff[nCandPos++]='.';
				lpEng++;
				while (*lpEng != ' ' && *lpEng != '\0')
				{
					CANDLIST.Buff[nCandPos++]=*lpEng++;
				}
				CANDLIST.Buff[nCandPos++]=' ';
				CANDLIST.Buff[nCandPos++]=' ';
				num++;
			}//if
		}//if
		else	//���ȫ��Ӣ��
		{
			while (num <= nPageSize)
			{
				CANDLIST.Buff[nCandPos++]	=(char)(48+num);
				CANDLIST.Buff[nCandPos++]	='.';
				while (*lpEng!=' ' &&  *lpEng!='\0')
				{
					CANDLIST.Buff[nCandPos++]	=*lpEng++;
				}
				if (*lpEng++ == '\0')
				{
					break;
				}
				CANDLIST.Buff[nCandPos++]	=' ';
				CANDLIST.Buff[nCandPos++]	=' ';
				num++;
					
			}//while

		}//else
		
	}
	// 3.��ӵ�ƴ��
	if (ResultMode & MODE_PINYIN)
	{
		LPSTR lpPY=PYINFO.lpStr;

		while (num <= nPageSize)
		{
			CANDLIST.Buff[nCandPos++]	=(char)(48+num);
			CANDLIST.Buff[nCandPos++]	='.';
			CANDLIST.Buff[nCandPos++]	=*lpPY++;
			CANDLIST.Buff[nCandPos++]	=*lpPY++;
			CANDLIST.Buff[nCandPos++]	=' ';
			CANDLIST.Buff[nCandPos++]	=' ';
				
			num++;

			if (*lpPY == '\0' || *(lpPY+1) =='\0')
			{
				break;
			}
		}//while	
		PYINFO.lpStr=lpPY;
	}

	// 4.���ƴ������
	if(ResultMode & MODE_CIZU)
	{
		LPSTR	lpCZ=CIZUINFO.strResult;
		BOOL	bFalg=TRUE;
		while (num <= nPageSize && bFalg)
		{
			CANDLIST.Buff[nCandPos++]	=(char)(48+num);
			CANDLIST.Buff[nCandPos++]	='.';

			while(TRUE)
			{
				if (*lpCZ == ' ')
				{
					lpCZ++;

					if (*lpCZ == '\0')
					{
						bFalg=FALSE;
					}

					break;
				}

				if(*lpCZ =='\0')
				{
					bFalg=FALSE;
					break;
				}
				CANDLIST.Buff[nCandPos++] =*lpCZ++;
			}

			CANDLIST.Buff[nCandPos++]	=' ';
			CANDLIST.Buff[nCandPos++]	=' ';
			num++;
		} //while
	}//if

	CANDLIST.nLen=nCandPos;
}

/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
void MakeCandBHStr(FILE *fp,LONG offest)
{
	if (!fp)
	{
		AssPrint("MakeCandBHStr: ���ļ�ʧ��!");
		return;
	}
	
	int CompLen=COMPLIST.nLen;
	
	int i=1;					//������������ǰ��ѡ������
	char hz[3];					//����
	char bihua[60];				//�ʻ�����
	
	fseek(fp,offest,SEEK_SET);	//�ƶ�����Ӧ��ƫ��λ��
	
	fpos_t pos=NULL;			//���ڼ�¼�ļ�λ��
	
	while (!feof(fp))
	{
		fgetpos(fp,&pos);			//���浱ǰλ��
		fscanf(fp,"%s%s",hz,bihua);
		
		if (_mbsnbcmp((UCHAR *)COMPLIST.Buffer,(UCHAR *)bihua,CompLen) == 0)
		{
			MakeBHStr(fp,&pos);
			break;
		}
	}
	
}

/************************************************************************/
/* 
/*
/*                                                                      */
/************************************************************************/
void MakeBHStr(FILE *fp,fpos_t *fpos)
{
	int PageNum=CANDLIST.nPageSize;			//һҳ���ٸ���ѡ��
	char hz[3];								//����
	char bihua[60];							//�ļ��еıʻ�����
	
	int i=1;								//ѡ����ǰ������
	int pos=0;

	fsetpos(fp,fpos);						//�Ƶ���Ӧ��

	while(PageNum -- )
	{
		fscanf(fp,"%s%s",hz,bihua);
		if (feof(fp))
		{
			return;
		}
		//�ŵ������������㷭ҳ
		SAVE.szSave[pos++]=hz[0];
		SAVE.szSave[pos++]=hz[1];
		SAVE.szSave[pos++]=' ';				//����֮��ķָ�
		
	}


	SAVE.szSave[pos++]='|';
	SAVE.szSave[pos]='\0';

	SAVE.nPos=pos;


	//�ŵ���ѡ�б�
	pos=0;
	int j=0;
	while (SAVE.szSave[j]!='|')
	{
		CANDLIST.Buff[pos++]=(char)(i+48);
		CANDLIST.Buff[pos++]='.';
		CANDLIST.Buff[pos++]=SAVE.szSave[j++];
		CANDLIST.Buff[pos++]=SAVE.szSave[j++];
		CANDLIST.Buff[pos++]=' ';
		CANDLIST.Buff[pos++]=' ';
		i++;
		j++;
	}
	CANDLIST.nLen=pos;
}
/************************************************************************/
/* 
/* 
/*                                                                      */
/************************************************************************/
void MakeWBCandStr()
{
	int pos=0;
	int i=0;
	int num=1;

	while (SAVE.szSave[i]!='|')
	{
		CANDLIST.Buff[pos++]=(char)(num+48);
		CANDLIST.Buff[pos++]='.';
		while ( (CANDLIST.Buff[pos++] = SAVE.szSave[i++] ) !=' ');

		CANDLIST.Buff[pos++]=' ';
		

		num++;
	}
	CANDLIST.Buff[pos]='\0';
	CANDLIST.nLen=pos;
}
/************************************************************************/
/* 
/* ClearCand()	: ��պ�ѡ�б�
/*                                                                      */
/************************************************************************/
void ClearCand()
{
	CANDLIST.nLen=0;
}
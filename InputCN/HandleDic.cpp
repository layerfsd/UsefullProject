#include "StdAfx.h"
#include "HandleDic.h"
#include <stdio.h>
#include "Global.h"
#include "Assistance.h"

//////////////////////////////////////////////////////////////////////////
BOOL MapBHFile();

/************************************************************************/
/* 
/*	InitPyInfo()	:��ȡƴ���ļ���Ϣ
/*                                                                      */
/************************************************************************/

BOOL InitPYInfo()
{
	
	PYINFO.fp=fopen("DIC\\DicInfo.ini","r");	//����Ϣ�ļ�
	if (!PYINFO.fp)
	{
		AssPrint("��ƴ����Ϣ����!");
		return FALSE;
	}

	int i=0;
	
	while (!feof(PYINFO.fp))					//������Ϣ
	{
		fscanf(PYINFO.fp,"%s %ld",PYINFO.MapFile[i].PinYin,&PYINFO.MapFile[i].Offest);
		i++;
	}
	fclose(PYINFO.fp);
	
	
	PYINFO.fp=fopen("DIC\\PYDic.txt","r");		//�ҿ�ƴ���ֿ�
	if (!PYINFO.fp)
	{
		AssPrint("��ƴ���ֿ����!");
		return FALSE;
	}

	PYINFO.lpStr=NULL;

	return TRUE;
};

/************************************************************************/
/* 
/* SearchPY()	: ������Ӧ��ƴ�����Ҳ������� FALSE
/*                                                                     */
/************************************************************************/

BOOL SearchPY()
{
	int i;									//������
	LPSTR lpCompStr=COMPLIST.Buffer;		//ָ������ı���


	for (i=0;i<417;i++)						//��ӳ�����ҵ���Ӧ��ƫ��
	{
		if (_mbsnbcmp((UCHAR*)lpCompStr,(UCHAR *) PYINFO.MapFile[i].PinYin,strlen(lpCompStr)) == 0)
		{
			fseek(PYINFO.fp,PYINFO.MapFile[i].Offest,SEEK_SET);
			MakePYStr();					//����ƴ���ĺ�ѡ�б�
			return TRUE;
		}
	}

	//�Ҳ���
	return FALSE;
}

/************************************************************************/
/* 
/* MakePYStr()	:����ƴ���ĺ�ѡ�б�������ResultStr
/*                                                                      */
/************************************************************************/
void MakePYStr()
{
	fgets(PYINFO.strResult,2000,PYINFO.fp);

	LPSTR lpRead=PYINFO.strResult;

	if (lpRead != NULL)	//����ϴμ�¼
	{
		*lpRead='\0';
	}

	while (*lpRead++ !='\t');

	PYINFO.lpStr=lpRead;				//ָ������
}


/************************************************************************/
/* 
/* ��ȡ�����Ϣ�ļ�����������ֿ�
/*                                                                      */
/************************************************************************/
BOOL InitWBInfo()
{
	WBINFO.fp=fopen("DIC\\WBINI.ini","r");
	if(!WBINFO.fp)
	{
		AssPrint("��ȡ�����Ϣ�ļ�����!");
		return FALSE;
	}

	//��ȡ��Ϣ������

	int i=0;
	while (!feof(WBINFO.fp))
	{
		fscanf(WBINFO.fp,"%s %ld",WBINFO.MapWB[i].FirstChar,&WBINFO.MapWB[i].Offest);
		i++;
	}
	fclose(WBINFO.fp);

	WBINFO.fp=fopen("DIC\\WB.txt","r");
	if (!WBINFO.fp)
	{
		AssPrint("�ҿ�����ֿ��ļ�����!");
		return FALSE;
	}

	return TRUE;
}
/************************************************************************/
/* 
/*  �������
/*                                                                     */
/************************************************************************/
BOOL SearchWB()
{
	WBINFO.strResult[0]='\0';

	CHAR FirstChar=COMPLIST.Buffer[0];
	LONG Offest=WBINFO.MapWB[FirstChar-97].Offest;
	
	//�Ƶ���Ӧ�ļ���
	fseek(WBINFO.fp,Offest,SEEK_SET);
	
	CHAR strFile[10];
	CHAR strHZ[30];
	while (!feof(WBINFO.fp))
	{
		fscanf(WBINFO.fp,"%s%s",strFile,strHZ);
		if(strFile[0] != FirstChar )
		{
			return FALSE;
		}
		if ( strcmp(strFile,COMPLIST.Buffer) == 0 )
		{
			strcpy(WBINFO.strResult,strHZ);
			return TRUE;
		}
	}

	return FALSE;
}

/************************************************************************/
/* 
/* ������ģʽ�µ����
/*                                                                      */
/************************************************************************/
BOOL SearchSingleWB()
{
	WBINFO.strResult[0]='\0';			//�����ʽ���Ļ�����
	CHAR FirstChar=COMPLIST.Buffer[0];	//ָ�յ�һ���ַ�
	LONG Offest=WBINFO.MapWB[FirstChar-97].Offest; //�ҵ���Ӧ��ƫ����

	int nLenComp=0;
	int	nLenFile=0;
	int nLenHZ=0;

	LPSTR pFileComp;					//ָ���ļ��еı���

	SAVE.nPos=0;

	//�Ƶ���Ӧ�ļ���
	fseek(WBINFO.fp,Offest,SEEK_SET);
	
	CHAR strFile[10];					//�ļ��е���ʱ���
	CHAR strHZ[30];						//�ļ��е������

	while (!feof(WBINFO.fp))
	{
		fscanf(WBINFO.fp,"%s%s",strFile,strHZ);
		if(strFile[0] != FirstChar )
		{
			return FALSE;
		}
		if ( _mbsnbcmp((unsigned char *)strFile,(unsigned char *)COMPLIST.Buffer,COMPLIST.nLen) == 0 )
		{
			nLenComp=strlen(COMPLIST.Buffer);		//�ļ��б���ĳ���
			nLenFile=strlen(strFile);		//�������ĳ���
			nLenHZ=strlen(strHZ);

			SAVE.nPos+=nLenHZ;

			strcpy(SAVE.szSave,strHZ);		//���ƺ��ֵ�������

			if (nLenComp <nLenFile)			//�ѻ�û����ı�����ӵ����ֺ���
			{
				pFileComp=strFile;
				pFileComp+=nLenComp;		//ָ��δ����ı���

				strcpy(&SAVE.szSave[SAVE.nPos],pFileComp);

				SAVE.nPos+=nLenFile-nLenComp;
	
			}

			SAVE.szSave[SAVE.nPos++]=' ';

			int nPageNumber=CANDLIST.nPageSize-1;

			while (nPageNumber -- )
			{
				fscanf(WBINFO.fp,"%s%s",strFile,strHZ);

				nLenFile	= strlen(strFile);
				nLenHZ		= strlen(strHZ);
				
				strcpy(&SAVE.szSave[SAVE.nPos],strHZ);

				SAVE.nPos+=nLenHZ;

				if (nLenComp < nLenFile)
				{
					pFileComp=strFile;
					pFileComp+=nLenComp;
					strcpy(&SAVE.szSave[SAVE.nPos],pFileComp);
					SAVE.nPos+=nLenFile-nLenComp;
				}
				SAVE.szSave[SAVE.nPos++]=' ';

				if (feof(WBINFO.fp))
				{
					break;
				}	
			}

			SAVE.szSave[SAVE.nPos++]='|';
			SAVE.szSave[SAVE.nPos]='\0';
			SAVE.nLen=SAVE.nPos;

			return TRUE;
		}
	}
	
	return FALSE;
}

/************************************************************************/
/* 
/* �ر�����ֿ��ļ�
/*                                                                     */
/************************************************************************/
void CloseWBFile()
{
	fclose(WBINFO.fp);
}

/************************************************************************/
/* 
/* ��ʼ��������Ϣ�ļ�
/*                                                                     */
/************************************************************************/
BOOL InitCZInfo()
{
	CIZUINFO.fp=fopen("DIC\\PYCZ.ini","r");
	if (!CIZUINFO.fp)
	{
		AssPrint("��ȡƴ��������Ϣ�ļ�����!");
		return FALSE;
	}

	int i=0;
	while (!feof(CIZUINFO.fp))
	{
		fscanf(CIZUINFO.fp,"%s%ld",CIZUINFO.MapCZ[i].FirstChar,&CIZUINFO.MapCZ[i].Offest);
		i++;
	}

	fclose(CIZUINFO.fp);

	CIZUINFO.fp=fopen("DIC\\PYCZ.txt","r");

	if (!CIZUINFO.fp)
	{
		AssPrint("��ȡƴ���ʿ��ļ�����!");
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/* 
/* ����ƴ������ 
/*                                                                      */
/************************************************************************/
BOOL SearchPYCZ()
{
	char	ReadLine[80];					//���ļ��������
	char	PinYin[20];						//�����ƴ��
	char	hz[50];							//����ĺ���
	
	char	FirstChar;

	int		nPos=0;							//������Ϣ�ṹ�еĽ�����ĵ�ǰλ��

	int		nBuffLen=300;					//�������

	BOOL	bRet=FALSE;

	FirstChar = COMPLIST.Buffer[0];
	
	if (FirstChar<= 'h')
	{
		FirstChar-=97;
	}
	else if(FirstChar >= 'j' && FirstChar <= 't')
	{
		FirstChar-=98;
	}
	else if (FirstChar >= 'w' && FirstChar <='z')
	{
		FirstChar-=100;
	}
	
	LONG Offest=CIZUINFO.MapCZ[FirstChar].Offest;
	
	fseek(CIZUINFO.fp,Offest,SEEK_SET);
	
	BOOL bFalg=TRUE;
	
	static char *pComp;
	static char *pPinYin;
	
	while (!feof(CIZUINFO.fp) && bFalg)
	{
		pComp	=COMPLIST.Buffer;				//ָ����봮
		pPinYin	=PinYin;						//ָ���ļ��е�ƴ��
		
		fgets(ReadLine,80,CIZUINFO.fp);		//��ȡһ��
		
		int i=0;							//�������
		while(ReadLine[i] != '\t')
		{
			hz[i]=ReadLine[i];
			i++;
		}
		hz[i]='\0';
		
		strcpy(PinYin,&ReadLine[i+1]);		//���ƴ������
		
		if (COMPLIST.Buffer[0] != PinYin[0])
		{
			break;
		}

		BOOL bNext=TRUE;
		while (bNext)
		{
			if (*pPinYin == *pComp)
			{
				pPinYin++;
				pComp++;
				if (*pComp == '\0')							//�ҵ����������Ĵ���
				{
					int nLen=strlen(hz);
					for (i=0;i<nLen;i++)
					{
						if (nPos+1>nBuffLen)
						{
							bFalg=FALSE;
							bNext=FALSE;
							break;
						}
						CIZUINFO.strResult[nPos++]=hz[i];	//���һ�����鵽������Ϣ������
					}
					CIZUINFO.strResult[nPos++]=' ';
					bRet=TRUE;								//�ҵ���ش���
					break;
				}
			}
			else
			{
				while (*pPinYin++ !=' ' && *pPinYin !='\0');
				
				if (*pPinYin == '\0' || *pPinYin!=*pComp )
				{
					break;
				}
				
			}//else
		}//while
	}//while

	CIZUINFO.strResult[nPos]='\0';
	return bRet;
}

/************************************************************************/
/* 
/* ��ʼ��Ӣ����Ϣ�ļ���
/*                                                                      */
/************************************************************************/
BOOL InitEngInfo()
{
	ENGINFO.fp=fopen("DIC\\EngInfo.ini","r");	//����Ϣ�ļ�
	if (!ENGINFO.fp)
	{
		AssPrint("��Ӣ����Ϣ����!");
		return FALSE;
	}
	
	int i=0;
	char tmpFirstChar[2];						//��¼��һ����ĸ
	while (!feof(ENGINFO.fp))					//������Ϣ
	{
		fscanf(ENGINFO.fp,"%s %ld",tmpFirstChar,&ENGINFO.Offest[i]);	
		i++;
	}
	fclose(ENGINFO.fp);
	
	ENGINFO.fp=fopen("DIC\\EngDic.txt","r");		//�ҿ�Ӣ���ֿ�
	if (!ENGINFO.fp)
	{
		AssPrint("��Ӣ���ֿ����!");
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/* 
/*	����Ӣ�ģ��ɹ������棬û�м��������ؼ�
/*                                                                     */
/************************************************************************/
BOOL SearchEng()
{
	ENGINFO.strRestult[0]='\0';					//���ԭ������
	char	FirstChar=COMPLIST.Buffer[0];		
	LONG	offest=ENGINFO.Offest[FirstChar-97];
	
	char	ReadLine[80];
	char	comp[20];

	fseek(ENGINFO.fp,offest,SEEK_SET);			//�ƶ��ļ�ָ��

	while (!feof(ENGINFO.fp))
	{
		fgets(ReadLine,80,ENGINFO.fp);
		if (ReadLine[0] != FirstChar)
		{
			break;
		}

		int i=0;
		while (ReadLine[i] != '\t')
		{
			comp[i]=ReadLine[i];
			i++;
		}
		comp[i]='\0';
		
		if (strcmp(comp,COMPLIST.Buffer) == 0 )
		{
			strcpy(ENGINFO.strRestult,&ReadLine[i+1]);
			return TRUE;
		}
	}

	return FALSE;
}

/************************************************************************/
/* 
/* Ӣ��ģʽ�£�����Ӣ�ﵥ��
/*                                                                      */
/************************************************************************/
void SearchEnglish()
{
	CHAR FirstChar	=COMPLIST.Buffer[0];			//�����������ַ�
	LONG offest		=ENGINFO.Offest[FirstChar-97];	//���ƫ����
	CHAR Line[100];
	fseek(ENGINFO.fp,offest,SEEK_SET);				//�ƶ��ļ�ָ��

	CANDLIST.nLen=0;
	
	while (!feof(ENGINFO.fp))
	{
		fgets(Line,100,ENGINFO.fp);

		if (_mbsnbcmp((UCHAR*)COMPLIST.Buffer,(UCHAR*)Line,COMPLIST.nLen) == 0 )
		{
			//////////////////////////////////////////////////////////////////////////
			// ֱ�����ɺ�ѡ��

			int num=1;
			int pos=0;
			int i=0;

			ENGINFO.strList[0][0]=(char)(num+48);
			ENGINFO.strList[0][1]=' ';

			pos=2;
			while(Line[i]!='\t')
			{
				ENGINFO.strList[0][pos]=Line[i];
				pos++;
				i++;
			}

			ENGINFO.strList[0][pos++]=' ';
			ENGINFO.strList[0][pos++]=' ';
			ENGINFO.strList[0][pos]='\0';

			ENGINFO.nListLen=1;
			num++;


			//���ɵڶ��Ժ�ĺ�ѡ��
			while (num <= CANDLIST.nPageSize && !feof(ENGINFO.fp) )
			{
				
				fgets(Line,100,ENGINFO.fp);
				
				ENGINFO.strList[num-1][0]=(char)(num+48);
				ENGINFO.strList[num-1][1]=' ';
				i=0;
				pos=2;
				while (Line[i] !='\t')
				{
					ENGINFO.strList[num-1][pos++]=Line[i++];
				}
				ENGINFO.strList[num-1][pos++]=' ';
				ENGINFO.strList[num-1][pos++]=' ';
				ENGINFO.strList[num-1][pos]='\0';
				
				num++;
			}

			ENGINFO.nListLen=num;
			
			return;
		}

	}
}
/************************************************************************/
/* 
/*  ����ʻ��ֿ⣬װ���ڴ�
/*	                                                                    */
/************************************************************************/
BOOL InitBHInfo()
{
	//��ʼ��ָ��
	BHINFO.LPBHTABLE[1]=BHINFO.BHTABLE1;
	BHINFO.LPBHTABLE[2]=BHINFO.BHTABLE2;
	BHINFO.LPBHTABLE[3]=BHINFO.BHTABLE3;
	BHINFO.LPBHTABLE[4]=BHINFO.BHTABLE4;
	BHINFO.LPBHTABLE[5]=BHINFO.BHTABLE5;
	
	
	LPBHTABLESTRUCT lpBHTable;
	//��ȡ�ļ���Ϣ
	int i;
	
	
	char FileNameBuff[20];			//�ļ���������
	for (i=1;i<6;i++)
	{
		sprintf(FileNameBuff,"DIC\\bhinfo%d.ini",i);
		FILE *fp=fopen(FileNameBuff,"r");
		if (!fp)
		{
			AssPrint("��ȡ�ʻ���Ϣ�ļ�%s ����!",FileNameBuff);
			return FALSE;
		}
		lpBHTable=BHINFO.LPBHTABLE[i];
		while (!feof(fp))
		{
			fscanf(fp,"%d%ld",&lpBHTable->nLen,&lpBHTable->lOffest);
			lpBHTable++;
		}
		
		fclose(fp);
	}
	
	return MapBHFile();
}
/************************************************************************/
/* 
/* ����ڻ���Ϣ�ļ�
/*                                                                     */
/************************************************************************/
BOOL MapBHFile()
{

		char DicNameBuff[20];
		
		for (int i=1;i<6;i++)
		{
			sprintf(DicNameBuff,"DIC\\bh%d.txt",i);
			BHINFO.fpBHDic[i]=fopen(DicNameBuff,"r");
			if (!BHINFO.fpBHDic[i])
			{
				return FALSE;
			}
		}
		return TRUE;
}

/************************************************************************/
/* 
/* �ر��Ѵ򿪵ıʻ��ֿ��ļ�
/*                                                                      */
/************************************************************************/
void CloseBHFile()
{
	for (int i=1;i<6;i++)
	{
		fclose(BHINFO.fpBHDic[i]);
	}
}

/************************************************************************/
/* 
/*	���ұʻ��ļ�ͷ���ҵ���Ӧ��ͷ���ҳ�����ȵ�ƫ�ƣ�
/*	fp: ��Ӧ���ֿ��ļ�
/*                                                                      */
/************************************************************************/
LONG SearchBH(FILE **fp)
{
	char cFirstChar=COMPLIST.Buffer[0];		//��һ�����������
	int i;
	switch(cFirstChar)
	{
	case '1':
		{
			for (i=0;i<36;i++)
			{
				if (BHINFO.BHTABLE1[i].nLen == COMPLIST.nLen)
				{
					*fp=BHINFO.fpBHDic[1];
					return BHINFO.BHTABLE1[i].lOffest;
					
				}
			}
			return -1;
		}
		break;
		
	case '2':
		{
			for (i=0;i<36;i++)
			{
				if (BHINFO.BHTABLE1[i].nLen == COMPLIST.nLen)
				{
					*fp=BHINFO.fpBHDic[2];
					return BHINFO.BHTABLE2[i].lOffest;
				}
			}
			return -1;
		}
		break;
		
	case '3':
		{
			
			for (i=0;i<36;i++)
			{
				if (BHINFO.BHTABLE1[i].nLen == COMPLIST.nLen)
				{
					*fp=BHINFO.fpBHDic[3];
					return BHINFO.BHTABLE3[i].lOffest;
				}
			}
			return -1;
		}
		break;
		
	case '4':
		{
			for (i=0;i<36;i++)
			{
				if (BHINFO.BHTABLE1[i].nLen == COMPLIST.nLen)
				{
					*fp=BHINFO.fpBHDic[4];
					return BHINFO.BHTABLE4[i].lOffest;
				}
			}
			return -1;
			
		}
		break;
		
	case '5':
		{
			for (i=0;i<36;i++)
			{
				if (BHINFO.BHTABLE1[i].nLen == COMPLIST.nLen)
				{
					*fp=BHINFO.fpBHDic[5];
					return BHINFO.BHTABLE5[i].lOffest;
				}
			}
			return -1;
		}
		break;
		
	default:
		break;
	}
	return -1;						//�Ҳ�����ͬ���� -1 
}

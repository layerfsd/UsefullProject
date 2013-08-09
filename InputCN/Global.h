#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <commctrl.h>
#include <Commdlg.h>
#include <vector>
#include<string>
//////////////////////////////////////////////////////////////////////////
//ȫ�ֽṹ��
//////////////////////////////////////////////////////////////////////////

//���봰��
typedef	struct _tagInputStruct
{
	SIZE	sz;
	POINT	pos;
	BOOL	bShow;
	
}INPUTSTRUCT;
//////////////////////////////////////////////////////////////////////////
//��ѡ�б�
typedef struct _tagCANDLIST 
{
	char	Buff[200];					//��ѡ���б�
	int		nPageSize;					//һҳ��Ŷ��ٸ�
	int		nLen;						//������
	DWORD	dwPageUp;					//���Ϸ�ҳ
	DWORD	dwPageDown;					//���·�ҳ

}CANDLISTSTRUCT;

//�����б�
typedef struct _tagCOMPLIST 
{
	char	Buffer[200];				//����ı���
	int		nCurPos;					//��ǰ�����ڱ��봮��λ��
	int		nLen;						//����ĳ���
}COMPLISTSTRUCT;

//////////////////////////////////////////////////////////////////////////
//
//ƴ��ӳ���ļ��ṹ
typedef struct _tagMapPY 
{
	char PinYin[8];
	long Offest;

}MapPY;

typedef struct _tagPYINFO 
{
	MapPY	MapFile[417];
	FILE	*fp;
	CHAR	strResult[2000];
	LPSTR	lpStr;

}PYINFOSTRUCT,*LPPYINFOSTRUCT;
//======================================
//����ļ���Ϣͷ
typedef struct _tagWBINFO 
{
	char FirstChar[2];
	long Offest;
	
}WBINFOSTRUCT,LPWBINFOSTRUCT;

//��ʽṹ
typedef struct _tagWB 
{
	WBINFOSTRUCT	MapWB[25];
	FILE			*fp;
	CHAR			strResult[200];

}WBINFOST;

//=====================================
//����
typedef struct _tagPYCZ 
{
	char	FirstChar[2];
	long	Offest;
}CZSTRUCT;

typedef struct _tagCIZU 
{
	CZSTRUCT	MapCZ[23];
	FILE		*fp;
	CHAR		strResult[300];
}CIZUINFOSTRUCT;


//========================================
//Ӣ��
typedef struct _tagEng 
{
	LONG		Offest[26];
	FILE		*fp;
	CHAR		strRestult[100];

	char		strList[10][25];		//Ӣ������ʱ�ĺ�ѡ�б�
	int			nListLen;				//�б���
}ENGSTRUCTINFO;


//�ڻ��ļ�ͷ��Ϣ
typedef struct _tagBHTABLE
{
	int		nLen;
	long	lOffest;
}BHTABLESTRUCT,* LPBHTABLESTRUCT;

//�ʻ���Ϣ
typedef struct _tagBHINFO 
{
	BHTABLESTRUCT	BHTABLE1[36];		//���ڴ��bh.ini�ļ���Ϣ���ڴ���
	BHTABLESTRUCT	BHTABLE2[36];
	BHTABLESTRUCT	BHTABLE3[36];
	BHTABLESTRUCT	BHTABLE4[36];
	BHTABLESTRUCT	BHTABLE5[36];
	LPBHTABLESTRUCT	LPBHTABLE[6];		//ָ��ǰ��5���ṹ��ָ��
	
	FILE			*fpBHDic[6];		//ָ��ʻ��ֿ���ļ�ָ��
}BHINFOSTRUCT,*LPBHINFOSTRUCT;

//////////////////////////////////////////////////////////////////////////
typedef struct  _tagSave 
{
	CHAR	szSave[1000];				//�������������
	int		nLen;						//����
	int		nPos;						//��ǰλ��
	fpos_t	fPos;						//�ļ�λ��
}SAVESTRUCT;
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���

extern	HINSTANCE		hInst;			//��ǰ�߳�ʵ��
extern	HWND			hStatus;		//״̬���ھ��
extern	HWND			hInput;			//���봰�ھ��
extern	HWND			hComp;			//���봰��
extern	HWND			hCand;			//��ѡ�б���
extern	HWND			hForeWnd;		//��ǰӵ�й��Ĵ���
//////////////////////////////////////////////////////////////////////////

extern	DWORD			KeyState;		//��ǰ��Ϣ״̬
extern	DWORD			CurMode;		//��ǰ����ģʽ
extern	DWORD			ResultMode;		//���ģʽ
extern	DWORD			dwType;

extern	PYINFOSTRUCT	PYINFO;			//ƴ���ṹ
extern	WBINFOST		WBINFO;			//��ʽṹ
extern	CIZUINFOSTRUCT	CIZUINFO;		//����ṹ
extern	ENGSTRUCTINFO	ENGINFO;		//Ӣ��ṹ
extern	BHINFOSTRUCT	BHINFO;	


extern	INPUTSTRUCT		INPUTWNDSTRUCT;	//���봰������
extern	CANDLISTSTRUCT	CANDLIST;		//��ѡ�����б�
extern	COMPLISTSTRUCT	COMPLIST;		//����ṹ

extern	SAVESTRUCT		SAVE;	

extern	CHOOSEFONT		cf; 
extern	LOGFONT			lf; 		

		
//////////////////////////////////////////////////////////////////////////
		
#define			HANDLE_CHAR			0x01		//��ͨ�ַ�״̬
#define			HANDLE_NUM			0x02		//����״̬
#define			HANDLE_SPACE		0x04		//�ո�
#define			HANDLE_BACK			0x08		//ɾ��
#define			HANDLE_UP			0x010		//����
#define			HANDLE_DOWN			0x040		//����
#define			HANDLE_LEFT			0x080		//����
#define			HANDLE_RIGHT		0x0100		//����
#define			HANDLE_SMALL_NUM	0x0400		//С����

#define			MODE_PINYIN			0x01		//ƴ��ģʽ
#define			MODE_BIHUA			0x02		//�ʻ�
#define			MODE_WUBI			0x04		//���
#define			MODE_ENGLIS			0x08		//Ӣ��
#define			MODE_CIZU			0x010		//ƴ������
#define			MODE_FULL			0x020		//���ģʽ

#endif


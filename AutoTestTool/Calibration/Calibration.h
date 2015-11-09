
// Calibration.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#define CAPTURE_NUM 1000
#define MODULENAMEMAXLEN 64


// Enumeration used for distinguishing different events.
enum MyEvents
{
	eMyExposureEndEvent,      // Triggered by a camera event.
	eMyFrameStartOvertrigger, // Triggered by a camera event.
	eMyImageReceivedEvent,    // Triggered by the receipt of an image.
	eMyMoveEvent,             // Triggered when the imaged item or the sensor head can be moved.
	eMyNoEvent                // Used as default setting.
};

typedef struct _BITMAPINFO_8bpp
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[256];
} BITMAPINFO_8bpp;

typedef enum TestModule{
    MODULE_INITSETTING = 0,
    MODULE_PM,
    MODULE_DRAG,
    MODULE_MOVE,
    MODULE_TRIGGER,
    MODULE_ROTATE,
    MODULE_DELAY,
}TestModule;

typedef struct ModuleInitSet{
    char cScriptFile[MAX_PATH];
}ModuleInitSet;

typedef struct ModulePM{
    char cPMTemplate[MAX_PATH];
}ModulePM;

typedef struct ModuleFingerMove{
    int nFingerType;                // 0 ������ָ 1 �����ָ
    int nMode;                      // 0 ����ʶ��ƥ�����ƶ�������X��Y������ 1 ָ��Ŀ��λ�ã�����X��Y������
    int nMoveToX;
    int nMoveToY;
	int nClick;						// �ƶ���ָ��λ�ú���ָ�Ƿ���
	int nFrequency;
	int nTimes;
}ModuleFingerMove;

typedef struct ModuleIconDrag{
    int nMode;                      // 0 ����ʶ��ƥ����ȷ��ͼ���ʼλ�� 2 ָ����ʼλ��
    int nStartX;
    int nStartY;
    int nEndX;
    int nEndY;
	int nTimeBefore;
	int nTimeAfter;
}ModuleIconDrag;

typedef struct ModuleTriggerMode{
    int nTriggerIndex;              // �����������
	int nTriggerMode;				// 0 �������� 1 ��������
	int nTimeDelay;
	int nWaveFrq;
	int nWaveCnt;
	int nDBClkFrq;
	int nDBClkCnt;
}ModuleTriggerMode;

typedef struct ModuleDelay{
    int nDelayTime;                // ��ʱʱ�䵥λ����           
}ModuleDelay;

typedef struct ModuleRotate{
	int nAntiWise;					// �Ƿ���ʱ����ת
    int nRotateAngle;				// 0~1800
}ModuleRotate;

typedef struct ModuleData{
    int nType;
    char cName[MODULENAMEMAXLEN];
    int nSize;
    void *pData;
	ModuleData *pNext;
}ModuleData;

// CCalibrationApp:
// �йش����ʵ�֣������ Calibration.cpp
//

class CCalibrationApp : public CWinApp
{
public:
	CCalibrationApp();

    BOOL DeleteDirectory(char* psDirName);

	char m_szConfigFile[MAX_PATH];
	char m_imageDirectory[MAX_PATH];
	char m_szUserCaseFile[MAX_PATH];

	int m_nUserCaseCount;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCalibrationApp theApp;
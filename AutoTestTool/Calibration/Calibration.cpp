
// Calibration.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Calibration.h"
#include "CalibrationDlg.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalibrationApp

BEGIN_MESSAGE_MAP(CCalibrationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCalibrationApp ����

CCalibrationApp::CCalibrationApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCalibrationApp ����

CCalibrationApp theApp;


// CCalibrationApp ��ʼ��

BOOL CCalibrationApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
    AfxInitRichEdit2();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	GetModuleFileName(NULL, m_szConfigFile, MAX_PATH);
	(strrchr(m_szConfigFile, '\\'))[1] = 0;         //ɾ���ļ�����ֻ���·��
	strcpy(m_szUserCaseFile, m_szConfigFile);
	strcat(m_szConfigFile, "config.ini");
    TCHAR chTemp[MAX_PATH];
    GetPrivateProfileString("COMMON", "CASEPATH", "", chTemp, MAX_PATH-1, m_szConfigFile);
    DWORD dwAttr = GetFileAttributes(chTemp);
    if(dwAttr == 0xFFFFFFFF){
        strcat(m_szUserCaseFile, "UserCase");
        DWORD dwAttr = GetFileAttributes(m_szUserCaseFile);
        if(dwAttr == 0xFFFFFFFF){						//���ļ��в����ڣ������ļ���
            CreateDirectory(m_szUserCaseFile, NULL);
        }else if(dwAttr & FILE_ATTRIBUTE_DIRECTORY){	//�ļ��д���
            //do something
        }
    }else{
	    strcpy(m_szUserCaseFile, chTemp);
    }
    if(theApp.m_szUserCaseFile[strlen(theApp.m_szUserCaseFile)-1] == '\\')
        strcat(theApp.m_szUserCaseFile, "usercase.ini");
    else
        strcat(theApp.m_szUserCaseFile, "\\usercase.ini");

	/*CString strPath;
	GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
	strPath.ReleaseBuffer();*/
	GetCurrentDirectory(MAX_PATH, m_imageDirectory);
	strcat(m_imageDirectory, "\\images");
	dwAttr = GetFileAttributes(m_imageDirectory);
	if(dwAttr == 0xFFFFFFFF){						//���ļ��в����ڣ������ļ���
		CreateDirectory(m_imageDirectory, NULL);
	}else if(dwAttr & FILE_ATTRIBUTE_DIRECTORY){	//�ļ��д���
		//do something
	}

    CSplashWnd::EnableSplashScreen(TRUE);
    CSplashWnd::ShowSplashScreen();
    CCalibrationDlg dlg;
    dlg.PointGreyCameraInit();
	dlg.BaslerCameraInit();
	/*if(!dlg.LedPannelInit())
		AfxMessageBox("LED pannel init failed");*/
    CSplashWnd::HideSplashScreen();

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CCalibrationApp::DeleteDirectory(char* psDirName) 
{ 
    CFileFind tempFind; 
    char sTempFileFind[ _MAX_PATH ] = { 0 }; 
    sprintf(sTempFileFind,"%s//*.*",psDirName); 
    BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
    while (IsFinded) 
    { 
        IsFinded = tempFind.FindNextFile(); 
        if (!tempFind.IsDots())
        { 
            char sFoundFileName[ _MAX_PATH ] = { 0 }; 
            strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 
            if (tempFind.IsDirectory()) 
            { 
                char sTempDir[ _MAX_PATH ] = { 0 }; 
                sprintf(sTempDir,"%s//%s",psDirName,sFoundFileName); 
                DeleteDirectory(sTempDir); 
            } 
            else 
            { 
                char sTempFileName[ _MAX_PATH ] = { 0 };
                sprintf(sTempFileName,"%s//%s",psDirName,sFoundFileName); 
                DeleteFile(sTempFileName); 
            } 
        } 
    } 
    tempFind.Close(); 
    if(!RemoveDirectory(psDirName)) 
    { 
        return FALSE; 
    } 
    return TRUE; 
}
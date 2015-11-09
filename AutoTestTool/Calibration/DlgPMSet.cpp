// DlgPMSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Calibration.h"
#include "DlgPMSet.h"
#include "afxdialogex.h"

#include "CalibrationDlg.h"
#include "libControl.h"

#include "DlgTemplate.h"

// CDlgPMSet �Ի���

IMPLEMENT_DYNAMIC(CDlgPMSet, CDialogEx)

CDlgPMSet::CDlgPMSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPMSet::IDD, pParent)
{
}

CDlgPMSet::~CDlgPMSet()
{
}

void CDlgPMSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK_ENABLEROI, m_CkEnableROI);
    DDX_Control(pDX, IDC_DEV_IPADDR, m_DevIpAddr);
    DDX_Control(pDX, IDC_CK_LASER, m_CkOpenLaser);
    DDX_Control(pDX, IDC_EDIT_CLICESPEED, m_EditClkSpeed);
}


BEGIN_MESSAGE_MAP(CDlgPMSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETPREPOS, &CDlgPMSet::OnBnClickedBtnSetprepos)
	ON_BN_CLICKED(IDC_BTN_CAMASET, &CDlgPMSet::OnBnClickedBtnCamaset)
    ON_BN_CLICKED(IDC_BTN_SETROI, &CDlgPMSet::OnBnClickedBtnSetroi)
    ON_BN_CLICKED(IDC_CHECK_ENABLEROI, &CDlgPMSet::OnBnClickedCheckEnableroi)
    ON_BN_CLICKED(IDOK, &CDlgPMSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CK_LASER, &CDlgPMSet::OnBnClickedCkLaser)
	ON_BN_CLICKED(IDCANCEL, &CDlgPMSet::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BTN_SETPATH, &CDlgPMSet::OnBnClickedBtnSetpath)
END_MESSAGE_MAP()


// CDlgPMSet ��Ϣ�������


BOOL CDlgPMSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	TCHAR chTemp[MAX_PATH];
	GetPrivateProfileString("ROBOT", "IPADDR", "192.168.1.144", chTemp, sizeof(chTemp), theApp.m_szConfigFile);
	m_DevIpAddr.SetAddress(ntohl(inet_addr(chTemp)));

	LONG lPrePosX = GetPrivateProfileInt("COMMON", "PREPOSX", -1, theApp.m_szConfigFile);
	LONG lPrePosY = GetPrivateProfileInt("COMMON", "PREPOSY", -1, theApp.m_szConfigFile);
    LONG lPrePosZ = GetPrivateProfileInt("COMMON", "PREPOSZ", -1, theApp.m_szConfigFile);
	SetDlgItemInt(IDC_EDIT_PREPOSX, lPrePosX);
	SetDlgItemInt(IDC_EDIT_PREPOSY, lPrePosY);
    SetDlgItemInt(IDC_EDIT_PREPOSZ, lPrePosZ);

    int nValue = GetPrivateProfileInt("POINTGREY", "ENABLEROI", 0, theApp.m_szConfigFile);
    m_CkEnableROI.SetCheck(nValue);
    GetDlgItem(IDC_BTN_SETROI)->EnableWindow(nValue);

    nValue = GetPrivateProfileInt("ROBOT", "CLICKSPEED", 5, theApp.m_szConfigFile);
    SetDlgItemInt(IDC_EDIT_CLICESPEED, nValue);

    strcpy(m_pszPath, theApp.m_szUserCaseFile);
    (strrchr(m_pszPath, '\\'))[1] = 0;         //ɾ���ļ�����ֻ���·��
    SetDlgItemText(IDC_EDIT_CASEPATH, m_pszPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgPMSet::OnBnClickedBtnSetprepos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	LONG lPosX;
	LONG lPosY;
	LONG lPosZ;
	INT nStatus;
	if(g_pMainDlg->m_lLoginID > 0){
		BOOL bGetPos = CONTROL_QueryPos(g_pMainDlg->m_lLoginID, &lPosX, &lPosY, &nStatus, &lPosZ);
		if(bGetPos){
			g_pMainDlg->m_lPrePosX = lPosX;
			str.Format("%d", lPosX);
			WritePrivateProfileString("COMMON", "PREPOSX", str.GetBuffer(0), theApp.m_szConfigFile);
			g_pMainDlg->m_lPrePosY = lPosY;
			str.Format("%d", lPosY);
			WritePrivateProfileString("COMMON", "PREPOSY", str.GetBuffer(0), theApp.m_szConfigFile);
			g_pMainDlg->m_lPrePosZ = lPosZ;
			str.Format("%d", lPosZ);
			WritePrivateProfileString("COMMON", "PREPOSZ", str.GetBuffer(0), theApp.m_szConfigFile);

			SetDlgItemInt(IDC_EDIT_PREPOSX, lPosX);
			SetDlgItemInt(IDC_EDIT_PREPOSY, lPosY);
            SetDlgItemInt(IDC_EDIT_PREPOSZ, lPosZ);
		}else{
			MessageBox("����Ԥ��λʧ��");
		}
	}else{
		MessageBox("�������ӻ�����");
	}
}


void CDlgPMSet::OnBnClickedBtnCamaset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_pMainDlg){
		if(g_pMainDlg->m_Inited >= 2){
			if( g_pMainDlg->m_camCtlDlg.IsVisible() == true )
			{
				g_pMainDlg->m_camCtlDlg.Hide();
			}
			else
			{
				g_pMainDlg->m_camCtlDlg.Show();
			}
		}else{
			MessageBox("ʶ�����δ��ʼ����");
		}
	}
}

void CDlgPMSet::OnBnClickedBtnSetroi()
{
    // TODO: Add your control notification handler code here
    CRect rect;
    g_pMainDlg->m_VideoWnd.GetWindowRect(&rect);
    CString str;
    int nROILeft, nROIWidth, nROITop, nROIHeight;
    CDlgTemplate templateDlg;
    templateDlg.m_Rect = rect;
    templateDlg.DoModal();

    if(!templateDlg.m_bMakeTemplate)
        return;
    
    templateDlg.m_TemplateRectTracker.GetTrueRect(&rect);
    if(rect.left < 0)
        rect.left = 0;
    nROILeft = rect.left/g_pMainDlg->m_dScale;
    nROIWidth = rect.Width()/g_pMainDlg->m_dScale;
    if(nROIWidth > g_pMainDlg->m_nCamWidth)
        nROIWidth = g_pMainDlg->m_nCamWidth;
    if(rect.top < 0)
        rect.top = 0;
    nROITop = rect.top/g_pMainDlg->m_dScale;
    nROIHeight = rect.Height()/g_pMainDlg->m_dScale;
    if(nROIHeight > g_pMainDlg->m_nCamHeight)
        nROIHeight = g_pMainDlg->m_nCamHeight;
    TRACE("left %d width %d top %d height %d", nROILeft, nROIWidth, nROITop, nROIHeight);
    str.Format("%d", nROILeft);
    WritePrivateProfileString("POINTGREY", "ROILEFT", str.GetBuffer(0), theApp.m_szConfigFile);
    str.Format("%d", nROIWidth);
    WritePrivateProfileString("POINTGREY", "ROIWIDTH", str.GetBuffer(0), theApp.m_szConfigFile);
    str.Format("%d", nROITop);
    WritePrivateProfileString("POINTGREY", "ROITOP", str.GetBuffer(0), theApp.m_szConfigFile);
    str.Format("%d", nROIHeight);
    WritePrivateProfileString("POINTGREY", "ROIHEIGHT", str.GetBuffer(0), theApp.m_szConfigFile);
}


void CDlgPMSet::OnBnClickedCheckEnableroi()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_BTN_SETROI)->EnableWindow(m_CkEnableROI.GetCheck());
}


void CDlgPMSet::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
	CString str;
	m_DevIpAddr.GetWindowText(str);
	WritePrivateProfileString("ROBOT", "IPADDR", str.GetBuffer(0), theApp.m_szConfigFile);

    if(m_CkEnableROI.GetCheck()){
        WritePrivateProfileString("POINTGREY", "ENABLEROI", "1", theApp.m_szConfigFile);
        g_pMainDlg->m_nEnableROI = 1;
    }else{
        WritePrivateProfileString("POINTGREY", "ENABLEROI", "0", theApp.m_szConfigFile);
        g_pMainDlg->m_nEnableROI = 0;
    }

	if(m_CkOpenLaser.GetCheck()){
		WritePrivateProfileString("ROBOT", "LASEROPEN", "1", theApp.m_szConfigFile);
	}else{
		WritePrivateProfileString("ROBOT", "LASEROPEN", "0", theApp.m_szConfigFile);
	}

    m_EditClkSpeed.GetWindowText(str);
    WritePrivateProfileString("ROBOT", "CLICKSPEED", str.GetBuffer(0), theApp.m_szConfigFile);

    strcpy(theApp.m_szUserCaseFile, m_pszPath);
    WritePrivateProfileString("COMMON", "CASEPATH", m_pszPath, theApp.m_szConfigFile);
    if(theApp.m_szUserCaseFile[strlen(theApp.m_szUserCaseFile)-1] == '\\')
        strcat(theApp.m_szUserCaseFile, "usercase.ini");
    else
        strcat(theApp.m_szUserCaseFile, "\\usercase.ini");
    g_pMainDlg->InitUserCase();

	if(g_pMainDlg->IsWindowEnabled()){
		DestroyWindow();
		::SendMessage(g_pMainDlg->m_hWnd, PMSETDLG_EXIT, 0, 0);
	}else{
		CDialogEx::OnOK();
	}
}


void CDlgPMSet::OnBnClickedCkLaser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_CkOpenLaser.GetCheck()){
		CONTROL_Extend(g_pMainDlg->m_lLoginID, LASER_OPEN, 0);
	}else{
		CONTROL_Extend(g_pMainDlg->m_lLoginID, LASER_CLOSE, 0);
	}
}


void CDlgPMSet::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_pMainDlg->IsWindowEnabled()){
		DestroyWindow();
		::SendMessage(g_pMainDlg->m_hWnd, PMSETDLG_EXIT, 0, 0);
	}else{
		CDialogEx::OnCancel();
	}
}

//ѡ���ļ��жԻ���ص�����  
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
{
    switch(uMsg)
    {
    case BFFM_INITIALIZED:  //ѡ���ļ��жԻ����ʼ��
        //����Ĭ��·��ΪlpData��'D:\'
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        //��STATUSTEXT������ʾ��ǰ·��
        ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);
        //����ѡ���ļ��жԻ���ı���  
        ::SetWindowText(hwnd, TEXT("�����ò�����������Ŀ¼"));
        break;
    case BFFM_SELCHANGED:   //ѡ���ļ��б��ʱ
        {
            TCHAR pszPath[MAX_PATH];
            //��ȡ��ǰѡ��·��
            SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);
            //��STATUSTEXT������ʾ��ǰ·��
            ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);
        }
        break;
    }
    return 0;
} 

void CDlgPMSet::OnBnClickedBtnSetpath()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    BROWSEINFO bi; 
    bi.hwndOwner      = this->GetSafeHwnd();
    bi.pidlRoot       = NULL;
    bi.pszDisplayName = NULL; 
    bi.lpszTitle      = TEXT("��ѡ���ļ���"); 
    bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;//BIF_USENEWUI;//
    bi.lpfn           = BrowseCallBackFun; 
    bi.lParam         = 0;          // (LPARAM)TEXT("D:\\");  //�����ص������Ĳ���,����Ĭ��·��
    bi.iImage         = 0; 

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl == NULL)
    {
        return;
    }

    if (SHGetPathFromIDList(pidl, m_pszPath))
    {
        //AfxMessageBox(pszPath);
        SetDlgItemText(IDC_EDIT_CASEPATH, m_pszPath);
    }
}

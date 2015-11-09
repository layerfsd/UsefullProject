// DlgTimeDelay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Calibration.h"
#include "DlgTimeDelay.h"
#include "afxdialogex.h"
#include "CalibrationDlg.h"

// CDlgTimeDelay �Ի���

IMPLEMENT_DYNAMIC(CDlgTimeDelay, CDialogEx)

CDlgTimeDelay::CDlgTimeDelay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTimeDelay::IDD, pParent)
{

}

CDlgTimeDelay::~CDlgTimeDelay()
{
}

void CDlgTimeDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTimeDelay, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgTimeDelay::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDlgTimeDelay::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTimeDelay ��Ϣ�������


BOOL CDlgTimeDelay::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    ModuleDelay *pTimeDelay = (ModuleDelay *)m_pModuleData->pData;
    SetDlgItemInt(IDC_EDIT_TIMEDELAY, pTimeDelay->nDelayTime);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTimeDelay::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ModuleDelay *pTimeDelay = (ModuleDelay *)m_pModuleData->pData;
    pTimeDelay->nDelayTime = GetDlgItemInt(IDC_EDIT_TIMEDELAY);
    if(g_pMainDlg->IsWindowEnabled()){
        DestroyWindow();
        ::SendMessage(g_pMainDlg->m_hWnd, TIMEDELAY_EXIT, 0, 0);
    }else{
        CDialogEx::OnOK();
    }
}


void CDlgTimeDelay::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(g_pMainDlg->IsWindowEnabled()){
        DestroyWindow();
        ::SendMessage(g_pMainDlg->m_hWnd, TIMEDELAY_EXIT, 0, 0);
    }else{
        CDialogEx::OnCancel();
    }
}

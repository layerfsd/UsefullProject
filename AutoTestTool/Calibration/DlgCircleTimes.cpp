// DlgCircleTimes.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Calibration.h"
#include "DlgCircleTimes.h"
#include "afxdialogex.h"
#include "CalibrationDlg.h"

// CDlgCircleTimes �Ի���

IMPLEMENT_DYNAMIC(CDlgCircleTimes, CDialogEx)

CDlgCircleTimes::CDlgCircleTimes(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCircleTimes::IDD, pParent)
{

}

CDlgCircleTimes::~CDlgCircleTimes()
{
}

void CDlgCircleTimes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCircleTimes, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCircleTimes::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCircleTimes::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgCircleTimes ��Ϣ�������


BOOL CDlgCircleTimes::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemInt(IDC_EDIT_CIRCLETIMES, m_nTimes);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgCircleTimes::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_pMainDlg->IsWindowEnabled()){
		m_nTimes = GetDlgItemInt(IDC_EDIT_CIRCLETIMES);
		DestroyWindow();
		::SendMessage(g_pMainDlg->m_hWnd, CIRCLETIMES_EXIT, 0, 0);
	}else{
		CDialogEx::OnOK();
	}
}


void CDlgCircleTimes::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_pMainDlg->IsWindowEnabled()){
		DestroyWindow();
		::SendMessage(g_pMainDlg->m_hWnd, CIRCLETIMES_EXIT, 0, 0);
	}else{
		CDialogEx::OnCancel();
	}
}

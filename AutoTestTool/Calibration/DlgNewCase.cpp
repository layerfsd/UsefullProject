// DlgNewCase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Calibration.h"
#include "DlgNewCase.h"
#include "afxdialogex.h"


// CDlgNewCase �Ի���

IMPLEMENT_DYNAMIC(CDlgNewCase, CDialogEx)

CDlgNewCase::CDlgNewCase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgNewCase::IDD, pParent)
{

}

CDlgNewCase::~CDlgNewCase()
{
}

void CDlgNewCase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNewCase, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgNewCase::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewCase ��Ϣ�������


BOOL CDlgNewCase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	str.Format("UserCase%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	SetDlgItemText(IDC_EDIT_CASENAME, str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgNewCase::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT_CASENAME, m_UserCaseName);
	if(m_UserCaseName.IsEmpty()){
		MessageBox("User case name should not be empty!", MB_OK);
		return;
	}else{
		// �жϸ��������Ƿ���� ������
	}

	CDialogEx::OnOK();
}

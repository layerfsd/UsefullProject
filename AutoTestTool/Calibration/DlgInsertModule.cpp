// DlgInsertModule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Calibration.h"
#include "DlgInsertModule.h"
#include "afxdialogex.h"


// CDlgInsertModule �Ի���

IMPLEMENT_DYNAMIC(CDlgInsertModule, CDialogEx)

CDlgInsertModule::CDlgInsertModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInsertModule::IDD, pParent)
{

}

CDlgInsertModule::~CDlgInsertModule()
{
}

void CDlgInsertModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInsertModule, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgInsertModule::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInsertModule ��Ϣ�������


BOOL CDlgInsertModule::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    CheckRadioButton(IDC_RADIO1, IDC_RADIO7, IDC_RADIO1);
    m_nCheckRadio = 0;

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgInsertModule::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_nCheckRadio = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO7);
    m_nCheckRadio -= IDC_RADIO1;
    CDialogEx::OnOK();
}

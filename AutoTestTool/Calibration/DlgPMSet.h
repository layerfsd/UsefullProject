#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CDlgPMSet �Ի���

class CDlgPMSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPMSet)

public:
	CDlgPMSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPMSet();

    TCHAR m_pszPath[MAX_PATH];

// �Ի�������
	enum { IDD = IDD_DLG_PMSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSetprepos();
	afx_msg void OnBnClickedBtnCamaset();
    afx_msg void OnBnClickedBtnSetroi();
    afx_msg void OnBnClickedCheckEnableroi();
    CButton m_CkEnableROI;
    afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_DevIpAddr;
	afx_msg void OnBnClickedCkLaser();
	CButton m_CkOpenLaser;
	afx_msg void OnBnClickedCancel();
    CEdit m_EditClkSpeed;
    afx_msg void OnBnClickedBtnSetpath();
};

#pragma once


// CDlgNewCase �Ի���

class CDlgNewCase : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNewCase)

public:
	CDlgNewCase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewCase();

	CString m_UserCaseName;

// �Ի�������
	enum { IDD = IDD_DLG_NEWCASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

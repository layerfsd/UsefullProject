#pragma once


// CDlgCircleTimes �Ի���

class CDlgCircleTimes : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCircleTimes)

public:
	CDlgCircleTimes(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCircleTimes();

	int m_nTimes;

// �Ի�������
	enum { IDD = IDD_DLG_CIRCLETIMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

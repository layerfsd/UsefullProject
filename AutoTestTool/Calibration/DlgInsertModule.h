#pragma once


// CDlgInsertModule �Ի���

class CDlgInsertModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInsertModule)

public:
	CDlgInsertModule(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInsertModule();

    int m_nCheckRadio;

// �Ի�������
	enum { IDD = IDD_DLG_INSERTMODULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
};

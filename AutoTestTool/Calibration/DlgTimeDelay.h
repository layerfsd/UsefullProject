#pragma once


// CDlgTimeDelay �Ի���

class CDlgTimeDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTimeDelay)

public:
	CDlgTimeDelay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTimeDelay();

    ModuleData *m_pModuleData;

// �Ի�������
	enum { IDD = IDD_DLG_TIMEDELAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CDlgRobotSet �Ի���
class CDlgRobotSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRobotSet)

public:
	CDlgRobotSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRobotSet();

// �Ի�������
	enum { IDD = IDD_DLG_ROBOTSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CIPAddressCtrl m_RobotIpAddr;
	CEdit m_EditUpX;
	CEdit m_EditUpY;
	CEdit m_EditDownX;
	CEdit m_EditDownY;
	CEdit m_EditLeftX;
	CEdit m_EditLeftY;
	CEdit m_EditRightX;
	CEdit m_EditRightY;
	CEdit m_EditClkSpeed;
	CEdit m_EditClkTimes;
	CEdit m_EditDBClkFreq;
	CEdit m_EditDBClkDuration;
	CButton m_CkOpenLaser;
	afx_msg void OnBnClickedCkLaser();
	afx_msg void OnBnClickedBtnSetmark();
	afx_msg void OnBnClickedCancel();
	CEdit m_EditTimeBefore;
	CEdit m_EditTimeAfter;
	CButton m_CkDragIcon;
};

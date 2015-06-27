
// MainSysDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainSys.h"
#include "MainSysDlg.h"

#include "Mail/CMail.h"
#include "Utils.h"
#include "H264ToMP4Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ATOM nShowDlg;
ATOM nHideDlg;
ATOM nShowDlg2;

char g_dbgInfo[500];

void CapScreen(char filename[]);
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMainSysDlg �Ի���


CMainSysDlg::CMainSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainSysDlg::IDD, pParent)
	, m_strMailServer(_T("pop.163.com"))
	, m_strUserid(_T("15210882403"))
	, m_strPassword(_T("yanda19841216"))
	, m_strBody(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pDesktopSender = NULL;
    m_pDesktopRecorder = NULL;

    m_bListening = FALSE;
    m_sListen = INVALID_SOCKET;
    m_bProcessCur = FALSE;
}

void CMainSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAILS, m_ctlMessagesList);
	DDX_Text(pDX, IDC_EDIT_POPSERV, m_strMailServer);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strUserid);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_BODY, m_strBody);
}

BEGIN_MESSAGE_MAP(CMainSysDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SEND, &CMainSysDlg::OnBnClickedBtnSend)
    ON_BN_CLICKED(IDC_BTN_TEST, &CMainSysDlg::OnBnClickedBtnTest)
    ON_WM_NCPAINT()
	ON_BN_CLICKED(IDC_BTN_RECEIVE, &CMainSysDlg::OnBnClickedBtnReceive)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MAILS, &CMainSysDlg::OnLvnItemchangedListMails)
    ON_BN_CLICKED(IDC_BTN_TEST2, &CMainSysDlg::OnBnClickedBtnTest2)
    ON_BN_CLICKED(IDC_BTN_STARTCAPDESKTOP, &CMainSysDlg::OnBnClickedBtnStartcapdesktop)
    ON_BN_CLICKED(IDC_BTN_STOPCAPDESKTOP, &CMainSysDlg::OnBnClickedBtnStopcapdesktop)
    ON_BN_CLICKED(IDC_BTN_STARTRECDESKTOP, &CMainSysDlg::OnBnClickedBtnStartrecdesktop)
    ON_BN_CLICKED(IDC_BTN_STOPRECDESKTOP, &CMainSysDlg::OnBnClickedBtnStoprecdesktop)
    ON_BN_CLICKED(IDC_BTN_TRANSFORMH264, &CMainSysDlg::OnBnClickedBtnTransformh264)
END_MESSAGE_MAP()

DWORD WINAPI ServListenFunc(LPVOID pServParam)
{
    CMainSysDlg *pMainDlg = (CMainSysDlg*)pServParam;
    sockaddr_in remoteAddr = {0};
    int nAddrLen = sizeof(remoteAddr);
    char buf[1024] = {0};

    SOCKET sClient = INADDR_ANY;
    int ret;
    while (pMainDlg->m_bListening) {
        sClient = accept(pMainDlg->m_sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
        if (sClient == INVALID_SOCKET) {
            OutputDebugStringA("accept failed!");
            Sleep(100);
            continue;
        }
        OutputDebugStringA("�µ���λ������\r\n\r\n");
        pMainDlg->m_bProcessCur = TRUE;

        int nCapNum = 0;
        while(pMainDlg->m_bProcessCur){
            ret = recv(sClient, buf, sizeof(buf), 0);
            if(ret == 0 || ret == SOCKET_ERROR ){
                int nErr = WSAGetLastError();
                if(nErr == WSAECONNRESET || nErr == WSAENOTSOCK || nErr == WSAECONNABORTED || nErr == ERROR_SUCCESS) {
                    sprintf(g_dbgInfo, "��λ���Ͽ�����\r\n\r\n");
                    OutputDebugStringA(g_dbgInfo);
                    pMainDlg->m_bProcessCur = FALSE;
                    //pMainDlg->m_bWorkRun = FALSE;
                    break;
                }
                TRACE("Recv data error: %d\n", nErr);
                Sleep(10);
                continue;
            }

            if(strstr(buf, "CapScreen")){
                sprintf(g_dbgInfo, "netCommad%d.bmp", nCapNum++);
                CapScreen(g_dbgInfo);
            }

            OutputDebugStringA(buf);
        }
    }

    return 1;
}

int CMainSysDlg::startServer()
{
    sockaddr_in sin  = {0};
    sockaddr_in remoteAddr = {0};
    char szText[] = "TCP Server Demo";
    int nAddrLen = 0;
    int nRet;

    nAddrLen = sizeof(sockaddr_in);
    //fill sin
#if 0
    sin.sin_port = htons(65521);			// ��˾
#else
	sin.sin_port = htons(555);			// ��������
#endif
    sin.sin_family = AF_INET;
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    //sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    m_sListen = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sListen == INVALID_SOCKET){
        MessageBox(_T("����socketʧ��"));
        return 0;
    }

    char chReuse = 1;
    nRet = setsockopt(m_sListen, SOL_SOCKET, SO_REUSEADDR, &chReuse, sizeof(chReuse));
    if(nRet == SOCKET_ERROR)
    {
        char temp[100];
        sprintf(temp, "setsockopt error %d", WSAGetLastError());
        OutputDebugStringA(temp);
    }

    int optval = 1;
    //setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, (char *)(&optval), sizeof(optval));
    if (bind(m_sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        MessageBox(_T("�󶨼���socketʧ��"));
        return 0;
    }

    if (listen(m_sListen, 5) == SOCKET_ERROR)
    {
        MessageBox(_T("listen failed!"));
        return 0;
    }
    m_bListening = TRUE;

    HANDLE handl = CreateThread(NULL, 0, ServListenFunc, this, 0, NULL);
    if(handl) {
        CloseHandle(handl);
        return 1;
    }else{
        m_bListening = FALSE;
        return 0;
    }
}

// CMainSysDlg ��Ϣ�������

BOOL CMainSysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    nShowDlg = GlobalAddAtom(_T("ShowDlg"));     
    nHideDlg = GlobalAddAtom(_T("HideDlg")); 
    BOOL nRes = RegisterHotKey(NULL,nShowDlg,MOD_CONTROL | MOD_ALT,VK_NUMPAD0);// ctrl+alt+0(С���̵�0)
    nRes = RegisterHotKey(NULL,nHideDlg,MOD_CONTROL | MOD_ALT,VK_NUMPAD1); //ctrl+alt+1(С���̵�1)
    nRes = RegisterHotKey(NULL,nShowDlg2,MOD_CONTROL | MOD_ALT, VK_F1);

	CUtils utils;
	CString title = utils.GetIPAddress();
	SetWindowText(_T("MailSys-"+title));

    m_ctlMessagesList.SetExtendedStyle(m_ctlMessagesList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_ctlMessagesList.InsertColumn(0, _T("������"), LVCFMT_LEFT, 120);
	m_ctlMessagesList.InsertColumn(1, _T("����"), LVCFMT_LEFT, 350);
	m_ctlMessagesList.InsertColumn(2, _T("����"), LVCFMT_LEFT, 100);
    m_nMailCount = 0;

    startServer();
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMainSysDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainSysDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMainSysDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainSysDlg::OnBnClickedBtnSend()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    string str = "this is from MainSys";
    CMail mail;
    mail.SendMail("smtp.163.com","15210882403@163.com","yanda19841216","284454806@qq.com","�����ʼ�", str);
}

BYTE *image_buffer; //ָ��λͼbuffer��ȫ��ָ�룬window�����ظ�ʽ: BGRA(4���ֽ�)

void CapScreen(char filename[])
{
    CDC *pDC;
    pDC = CDC::FromHandle(GetDC(GetDesktopWindow()));
    if(pDC == NULL) return;
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);

    CDC memDC;
    if(memDC.CreateCompatibleDC(pDC) == 0) return;

    CBitmap memBitmap, *oldmemBitmap;
    if(memBitmap.CreateCompatibleBitmap(pDC, Width, Height) == NULL) return;

    oldmemBitmap = memDC.SelectObject(&memBitmap);
    if(oldmemBitmap == NULL) return;
    if(memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY) == 0) return;

    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);

    FILE *fp = fopen(filename, "w+b");

    BITMAPINFOHEADER bih = {0};
    bih.biBitCount = bmp.bmBitsPixel;
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;
    bih.biWidth = bmp.bmWidth;

    BITMAPFILEHEADER bfh = {0};
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;
    bfh.bfType = (WORD)0x4d42;

    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);

    image_buffer = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];

    GetDIBits(memDC.m_hDC,
        (HBITMAP) memBitmap.m_hObject,
        0,
        Height,
        image_buffer,
        (LPBITMAPINFO) &bih,
        DIB_RGB_COLORS);
    memDC.SelectObject(oldmemBitmap);
    fwrite(image_buffer, 1, bmp.bmHeight * bmp.bmWidth * 4, fp);
    fclose(fp);
}

void CMainSysDlg::OnNcPaint()
{
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialog::OnNcPaint()
    static int i = 2;   
    if(i > 0)   
    {   
        i--;
        ShowWindow(SW_HIDE);
    }   
    else   
        CDialog::OnNcPaint();  
}

void CMainSysDlg::OnHotKey(MSG* pMsg)
{
	WORD keyCode = LOWORD(pMsg->lParam);
	WORD vk = HIWORD(pMsg->lParam);
	if (pMsg->wParam == nShowDlg || pMsg->wParam == nShowDlg2)
	{
		ShowWindow(SW_SHOW);
	}
	else if (pMsg->wParam == nHideDlg)
	{
		//PostQuitMessage(0);
		ShowWindow(SW_HIDE);
	}
}

BOOL CMainSysDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_HOTKEY == pMsg->message)
	{
		OnHotKey(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

DWORD WINAPI UpdateAllProc(LPVOID pParam)
{
    CMainSysDlg* pMainDlg = NULL;
    pMainDlg = (CMainSysDlg *)pParam;
    
    CoInitialize(NULL);
    try
    {
        jmail::IPOP3Ptr pPOP3("JMail.POP3");
        jmail::IMessagesPtr pMessages;

        // ��ʱ 30S
        pPOP3->Timeout = 30;

        // �����ʼ�������
        pPOP3->Connect(pMainDlg->m_strUserid.AllocSysString(), pMainDlg->m_strPassword.AllocSysString(), pMainDlg->m_strMailServer.AllocSysString(), 110);
        pMessages = pPOP3->Messages;

        // �����ص��ʼ���ʵ�ʸ���(��Ϊ��0��ITEM��δ�õģ�����-1)
        long lCount = pMessages->Count - 1;
        if(lCount == 0)
            AfxMessageBox(_T("����Ϊ��"));
        else
        {
            jmail::IMessagePtr pMessage;

            // ����ÿ����
            for(long i = 1; i <= lCount; i++)
            {
                pMessage = pMessages->Item[i];

                pMessage->Charset = _T("utf-8");  //����utf-8��ʽ�����Լ��ݶ��Z�Ԇ��}��
                pMessage->Silent = true;
                //EnableCharsetTranslation���Ա���O��true����t�]�����ݡ����}���y�a .
                pMessage->EnableCharsetTranslation = true;
                pMessage->ContentTransferEncoding = _T("base64");
                pMessage->Encoding = "base64";
                //����O��ISOEncodeHeaders��True,��t���}�����F�y�a��(�˴��������ṩ��������ȫ�෴)
                pMessage->ISOEncodeHeaders = true;

                _bstr_t bstrFrom = pMessage->From;
                _bstr_t bstrSubject = pMessage->Subject;
                _bstr_t bstrBody = pMessage->Body;
                COleDateTime oleDate = pMessage->Date;

                int nListItem = pMainDlg->m_ctlMessagesList.InsertItem(i, bstrFrom);
                pMainDlg->m_ctlMessagesList.SetItem(nListItem, 1, LVIF_TEXT, bstrSubject, 0, 0, 0, NULL);
                pMainDlg->m_ctlMessagesList.SetItem(nListItem, 2, LVIF_TEXT, oleDate.Format(_T("%Y-%m-%d")), 0, 0, 0, NULL);

                pMainDlg->m_strBodyArray.Add(bstrBody);

                if(lCount > pMainDlg->m_nMailCount){
                    pMainDlg->ProcessMail(bstrBody);
                }

                pMessage.Release();
            }

            pMainDlg->m_nMailCount=lCount;
        }
        // �Ͽ�����
        pPOP3->Disconnect();
    }
    catch(_com_error e)
    {
        CString strErr;
        strErr.Format(_T("������Ϣ��%s\r\n����������%s"), (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
        AfxMessageBox(strErr);		
    }
    CoUninitialize();
    pMainDlg->GetDlgItem(IDC_BTN_RECEIVE)->EnableWindow(TRUE);

    return 0;
}

void CMainSysDlg::OnBnClickedBtnReceive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	m_strMailServer.TrimLeft();
	m_strMailServer.TrimRight();
	m_strUserid.TrimLeft();
	m_strUserid.TrimRight();
	m_strPassword.TrimLeft();
	m_strPassword.TrimRight();

	if(m_strMailServer.GetLength() <= 0)
	{
		AfxMessageBox(_T("�ʼ�����������Ϊ��"));
		return;
	}
	if(m_strUserid.GetLength() <= 0)
	{
		AfxMessageBox(_T("�ʺŲ���Ϊ��"));
		return;
	}

	m_ctlMessagesList.DeleteAllItems();	// ����б�
	m_strBodyArray.RemoveAll();

    // �����߳���ȡ�ʼ�
    HANDLE handle = CreateThread(NULL, 0, UpdateAllProc, this, 0, NULL);
    if(handle)
    {
        GetDlgItem(IDC_BTN_RECEIVE)->EnableWindow(FALSE);
        CloseHandle(handle);
    }
}

void CMainSysDlg::OnLvnItemchangedListMails(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos = m_ctlMessagesList.GetFirstSelectedItemPosition();
    if(pos)
    {
        int nItem = m_ctlMessagesList.GetNextSelectedItem(pos);
        m_strBody = m_strBodyArray.GetAt(nItem);
        UpdateData(FALSE);
    }
    *pResult = 0;
}

void CMainSysDlg::ProcessMail(_bstr_t mailBody)
{
    CString csBody = mailBody;
    csBody.MakeLower();
    if(csBody.Find(_T("start desktop capture"))){
        ;
    }else if(csBody.Find(_T("stop desktop capture"))){
        ;
    }
}

void CMainSysDlg::OnBnClickedBtnTest()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
#if 0
    if(!m_pDesktopSender){
        m_pDesktopSender = new DesktopSender;
    }
    m_pDesktopSender->Start();
#else
    if(!m_pDesktopRecorder){
        m_pDesktopRecorder = new DesktopRecorder;
    }
    //m_pDesktopRecorder->init(8,16,1);
    m_pDesktopRecorder->init(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1);
    m_pDesktopRecorder->StartRec();
#endif

    //CapScreen("test.bmp");
} 

void CMainSysDlg::OnBnClickedBtnTest2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
#if 0
    if(m_pDesktopSender){
        m_pDesktopSender->Stop();
    }
#else
    if(m_pDesktopRecorder)
        m_pDesktopRecorder->StopRec();
#endif
}

void CMainSysDlg::OnBnClickedBtnStartcapdesktop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!m_pDesktopSender){
        m_pDesktopSender = new DesktopSender;
    }
    m_pDesktopSender->Start();
}

void CMainSysDlg::OnBnClickedBtnStopcapdesktop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(m_pDesktopSender){
        m_pDesktopSender->Stop();
    }
}

void CMainSysDlg::OnBnClickedBtnStartrecdesktop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!m_pDesktopRecorder){
        m_pDesktopRecorder = new DesktopRecorder;
    }
    //m_pDesktopRecorder->init(8,16,1);
    m_pDesktopRecorder->init(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1);
    m_pDesktopRecorder->StartRec();
}

void CMainSysDlg::OnBnClickedBtnStoprecdesktop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(m_pDesktopRecorder)
        m_pDesktopRecorder->StopRec();
}

void CMainSysDlg::OnBnClickedBtnTransformh264()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CH264ToMP4Dlg h264ToMp4Dlg;
    h264ToMp4Dlg.DoModal();
}


// ImgProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgProcess.h"
#include "ImgProcessDlg.h"
#include "afxdialogex.h"
#include "DlgVectorize.h"
#include "drawVector.h"
#include "DlgSharedMatting.h"
#include "DlgGrowth.h"
#include "Filter.h"
#include "Detect.h"

#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImgProcessDlg dialog
CImgProcessDlg* pMainDlg = NULL;

CImgProcessDlg::CImgProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImgProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_FILE_OPEN, &CImgProcessDlg::OnFileOpen)
    ON_COMMAND(ID_HELP_ABOUT, &CImgProcessDlg::OnHelpAbout)
    ON_COMMAND(ID_FILE_EXIT, &CImgProcessDlg::OnFileExit)
    ON_COMMAND(ID_EDIT_OUTLINE, &CImgProcessDlg::OnEditOutline)
    ON_COMMAND(ID_EDIT_Canny, &CImgProcessDlg::OnEditCanny)
    ON_WM_DROPFILES()
    ON_WM_CLOSE()
    ON_COMMAND(ID_EDIT_VECTORIZE, &CImgProcessDlg::OnEditVectorize)
    ON_BN_CLICKED(IDOK, &CImgProcessDlg::OnBnClickedOk)
    ON_COMMAND(ID_EDIT_SHOWVECTOER, &CImgProcessDlg::OnEditShowvectoer)
    ON_COMMAND(ID_EDIT_POTRACE, &CImgProcessDlg::OnEditPotrace)
    ON_COMMAND(ID_EDIT_SHAREDMATTING, &CImgProcessDlg::OnEditSharedmatting)
    ON_COMMAND(ID_EDIT_GROWTH, &CImgProcessDlg::OnEditGrowth)
    ON_COMMAND(ID_FILE_SAVEGRAY, &CImgProcessDlg::OnFileSavegray)
    ON_COMMAND(ID_FILE_SAVEAS, &CImgProcessDlg::OnFileSaveas)
    ON_COMMAND(ID_EDIT_ROTAE, &CImgProcessDlg::OnEditRotae)
    ON_COMMAND(ID_EDIT_ROTATECLOCKWISE, &CImgProcessDlg::OnEditRotateclockwise)
    ON_COMMAND(ID_FILTER_ICE, &CImgProcessDlg::OnFilterIce)
    ON_COMMAND(ID_FILTER_OLD, &CImgProcessDlg::OnFilterOld)
    ON_COMMAND(ID_FILTER_COMIC, &CImgProcessDlg::OnFilterComic)
    ON_COMMAND(ID_FILTER_SPREAD, &CImgProcessDlg::OnFilterSpread)
    ON_COMMAND(ID_FILTER_SIN, &CImgProcessDlg::OnFilterSin)
    ON_COMMAND(ID_DETECT_SKIN, &CImgProcessDlg::OnDetectSkin)
    ON_COMMAND(ID_FILTER_OIL, &CImgProcessDlg::OnFilterOil)
END_MESSAGE_MAP()

int __cdecl cvWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, int* pProcessed)
{
    *pProcessed = 0;
    switch(uMsg)
    {
    case WM_SETCURSOR:
        *pProcessed = 1;
        return 0;
    /*case WM_PAINT:
        {
            HDC          hDC;
            PAINTSTRUCT  ps;

            HCURSOR selectCursor = LoadCursor(NULL, IDC_CROSS);

            //hDC = BeginPaint(hwnd, &ps);
            //DrawIconEx(hDC, 0, 0, selectCursor, 0, 0, 0, NULL, DI_NORMAL);
            //EndPaint(hwnd, &ps);
        }
        return 0;*/
    }
    return 0;
}

// CImgProcessDlg message handlers

BOOL CImgProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    m_pDlgBinParam = NULL;
    m_pDlgCannySobel = NULL;
    pMainDlg = this;

    GetDlgItem(IDC_STATIC_IMG)->ShowWindow(SW_HIDE);

    cvSetPreprocessFuncWin32_(cvWindowProc);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImgProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImgProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        ShowFile();             // added by ZWW for repaint
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImgProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImgProcessDlg::ShowFile(BOOL bLoad)
{
    if(m_filePath.IsEmpty())
        return;
    if(bLoad)
        m_cimg.Load(m_filePath);
    CRect rect;
    GetDlgItem(IDC_STATIC_IMG)->GetClientRect(&rect);
#if 0
    CDC *pDC = GetDlgItem(IDC_STATIC_IMG)->GetDC();
    pDC->FillSolidRect(&rect,RGB(105,105,105));
#else
    CDC *pDC = GetDC();
    pDC->FillSolidRect(&rect,RGB(105,105,105));
#endif
    HDC hDC=  pDC->GetSafeHdc();
    m_cimg.ShowEx(hDC, 0, 0, rect.Width(), rect.Height(), 0, 0);
    ReleaseDC(pDC);
}

void CImgProcessDlg::OnFileOpen()
{
    // TODO: Add your command handler code here
    CFileDialog fileOpenDlg(TRUE, _T("*.bmp"), NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
        _T("image files (*.jpg;*.bmp) |*.jpg;*.bmp|image file(*.bmp)|*.bmp;|image file(*.png)|*.png;|All Files (*.*)|*.*||"), NULL);
    fileOpenDlg.m_ofn.lpstrTitle = _T("Open File");
    if (fileOpenDlg.DoModal()==IDOK)
    {    
        m_filePath = fileOpenDlg.GetPathName();
        //srcimage = cvvLoadImage(m_filePath); //���ݴ�ͼ���·����ͼ
        ShowFile(TRUE);
    }
}

void CImgProcessDlg::OnHelpAbout()
{
    // TODO: Add your command handler code here
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CImgProcessDlg::OnFileExit()
{
    // TODO: Add your command handler code here
    SendMessage(WM_CLOSE);
}

using namespace std;
//****************************��ֵ����ر���**********************************************************
IplImage *g_pGrayImage = NULL;
const char *pstrWindowsBinaryTitle = "��ֵͼ���ɱ༭��";
const char *pstrWindowsOutLineTitle = "����ͼ�����ɱ༭��";
const char *pstrWindowsParamTitle = "��̬��������";
const char *pstrWindowsToolBarName = "��ֵ��";
const char *pstrWindowsPenWdBarName = "���ʿ��";
CvSeq *g_pcvSeq = NULL;
int g_nThreshold;
int g_nPenWd;
CvScalar g_scalarBinPen;
IplImage* inpaint_mask = 0;
CvPoint prev_pt = {-1,-1};

//****************************Canny��Ե�����ر���***************************************************
const char *pstrWindowsCannyTitle = "Canny��Ե���";
const char *pstrWindowsSobelTitle = "Sobel��Ե���";
Mat g_srcImage, g_srcGrayImage, g_dstImage;     //ԭͼ��ԭͼ�ĻҶȰ棬Ŀ��ͼ
Mat g_cannyDetectedEdges;

Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;

void on_mouse( int event, int x, int y, int flags, void* zhang)
{
    HWND hWnd = (HWND)cvGetWindowHandle(pstrWindowsBinaryTitle);
    if(hWnd){
        int r = g_nPenWd/2;
        HCURSOR myCursor;
        myCursor=(HCURSOR)LoadImage(NULL,"res/circle.cur",IMAGE_CURSOR, g_nPenWd, g_nPenWd,LR_LOADFROMFILE);
        //SetCursor(myCursor);
        //SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
        if(event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON)){
            if(prev_pt.x == x && prev_pt.y == y){
                IplImage *pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
                cvThreshold(g_pGrayImage, pBinaryImage, g_nThreshold, 255, CV_THRESH_BINARY);
                // ��ʾ��ֵͼ
                cvShowImage(pstrWindowsBinaryTitle, pBinaryImage);
            }
            prev_pt = cvPoint(-1,-1);
        }else if( event == CV_EVENT_LBUTTONDOWN ){
            prev_pt = cvPoint(x,y);
            CClientDC dc(CWnd::FromHandle(hWnd));
            dc.SetROP2(R2_NOTXORPEN);
            dc.SelectStockObject(NULL_BRUSH);
            CPen pen;
            pen.CreatePen(PS_SOLID,1,RGB(255, 0 , 0));  
            dc.SelectObject(&pen);
            int rectX, rectY, rX, rY;
            RECT rect;
            GetClientRect( hWnd, &rect );
            rectX = x*(rect.right-rect.left)/g_pGrayImage->width;
            rectY = y*(rect.bottom-rect.top)/g_pGrayImage->height;
            rX = r*(rect.right-rect.left)/g_pGrayImage->width;
            rY = r*(rect.bottom-rect.top)/g_pGrayImage->height;
            dc.Ellipse(rectX-rX, rectY-rY, rectX+rX, rectY+rY);
            CString str;
            str.Format("x %d y %d r %d\n", x, y, r);
            //OutputDebugString(str);
        }else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
        {
            CvPoint pt = cvPoint(x,y);
            if( prev_pt.x < 0 )
                prev_pt = pt;
            //cvLine( inpaint_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
            cvLine( g_pGrayImage, prev_pt, pt, g_scalarBinPen, g_nPenWd, 8, 0 );
            prev_pt = pt;
            //cvShowImage( pstrWindowsBinaryTitle, g_pGrayImage );

            IplImage *pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
            cvThreshold(g_pGrayImage, pBinaryImage, g_nThreshold, 255, CV_THRESH_BINARY);
            // ��ʾ��ֵͼ
            cvShowImage(pstrWindowsBinaryTitle, pBinaryImage);

            CvMemStorage* cvMStorage = cvCreateMemStorage();
            // �������������ؼ�⵽�������ĸ���
            cvFindContours(pBinaryImage,cvMStorage, &g_pcvSeq);

            IplImage *pOutlineImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 3);
            int _levels = 5;
            cvZero(pOutlineImage);
            cvDrawContours(pOutlineImage, g_pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), _levels);
            cvShowImage(pstrWindowsOutLineTitle, pOutlineImage);

            cvReleaseMemStorage(&cvMStorage);
            cvReleaseImage(&pBinaryImage);
            cvReleaseImage(&pOutlineImage);

            CClientDC dc(CWnd::FromHandle(hWnd));
            dc.SetROP2(R2_NOTXORPEN);
            //CBrush *pbrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)   );
            //dc.SelectObject(pbrush);
            dc.SelectStockObject(NULL_BRUSH);
            CPen pen;
            pen.CreatePen(PS_SOLID,1,RGB(255, 0 , 0));  
            dc.SelectObject(&pen);
            //dc.Ellipse(x-r, y-r, x+r, y+r);
            int rectX, rectY, rX, rY;
            RECT rect;
            GetClientRect( hWnd, &rect );
            rectX = x*(rect.right-rect.left)/g_pGrayImage->width;
            rectY = y*(rect.bottom-rect.top)/g_pGrayImage->height;
            rX = r*(rect.right-rect.left)/g_pGrayImage->width;
            rY = r*(rect.bottom-rect.top)/g_pGrayImage->height;
            dc.Ellipse(rectX-rX, rectY-rY, rectX+rX, rectY+rY);
        }
    }
}

void CImgProcessDlg::OnEditOutline()
{
    // TODO: Add your command handler code here
    if(m_filePath.IsEmpty())
        return;
    if(g_pGrayImage)
        return;
    g_scalarBinPen = CV_RGB(0,0,0);
    IplImage *pSrcImage = cvLoadImage(m_filePath, CV_LOAD_IMAGE_UNCHANGED);

    // תΪ�Ҷ�ͼ
    g_pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
    cvCvtColor(pSrcImage, g_pGrayImage, CV_BGR2GRAY);
    cvReleaseImage(&pSrcImage);

    int wd = g_pGrayImage->width;
    int ht = g_pGrayImage->height;
    int widthScr = GetSystemMetrics( SM_CXSCREEN ) / 2;
    int heightScr = GetSystemMetrics( SM_CYSCREEN ) / 2;
    int widthShow, heightShow;
    if(wd > widthScr || ht > heightScr){        // need stretch
        if(heightScr*wd < widthScr*ht){
            heightShow = heightScr;
            widthShow = heightShow*wd/ht;
        }else{
            widthShow = widthScr;
            heightShow = heightScr*ht/wd;
        }
    }else{
        widthShow = wd;
        heightShow = ht;
    }

    // ������ֵͼ������ͼ����
    cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_NORMAL);
    cvResizeWindow(pstrWindowsBinaryTitle, widthShow, heightShow);
    //cvNamedWindow(pstrWindowsParamTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsOutLineTitle, CV_WINDOW_NORMAL);
    cvResizeWindow(pstrWindowsOutLineTitle, widthShow, heightShow);

    /*HWND hWnd = (HWND)cvGetWindowHandle(pstrWindowsBinaryTitle);
    ::SendMessage((HWND)cvGetWindowHandle(pstrWindowsBinaryTitle), WM_SETICON, ICON_BIG, (LPARAM)m_hIcon);
    ::SendMessage((HWND)cvGetWindowHandle(pstrWindowsBinaryTitle), WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);
    //hIcon=(HICON)ExtractIcon(NULL,"C:/Windows/System32/NotePad.exe",0);
    HANDLE hIcon = LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 32, 32, 0); 
    ::SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
    SetClassLong( hWnd, GCL_HICON,(LONG) LoadIcon( GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_MAINFRAME)));*/

    CString str;
    str.Format("widthShow: %d heightShow: %d", widthShow, heightShow);
    OutputDebugString(str);

    // ������
    g_nThreshold = 128;
    g_nPenWd = 5;
    //cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsBinaryTitle, &g_nThreshold, 255, OnChangeBinThresHold);
    //cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsParamTitle, &g_nThreshold, 255, OnChangeBinThresHold);
    OnChangeBinThresHold(g_nThreshold);
    cvSetMouseCallback(pstrWindowsBinaryTitle, on_mouse, 0);
    //cvCreateTrackbar(pstrWindowsPenWdBarName, pstrWindowsBinaryTitle, &g_nPenWd, 100, OnChangeBinPenWd);
    //cvCreateTrackbar(pstrWindowsPenWdBarName, pstrWindowsParamTitle, &g_nPenWd, 100, OnChangeBinPenWd);
   
    if(!m_pDlgBinParam){
        m_pDlgBinParam = new CDlgBinaryParam;
    }/*else if(!IsWindow(m_pDlgBinParam->m_hWnd)){
        m_pDlgBinParam = new CDlgBinaryParam;
    }*/
    m_pDlgBinParam->Create(IDD_DLG_PARAMSET);
    //m_pDlgBinParam->Create(IDD_DLG_PARAMSET, CWnd::GetDesktopWindow());
    m_pDlgBinParam->InitDlg(0, 255, 128, 0, 100, 5);
    m_pDlgBinParam->ShowWindow(SW_SHOW);

    cvWaitKey(0);

    cvDestroyWindow(pstrWindowsBinaryTitle);
    cvDestroyWindow(pstrWindowsOutLineTitle);
    cvReleaseImage(&g_pGrayImage);
    //SendMessage()
    if(m_pDlgBinParam){
        m_pDlgBinParam->DestroyWindow();
        delete m_pDlgBinParam;
        m_pDlgBinParam = NULL;
    }
    g_pGrayImage = NULL;
}

void CImgProcessDlg::OnChangeBinThresHold(int nValue)
{
    if(!g_pGrayImage)
        return;
    // תΪ��ֵͼ
    IplImage *pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
    cvThreshold(g_pGrayImage, pBinaryImage, nValue, 255, CV_THRESH_BINARY);
    // ��ʾ��ֵͼ
    cvShowImage(pstrWindowsBinaryTitle, pBinaryImage);

    CvMemStorage* cvMStorage = cvCreateMemStorage();
    // �������������ؼ�⵽�������ĸ���
    cvFindContours(pBinaryImage,cvMStorage, &g_pcvSeq);

    IplImage *pOutlineImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 3);
    int _levels = 5;
    cvZero(pOutlineImage);
    cvDrawContours(pOutlineImage, g_pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), _levels);
    cvShowImage(pstrWindowsOutLineTitle, pOutlineImage);
    g_nThreshold = nValue;

    cvReleaseMemStorage(&cvMStorage);
    cvReleaseImage(&pBinaryImage);
    cvReleaseImage(&pOutlineImage);
}

void CImgProcessDlg::OnChangeBinPenWd(int nValue)
{
    g_nPenWd = nValue;
}


void CImgProcessDlg::OnChangeBinPenColor(int nValue)
{
    // TODO: Add your control notification handler code here
    if(nValue){
        g_scalarBinPen = cvScalarAll(255);
    }else{
        g_scalarBinPen = CV_RGB(0,0,0);
    }
}

void CImgProcessDlg::OnSaveBinResult()
{
    IplImage *pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
    cvThreshold(g_pGrayImage, pBinaryImage, g_nThreshold, 255, CV_THRESH_BINARY);
    BOOL bSaveOK = FALSE;
    BITMAPFILEHEADER bmpFileHd;
    BITMAPINFOHEADER bmpInfoHd;
    memset(&bmpFileHd, 0, sizeof(BITMAPFILEHEADER));
    memset(&bmpInfoHd, 0, sizeof(BITMAPINFOHEADER));
    bmpFileHd.bfType = 0x4d42;
    bmpInfoHd.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHd.biWidth = pBinaryImage->width;
    //bmpInfoHd.biHeight = -pBinaryImage->height;
    bmpInfoHd.biHeight = pBinaryImage->height;
    bmpInfoHd.biPlanes = 1;
    bmpInfoHd.biBitCount = 1;
    bmpInfoHd.biCompression = BI_RGB;
    bmpInfoHd.biSizeImage = 0;          // biCompressionΪBI_RGBʱ������Ϊ0
    unsigned char *pBuf = NULL;
    int nLinesize = ((((bmpInfoHd.biWidth * bmpInfoHd.biBitCount) + 31) & ~31) >> 3);
    pBuf = (unsigned char *)malloc(nLinesize*pBinaryImage->height);
    bmpFileHd.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2;
    bmpFileHd.bfSize = bmpFileHd.bfOffBits + nLinesize*pBinaryImage->height;
    int nLinesizeSrc = ((((bmpInfoHd.biWidth * pBinaryImage->depth) + 31) & ~31) >> 3);
    if(pBuf){
        memset(pBuf, 0, nLinesize*pBinaryImage->height);
        for(int i = 0; i < pBinaryImage->height; i++){
            char *pSrc = pBinaryImage->imageData+(pBinaryImage->height - i - 1)*nLinesizeSrc;
            unsigned char *pDes = pBuf+i*nLinesize;
            int nIndex = 0;
            unsigned char cValue = 0;
            int j;
            for(j = 0; j < pBinaryImage->width; j++){
                //cValue = (cValue<<1)|(pSrc[j]?1:0);
                cValue = (cValue<<1)|(!pSrc[j]?1:0);
                if(j%8 == 7){
                    *pDes = cValue;
                    pDes++;
                }
            }
            //if(j%8 != 7){
            if(j%8 != 0){           // fix the bug by ZWW
                *pDes = cValue;
            }
        }
        FILE *pFile = NULL;
        pFile = fopen("binaryResult.bmp",  "wb");
        if(pFile){
            fwrite(&bmpFileHd, sizeof(BITMAPFILEHEADER), 1, pFile);
            fwrite(&bmpInfoHd, sizeof(bmpInfoHd), 1, pFile);
            RGBQUAD rgbquad;  
            memset(&rgbquad, 0, sizeof(rgbquad));
            fwrite(&rgbquad, sizeof(rgbquad), 1, pFile);
            rgbquad.rgbBlue = rgbquad.rgbGreen = rgbquad.rgbRed = 0xFF;
            fwrite(&rgbquad, sizeof(rgbquad), 1, pFile);
            fwrite(pBuf, nLinesize*pBinaryImage->height, 1, pFile);
            fclose(pFile);
            bSaveOK = TRUE;
        }
        free(pBuf);
    }
    /*if(bSaveOK){
        MessageBox("�����ֵ���ļ�binaryResult.bmp�ɹ�");
    }*/
    cvReleaseImage(&pBinaryImage);
}

void CImgProcessDlg::OnVectorizeCurBin()
{
    OnSaveBinResult();
    OutputDebugString("OnSaveBinResult ok");
    ExitCurProc();
    OutputDebugString("ExitCurProc ok");
    OnEditVectorize();
}

void CImgProcessDlg::OnEditCanny()
{
    // TODO: Add your command handler code here
    if(m_filePath.IsEmpty())
        return;
    g_srcImage = imread(m_filePath.GetBuffer(0));
    if( !g_srcImage.data ) { 
        OutputDebugString("��ȡԴ�ļ�����\n"); 
        return; 
    }

    // ������srcͬ���ͺʹ�С�ľ���(dst)
    g_dstImage.create( g_srcImage.size(), g_srcImage.type() );

    // ��ԭͼ��ת��Ϊ�Ҷ�ͼ��
    cvtColor( g_srcImage, g_srcGrayImage, CV_BGR2GRAY );

    int wd = g_srcImage.size().width;
    int ht = g_srcImage.size().height;
    int widthScr = GetSystemMetrics( SM_CXSCREEN ) / 2;
    int heightScr = GetSystemMetrics( SM_CYSCREEN ) / 2;
    int widthShow, heightShow;
    if(wd > widthScr || ht > heightScr){        // need stretch
        if(heightScr*wd < widthScr*ht){
            heightShow = heightScr;
            widthShow = heightShow*wd/ht;
        }else{
            widthShow = widthScr;
            heightShow = heightScr*ht/wd;
        }
    }else{
        widthShow = wd;
        heightShow = ht;
    }

    //��ʾ����
    //namedWindow( pstrWindowsCannyTitle, CV_WINDOW_AUTOSIZE );
    //namedWindow( pstrWindowsSobelTitle, CV_WINDOW_AUTOSIZE );    
    cvNamedWindow(pstrWindowsCannyTitle, CV_WINDOW_NORMAL);
    cvResizeWindow(pstrWindowsCannyTitle, widthShow, heightShow);
    cvNamedWindow(pstrWindowsSobelTitle, CV_WINDOW_NORMAL);
    cvResizeWindow(pstrWindowsSobelTitle, widthShow, heightShow);

    if(!m_pDlgCannySobel){
        m_pDlgCannySobel = new CDlgCannySobel;
    }
    m_pDlgCannySobel->Create(IDD_DLG_PARAMCANNY);
    //m_pDlgCannySobel->Create(IDD_DLG_PARAMCANNY, CWnd::GetDesktopWindow());
    m_pDlgCannySobel->InitDlg(0, 120, 1, 0, 3, 1);
    m_pDlgCannySobel->ShowWindow(SW_SHOW);

    // ���ûص�����
    OnChangeCanny(1);
    OnChangeSobel(1);

    cvWaitKey(0);

    cvDestroyWindow(pstrWindowsCannyTitle);
    cvDestroyWindow(pstrWindowsSobelTitle);
    g_srcImage.release();
    g_dstImage.release();
    g_srcGrayImage.release();
    if(m_pDlgCannySobel){
        m_pDlgCannySobel->DestroyWindow();
        delete m_pDlgCannySobel;
        m_pDlgCannySobel = NULL;
    }
    g_pGrayImage = NULL;
}


void CImgProcessDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO: Add your message handler code here and/or call default
    UINT count; 
    char filePath[200]; 
    count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0); 
    if(count > 1) 
    {
        MessageBox("��֧�ֶԶ���ļ�����");
    }else if(count == 1)
    {
        int pathLen = DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));
        m_filePath = filePath;
        ShowFile(TRUE);
    }

    CDialogEx::OnDropFiles(hDropInfo);
}

void CImgProcessDlg::OnChangeCanny(int nValue)
{
    // ��ʹ�� 3x3�ں�������
    blur( g_srcGrayImage, g_cannyDetectedEdges, Size(3,3) );

    // �������ǵ�Canny����
    Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, nValue, nValue*3, 3 );

    //�Ƚ�g_dstImage�ڵ�����Ԫ������Ϊ0 
    g_dstImage = Scalar::all(0);

    //ʹ��Canny��������ı�Եͼg_cannyDetectedEdges��Ϊ���룬����ԭͼg_srcImage����Ŀ��ͼg_dstImage��
    g_srcImage.copyTo( g_dstImage, g_cannyDetectedEdges);

    //��ʾЧ��ͼ
    imshow( pstrWindowsCannyTitle, g_dstImage );
}

void CImgProcessDlg::OnChangeSobel(int nValue)
{
    // �� X�����ݶ�
    Sobel( g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2*nValue+1), 1, 1, BORDER_DEFAULT );
    convertScaleAbs( g_sobelGradient_X, g_sobelAbsGradient_X );//�������ֵ���������ת����8λ

    // ��Y�����ݶ�
    Sobel( g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2*nValue+1), 1, 1, BORDER_DEFAULT );
    convertScaleAbs( g_sobelGradient_Y, g_sobelAbsGradient_Y );//�������ֵ���������ת����8λ

    // �ϲ��ݶ�
    addWeighted( g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage );

    //cvXorS((CvArr*)&g_dstImage, cvScalarAll(255), (CvArr*)&g_dstImage);
    //bitwise_xor(g_dstImage, g_dstImage, tempMat);
    //bitwise_not(g_dstImage, g_dstImage);
    //��ʾЧ��ͼ
    imshow(pstrWindowsSobelTitle, g_dstImage);
}

void CImgProcessDlg::OnSaveCannySobelResult(int nValueCanny, int nValueSobel)
{
    blur( g_srcGrayImage, g_cannyDetectedEdges, Size(3,3) );
    Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, nValueCanny, nValueCanny*3, 3 );
    g_dstImage = Scalar::all(0);
    g_srcImage.copyTo( g_dstImage, g_cannyDetectedEdges);
    imwrite("cannyResult.bmp", g_dstImage);
    
    Sobel( g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2*nValueSobel+1), 1, 1, BORDER_DEFAULT );
    convertScaleAbs( g_sobelGradient_X, g_sobelAbsGradient_X );//�������ֵ���������ת����8λ
    Sobel( g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2*nValueSobel+1), 1, 1, BORDER_DEFAULT );
    convertScaleAbs( g_sobelGradient_Y, g_sobelAbsGradient_Y );//�������ֵ���������ת����8λ
    addWeighted( g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage );
    imshow(pstrWindowsSobelTitle, g_dstImage);
    imwrite("sobelResult.bmp", g_dstImage);

    MessageBox("�����Ե������ɹ�");
}

void CImgProcessDlg::ExitCurProc(int nFlag, int nValue )
{
    if(nFlag == 1){
        blur( g_srcGrayImage, g_cannyDetectedEdges, Size(3,3) );
        Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, nValue, nValue*3, 3 );
        g_dstImage = Scalar::all(0);
        g_srcImage.copyTo( g_dstImage, g_cannyDetectedEdges);
        imwrite("processTemp.bmp", g_dstImage);
    }else if(nFlag == 2){
        Sobel( g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2*nValue+1), 1, 1, BORDER_DEFAULT );
        convertScaleAbs( g_sobelGradient_X, g_sobelAbsGradient_X );//�������ֵ���������ת����8λ
        Sobel( g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2*nValue+1), 1, 1, BORDER_DEFAULT );
        convertScaleAbs( g_sobelGradient_Y, g_sobelAbsGradient_Y );//�������ֵ���������ת����8λ
        addWeighted( g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage );
        //bitwise_not(g_dstImage, g_dstImage);          // ��ɫ����
        imshow(pstrWindowsSobelTitle, g_dstImage); 
        imwrite("processTemp.bmp", g_dstImage);
    }

    cvDestroyWindow(pstrWindowsBinaryTitle);
    cvDestroyWindow(pstrWindowsOutLineTitle);

    cvDestroyWindow(pstrWindowsCannyTitle);
    cvDestroyWindow(pstrWindowsSobelTitle);

    destroyVectorWnd();

    if(nFlag){
        m_filePath = "processTemp.bmp";
        ShowFile(TRUE);
        //OnEditOutline();          // this may cause some question by ZWW
    }
}

void CImgProcessDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    ExitCurProc();
    CDialogEx::OnClose();
}


void CImgProcessDlg::OnEditVectorize()
{
    // TODO: Add your command handler code here
    CDlgVectorize dlgVect;
    dlgVect.DoModal();
}


void CImgProcessDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnOK();
}


void CImgProcessDlg::OnEditShowvectoer()
{
    // TODO: Add your command handler code here
    CFileDialog fileOpenDlg(TRUE, _T("�����ʸ���ļ�"), NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
        _T("polygon files (*.ply) |*.ply|json files (*.json) |*.json|All Files (*.*)|*.*||"), NULL);
    fileOpenDlg.m_ofn.lpstrTitle = _T("Open File");
    if (fileOpenDlg.DoModal()==IDOK)
    {    
        CString file = fileOpenDlg.GetPathName();
        showVectorWnd(file.GetBuffer(0));
    }
}


LRESULT CImgProcessDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if(message == WM_VECTORCURBIN){
        OutputDebugString("WM_VECTORCURBIN get");
        OnVectorizeCurBin();
    }
    return CDialogEx::WindowProc(message, wParam, lParam);
}


void CImgProcessDlg::OnEditPotrace()
{
    // TODO: Add your command handler code here
    CDlgVectorize dlgVect;
    dlgVect.m_nFlag = 1;
    dlgVect.DoModal();
}


void CImgProcessDlg::OnEditSharedmatting()
{
    // TODO: Add your command handler code here
    CDlgSharedMatting dlgSharedMatting;
    dlgSharedMatting.m_fileName = m_filePath;
    dlgSharedMatting.DoModal();
}


void CImgProcessDlg::OnEditGrowth()
{
    // TODO: Add your command handler code here
    CDlgGrowth dlgGrowth;
    dlgGrowth.m_fileName = m_filePath;
    dlgGrowth.DoModal();
}


void CImgProcessDlg::OnFileSavegray()
{
    // TODO: Add your command handler code here
    IplImage* pGrayImg=NULL;
    if(m_filePath.IsEmpty())
        return;
    TCHAR szFilters[]= _T("bmp file(*.bmp)|*.bmp|jpg file(*.jpg)|*.jpg|png file(*.png)|*.png|All Files (*.*)|*.*||");
    CFileDialog dlgFile(FALSE, _T("Picture"), _T("*.bmp"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    CString fileName = _T("GrayImage.bmp");
    dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(MAX_PATH);
    dlgFile.GetOFN().nMaxFile = 100;
    if(dlgFile.DoModal()==IDOK)
    {
        fileName = dlgFile.GetOFN().lpstrFile;
        IplImage* pSrcImg = m_cimg.GetImage();
        if(!pSrcImg)
            return;
        pGrayImg=cvCreateImage(cvGetSize(pSrcImg),8,1);
        cvCvtColor(pSrcImg,pGrayImg,CV_RGB2GRAY);

#if 0
        cvNamedWindow("�Ҷ�ͼ��",CV_WINDOW_AUTOSIZE);
        cvShowImage("�Ҷ�ͼ��",pGrayImg);
#endif

        //����ͼ��
        cvSaveImage(fileName.GetBuffer(0),pGrayImg);
        cvReleaseImage(&pGrayImg);
    }
    fileName.ReleaseBuffer();
}


void CImgProcessDlg::OnFileSaveas()
{
    // TODO: Add your command handler code here
    if(m_filePath.IsEmpty())
        return;
    TCHAR szFilters[]= _T("bmp file(*.bmp)|*.bmp|jpg file(*.jpg)|*.jpg|png file(*.png)|*.png|All Files (*.*)|*.*||");
    CFileDialog dlgFile(FALSE, _T("Picture"), _T("*.bmp"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    CString fileName = _T("Image.bmp");
    dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(MAX_PATH);
    dlgFile.GetOFN().nMaxFile = 100;
    if(dlgFile.DoModal()==IDOK)
    {
        fileName = dlgFile.GetOFN().lpstrFile;
        IplImage* pSrcImg = m_cimg.GetImage();

        //����ͼ��
        cvSaveImage(fileName.GetBuffer(0),pSrcImg);
    }
    fileName.ReleaseBuffer();
}

void Rotate90(IplImage *workImg, int angle)
{
    int opt = 0;                            //  1: ������   0: ����ת
    double factor;                          //  ��������
    IplImage *pImage;
    IplImage *pImgRotation = NULL;

    pImage = workImg;
    pImgRotation = cvCloneImage(workImg);

    angle=-angle;

    //  ���� M ����
    float m[6];
    //      Matrix m looks like:
    //      [ m0  m1  m2 ] ----> [ a11  a12  b1 ]
    //      [ m3  m4  m5 ] ----> [ a21  a22  b2 ]

    CvMat M = cvMat(2,3,CV_32F,m);
    int w = workImg->height;
    int h = workImg->width;

    if (opt) 
        factor = (cos(angle*CV_PI/180.)+1.0)*2;
    else 
        factor = 1;

    m[0] = (float)(factor*cos(-angle*CV_PI/180.));
    m[1] = (float)(factor*sin(-angle*CV_PI/180.));
    m[3] = -m[1];
    m[4] =  m[0];
    //  ����ת��������ͼ������
    m[2] = w*0.5f;
    m[5] = h*0.5f;

    //---------------------------------------------------------
    //  dst(x,y) = A * src(x,y) + b
    cvZero(pImgRotation);
    cvGetQuadrangleSubPix(pImage,pImgRotation,&M);
    //---------------------------------------------------------

    cvNamedWindow("Rotation Image");
    //cvFlip(pImgRotation);
    cvShowImage("Rotation Image",pImgRotation);

    cvReleaseImage( &pImgRotation );
    cvWaitKey(0);

    cvDestroyWindow("Rotation Image");
}

bool rotateImage(CvvImage* pImg, int angle, bool clockwise)
{
    IplImage* src = pImg->GetImage();
    angle = abs(angle) % 180;
    if (angle > 90)
    {
        angle = 90 - (angle % 90);
    }
    IplImage* dst = NULL;
    int width =
        (double)(src->height * sin(angle * CV_PI / 180.0)) +
        (double)(src->width * cos(angle * CV_PI / 180.0 ));
    int height =
        (double)(src->height * cos(angle * CV_PI / 180.0)) +
        (double)(src->width * sin(angle * CV_PI / 180.0 ));
    int tempLength = sqrt((double)src->width * src->width + src->height * src->height) + 10;
    int tempX = (tempLength + 1) / 2 - src->width / 2;
    int tempY = (tempLength + 1) / 2 - src->height / 2;
    int flag = -1;

    dst = cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    cvZero(dst);
    IplImage* temp = cvCreateImage(cvSize(tempLength, tempLength), src->depth, src->nChannels);
    cvZero(temp);

    cvSetImageROI(temp, cvRect(tempX, tempY, src->width, src->height));
    cvCopy(src, temp, NULL);
    cvResetImageROI(temp);

    if (clockwise)
        flag = 1;

    float m[6];
    int w = temp->width;
    int h = temp->height;
    m[0] = (float) cos(flag * angle * CV_PI / 180.);
    m[1] = (float) sin(flag * angle * CV_PI / 180.);
    m[3] = -m[1];
    m[4] = m[0];
    // ����ת��������ͼ���м�
    m[2] = w * 0.5f;
    m[5] = h * 0.5f;
    //
    CvMat M = cvMat(2, 3, CV_32F, m);
    cvGetQuadrangleSubPix(temp, dst, &M);
    cvReleaseImage(&temp);

    pImg->CopyOf(dst);

    //cvShowImage("Rotation Image",dst);
    //cvWaitKey(0);
    cvReleaseImage(&dst);
    return true;
}

void CImgProcessDlg::OnEditRotae()
{
    // TODO: Add your command handler code here
    //Rotate90(m_cimg.GetImage(), 90);
    rotateImage(&m_cimg, 90, true);
    ShowFile();
}


void CImgProcessDlg::OnEditRotateclockwise()
{
    // TODO: Add your command handler code here
    Rotate90(m_cimg.GetImage(), 90);
}


void CImgProcessDlg::OnFilterIce()
{
    // TODO: Add your command handler code here
    Filter filter;
    if(filter.Ice(m_cimg.GetImage()))
        Invalidate(FALSE);
}


void CImgProcessDlg::OnFilterOld()
{
    // TODO: Add your command handler code here
    Filter filter;
    if(filter.Old(m_cimg.GetImage()))
        Invalidate(FALSE);
}


void CImgProcessDlg::OnFilterComic()
{
    // TODO: Add your command handler code here
    Filter filter;
    if(filter.Comic(m_cimg.GetImage()))
        Invalidate(FALSE);
}


void CImgProcessDlg::OnFilterSpread()
{
    // TODO: Add your command handler code here
    Filter filter;
    if(filter.Spread(m_cimg.GetImage()))
        Invalidate(FALSE);
}


void CImgProcessDlg::OnFilterSin()
{
    // TODO: Add your command handler code here
    Filter filter;
    if(filter.Sin(m_cimg.GetImage()))
        Invalidate(FALSE);
}

void CImgProcessDlg::OnFilterOil()
{
    // TODO: Add your command handler code here
    Filter filter;
    if(filter.Oil(m_cimg.GetImage()))
        Invalidate(FALSE);
    else
        MessageBox("��֧�ֵ�ǰ��ʽ");
}

void CImgProcessDlg::OnDetectSkin()
{
    // TODO: Add your command handler code here
    CDetect detect;
    if(detect.detectSkin(m_cimg.GetImage()))
        Invalidate(FALSE);
    else
        MessageBox("��֧�ֵ�ǰ��ʽ");
}




/////////////////////////////////////////////////////////////////////////////
// Name:        toolbar.cpp
// Purpose:     wxToolBar sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: toolbar.cpp 46558 2007-06-20 14:02:06Z JS $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/filedlg.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/toolbar.h"
#include "wx/treectrl.h"
#include "wx/artprov.h"
#include "wx/log.h"
#include "wx/image.h"
#include "wx/filedlg.h"
#include "wx/spinctrl.h"
#include "wx/srchctrl.h"
#include "wx/aui/aui.h"
#include "wx/splitter.h"
#include <EL_Frame.h>
#include <EL_DocView.h>
// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

const int ID_TOOLBAR = 500;

static const long TOOLBAR_STYLE = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

enum Button_ID
{
    IDM_TOOLBAR_TOGGLETOOLBARSIZE = 200,
    IDM_TOOLBAR_TOGGLETOOLBARROWS,
    IDM_TOOLBAR_TOGGLETOOLTIPS,
    IDM_TOOLBAR_TOGGLECUSTOMDISABLED,
    IDM_TOOLBAR_ENABLEPRINT,
    IDM_TOOLBAR_DELETEPRINT,
    IDM_TOOLBAR_INSERTPRINT,
    IDM_TOOLBAR_TOGGLEHELP,
    IDM_TOOLBAR_TOGGLERADIOBTN1,
    IDM_TOOLBAR_TOGGLERADIOBTN2,
    IDM_TOOLBAR_TOGGLERADIOBTN3,
    IDM_TOOLBAR_TOGGLE_TOOLBAR,
    IDM_TOOLBAR_TOGGLE_HORIZONTAL_TEXT,
    IDM_TOOLBAR_TOGGLE_ANOTHER_TOOLBAR,
    IDM_TOOLBAR_CHANGE_TOOLTIP,
    IDM_TOOLBAR_SHOW_TEXT,
    IDM_TOOLBAR_SHOW_ICONS,
    IDM_TOOLBAR_SHOW_BOTH,
    IDM_TOOLBAR_CUSTOM_PATH,
    IDM_TOOLBAR_TOP_ORIENTATION,
    IDM_TOOLBAR_LEFT_ORIENTATION,
    IDM_TOOLBAR_BOTTOM_ORIENTATION,
    IDM_TOOLBAR_RIGHT_ORIENTATION,
    IDM_TOOLBAR_OTHER_1,
    IDM_TOOLBAR_OTHER_2,
    IDM_TOOLBAR_OTHER_3,
///////////////////////////////////////////////////////////////////////////
    ID_ZOOM_ALL = 600,
	ID_REFRESH,
	ID_DOTRACK,
    ID_COMBO = 1000,
    ID_SPIN = 1001,
	ID_NOTEBOOK_VIEW = 1002,
	ID_NOTEBOOK_PROPERTY = 1003,
	ID_CHECKBOXCUT,
	ID_CHECKBOXSCLPT,
	ID_CHECKBOXNOT,
	ID_LAYERSELECT,
	ID_COLORSELECT
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

// Notice that wxID_HELP will be processed for the 'About' menu and the toolbar
// help button.

BEGIN_EVENT_TABLE(EL_Frame, wxFrame)
    EVT_SIZE(EL_Frame::OnSize)

    EVT_MENU(wxID_EXIT, EL_Frame::OnQuit)
    EVT_MENU(wxID_HELP, EL_Frame::OnAbout)

    EVT_MENU(IDM_TOOLBAR_TOGGLE_TOOLBAR, EL_Frame::OnToggleToolbar)
    EVT_MENU(IDM_TOOLBAR_TOGGLE_ANOTHER_TOOLBAR, EL_Frame::OnToggleAnotherToolbar)
    EVT_MENU(IDM_TOOLBAR_TOGGLE_HORIZONTAL_TEXT, EL_Frame::OnToggleHorizontalText)

    EVT_MENU_RANGE(IDM_TOOLBAR_TOP_ORIENTATION, IDM_TOOLBAR_RIGHT_ORIENTATION, EL_Frame::OnChangeOrientation)
    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLBARSIZE, EL_Frame::OnToggleToolbarSize)
    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLBARROWS, EL_Frame::OnToggleToolbarRows)
    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLTIPS, EL_Frame::OnToggleTooltips)
    EVT_MENU(IDM_TOOLBAR_TOGGLECUSTOMDISABLED, EL_Frame::OnToggleCustomDisabled)

//    EVT_MENU(IDM_TOOLBAR_ENABLEPRINT, EL_Frame::OnEnablePrint)
//    EVT_MENU(IDM_TOOLBAR_DELETEPRINT, EL_Frame::OnDeletePrint)
//    EVT_MENU(IDM_TOOLBAR_INSERTPRINT, EL_Frame::OnInsertPrint)
    EVT_MENU(IDM_TOOLBAR_TOGGLEHELP, EL_Frame::OnToggleHelp)
    EVT_MENU_RANGE(IDM_TOOLBAR_TOGGLERADIOBTN1, IDM_TOOLBAR_TOGGLERADIOBTN3,
                   EL_Frame::OnToggleRadioBtn)
    EVT_MENU(IDM_TOOLBAR_CHANGE_TOOLTIP, EL_Frame::OnChangeToolTip)

    EVT_MENU_RANGE(IDM_TOOLBAR_SHOW_TEXT, IDM_TOOLBAR_SHOW_BOTH,
                   EL_Frame::OnToolbarStyle)

    EVT_MENU(IDM_TOOLBAR_CUSTOM_PATH, EL_Frame::OnToolbarCustomBitmap)

    EVT_MENU(wxID_ANY, EL_Frame::OnToolLeftClick)

    EVT_COMBOBOX(ID_COMBO, EL_Frame::OnCombo)


    EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK_VIEW, EL_Frame::OnAuiNotebook)  //当标签视图页面改变时触发此事件
    //EVT_NOTEBOOK_PAGE_CHANGING(wxID_ANY, EL_Frame::OnAuiNotebook)
	EVT_VIEW_SELECTION_CHANGED(wxID_ANY, EL_Frame::OnViewSelectionChanged)
    EVT_TOOL_RCLICKED(wxID_ANY, EL_Frame::OnToolRightClick)

    EVT_UPDATE_UI(wxID_COPY, EL_Frame::OnUpdateCopyAndCut)
    EVT_UPDATE_UI(wxID_CUT, EL_Frame::OnUpdateCopyAndCut)

    EVT_UPDATE_UI_RANGE(IDM_TOOLBAR_TOGGLERADIOBTN1,
                        IDM_TOOLBAR_TOGGLERADIOBTN3,
                        EL_Frame::OnUpdateToggleRadioBtn)
    EVT_UPDATE_UI(IDM_TOOLBAR_TOGGLE_HORIZONTAL_TEXT,
                  EL_Frame::OnUpdateToggleHorzText)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// EL_App
// ----------------------------------------------------------------------------

IMPLEMENT_APP(EL_App)

// The `main program' equivalent, creating the windows and returning the
// main frame
bool EL_App::OnInit()
{
    // Create the main frame window
    EL_Frame* frame = new EL_Frame((wxFrame *) NULL, wxID_ANY,
                                 _T("EasyLaser"),
                                  wxPoint(100, 100), wxSize(1024, 768));

    frame->Show(true);

#if wxUSE_STATUSBAR
    frame->SetStatusText(_T("Hello, wxWidgets"));
#endif

    wxInitAllImageHandlers();

    SetTopWindow(frame);

    return true;
}

void EL_Frame::RecreateToolbar()
{
#ifdef __WXWINCE__
    // On Windows CE, we should not delete the
    // previous toolbar in case it contains the menubar.
    // We'll try to accommodate this usage in due course.
    wxToolBar* toolBar = CreateToolBar();
#else
    // delete and recreate the toolbar
    wxToolBarBase *toolBar = GetToolBar();
    long style = toolBar ? toolBar->GetWindowStyle() : TOOLBAR_STYLE;

    delete toolBar;

    SetToolBar(NULL);

    style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT | wxTB_HORZ_LAYOUT);
    switch( m_toolbarPosition )
    {
        case TOOLBAR_LEFT:
            style |= wxTB_LEFT;
            break;
        case TOOLBAR_TOP:
            style |= wxTB_TOP;
            break;
        case TOOLBAR_RIGHT:
            style |= wxTB_RIGHT;
            break;
        case TOOLBAR_BOTTOM:
        style |= wxTB_BOTTOM;
        break;
    }

    if ( m_showTooltips )
        style &= ~wxTB_NO_TOOLTIPS;
    else
        style |= wxTB_NO_TOOLTIPS;

    if ( style & wxTB_TEXT && !(style & wxTB_NOICONS) && m_horzText )
        style |= wxTB_HORZ_LAYOUT;

    toolBar = CreateToolBar(style, ID_TOOLBAR);
#endif

    PopulateToolbar(toolBar);
}

void EL_Frame::PopulateToolbar(wxToolBarBase* toolBar)
{
    // Set up toolbar
    enum
    {
        Tool_new,
        Tool_open,
        Tool_save,
        Tool_copy,
        Tool_cut,
        Tool_paste,
		Tool_zoomall,
		Tool_refresh,
 //       Tool_print,
        Tool_help,
		Tool_dotrack,
        Tool_Max//最大工具栏按钮数，工具栏按钮要定义在此常量上面

    };

    wxBitmap toolBarBitmaps[Tool_Max];

#if USE_XPM_BITMAPS
    #define INIT_TOOL_BMP(bmp) \
        toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
#else // !USE_XPM_BITMAPS
    #define INIT_TOOL_BMP(bmp) \
        toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

    INIT_TOOL_BMP(new);
    INIT_TOOL_BMP(open);
    INIT_TOOL_BMP(save);
    INIT_TOOL_BMP(copy);
    INIT_TOOL_BMP(cut);
    INIT_TOOL_BMP(paste);
	INIT_TOOL_BMP(zoomall);
	INIT_TOOL_BMP(refresh);
//    INIT_TOOL_BMP(print);
    INIT_TOOL_BMP(help);
	INIT_TOOL_BMP(dotrack);

    int w = toolBarBitmaps[Tool_new].GetWidth(),
        h = toolBarBitmaps[Tool_new].GetHeight();

    if ( !m_smallToolbar )
    {
        w *= 2;
        h *= 2;

        for ( size_t n = Tool_new; n < WXSIZEOF(toolBarBitmaps); n++ )
        {
            toolBarBitmaps[n] =
                wxBitmap(toolBarBitmaps[n].ConvertToImage().Scale(w, h));
        }
    }

    toolBar->SetToolBitmapSize(wxSize(w, h));

//    toolBar->AddTool(wxID_NEW, _T("新建"),
 //                    toolBarBitmaps[Tool_new], wxNullBitmap, wxITEM_NORMAL,
//                     _T("新建文件"), _T("This is help for new file tool"));
    toolBar->AddTool(wxID_OPEN, _T("打开"),
                     toolBarBitmaps[Tool_open], wxNullBitmap, wxITEM_NORMAL,
                     _T("打开文件"), _T("This is help for open file tool"));

    // the generic toolbar doesn't really support this
#if wxUSE_TOOLBAR_NATIVE && !defined(__WXX11__) || defined(__WXUNIVERSAL__)
    // adding a combo to a vertical toolbar is not very smart
/*    if ( !( toolBar->IsVertical() ) )
    {
        wxComboBox *combo = new wxComboBox(toolBar, ID_COMBO, wxEmptyString, wxDefaultPosition, wxSize(100,-1) );
        combo->Append(_T("This"));
        combo->Append(_T("is a"));
        combo->Append(_T("combobox"));
        combo->Append(_T("in a"));
        combo->Append(_T("toolbar"));
        toolBar->AddControl(combo);

        wxSpinCtrl *spin = new wxSpinCtrl( toolBar, ID_SPIN, wxT("0"), wxDefaultPosition, wxSize(80,wxDefaultCoord), 0, 0, 100 );
        toolBar->AddControl( spin );
        
        wxTextCtrl *text = new wxTextCtrl( toolBar, -1, wxT("text"), wxDefaultPosition, wxSize(80,wxDefaultCoord) );
        toolBar->AddControl( text );
        
        wxSearchCtrl *srch = new wxSearchCtrl( toolBar, -1, wxT("xx"), wxDefaultPosition, wxSize(80,wxDefaultCoord), wxSUNKEN_BORDER );
        toolBar->AddControl( srch );
    }
*/
#endif // toolbars which don't support controls

    toolBar->AddTool(wxID_SAVE, _T("保存"), toolBarBitmaps[Tool_save], wxNullBitmap, wxITEM_NORMAL, _T("保存文件"), _T("This is help for save file tool"));
    //toolBar->AddTool(wxID_SAVE, _T("保存"), toolBarBitmaps[Tool_save], _T("保存文件"), wxITEM_CHECK);
    toolBar->AddTool(wxID_COPY, _T("复制"), toolBarBitmaps[Tool_copy], _T("复制图形"), wxITEM_CHECK);
    toolBar->AddTool(wxID_CUT, _T("剪切"), toolBarBitmaps[Tool_cut], _T("剪切图形"));
    toolBar->AddTool(wxID_PASTE, _T("粘贴"), toolBarBitmaps[Tool_paste], _T("粘贴图形"));
	toolBar->AddSeparator();//其后为视图操作区
	toolBar->AddTool(ID_ZOOM_ALL, _T("全部显示"), toolBarBitmaps[Tool_zoomall], _T("显示图形区域中所有图形"));
	toolBar->AddTool(ID_REFRESH, _T("刷新"), toolBarBitmaps[Tool_refresh], _T("刷新视图中显示的图形"));
	toolBar->AddSeparator();//其后为加工操作区
	toolBar->AddTool(ID_DOTRACK, _T("生成路径"), toolBarBitmaps[Tool_dotrack], _T("生成图形的加工路径"));
/*
    if ( m_useCustomDisabled )
    {
        wxBitmap bmpDisabled(w, h);
        {
            wxMemoryDC dc;
            dc.SelectObject(bmpDisabled);
            dc.DrawBitmap(toolBarBitmaps[Tool_print], 0, 0);

            wxPen pen(*wxRED, 5);
            dc.SetPen(pen);
            dc.DrawLine(0, 0, w, h);
        }

        toolBar->AddTool(wxID_PRINT, _T("Print"), toolBarBitmaps[Tool_print],
                         bmpDisabled);
    }
    else
    {
        toolBar->AddTool(wxID_PRINT, _T("Print"), toolBarBitmaps[Tool_print],
                         _T("Delete this tool. This is a very long tooltip to test whether it does the right thing when the tooltip is more than Windows can cope with."));
    }
*/
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_HELP, _T("帮助"), toolBarBitmaps[Tool_help], _T("显示软件信息"));

    if ( !m_pathBmp.empty() )
    {
        // create a tool with a custom bitmap for testing
        wxImage img(m_pathBmp);
        if ( img.Ok() )
        {
            if ( img.GetWidth() > w && img.GetHeight() > h )
                img = img.GetSubImage(wxRect(0, 0, w, h));

            toolBar->AddSeparator();
            toolBar->AddTool(wxID_ANY, _T("Custom"), img);
        }
    }

    // after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes
    toolBar->Realize();

    toolBar->SetRows(!(toolBar->IsVertical()) ? m_rows : 10 / m_rows);
}

// ----------------------------------------------------------------------------
// EL_Frame
// ----------------------------------------------------------------------------

// Define my frame constructor
EL_Frame::EL_Frame(wxFrame* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
       : wxFrame(parent, id, title, pos, size, style)
{
    m_tbar = NULL;
    m_smallToolbar = true;
    m_horzText = false;
    m_useCustomDisabled = false;
    m_showTooltips = true;
    layerselect = NULL;
	colorselect = NULL;
	notebook = NULL;
    m_rows = 1;
//    m_nPrint = 1;

#if wxUSE_STATUSBAR
    // Give it a status line
    CreateStatusBar();
#endif

    // Give it an icon
    SetIcon(wxICON(mondrian));

    // Make a menubar
    wxMenu *tbarMenu = new wxMenu;
    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLE_TOOLBAR,
                              _T("Toggle &toolbar\tCtrl-Z"),
                              _T("Show or hide the toolbar"));

/*    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLE_ANOTHER_TOOLBAR,
                              _T("Toggle &another toolbar\tCtrl-A"),
                              _T("Show/hide another test toolbar"));

    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLE_HORIZONTAL_TEXT,
                              _T("Toggle hori&zontal text\tCtrl-H"),
                              _T("Show text under/alongside the icon"));
*/
    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLETOOLBARSIZE,
                              _T("&Toggle toolbar size\tCtrl-S"),
                              _T("Toggle between big/small toolbar"));

    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLETOOLBARROWS,
                              _T("Toggle number of &rows\tCtrl-R"),
                              _T("Toggle number of toolbar rows between 1 and 2"));

    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLETOOLTIPS,
                              _T("Show &tooltips\tCtrl-L"),
                              _T("Show tooltips for the toolbar tools"));

/*    tbarMenu->AppendCheckItem(IDM_TOOLBAR_TOGGLECUSTOMDISABLED,
                              _T("Use c&ustom disabled images\tCtrl-U"),
                              _T("Switch between using system-generated and custom disabled images"));

*/
    tbarMenu->AppendSeparator();
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_TOP_ORIENTATION,
                              _T("Set toolbar at the top of the window"),
                              _T("Set toolbar at the top of the window"));
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_LEFT_ORIENTATION,
                              _T("Set toolbar at the left of the window"),
                              _T("Set toolbar at the left of the window"));
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_BOTTOM_ORIENTATION,
                              _T("Set toolbar at the bottom of the window"),
                              _T("Set toolbar at the bottom of the window"));
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_RIGHT_ORIENTATION,
                              _T("Set toolbar at the right edge of the window"),
                              _T("Set toolbar at the right edge of the window"));
    tbarMenu->AppendSeparator();

//    tbarMenu->Append(IDM_TOOLBAR_ENABLEPRINT, _T("&Enable print button\tCtrl-E"));
//    tbarMenu->Append(IDM_TOOLBAR_DELETEPRINT, _T("&Delete print button\tCtrl-D"));
//    tbarMenu->Append(IDM_TOOLBAR_INSERTPRINT, _T("&Insert print button\tCtrl-I"));
/*    tbarMenu->Append(IDM_TOOLBAR_TOGGLEHELP, _T("Toggle &help button\tCtrl-T"));
    tbarMenu->AppendSeparator();
    tbarMenu->Append(IDM_TOOLBAR_TOGGLERADIOBTN1, _T("Toggle &1st radio button\tCtrl-1"));
    tbarMenu->Append(IDM_TOOLBAR_TOGGLERADIOBTN2, _T("Toggle &2nd radio button\tCtrl-2"));
    tbarMenu->Append(IDM_TOOLBAR_TOGGLERADIOBTN3, _T("Toggle &3rd radio button\tCtrl-3"));
    tbarMenu->AppendSeparator();
    tbarMenu->Append(IDM_TOOLBAR_CHANGE_TOOLTIP, _T("Change tool tip"));
    tbarMenu->AppendSeparator();
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_SHOW_TEXT, _T("Show &text\tCtrl-Alt-T"));
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_SHOW_ICONS, _T("Show &icons\tCtrl-Alt-I"));
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_SHOW_BOTH, _T("Show &both\tCtrl-Alt-B"));
    tbarMenu->AppendSeparator();
    tbarMenu->Append(IDM_TOOLBAR_CUSTOM_PATH, _T("Custom &bitmap...\tCtrl-B"));
*/
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, _T("退出(&X)\tAlt-X"), _T("Quit EasyLaser") );

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, _T("关于(&A)"), _T("EasyLaser 0.10 /n All Right Reserved!"));

    wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );

    menuBar->Append(fileMenu, _T("文件(&F)"));
    menuBar->Append(tbarMenu, _T("工具栏(&T)"));
    menuBar->Append(helpMenu, _T("帮助(&H)"));

    // Associate the menu bar with the frame
    SetMenuBar(menuBar);

//    menuBar->Check(IDM_TOOLBAR_SHOW_BOTH, true);
//    menuBar->Check(IDM_TOOLBAR_TOGGLETOOLTIPS, true);

    menuBar->Check(IDM_TOOLBAR_TOP_ORIENTATION, true );
    m_toolbarPosition = TOOLBAR_TOP;
    // Create the toolbar
    RecreateToolbar();
	
	m_topsplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER);
	//m_topsplitter->SetSashGravity(1.0);
	m_topsplitter->SetMinimumPaneSize(4);    //设置切分窗口最小尺寸为20

//    m_panel = new wxPanel(this, wxID_ANY);
#if USE_UNMANAGED_TOOLBAR
//    m_extraToolBar = new wxToolBar(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_TEXT|wxTB_FLAT|wxTB_TOP);
    PopulateToolbar(m_extraToolBar);
#else
    m_extraToolBar = NULL;
#endif


    // Second page: a text ctrl and a button
    wxPanel *panel = new wxPanel(m_topsplitter, wxID_ANY );
	wxSize client_size = GetClientSize();
	long m_notebook_style = wxAUI_NB_BOTTOM | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS |
                                     wxAUI_NB_MIDDLE_CLICK_CLOSE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
       notebookproperty = new wxAuiNotebook(panel, ID_NOTEBOOK_PROPERTY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    m_notebook_style);
	   notebookproperty->SetSizeHints(50,60);
  //  notebook->AddPage( panel, _T("Page 5") );
	m_graphicsTree = new wxTreeCtrl(notebookproperty, wxID_ANY,
                                      wxDefaultPosition, wxDefaultSize,
                                      wxTR_DEFAULT_STYLE | wxDOUBLE_BORDER| wxTR_EDIT_LABELS);  //创建显示图形结构的树列表
    wxImageList* imglist = new wxImageList(16, 16, true, 4);
	imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxBITMAP(layer));
    imglist->Add(wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK, wxART_OTHER, wxSize(16,16)));
	imglist->Add(wxArtProvider::GetBitmap(wxART_DEL_BOOKMARK, wxART_OTHER, wxSize(16,16)));
    m_graphicsTree->AssignImageList(imglist);
	m_colorTree = new wxTreeCtrl(notebookproperty, wxID_ANY,
                                      wxDefaultPosition, wxDefaultSize,
                                      wxTR_DEFAULT_STYLE | wxDOUBLE_BORDER| wxTR_EDIT_LABELS);  //创建显示颜色结构的树列表
    notebookproperty->AddPage(m_graphicsTree,_T("图层"),true);
	notebookproperty->AddPage(m_colorTree,_T("颜色"));
    wxSizer *panelsizer = new wxBoxSizer( wxVERTICAL );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////创建图层，颜色选择框
	wxBoxSizer *statsizerproperty = new wxStaticBoxSizer(new wxStaticBox(panel, wxID_ANY, _T("属性")), wxVERTICAL );
	panelsizer->Add(statsizerproperty, wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	wxFlexGridSizer* flexGridSizerproperty = new wxFlexGridSizer(2, 2, 0, 0);
	statsizerproperty->Add(flexGridSizerproperty,wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizerproperty->AddGrowableCol(1);
	flexGridSizerproperty->Add(new wxStaticText(panel, wxID_ANY, _T("图层")),wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	layerselect = new EL_LayerSelectCombo();
	layerselect->Create(panel, ID_LAYERSELECT, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultCoord,20),wxCB_READONLY);
	flexGridSizerproperty->Add(layerselect, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));

	flexGridSizerproperty->Add(new wxStaticText(panel, wxID_ANY, _T("颜色")),wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	colorselect = new EL_ColorSelectCombo();
	for(int i = 0; i < 257; i ++)
	{
		colorselect->AppendString(wxString::Format("%d",i));
	}
	colorselect->Create(panel, ID_COLORSELECT, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultCoord,20),wxCB_READONLY);
	flexGridSizerproperty->Add(colorselect, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////创建加工方式选项框
	wxBoxSizer *statsizer = new wxStaticBoxSizer(new wxStaticBox(panel, wxID_ANY, _T("加工方式")), wxVERTICAL );
	panelsizer->Add(statsizer,wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer(1, 3, 0, 0);
	statsizer->Add(flexGridSizer,wxGROW|wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL|wxFIXED_MINSIZE);
	flexGridSizer->AddGrowableRow(0);
	wxCheckBox* checkBoxcut = new wxCheckBox( panel, ID_CHECKBOXCUT,wxT("&切割"), wxDefaultPosition, wxDefaultSize);
	flexGridSizer->Add(checkBoxcut, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);
	wxCheckBox* checkBoxsclpt = new wxCheckBox( panel, ID_CHECKBOXSCLPT,wxT("&雕刻"), wxDefaultPosition, wxDefaultSize);
	flexGridSizer->Add(checkBoxsclpt, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);
	wxCheckBox* checkBoxnot = new wxCheckBox( panel, ID_CHECKBOXNOT,wxT("&不加工"), wxDefaultPosition, wxDefaultSize);
	flexGridSizer->Add(checkBoxnot, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);
	statsizer->Fit(panel);
	statsizer->SetSizeHints(panel);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////创建切割参数设置框
	wxBoxSizer *statsizercut = new wxStaticBoxSizer(new wxStaticBox(panel, wxID_ANY, _T("切割参数设置")), wxVERTICAL );
	panelsizer->Add(statsizercut, wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	wxFlexGridSizer* flexGridSizercut = new wxFlexGridSizer(4, 3, 0, 0);
	statsizercut->Add(flexGridSizercut,wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizercut->AddGrowableCol(1);
	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("拐角功率")),wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	conerpower = new wxTextCtrl( panel, wxID_ANY, _T("40"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	
	flexGridSizercut->Add(conerpower, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("%")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("切割功率")),wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	cutpower = new wxTextCtrl( panel, wxID_ANY, _T("100"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizercut->Add(cutpower, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("%")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("切割速度")),wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	cutspeed = new wxTextCtrl( panel, wxID_ANY, _T("30"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizercut->Add(cutspeed, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("m/min")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("切割加速度")),wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	cutaddspeed = new wxTextCtrl( panel, wxID_ANY, _T("0.6"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizercut->Add(cutaddspeed, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizercut->Add(new wxStaticText(panel, wxID_ANY, _T("m/s2")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));
	statsizercut->Fit(panel);
	statsizercut->SetSizeHints(panel);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////创建雕刻参数设置框
	wxBoxSizer *statsizersclpt = new wxStaticBoxSizer(new wxStaticBox(panel, wxID_ANY, _T("雕刻参数设置")), wxVERTICAL );
	panelsizer->Add(statsizersclpt,wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	wxFlexGridSizer* flexGridSizersclpt = new wxFlexGridSizer(5, 3, 0, 0);
	statsizersclpt->Add(flexGridSizersclpt,wxSizerFlags().Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizersclpt->AddGrowableCol(1);
	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("雕刻功率")), wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	sclptpower = new wxTextCtrl( panel, wxID_ANY, _T("50"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizersclpt->Add(sclptpower, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("%")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("雕刻速度")), wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	sclptspeed = new wxTextCtrl( panel, wxID_ANY, _T("24"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizersclpt->Add(sclptspeed, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("m/min")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("雕刻加速度")), wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	sclptaddspeed = new wxTextCtrl( panel, wxID_ANY, _T("16"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizersclpt->Add(sclptaddspeed, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("m/s2")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("雕刻密度")), wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	sclptdensity = new wxTextCtrl( panel, wxID_ANY, _T("0.05"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizersclpt->Add(sclptdensity, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("mm")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));

	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("横向补偿量")), wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
	sclptXfix = new wxTextCtrl( panel, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(wxDefaultCoord,20) );
	flexGridSizersclpt->Add(sclptXfix, wxSizerFlags(1).Align(wxGROW | wxALIGN_CENTER_VERTICAL));
	flexGridSizersclpt->Add(new wxStaticText(panel, wxID_ANY, _T("mm")),wxSizerFlags().Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL));
	statsizersclpt->Fit(panel);
	statsizersclpt->SetSizeHints(panel);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    wxButton *button2 = new wxButton( panel, wxID_ANY, _T("应用") );
    panelsizer->Add( button2,wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
    panelsizer->Add(notebookproperty,wxSizerFlags(1).Expand().Border(wxALL, 1));
    panel->SetAutoLayout( true );
    panel->SetSizer(panelsizer);
	panelsizer->SetSizeHints( this );
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	client_size = GetClientSize();
	m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_CLOSE_BUTTON| wxNO_BORDER;
       notebook = new wxAuiNotebook(m_topsplitter, ID_NOTEBOOK_VIEW,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    m_notebook_style);
	EL_DocView *docview = new EL_DocView(notebook,wxDefaultPosition, wxDefaultSize, 0);	
	m_topsplitter->SplitVertically(panel, notebook,200);
    docview->setTree(m_graphicsTree);
	docview->SetBackgroundColour(wxColour(wxT("BLACK")));
	notebook->AddPage(docview, _T("DocView"));
	this->SetSize(1024,768);
}

wxTreeCtrl* EL_Frame::CreateTreeCtrl(wxWindow *  parent)
{
    wxTreeCtrl* tree = new wxTreeCtrl(parent, wxID_ANY,
                                      wxPoint(0,0), wxSize(160,250),
                                      wxTR_DEFAULT_STYLE | wxDOUBLE_BORDER);

    wxImageList* imglist = new wxImageList(16, 16, true, 3);
    imglist->Add(wxBITMAP(layer));
    imglist->Add(wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK, wxART_OTHER, wxSize(16,16)));
	imglist->Add(wxArtProvider::GetBitmap(wxART_DEL_BOOKMARK, wxART_OTHER, wxSize(16,16)));
    tree->AssignImageList(imglist);

    wxTreeItemId root = tree->AddRoot(wxT("wxAUI Project"), 0);
    wxArrayTreeItemIds items;

    items.Add(tree->AppendItem(root, wxT("Item 1"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 2"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 3"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 4"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 5"), 0));

    int i, count;
    for (i = 0, count = items.Count(); i < count; ++i)
    {
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, wxT("Subitem 1"), 1);
        tree->AppendItem(id, wxT("Subitem 2"), 1);
        tree->AppendItem(id, wxT("Subitem 3"), 1);
        tree->AppendItem(id, wxT("Subitem 4"), 1);
        tree->AppendItem(id, wxT("Subitem 5"), 1);
    }


    tree->Expand(root);

    return tree;
}
void EL_Frame::LayoutChildren()
{
    wxSize size = GetClientSize();

    int offset;
    if ( m_tbar )
    {
        m_tbar->SetSize(0, 0, wxDefaultCoord, size.y);

        offset = m_tbar->GetSize().x;
    }
    else
    {
        offset = 0;
    }

    m_topsplitter->SetSize(offset, 0, size.x - offset, size.y);
}

void EL_Frame::OnSize(wxSizeEvent& event)
{
    if ( m_tbar )
    {
        LayoutChildren();
    }
    else
    {
        event.Skip();
    }
}

void EL_Frame::OnToggleToolbar(wxCommandEvent& WXUNUSED(event))
{
    wxToolBar *tbar = GetToolBar();

    if ( !tbar )
    {
        RecreateToolbar();
    }
    else
    {
        delete tbar;

        SetToolBar(NULL);
    }
}

void EL_Frame::OnToggleHorizontalText(wxCommandEvent& WXUNUSED(event))
{
    m_horzText = !m_horzText;

    RecreateToolbar();
}

void EL_Frame::OnToggleAnotherToolbar(wxCommandEvent& WXUNUSED(event))
{
    if ( m_tbar )
    {
        delete m_tbar;
        m_tbar = NULL;
    }
    else
    {
        long style = GetToolBar() ? GetToolBar()->GetWindowStyle()
                                  : TOOLBAR_STYLE;
        style &= ~wxTB_HORIZONTAL;
        style |= wxTB_VERTICAL;

        m_tbar = new wxToolBar(this, wxID_ANY,
                               wxDefaultPosition, wxDefaultSize,
                               style);

        m_tbar->SetMargins(4, 4);

        m_tbar->AddRadioTool(IDM_TOOLBAR_OTHER_1, _T("First"), wxBITMAP(new));
        m_tbar->AddRadioTool(IDM_TOOLBAR_OTHER_2, _T("Second"), wxBITMAP(open));
        m_tbar->AddRadioTool(IDM_TOOLBAR_OTHER_3, _T("Third"), wxBITMAP(save));
        m_tbar->AddSeparator();
        m_tbar->AddTool(wxID_HELP, _T("Help"), wxBITMAP(help));

        m_tbar->Realize();
    }

    LayoutChildren();
}

void EL_Frame::OnToggleToolbarSize(wxCommandEvent& WXUNUSED(event))
{
    m_smallToolbar = !m_smallToolbar;

    RecreateToolbar();
}

void EL_Frame::OnToggleToolbarRows(wxCommandEvent& WXUNUSED(event))
{
    // m_rows may be only 1 or 2
    m_rows = 3 - m_rows;

    GetToolBar()->SetRows(!(GetToolBar()->IsVertical()) ? m_rows : 10 / m_rows);

    //RecreateToolbar(); -- this is unneeded
}

void EL_Frame::OnToggleTooltips(wxCommandEvent& WXUNUSED(event))
{
    m_showTooltips = !m_showTooltips;

    RecreateToolbar();
}

void EL_Frame::OnToggleCustomDisabled(wxCommandEvent& WXUNUSED(event))
{
    m_useCustomDisabled = !m_useCustomDisabled;

    RecreateToolbar();
}

void EL_Frame::OnChangeOrientation(wxCommandEvent& event)
{
    switch( event.GetId() )
    {
        case IDM_TOOLBAR_LEFT_ORIENTATION:
            m_toolbarPosition = TOOLBAR_LEFT;
            break;
        case IDM_TOOLBAR_TOP_ORIENTATION:
            m_toolbarPosition = TOOLBAR_TOP;
            break;
        case IDM_TOOLBAR_RIGHT_ORIENTATION:
            m_toolbarPosition = TOOLBAR_RIGHT;
            break;
        case IDM_TOOLBAR_BOTTOM_ORIENTATION:
            m_toolbarPosition = TOOLBAR_BOTTOM;
            break;
    }
    RecreateToolbar();
}

void EL_Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void EL_Frame::OnAbout(wxCommandEvent& event)
{
    (void)wxMessageBox(_T("EasyLaser V0.10 \nAll Right Reserved!"), _T("About EasyLaser"));
	event.Skip();
}

void EL_Frame::OnToolLeftClick(wxCommandEvent& event)
{
    if(event.GetId() == wxID_OPEN)
	{
		DoFileOpen();
	}
    if (event.GetId() == wxID_COPY)
    {
 //       DoEnablePrint();
    }

    if (event.GetId() == wxID_CUT)
    {
        DoToggleHelp();
    }

    if (event.GetId() == wxID_PRINT)
    {
 //       DoDeletePrint();
    }
	if(event.GetId() == ID_ZOOM_ALL)
	{
		((EL_DocView*)notebook->GetPage(notebook->GetSelection()))->zoomAll();
	}
	if(event.GetId() == ID_REFRESH)
	{
		((EL_DocView*)notebook->GetPage(notebook->GetSelection()))->refreshGraphics();
	}
		if(event.GetId() == ID_DOTRACK)
	{
		((EL_DocView*)notebook->GetPage(notebook->GetSelection()))->getTrack();//生成路径
	}
    if(event.GetId() == wxID_SAVE){
        wxWindow* pTemp = notebook->GetPage(notebook->GetSelection());
        ((EL_DocView*)notebook->GetPage(notebook->GetSelection()))->saveFile();
    }
}

void EL_Frame::OnToolRightClick(wxCommandEvent& event)  //工具条右击的响应函数
{
	event.Skip();
}

void EL_Frame::OnCombo(wxCommandEvent& event)
{
    wxLogStatus(_T("Combobox string '%s' selected"), event.GetString().c_str());
}
void EL_Frame::OnAuiNotebook(wxAuiNotebookEvent& event)
{
	EL_DocView * view = (EL_DocView*)notebook->GetPage(notebook->GetSelection());
	vector<LP_Layer> *inlayers = NULL;
	vector<LP_Layer*> *selectedlayers = NULL;
	if(view != NULL)
	{
		m_graphicsTree->DeleteAllItems();
		wxTreeItemId oroot = m_graphicsTree->GetRootItem();
		m_graphicsTree->Delete(oroot);
		wxArrayTreeItemIds items;
		//	wxString sepchar = wxT("/");
		//	wxString sepchar2 = wxT("\\");
		wxTreeItemId root = m_graphicsTree->AddRoot(view->getFilePath(),0,1);
		inlayers = &(view->getGraphics()->getLayers());
		selectedlayers = &(view->getSelected()->layers);
		layerselect->Clear();
		for(unsigned int ct = 0; ct < inlayers->size(); ct ++)  //将当前文档所有层的名称加入层树列表
		{
			items.Add(m_graphicsTree->AppendItem(root, wxT(inlayers->at(ct).name), 1));
			layerselect->AppendString(wxT(inlayers->at(ct).name));
		}
		if(selectedlayers->empty() == false)    //加入层选择框
		{
			if(selectedlayers->size() < 2)
			{
				unsigned int index = layerselect->FindString(selectedlayers->front()->name);
				if((index >= 0) && (index < layerselect->GetCount()))
				{
					layerselect->Select(index);
				}	
			}
			else
			{
				layerselect->AppendString(wxT("多层"));
				layerselect->Select(layerselect->GetCount() - 1);
			}
		}
		m_graphicsTree->Expand(root);
	}
	event.Skip();
}
void EL_Frame::OnViewSelectionChanged(EL_DocViewEvent & event)
{
	int index = -1;
	vector<LP_Layer> *inlayers = NULL;
	vector<LP_Layer*> *selectedlayers = NULL;
	if(event.getView() != NULL)
	{
		inlayers = &(event.getView()->getGraphics()->getLayers());
		selectedlayers = &(event.getView()->getSelected()->layers);

		layerselect->Clear();
		for(unsigned int ct = 0; ct < inlayers->size(); ct ++)  //将当前文档所有层的名称加入层选择列表
		{
			layerselect->AppendString(wxT(inlayers->at(ct).name));
		}
		if(selectedlayers->size() > 0)
		{
			if(selectedlayers->size() < 2)
			{
				index = (int)(layerselect->FindString(selectedlayers->front()->name));
				if((index >= 0) && (index < (int)(layerselect->GetCount())))
				{
					layerselect->Select(index);
				}	
			}
			else
			{
				index = layerselect->FindString(wxT("多层"));
				if(index >= 0 && index < (int)(layerselect->GetCount()))
				{
					layerselect->Select(index);
				}
				else
				{
					layerselect->AppendString(wxT("多层"));
					layerselect->Select(layerselect->GetCount() - 1);
				}
			}
		}
		else
		{
			if(layerselect->GetCount() < inlayers->size())  //此情况不应出现否则就是出现了逻辑错误
			{
				layerselect->Clear();
				for(unsigned int ct = 0; ct < inlayers->size(); ct ++)  //将当前文档所有层的名称加入层选择列表
				{
					layerselect->AppendString(wxT(inlayers->at(ct).name));
				}
			}
			layerselect->Select(-1);
		}

	}
	vector<int> colors = event.getView()->getSelected()->getColors();
	if(colors.size() > 0)
	{
		if(colors.front() >= 0 && colors.front() < 257)
		{
	        colorselect->Select(colors.front());
		}
	}
	else
	{
		colorselect->Select(-1);
	}
	event.Skip();
}
/*
void EL_Frame::DoEnablePrint()
{
    if ( !m_nPrint )
        return;

    wxToolBarBase *tb = GetToolBar();
    tb->EnableTool(wxID_PRINT, !tb->GetToolEnabled(wxID_PRINT));
}

void EL_Frame::DoDeletePrint()
{
    if ( !m_nPrint )
        return;

    wxToolBarBase *tb = GetToolBar();
    tb->DeleteTool( wxID_PRINT );

    m_nPrint--;
}
*/
bool EL_Frame::DoFileOpen()
{
	wxString caption = wxT("Choose a file");
	wxString wildcard =wxT("dxf files (*.dxf)|*.dxf|GIF files (*.gif)|*.gif");
	wxString defaultDir = wxT("c:\\temp");
	wxString defaultFilename = wxEmptyString;
	wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxOPEN);
	if (dialog .ShowModal() == wxID_OK)
	{
		wxString path = dialog.GetPath();
		int pagecount = notebook->GetPageCount();
		for(int i = 0; i < pagecount; i++)
		{
			if(((EL_DocView*)notebook->GetPage(i))->getFilePath() == path)
			{
				notebook->GetPage(i)->SetFocus();
				if(wxMessageBox(wxT("File: ")+path+wxT("  always open! \n Reload it?"),wxT("Error"),wxYES_NO|wxICON_INFORMATION,this) == wxYES)
				{
					((EL_DocView *)notebook->GetPage(i))->OnFileOpen(path);
					return true;
				}
				else
				{
				    return false;
				}
			}
			else if(((EL_DocView*)notebook->GetPage(i))->getFilePath().size() < 1)
			{
				notebook->SetPageText(i,dialog.GetFilename());
				((EL_DocView *)notebook->GetPage(i))->OnFileOpen(path);
				return true;
			}
		}
	//	int filterIndex = dialog.GetFilterIndex();
		EL_DocView *ndocview = new EL_DocView(notebook,wxDefaultPosition, wxDefaultSize, 0);
		ndocview->setTree(m_graphicsTree);
		ndocview->SetBackgroundColour(wxColour(wxT("WHITE")));
	    notebook->AddPage(ndocview, dialog.GetFilename(),true);
		ndocview->OnFileOpen(path);
	}
	return true;
}
void EL_Frame::DoToggleHelp()
{
    wxToolBarBase *tb = GetToolBar();
    tb->ToggleTool( wxID_HELP, !tb->GetToolState( wxID_HELP ) );
}

void EL_Frame::OnUpdateCopyAndCut(wxUpdateUIEvent& event)  //是否允许复制和剪切，要从当前操作的控件中获得
{
    event.Enable(false);
}

void EL_Frame::OnUpdateToggleHorzText(wxUpdateUIEvent& event)
{
    wxToolBar *tbar = GetToolBar();
    event.Enable( tbar &&
                    tbar->HasFlag(wxTB_TEXT) &&
                        !tbar->HasFlag(wxTB_NOICONS) );
}

void EL_Frame::OnChangeToolTip(wxCommandEvent& WXUNUSED(event))
{
    GetToolBar()->SetToolShortHelp(wxID_NEW, _T("New toolbar button"));
}

void EL_Frame::OnToolbarStyle(wxCommandEvent& event)
{
    long style = GetToolBar()->GetWindowStyle();
    style &= ~(wxTB_NOICONS | wxTB_TEXT);

    switch ( event.GetId() )
    {
        case IDM_TOOLBAR_SHOW_TEXT:
            style |= wxTB_NOICONS | wxTB_TEXT;
            break;

        case IDM_TOOLBAR_SHOW_ICONS:
            // nothing to do
            break;

        case IDM_TOOLBAR_SHOW_BOTH:
            style |= wxTB_TEXT;
    }

    GetToolBar()->SetWindowStyle(style);
}

void EL_Frame::OnToolbarCustomBitmap(wxCommandEvent& WXUNUSED(event))
{
    m_pathBmp = wxFileSelector(_T("Custom bitmap path"));

    RecreateToolbar();
}
/*
void EL_Frame::OnInsertPrint(wxCommandEvent& WXUNUSED(event))
{
    m_nPrint++;

    wxToolBarBase *tb = GetToolBar();
    tb->InsertTool(0, wxID_PRINT, _T("New print"),
                   wxBITMAP(print), wxNullBitmap,
                   wxITEM_NORMAL,
                   _T("Delete this tool"),
                   _T("This button was inserted into the toolbar"));

    // must call Realize() after adding a new button
    tb->Realize();
}
*/
void EL_Frame::OnToggleRadioBtn(wxCommandEvent& event)
{
    if ( m_tbar )
    {
        m_tbar->ToggleTool(IDM_TOOLBAR_OTHER_1 +
                            event.GetId() - IDM_TOOLBAR_TOGGLERADIOBTN1, true);
    }
}

#pragma once

#include "f:\wxwidgets-2.8.7\include\wx\scrolwin.h"
#include "wx/toolbar.h"
#include "wx/treectrl.h"
#include "wx/artprov.h"
#include "wx/aui/aui.h"
#include "wx/splitter.h"
#include "../EasyLaserLib/Src/LP_Graph.h"
#include "../EasyLaserLib/Src/LP_dxfInterface.h"

// ----------------------------------------------------------------------------
// event constants
// ----------------------------------------------------------------------------

// declare a custom event type
//
// note that in wxWin 2.3+ these macros expand simply into the following code:
//
//  extern const wxEventType wxEVT_MY_CUSTOM_COMMAND;
//
//  const wxEventType wxEVT_MY_CUSTOM_COMMAND = wxNewEventType();
//
// and you may use this code directly if you don't care about 2.2 compatibility

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_VIEW_SELECTION_CHANGED, 8888)
END_DECLARE_EVENT_TYPES()

//DEFINE_EVENT_TYPE(wxEVT_VIEW_SELECTION_CHANGED)

// it may also be convenient to define an event table macro for this event type
#define EVT_VIEW_SELECTION_CHANGED(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_VIEW_SELECTION_CHANGED, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( EL_DocViewEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

class EL_DocView :
	public wxWindow
{
public:
	EL_DocView(wxWindow *window, const wxPoint& pos, const wxSize& size, const long style);
	~EL_DocView(void);
    void OnFileOpen(wxString path);
	void zoomAll();
	void refreshGraphics();
	void zoomRect(LP_Rect zrect);
	void setTree(wxTreeCtrl* graphicsTree);
	void OnMouseEvent(wxMouseEvent &event);
	void OnLeftDown(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);
    void OnRightDown(wxMouseEvent &event);      // added by ZWW
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseWheel(wxMouseEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnPaint(wxPaintEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	virtual void OnDraw(wxDC& dc);
	LP_Graphics_Container* getGraphics();
	LP_Selected* getSelected();
	wxString getFilePath();
	void getTrack();
    void saveFile();        // added by ZWW
private:
	bool m_mirror;
	LP_Coordinate coordinate;
	LP_Graphics_Container graphics;  //保存所有图形的类
	vector<LP_CAM_Group> camgroups; //指向加工路径的指针，不指向加工路径时应为NULL
	wxTreeCtrl *m_graphicsTree; 
	DL_Dxf dl_dxf;
	wxString filepath;
	wxPoint mpoldpoint;       //鼠标滚轮平移视图时旧点的坐标。
	double scalerate;
    int command;
	LP_Point3 firstpoint;   //选择命令的第一个点
	LP_Point3 secondpoint;  //选择命令的第二个点
	wxBitmap viewbmp;
	LP_Selected sel; //内部保存每一个被选择的图形的指针
	bool ongraphics; //表示鼠标点击时是否在图形上
    DECLARE_EVENT_TABLE()
};
class EL_DocViewEvent : public wxCommandEvent
{
public:
	EL_DocViewEvent(wxEventType commandType = wxEVT_NULL,int id = 0): wxCommandEvent(commandType, id)
	{
		view = NULL;
	}
	EL_DocViewEvent(const EL_DocViewEvent& event):wxCommandEvent(event)
	{
        setView(event.getView());
	}
	virtual wxEvent *Clone() const
	{ 
		return new EL_DocViewEvent(*this);
	}
	void setView(EL_DocView * ve)
	{
		view = ve;
	}
	EL_DocView * getView() const
	{
		return view;
	}
private:
	EL_DocView * view;
	DECLARE_DYNAMIC_CLASS(EL_DocViewEvent);
};
typedef void (wxEvtHandler::*EL_DocViewEventFunction) (EL_DocViewEvent&);
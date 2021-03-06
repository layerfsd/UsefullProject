#include "EL_DocView.h"
#include "wx/dcmirror.h"
#include "wx/msgdlg.h"
#include "wx/dcbuffer.h"
#include "wx/treectrl.h"
#include "wx/image.h"
#include "../EasyLaserLib/Src/LP_Advanced.h"
#include "../EasyLaserLib/Src/LP_dxfInterface.h"
enum
{
	SELECT = 2000,
	MOVE,
	ROTATE,
	SCALE,
	MIRROR,
	MOVEVIEW,
};
// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(EL_DocView, wxWindow)
    EVT_PAINT(EL_DocView::OnPaint)
	EVT_SET_FOCUS(EL_DocView::OnSetFocus)
    EVT_MOUSE_EVENTS(EL_DocView::OnMouseEvent)
//    EVT_LEFT_DOWN(EL_DocView::OnLeftDown)  //不能重载此事件。重载后会与MouseWheel事件冲突。将收不到MouseWheel事件
    EVT_MOTION(EL_DocView::OnMouseMove)
	EVT_MOUSEWHEEL(EL_DocView::OnMouseWheel)
	EVT_KEY_DOWN(EL_DocView::OnKeyDown)
END_EVENT_TABLE()
EL_DocView::EL_DocView(wxWindow *window, const wxPoint& pos, const wxSize& size, const long style):
    wxWindow(window, wxID_ANY, pos, size, style)
{
    scalerate = 1.5;
	command = 0;
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);// 通知系统不要自动擦除背景
	m_graphicsTree = NULL;
	filepath.clear();
}
void EL_DocView::setTree(wxTreeCtrl* graphicsTree)
{
	m_graphicsTree = graphicsTree;
	m_graphicsTree->Expand(m_graphicsTree->GetRootItem());
}
EL_DocView::~EL_DocView(void)
{

}
void EL_DocView::OnFileOpen(wxString path)
{
	graphics.getLayers().clear();
	graphics.blocks.clear();
    LP_dxfInterface dxfinterface(graphics);
    filepath = path;
	string ts = (const char*)path.mb_str(wxConvUTF8);
	if(dl_dxf.in(ts, &dxfinterface) == false){
		wxMessageBox(wxT("Can`t open the file!"),wxT("Error"),wxOK|wxICON_INFORMATION,this);
	}
	else{
		coordinate.recreate = true;
		zoomRect(graphics.getRect(2));
		zoomRect(graphics.getRect(0));          ////////////////////////////???????????????????只调用一次时对有嵌套块的图形不生效
	}

	if(m_graphicsTree != NULL)
	{
		m_graphicsTree->DeleteAllItems();
		wxTreeItemId oroot = m_graphicsTree->GetRootItem();
		m_graphicsTree->Delete(oroot);
		wxArrayTreeItemIds items;
	//	wxString sepchar = wxT("/");
	//	wxString sepchar2 = wxT("\\");
		wxTreeItemId root = m_graphicsTree->AddRoot(filepath,0,1);
		vector<LP_Layer> *inlayers = &(graphics.getLayers());
		for(unsigned int ct = 0; ct < inlayers->size(); ct ++)
		{
			items.Add(m_graphicsTree->AppendItem(root, wxT(inlayers->at(ct).name), 1));
		}
		m_graphicsTree->Expand(root);
		EL_DocViewEvent eventCustom(wxEVT_VIEW_SELECTION_CHANGED);
		eventCustom.setView(this);
        wxPostEvent(this, eventCustom);
	}
}

void EL_DocView::zoomAll(){
	zoomRect(graphics.getRect(0));
}

void EL_DocView::refreshGraphics(){
	coordinate.setSmooth(1/coordinate.getScale());
	graphics.Create(coordinate);
	wxClientDC	clientDC(this);
	OnDraw(clientDC);
}

//缩放矩形显示
void EL_DocView::zoomRect(LP_Rect zrect){  
	if(zrect.RightTop.distanceTo(zrect.LeftBottom) > 1.0e-6){
		wxRect client;
		wxPoint spoint;
		client=GetClientRect();
		coordinate.setScale(min((client.width - 10)/(zrect.RightTop.x - zrect.LeftBottom.x), 
			(client.height - 10)/(zrect.RightTop.y - zrect.LeftBottom.y)));                //设置缩放率，将所选矩形缩放到最大显示
		coordinate.setOriginInScreen(0,0);
		spoint = coordinate.toScreen(zrect.LeftBottom);
		coordinate.setOriginInScreen(- spoint.x + 5,-spoint.y + client.GetBottom() - 5);
	}
	wxClientDC	clientDC(this);
	OnDraw(clientDC);
}

void EL_DocView::OnMouseEvent(wxMouseEvent &event)
{
	if(event.LeftDown())
	{
		OnLeftDown(event);
	}
	if(event.LeftUp())
	{
		OnLeftUp(event);
	}
    if(event.RightDown())
    {
        OnRightDown(event);
    }
	event.Skip();
}

void EL_DocView::OnLeftDown(wxMouseEvent &event)
{
	wxPoint cp(event.GetPosition());
	LP_Point3 wp = coordinate.toWorld((double)cp.x, (double)cp.y);
	if(event.CmdDown())
	{
		coordinate.revsel = true;
	}
	else
	{
		coordinate.revsel = false;
	}
	if(command == MOVE)
	{
		command = 0;
		LP_Point3 mtpl;
		mtpl = wp - firstpoint;
		sel.move(mtpl);
		graphics.Create(coordinate);
		wxClientDC	clientDC(this);
		OnDraw(clientDC);
		return;
	}
	if(command == SELECT)
	{
		command = 0;

		graphics.rectPick(coordinate.toScreen(firstpoint),cp,coordinate);
		sel.clear();
		graphics.getPick(sel);
		if(sel.Empty() == false)
		{
		    EL_DocViewEvent eventCustom(wxEVT_VIEW_SELECTION_CHANGED);
			eventCustom.setView(this);
            wxPostEvent(this, eventCustom);
		}
		wxClientDC	clientDC(this);
		OnDraw(clientDC);
		return;
	}
	if(command ==0)
	{
		firstpoint = secondpoint = wp;
		sel.clear();
		if(graphics.getPick(sel))
		{
			if(sel.checkPick(wp,coordinate))
			{
				command = MOVE;
				ongraphics = true;
				return;
			}
		}
		if(graphics.checkPick(wp,coordinate))
		{
			ongraphics = true;
			graphics.getPick(sel);
			EL_DocViewEvent eventCustom(wxEVT_VIEW_SELECTION_CHANGED);
		    eventCustom.setView(this);
            wxPostEvent(this, eventCustom);
			wxClientDC	clientDC(this);
		    OnDraw(clientDC);
		}
		else
		{
			ongraphics = false;
			command = SELECT;
		}
		return;
	}
}

void EL_DocView::OnRightDown(wxMouseEvent &event)
{
    graphics.deSelectAll();
    sel.clear();
    command = 0;
    wxClientDC	clientDC(this);
    OnDraw(clientDC);
    EL_DocViewEvent eventCustom(wxEVT_VIEW_SELECTION_CHANGED);
    eventCustom.setView(this);
    wxPostEvent(this, eventCustom);
}

void EL_DocView::OnLeftUp(wxMouseEvent &event)
{
	event.Skip();
}

void EL_DocView::OnMouseMove(wxMouseEvent &event)
{
	wxClientDC	ttclientDC(this);
	wxBufferedDC clientDC(&ttclientDC);
	if(viewbmp.IsNull() == false)    //创建了DC以后一定要绘制，否则BufferedDC的图形缓冲可能已被破坏，图形显示会出现错误
	{
	    clientDC.DrawBitmap(viewbmp,0,0);
	}
	else
	{
		OnDraw(clientDC);
	}
	wxPoint cp(event.GetPosition());
	LP_Point3 wp = coordinate.toWorld((double)cp.x, (double)cp.y);
	secondpoint = wp;
	//wxPoint lastp = secondpoint;
	secondpoint = wp;
	//coordinate.pickrect.LeftBottom = coordinate.toWorld(cp.x-4,cp.y+4);
	//coordinate.pickrect.RightTop = coordinate.toWorld(cp.x+4,cp.y-4);
	if(event.MiddleIsDown()){
		LP_Point3 pt = coordinate.getOriginInScreen();
		pt.x += ((double)cp.x - (double)mpoldpoint.x);
		pt.y += ((double)cp.y - (double)mpoldpoint.y);
		coordinate.setOriginInScreen(pt.x,pt.y);

		OnDraw(clientDC);
	}
	else
	{
		if(command == MOVE)  //平移坐标原点的位置再绘制被选图形，以实现视觉上和移动效果
		{
			LP_Point3 oldorigin = coordinate.getOriginInScreen();
			wxPoint vfirstp = coordinate.toScreen(firstpoint);
			coordinate.setOriginInScreen(oldorigin.x +(cp.x - vfirstp.x),oldorigin.y +(cp.y - vfirstp.y));
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			wxRect clientrect;
			clientrect=GetClientRect();
			int cx,cy,cx2,cy2;
			cx = clientrect.GetLeft();
			cy = clientrect.GetBottom();
			cx2 = clientrect.GetRight();
			cy2 = clientrect.GetTop();
			LP_Rect vrect(coordinate.toWorld((double)clientrect.GetLeft(),(double)clientrect.GetBottom()),
				coordinate.toWorld((double)clientrect.GetRight(),(double)clientrect.GetTop()));
			coordinate.setViewRect(vrect);
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			clientDC.SetLogicalFunction(wxSRC_INVERT);
			sel.Draw(&clientDC,coordinate);
			coordinate.setOriginInScreen(oldorigin.x,oldorigin.y);
		}
	}
	if(command == SELECT)
	{
		if(firstpoint.x <= secondpoint.x)
		{

		    clientDC.SetPen(coordinate.windowselpen);
		    clientDC.SetBrush(coordinate.windowselbrush);
		}
		else
		{
			clientDC.SetPen(coordinate.rectselpen);
		    clientDC.SetBrush(coordinate.rectselbrush);
		}
		clientDC.DrawBitmap(viewbmp,0,0);
		clientDC.SetLogicalFunction(wxOR);
		wxPoint rectfp = coordinate.toScreen(firstpoint);
		wxPoint rectsp = coordinate.toScreen(secondpoint);
		clientDC.DrawRectangle(rectfp.x, rectfp.y, rectsp.x - rectfp.x, rectsp.y - rectfp.y);
	}

	mpoldpoint = cp;

}
void EL_DocView::OnMouseWheel(wxMouseEvent &event)
{
	if(event.m_controlDown)
	{
		event.Skip(true);
	}
	else
	{
		wxClientDC	clientDC(this);
		wxPoint cp(event.GetLogicalPosition(clientDC));
		//ScreenToClient(&lp);
		LP_Point3 sp = coordinate.toWorld((double)cp.x, (double)cp.y);
		int rot = event.GetWheelRotation();
		if(rot > 0){
			if(coordinate.getScale() < 1073741824.0)
			{
			    coordinate.setScale(coordinate.getScale() * scalerate);
			}
		}
		else if(rot < 0){
			coordinate.setScale(coordinate.getScale() / scalerate);
		}
		coordinate.toScreenD(sp);
		LP_Point3 pt = coordinate.getOriginInScreen();
		pt.x -= sp.x - (double)cp.x;
		pt.y -= sp.y - (double)cp.y;
		coordinate.setOriginInScreen(pt.x,pt.y);

		//selfirstpoint = coordinate.toScreen(selfp);  //先将选择矩形的第一个点转换到世界坐标中，视图缩放完成后再转换回屏幕坐标。
		OnDraw(clientDC);
	}
}
void EL_DocView::OnKeyDown(wxKeyEvent &event)
{
	if(event.GetKeyCode() == WXK_ESCAPE)
	{
		if(command != 0)
		{
			command = 0;
			refreshGraphics();
		}
		else
		{
			graphics.deSelectAll();
			sel.clear();
			command = 0;
			wxClientDC	clientDC(this);
		    OnDraw(clientDC);
			EL_DocViewEvent eventCustom(wxEVT_VIEW_SELECTION_CHANGED);
		    eventCustom.setView(this);
            wxPostEvent(this, eventCustom);
		}
	}
	if(event.GetKeyCode() == WXK_DELETE)
	{
		graphics.delSelected();
		sel.clear();
        graphics.Create(coordinate);
		refreshGraphics();
	}
	event.Skip();
}
void EL_DocView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	OnDraw(dc);
	event.StopPropagation();
}
void EL_DocView::OnDraw(wxDC& dcOrig)
{
	//PrepareDC(dcOrig);
    wxRect clientrect;
	clientrect=GetClientRect();
	int cx,cy,cx2,cy2;
	cx = clientrect.GetLeft();
	cy = clientrect.GetBottom();
	cx2 = clientrect.GetRight();
	cy2 = clientrect.GetTop();
	LP_Rect vrect(coordinate.toWorld((double)clientrect.GetLeft(),(double)clientrect.GetBottom()),
		                            coordinate.toWorld((double)clientrect.GetRight(),(double)clientrect.GetTop()));
	coordinate.setViewRect(vrect);
	wxBufferedDC bufdc(&dcOrig);    //创建wx绘图对象
	
	//绘制图形*************************************************************************************
	wxPen pen(wxColor(0,0,0,255));
	int he,wi,le,to,ri;
	he = clientrect.height;
	wi = clientrect.width;
	le = clientrect.GetLeft();
	ri = clientrect.GetRight()+1;
	to = clientrect.GetTop();
	for(int i = 0; i < he; i ++){
		pen.SetColour(wxColor( 0,150+i/20,150+i/10,255));
		bufdc.SetPen(pen);
		bufdc.DrawLine(le,to,ri,to);
		to++;
	}
	pen.SetColour(wxColor(255, 0,255,0));         //画十字线
	wxPoint orig = coordinate.toScreen(0,0,0);
	bufdc.SetPen(pen);
	bufdc.DrawLine(orig.x - 10,orig.y,orig.x + 10,orig.y);
	bufdc.DrawLine(orig.x,orig.y + 10,orig.x,orig.y-10);
	if(coordinate.recreate == true){
		coordinate.setSmooth(1/coordinate.getScale());
		if(coordinate.getSmooth() < 0.01){
			coordinate.setSmooth(0.01);
		}
		graphics.Create(coordinate.getSmooth());	
	}
	graphics.Draw(&bufdc, coordinate);
	if(sel.rect.LeftBottom != sel.rect.RightTop)
	    sel.rect.DrawRuler(&bufdc,coordinate);
	coordinate.recreate = false;
	viewbmp = bufdc.GetSelectedBitmap();
	//绘图结束***********************************************************************************************
}
void EL_DocView::OnSetFocus(wxFocusEvent& event)
{
	event.Skip();
}
LP_Graphics_Container* EL_DocView::getGraphics()
{
	return &graphics;
}
LP_Selected* EL_DocView::getSelected()
{
	return &sel;
}
wxString EL_DocView::getFilePath()
{
	return filepath;
}
void EL_DocView::getTrack()
{
	camgroups.clear();
	graphics.getTrack(camgroups);
}
void testWriting() {
    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out("myfile.dxf", exportVersion);
    if (dw==NULL) {
        printf("Cannot open file 'myfile.dxf' \
               for writing.");
        // abort function e.g. with return
    }
    dxf->writeHeader(*dw);
    // int variable:
    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, 4);
    // real (double, float) variable:
    dw->dxfString(9, "$DIMEXE");
    dw->dxfReal(40, 1.25);
    // string variable:
    dw->dxfString(9, "$TEXTSTYLE");
    dw->dxfString(7, "Standard");
    // vector variable:
    dw->dxfString(9, "$LIMMIN");
    dw->dxfReal(10, 0.0);
    dw->dxfReal(20, 0.0);
    dw->sectionEnd();

    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLineTypes(25);
    dxf->writeLineType(*dw, DL_LineTypeData("BYBLOCK", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BYLAYER", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("CONTINUOUS", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("ACAD_ISO02W100", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("ACAD_ISO03W100", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("ACAD_ISO04W100", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("ACAD_ISO05W100", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BORDER", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BORDER2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BORDERX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("CENTER", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("CENTER2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("CENTERX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHDOT", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHDOT2", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("DASHDOTX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHED", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHED2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHEDX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DIVIDE", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DIVIDE2", 0));
    dxf->writeLineType(*dw,
        DL_LineTypeData("DIVIDEX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DOT", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DOT2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DOTX2", 0));
    dw->tableEnd();

    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);
    dxf->writeLayer(*dw,
        DL_LayerData("0", 0),
        DL_Attributes(
        std::string(""),      // leave empty
        DL_Codes::black,        // default color
        100,                  // default width
        "CONTINUOUS"));       // default line style

    dxf->writeLayer(*dw,
        DL_LayerData("mainlayer", 0),
        DL_Attributes(
        std::string(""),
        DL_Codes::red,
        100,
        "CONTINUOUS"));

    dxf->writeLayer(*dw,
        DL_LayerData("anotherlayer", 0),
        DL_Attributes(
        std::string(""),
        DL_Codes::black,
        100,
        "CONTINUOUS"));
    dw->tableEnd();

    dw->tableStyle(1);
    dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "standard", ""));
    dw->tableEnd();

    dxf->writeView(*dw);
    dxf->writeUcs(*dw);

    dw->tableAppid(1);
    dw->tableAppidEntry(0x12);
    dw->dxfString(2, "ACAD");
    dw->dxfInt(70, 0);
    dw->tableEnd();

    dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);
    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();
    dw->sectionEnd();

    dw->sectionBlocks();
    dxf->writeBlock(*dw,
        DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");

    dxf->writeBlock(*dw,
        DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");

    dxf->writeBlock(*dw,
        DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw,
        DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");

    dxf->writeBlock(*dw,
        DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");

    dw->sectionEnd();
    dw->sectionEntities();

    // write all your entities..
    dxf->writePoint(
        *dw,
        DL_PointData(10.0,
        45.0,
        0.0),
        DL_Attributes("mainlayer", 256, -1, "BYLAYER"));

    dxf->writeLine(
        *dw,
        DL_LineData(25.0,   // start point
        30.0,
        0.0,
        100.0,   // end point
        120.0,
        0.0),
        DL_Attributes("mainlayer", 256, -1, "BYLAYER"));

    dw->sectionEnd();
    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);
    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;
}
void EL_DocView::saveFile()
{
    DL_Codes::version exportVersion;
    exportVersion = DL_Codes::AC1009;
#if 0
    testWriting();
    return;
#endif
    DL_Dxf* pDxf = new DL_Dxf;
    if(pDxf){
        DL_WriterA* dw = pDxf->out("myfile.dxf", exportVersion);
        if (dw==NULL) {
            printf("Cannot open file 'myfile.dxf' for writing.");
            delete pDxf;
            // abort function e.g. with return
            return;
        }
        pDxf->writeHeader(*dw);
        dw->sectionEnd();

        LP_Graphics_Container* pGraphics = getGraphics();
        dw->sectionTables();
        //pDxf->writeVPort(*dw);

        int nCount = pGraphics->Tables.LTypes.size();
        dw->tableLineTypes(nCount);
        for(int i = 0; i < nCount; i++){
            pDxf->writeLineType(*dw, pGraphics->Tables.LTypes.at(i));
        }
        dw->tableEnd();

        nCount = pGraphics->layers.size();
        dw->tableLayers(nCount);
        for(int i = 0; i < nCount; i++){
            LP_Layer layer = pGraphics->layers.at(i);
            pDxf->writeLayer(*dw, DL_LayerData(layer.name,layer.flags), layer.getAttributes());
        }
        dw->tableEnd();

        nCount = pGraphics->Tables.Styles.size();
        dw->tableStyle(nCount);
        for(int i = 0; i < nCount; i++){
            pDxf->writeStyle(*dw, pGraphics->Tables.Styles.at(i));
        }
        dw->tableEnd();

        pDxf->writeView(*dw);
        pDxf->writeUcs(*dw);

        // Appid:
        dw->tableAppid(1);
        pDxf->writeAppid(*dw, "ACAD");
        pDxf->writeAppid(*dw, "QCAD");
        dw->tableEnd();

        // DIMSTYLE:
        pDxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);
        pDxf->writeBlockRecord(*dw);
        //pDxf->writeBlockRecord(*dw, "myblock1");
        //pDxf->writeBlockRecord(*dw, "myblock2");
        dw->tableEnd();
        dw->sectionEnd();

        dw->sectionBlocks();
        nCount = pGraphics->blocks.size();
        for(int i = 0; i < nCount; i++){
            LP_Block block = pGraphics->blocks.at(i);
            LP_Point3 basePoint = block.getBasePoint();
            pDxf->writeBlock(*dw, DL_BlockData(block.getName(), block.getFlags(), basePoint.x, basePoint.y, basePoint.z));
            pDxf->writeEndBlock(*dw, block.getName());
        }
        dw->sectionEnd();

        dw->sectionEntities();
        nCount = pGraphics->layers.size();
        for(int i = 0; i < nCount; i++){
            LP_Layer layer = pGraphics->layers.at(i);
            int nNum = layer.points.size();
            for(int j = 0; j < nNum; j++){
                LP_Point3 point = layer.points.at(j);
                pDxf->writePoint(*dw, DL_PointData(point.x, point.y, point.z), point.getAttributes());
            }

            nNum = layer.inserts.size();
            for(int j = 0; j < nNum; j++){
                LP_Insert insert = layer.inserts.at(j);
                pDxf->writeInsert(*dw, DL_InsertData(insert.name, insert.basePoint.x, insert.basePoint.y, insert.basePoint.z, 
                    insert.sx, insert.sy, insert.sy, insert.angle, insert.cols, insert.rows, insert.colSp, insert.rowSp), insert.getAttributes());
            }

            nNum = layer.ellipses.size();
            for(int j = 0; j < nNum; j++){
                LP_Ellipse ellipse = layer.ellipses.at(j);
                pDxf->writeEllipse(*dw, DL_EllipseData(ellipse.center.x, ellipse.center.y, ellipse.center.z, 
                    ellipse.majorAxis.x, ellipse.majorAxis.y, ellipse.majorAxis.z, ellipse.ratio, ellipse.angle1, ellipse.angle2), ellipse.getAttributes());
            }

            nNum = layer.lines.size();
            for(int j = 0; j < nNum; j++){
                LP_Line line = layer.lines.at(j);
                pDxf->writeLine(*dw, DL_LineData(line.point1.x, line.point1.y, line.point1.z, line.point2.x, line.point2.y, line.point2.z), line.getAttributes());
            }

            nNum = layer.polylines.size();
            for(int j = 0; j < nNum; j++){
                LP_Polyline polyline = layer.polylines.at(j);
                pDxf->writePolyline(*dw, DL_PolylineData(polyline.number, polyline.m, polyline.n, polyline.flags), polyline.getAttributes());
                int nVertexNum = polyline.vertexs.size();
                for(int k = 0; k < nVertexNum; k++){
                    LP_Vertex vertex = polyline.vertexs.at(k);
                    pDxf->writeVertex(*dw, DL_VertexData(vertex.point.x, vertex.point.y, vertex.point.z, vertex.bulge));
                }
                pDxf->writePolylineEnd(*dw);
            }

            nNum = layer.splines.size();
            for(int j = 0; j < nNum; j++){
                LP_Spline spline = layer.splines.at(j);
                pDxf->writeSpline(*dw, DL_SplineData(spline.degree, spline.nKnots, spline.nControl, 0, spline.flags), spline.getAttributes());
            }

            nNum = layer.mtexts.size();
            for(int j = 0; j < nNum; j++){
                LP_MText mtext = layer.mtexts.at(j);
                pDxf->writeMText(*dw, DL_MTextData(mtext.inspoint.x, mtext.inspoint.y, mtext.inspoint.z, 0, 0, 0, mtext.height, mtext.width, mtext.attachmentPoint, 
                    mtext.drawingDirection, mtext.lineSpacingStyle, mtext.lineSpacingFactor, mtext.text, mtext.style, mtext.angle), mtext.getAttributes());
            }
        }
        dw->sectionEnd();

        dw->dxfEOF();
        dw->close();
        delete dw;
        delete pDxf;
    }
}
DEFINE_EVENT_TYPE(wxEVT_VIEW_SELECTION_CHANGED)
IMPLEMENT_DYNAMIC_CLASS(EL_DocViewEvent, wxCommandEvent)
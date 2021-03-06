/////////////////////////////////////////////////////////////////////////////
// Name:        combo.cpp
// Purpose:     wxComboCtrl sample
// Author:      Jaakko Salli
// Modified by:
// Created:     Apr-30-2006
// RCS-ID:      $Id: combo.cpp 42470 2006-10-26 20:35:57Z RR $
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifndef EL_COMBO
#define EL_COMBO
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !wxUSE_COMBOCTRL
    #error "Please set wxUSE_COMBOCTRL to 1 and rebuild the library."
#endif

#include "wx/image.h"

#include "wx/combo.h"
#include "wx/odcombo.h"
#include "../dxflib/src/dl_attributes.h" #include "../dxflib/src/dl_entities.h"
// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// wxOwnerDrawnComboBox with custom paint list items
// ----------------------------------------------------------------------------

class EL_LayerSelectCombo : public wxOwnerDrawnComboBox
{
public:
    virtual void OnDrawItem( wxDC& dc,
                             const wxRect& rect,
                             int item,
                             int flags ) const
    {
        if ( item == wxNOT_FOUND )
            return;

        wxRect r(rect);
        //r.Deflate(3);
        //r.height -= 2;

        int penStyle = wxSOLID;
        if ( item == 1 )
            penStyle = wxTRANSPARENT;
        else if ( item == 2 )
            penStyle = wxDOT;
        else if ( item == 3 )
            penStyle = wxLONG_DASH;
        else if ( item == 4 )
            penStyle = wxSHORT_DASH;
        else if ( item == 5 )
            penStyle = wxDOT_DASH;
        else if ( item == 6 )
            penStyle = wxBDIAGONAL_HATCH;
        else if ( item == 7 )
            penStyle = wxCROSSDIAG_HATCH;
        else if ( item == 8 )
            penStyle = wxFDIAGONAL_HATCH;
        else if ( item == 9 )
            penStyle = wxCROSS_HATCH;
        else if ( item == 10 )
            penStyle = wxHORIZONTAL_HATCH;
        else if ( item == 11 )
            penStyle = wxVERTICAL_HATCH;

        wxPen pen( dc.GetTextForeground(), 3, penStyle );

        // Get text colour as pen colour
        dc.SetPen( pen );
        dc.DrawText(GetString( item ),    //只绘制文字并局中显示
                        r.x + 3,
                        (r.y + 0) + ( (r.height) - dc.GetCharHeight() )/2
                       );
    }

    virtual void OnDrawBackground( wxDC& dc, const wxRect& rect,
                                   int item, int flags ) const
    {

        // If item is selected or even, or we are painting the
        // combo control itself, use the default rendering.
        if ( (flags & (wxODCB_PAINTING_CONTROL|wxODCB_PAINTING_SELECTED)) ||
             (item & 1) == 0 )
        {
            wxOwnerDrawnComboBox::OnDrawBackground(dc,rect,item,flags);
            return;
        }

        // Otherwise, draw every other background with different colour.
        wxColour bgCol(240,240,250);
        dc.SetBrush(wxBrush(bgCol));
        dc.SetPen(wxPen(bgCol));
        dc.DrawRectangle(rect);
    }

    virtual wxCoord OnMeasureItem( size_t item ) const
    {
        // Simply demonstrate the ability to have variable-height items
        if ( item & 1 )
            return 20;
        else
            return 20;
    }

    virtual wxCoord OnMeasureItemWidth( size_t WXUNUSED(item) ) const
    {
        return -1; // default - will be measured from text width
    }

};
class EL_ColorSelectCombo : public wxOwnerDrawnComboBox
{
public:
    virtual void OnDrawItem( wxDC& dc,
                             const wxRect& rect,
                             int item,
                             int flags ) const
    {
        if ( item == wxNOT_FOUND )
            return;
		wxRect rb(rect);
		rb.width = 30;
        wxRect r(rect);
        r.Deflate(2);
        r.height -= 2;

//        int penStyle = wxSOLID;
  /*      if ( item == 1 )
            penStyle = wxTRANSPARENT;
        else if ( item == 2 )
            penStyle = wxDOT;
        else if ( item == 3 )
            penStyle = wxLONG_DASH;
        else if ( item == 4 )
            penStyle = wxSHORT_DASH;
        else if ( item == 5 )
            penStyle = wxDOT_DASH;
        else if ( item == 6 )
            penStyle = wxBDIAGONAL_HATCH;
        else if ( item == 7 )
            penStyle = wxCROSSDIAG_HATCH;
        else if ( item == 8 )
            penStyle = wxFDIAGONAL_HATCH;
        else if ( item == 9 )
            penStyle = wxCROSS_HATCH;
        else if ( item == 10 )
            penStyle = wxHORIZONTAL_HATCH;
        else if ( item == 11 )
            penStyle = wxVERTICAL_HATCH;
*/


		wxFont ft = dc.GetFont();
		ft.SetWeight(wxBOLD);    //weight的属性可以是wxNORMAL, wxLIGHT或wxBOLD.
		dc.SetFont(ft);
		//dc.SetBackgroundMode(wxTRANSPARENT);
		wxColour bgCol(0,0,0);
		int textbgcolor = 8;
        dc.SetBrush(wxBrush(wxColour(dxfColors[textbgcolor][0] * 255,dxfColors[textbgcolor][1] * 255,dxfColors[textbgcolor][2] * 255)));
		wxPen bgpen(bgCol);
		dc.SetPen(bgpen);
		wxCoord wd,ht;
		dc.GetTextExtent(GetString(256),&wd,&ht);
        dc.DrawRectangle(r.x+1,r.y,wd+2,r.height);
		if (flags & (wxODCB_PAINTING_CONTROL|wxODCB_PAINTING_SELECTED))
		{
			dc.SetTextBackground(wxColour(0,0,0,50));
			dc.SetTextForeground(wxColour(255,255,255,50));
		}
		else
		{
			dc.SetTextBackground(wxColour(255,255,255,50));
			dc.SetTextForeground(wxColour(0,0,0,50));
		}
		wxBrush brush(dc.GetTextForeground());
		dc.SetBrush(brush);
        dc.DrawText(GetString( item ),    //只绘制文字并局中显示
                        r.x+2,
                        (r.y + 0) + ( (r.height) - dc.GetCharHeight())/2
                       );
		/*
        if ( !(flags & wxODCB_PAINTING_CONTROL) )  当用做自绘时，绘制文字和线条样式
        {
            dc.DrawText(GetString( item ),
                        r.x + 3,
                        (r.y + 0) + ( (r.height/2) - dc.GetCharHeight() )/2
                       );

            dc.DrawLine( r.x+5, r.y+((r.height/4)*3), r.x+r.width - 5, r.y+((r.height/4)*3) );
        }
        else
        {
            dc.DrawLine( r.x+5, r.y+r.height/2, r.x+r.width - 5, r.y+r.height/2 );
        }*/
    }

    virtual void OnDrawBackground( wxDC& dc, const wxRect& rect,
                                   int item, int flags ) const
    {

        // If item is selected or even, or we are painting the
        // combo control itself, use the default rendering.
    //    if ( (flags & (wxODCB_PAINTING_CONTROL|wxODCB_PAINTING_SELECTED)) ||
    //         (item & 1) == 0 )
    //    {
     //       wxOwnerDrawnComboBox::OnDrawBackground(dc,rect,item,flags);
     //       return;
     //   }

        // Otherwise, draw every other background with different colour.
		wxRect r(rect);
		//r.x+=30;
        wxColour bgCol(0,0,0);
		if(item <= 256)
		{
            dc.SetBrush(wxBrush(wxColour(dxfColors[item][0] * 255,dxfColors[item][1] * 255,dxfColors[item][2] * 255)));
		}
		wxPen bgpen(bgCol);
		dc.SetPen(bgpen);
        dc.DrawRectangle(r);
		if (flags & (wxODCB_PAINTING_CONTROL|wxODCB_PAINTING_SELECTED))
		{
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			bgpen.SetColour(0,0,0);
			bgpen.SetWidth(5);
			dc.SetPen(bgpen);
			dc.DrawRectangle(r);
			bgpen.SetColour(248,248,250);
			bgpen.SetStyle(wxDOT);
			bgpen.SetWidth(1);
			dc.SetPen(bgpen);
			//dc.DrawRectangle(r);
			dc.DrawRectangle(r.x+1,r.y+1,r.width-2,r.height-2);
		}
    }

    virtual wxCoord OnMeasureItem( size_t item ) const
    {
        // Simply demonstrate the ability to have variable-height items
        if ( item & 1 )
            return 22;
        else
            return 22;
    }

    virtual wxCoord OnMeasureItemWidth( size_t WXUNUSED(item) ) const
    {
        return -1; // default - will be measured from text width
    }

};
#endif
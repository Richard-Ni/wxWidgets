/////////////////////////////////////////////////////////////////////////////
// Name:        wx/motif/tglbtn.h
// Purpose:     Declaration of the wxToggleButton class, which implements a
//              toggle button under wxMotif.
// Author:      Mattia Barbon
// Modified by:
// Created:     10.02.03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Mattia Barbon
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TOGGLEBUTTON_H_
#define _WX_TOGGLEBUTTON_H_

#include "wx/checkbox.h"

class WXDLLEXPORT wxToggleButton : public wxCheckBox
{
public:
    wxToggleButton() { Init(); }
    wxToggleButton( wxWindow* parent, wxWindowID id, const wxString& label,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxValidator& val = wxDefaultValidator,
                    const wxString& name = wxCheckBoxNameStr )
    {
        Init();

        Create( parent, id, label, pos, size, style, val, name );
    }

    bool Create( wxWindow* parent, wxWindowID id, const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxValidator& val = wxDefaultValidator,
                 const wxString &name = wxCheckBoxNameStr );
private:
    DECLARE_DYNAMIC_CLASS(wxToggleButton);

    // common part of all constructors
    void Init()
    {
        m_evtType = wxEVT_COMMAND_TOGGLEBUTTON_CLICKED;
    }
};

#endif // _WX_TOGGLEBUTTON_H_

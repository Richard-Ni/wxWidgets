///////////////////////////////////////////////////////////////////////////////
// Name:        msw/statbr95.cpp
// Purpose:     native implementation of wxStatusBar
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "statbr95.h"
#endif

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/setup.h"
  #include "wx/frame.h"
  #include "wx/settings.h"
  #include "wx/dcclient.h"
#endif

#if defined(__WIN95__) && wxUSE_NATIVE_STATUSBAR

#include "wx/intl.h"
#include "wx/log.h"
#include "wx/statusbr.h"

#include "wx/msw/private.h"
#include <windowsx.h>

#if !(defined(__GNUWIN32_OLD__) || defined(__TWIN32__))
    #include <commctrl.h>
#endif

// ----------------------------------------------------------------------------
// wxWindows macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxStatusBar95, wxWindow);
IMPLEMENT_DYNAMIC_CLASS(wxStatusBar, wxStatusBar95)

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// windowsx.h and commctrl.h don't define those, so we do it here
#define StatusBar_SetParts(h, n, w) SendMessage(h, SB_SETPARTS, (WPARAM)n, (LPARAM)w)
#define StatusBar_SetText(h, n, t)  SendMessage(h, SB_SETTEXT, (WPARAM)n, (LPARAM)(LPCTSTR)t)
#define StatusBar_GetTextLen(h, n)  LOWORD(SendMessage(h, SB_GETTEXTLENGTH, (WPARAM)n, 0))
#define StatusBar_GetText(h, n, s)  LOWORD(SendMessage(h, SB_GETTEXT, (WPARAM)n, (LPARAM)(LPTSTR)s))

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------

// static WNDPROC gs_wndprocStatBar = NULL;
static WXFARPROC gs_wndprocStatBar = (WXFARPROC) NULL;

LRESULT APIENTRY wxStatusBarProc(HWND hwnd,
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam)
{
    switch (message) {
        case WM_COMMAND:
        case WM_SIZE:
        case WM_MOVE:
        case WM_MOUSEMOVE:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
            wxStatusBar95 *sb = (wxStatusBar95 *)GetWindowLong(hwnd, GWL_USERDATA);
            sb->MSWWindowProc(message, wParam, lParam);
            break;
    }

    return ::CallWindowProc(CASTWNDPROC gs_wndprocStatBar, hwnd, message, wParam, lParam);
}

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStatusBar95 class
// ----------------------------------------------------------------------------

wxStatusBar95::wxStatusBar95()
{
  SetParent(NULL);
  m_hWnd = 0;
  m_windowId = 0;
}

bool wxStatusBar95::Create(wxWindow *parent,
                           wxWindowID id,
                           long style,
                           const wxString& name)
{
    wxCHECK_MSG( parent, FALSE, wxT("status bar must have a parent") );

    SetName(name);
    SetWindowStyleFlag(style);
    SetParent(parent);

    parent->AddChild(this);

    m_windowId = id == -1 ? NewControlId() : id;

    DWORD wstyle = WS_CHILD | WS_VISIBLE;

    // setting SBARS_SIZEGRIP is perfectly useless: it's always on by default
    // (at least in the version of comctl32.dll I'm using), and the only way to
    // turn it off is to use CCS_TOP style - as we position the status bar
    // manually anyhow (see DoMoveWindow), use CCS_TOP style if wxST_SIZEGRIP
    // is not given
    if ( !(style & wxST_SIZEGRIP) )
    {
        wstyle |= CCS_TOP;
    }
    else
    {
        // may be some versions of comctl32.dll do need it - anyhow, it won't
        // do any harm
        wstyle |= SBARS_SIZEGRIP;
    }

    m_hWnd = (WXHWND)CreateStatusWindow(wstyle,
                                        wxEmptyString,
                                        GetHwndOf(parent),
                                        m_windowId);
    if ( m_hWnd == 0 )
    {
        wxLogSysError(_("Failed to create a status bar."));

        return FALSE;
    }

    SetFieldsCount(1);

    // we can't subclass this window as usual because the status bar window
    // proc processes WM_SIZE and WM_PAINT specially
    //  SubclassWin(m_hWnd);

    // but we want to process the messages from it still, so do custom
    // subclassing here
    gs_wndprocStatBar = (WXFARPROC)GetWindowLong(GetHwnd(), GWL_WNDPROC);
    SetWindowLong(GetHwnd(), GWL_WNDPROC, (LONG)wxStatusBarProc);
    SetWindowLong(GetHwnd(), GWL_USERDATA, (LONG)this);

    return TRUE;
}

wxStatusBar95::~wxStatusBar95()
{
    delete [] m_statusWidths;
}

void wxStatusBar95::CopyFieldsWidth(const int widths[])
{
  if (widths && !m_statusWidths)
    m_statusWidths = new int[m_nFields];

  if ( widths != NULL ) {
    for ( int i = 0; i < m_nFields; i++ )
      m_statusWidths[i] = widths[i];
  }
  else {
    delete [] m_statusWidths;
    m_statusWidths = NULL;
  }
}

void wxStatusBar95::SetFieldsCount(int nFields, const int *widths)
{
  // this is Windows limitation
  wxASSERT_MSG( (nFields > 0) && (nFields < 255), _T("too many fields") );

  m_nFields = nFields;

  CopyFieldsWidth(widths);
  SetFieldsWidth();
}

void wxStatusBar95::SetStatusWidths(int n, const int widths[])
{
  wxASSERT_MSG( n == m_nFields, _T("field number mismatch") );

  CopyFieldsWidth(widths);
  SetFieldsWidth();
}

void wxStatusBar95::SetFieldsWidth()
{
    if ( !m_nFields )
        return;

    int aBorders[3];
    SendMessage(GetHwnd(), SB_GETBORDERS, 0, (LPARAM)aBorders);

    int extraWidth = aBorders[2]; // space between fields

    int *pWidths = new int[m_nFields];

    int nWindowWidth, y;
    GetClientSize(&nWindowWidth, &y);

    if ( m_statusWidths == NULL ) {
        // default: all fields have the same width
        int nWidth = nWindowWidth / m_nFields;
        for ( int i = 0; i < m_nFields; i++ )
            pWidths[i] = (i + 1) * nWidth;
    }
    else {
        // -1 doesn't mean the same thing for wxWindows and Win32, recalc
        int nTotalWidth = 0,
        nVarCount = 0,
        i;
        for ( i = 0; i < m_nFields; i++ ) {
            if ( m_statusWidths[i] == -1 )
                nVarCount++;
            else
                nTotalWidth += m_statusWidths[i] + extraWidth;
        }

        if ( nVarCount == 0 ) {
            wxFAIL_MSG( _T("at least one field must be of variable width") );

            nVarCount++;
        }

        int nVarWidth = (nWindowWidth - nTotalWidth) / nVarCount;

        // do fill the array
        int nCurPos = 0;
        for ( i = 0; i < m_nFields; i++ ) {
            if ( m_statusWidths[i] == -1 )
                nCurPos += nVarWidth;
            else
                nCurPos += m_statusWidths[i] + extraWidth;
            pWidths[i] = nCurPos;
        }
    }

    if ( !StatusBar_SetParts(GetHwnd(), m_nFields, pWidths) ) {
        wxLogLastError(wxT("StatusBar_SetParts"));
    }

    delete [] pWidths;
}

void wxStatusBar95::SetStatusText(const wxString& strText, int nField)
{
    wxCHECK_RET( (nField >= 0) && (nField < m_nFields),
                 _T("invalid statusbar field index") );

  if ( !StatusBar_SetText(GetHwnd(), nField, strText) ) {
    wxLogLastError(wxT("StatusBar_SetText"));
  }
}

wxString wxStatusBar95::GetStatusText(int nField) const
{
    wxCHECK_MSG( (nField >= 0) && (nField < m_nFields), wxEmptyString,
                 _T("invalid statusbar field index") );

  wxString str;
  int len = StatusBar_GetTextLen(GetHwnd(), nField);
  if (len > 0)
  {
    StatusBar_GetText(GetHwnd(), nField, str.GetWriteBuf(len));
    str.UngetWriteBuf();
  }
  return str;
}

int wxStatusBar95::GetBorderX() const
{
    int aBorders[3];
    SendMessage(GetHwnd(), SB_GETBORDERS, 0, (LPARAM)aBorders);

    return aBorders[0];
}

int wxStatusBar95::GetBorderY() const
{
    int aBorders[3];
    SendMessage(GetHwnd(), SB_GETBORDERS, 0, (LPARAM)aBorders);

    return aBorders[1];
}

void wxStatusBar95::SetMinHeight(int height)
{
    SendMessage(GetHwnd(), SB_SETMINHEIGHT, height + 2*GetBorderY(), 0);

    // have to send a (dummy) WM_SIZE to redraw it now
    SendMessage(GetHwnd(), WM_SIZE, 0, 0);
}

bool wxStatusBar95::GetFieldRect(int i, wxRect& rect) const
{
    wxCHECK_MSG( (i >= 0) && (i < m_nFields), FALSE,
                 _T("invalid statusbar field index") );

    RECT r;
    if ( !::SendMessage(GetHwnd(), SB_GETRECT, i, (LPARAM)&r) )
    {
        wxLogLastError("SendMessage(SB_GETRECT)");
    }

    wxCopyRECTToRect(r, rect);

    return TRUE;
}

void wxStatusBar95::DoMoveWindow(int x, int y, int width, int height)
{
    // the status bar wnd proc must be forwarded the WM_SIZE message whenever
    // the stat bar position/size is changed because it normally positions the
    // control itself along bottom or top side of the parent window - failing
    // to do so will result in nasty visual effects
    FORWARD_WM_SIZE(GetHwnd(), SIZE_RESTORED, x, y, SendMessage);

    // but now, when the standard status bar wnd proc did all it wanted to do,
    // move the status bar to its correct location - usually this call may be
    // omitted because for normal status bars (positioned along the bottom
    // edge) the position is already set correctly, but if the user wants to
    // position them in some exotic location, this is really needed
    wxWindow::DoMoveWindow(x, y, width, height);

    // adjust fields widths to the new size
    SetFieldsWidth();
}

#endif // __WIN95__ && wxUSE_NATIVE_STATUSBAR


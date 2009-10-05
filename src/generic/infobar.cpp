///////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/infobar.cpp
// Purpose:     generic wxInfoBar implementation
// Author:      Vadim Zeitlin
// Created:     2009-07-28
// RCS-ID:      $Id: wxhead.cpp,v 1.10 2009-06-29 10:23:04 zeitlin Exp $
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_INFOBAR

#ifndef WX_PRECOMP
    #include "wx/bmpbuttn.h"
    #include "wx/button.h"
    #include "wx/settings.h"
    #include "wx/statbmp.h"
    #include "wx/stattext.h"
#endif // WX_PRECOMP

#include "wx/infobar.h"

#include "wx/artprov.h"
#include "wx/renderer.h"
#include "wx/scopeguard.h"
#include "wx/sizer.h"

BEGIN_EVENT_TABLE(wxInfoBarGeneric, wxInfoBarBase)
    EVT_BUTTON(wxID_ANY, wxInfoBarGeneric::OnButton)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// local helpers
// ----------------------------------------------------------------------------

namespace
{

#ifdef wxHAS_DRAW_TITLE_BAR_BITMAP

wxBitmap
GetCloseButtonBitmap(wxWindow *win,
                     const wxSize& size,
                     const wxColour& colBg,
                     int flags = 0)
{
    wxBitmap bmp(size);
    wxMemoryDC dc(bmp);
    dc.SetBackground(colBg);
    dc.Clear();
    wxRendererNative::Get().
        DrawTitleBarBitmap(win, dc, size, wxTITLEBAR_BUTTON_CLOSE, flags);
    return bmp;
}

#endif // wxHAS_DRAW_TITLE_BAR_BITMAP

} // anonymous namespace

// ============================================================================
// implementation
// ============================================================================

void wxInfoBarGeneric::Init()
{
    m_icon = NULL;
    m_text = NULL;
    m_button = NULL;

    m_showEffect = wxSHOW_EFFECT_SLIDE_TO_BOTTOM;
    m_hideEffect = wxSHOW_EFFECT_SLIDE_TO_TOP;

    // use default effect duration
    m_effectDuration = 0;
}

bool wxInfoBarGeneric::Create(wxWindow *parent, wxWindowID winid)
{
    // calling Hide() before Create() ensures that we're created initially
    // hidden
    Hide();
    if ( !wxWindow::Create(parent, winid) )
        return false;

    // use special, easy to notice, colours
    const wxColour colBg = wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK);
    SetBackgroundColour(colBg);
    SetOwnForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT));

    // create the controls: icon, text and the button to dismiss the
    // message.

    // the icon is not shown unless it's assigned a valid bitmap
    m_icon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);

    m_text = new wxStaticText(this, wxID_ANY, "");

#ifdef wxHAS_DRAW_TITLE_BAR_BITMAP
    const wxSize sizeBmp = wxArtProvider::GetSizeHint(wxART_FRAME_ICON);
    wxBitmap bmp = GetCloseButtonBitmap(this, sizeBmp, colBg);
#else // !wxHAS_DRAW_TITLE_BAR_BITMAP
    wxBitmap bmp = wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU);
#endif // wxHAS_DRAW_TITLE_BAR_BITMAP
    m_button = new wxBitmapButton
                   (
                    this,
                    wxID_ANY,
                    bmp,
                    wxDefaultPosition,
                    wxDefaultSize,
                    wxBORDER_NONE
                   );

#ifdef wxHAS_DRAW_TITLE_BAR_BITMAP
    m_button->SetBitmapPressed(
        GetCloseButtonBitmap(this, sizeBmp, colBg, wxCONTROL_PRESSED));

    m_button->SetBitmapCurrent(
        GetCloseButtonBitmap(this, sizeBmp, colBg, wxCONTROL_CURRENT));
#endif // wxHAS_DRAW_TITLE_BAR_BITMAP

    m_button->SetBackgroundColour(colBg);
    m_button->SetToolTip(_("Hide this notification message."));

    // center the text inside the sizer with an icon to the left of it and a
    // button at the very right
    //
    // NB: AddButton() relies on the button being the last control in the sizer
    //     and being preceded by a spacer
    wxSizer * const sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_icon, wxSizerFlags().Centre().Border());
    sizer->Add(m_text, wxSizerFlags().Centre());
    sizer->AddStretchSpacer();
    sizer->Add(m_button, wxSizerFlags().Centre().Border());
    SetSizer(sizer);

    return true;
}

bool wxInfoBarGeneric::SetFont(const wxFont& font)
{
    if ( !wxInfoBarBase::SetFont(font) )
        return false;

    // check that we're not called before Create()
    if ( m_text )
        m_text->SetFont(font);

    return true;
}

void wxInfoBarGeneric::UpdateParent()
{
    wxWindow * const parent = wxGetTopLevelParent(GetParent());
    parent->Layout();
}

void wxInfoBarGeneric::ChangeParentBackground()
{
    wxWindow * const parent = GetParent();
    m_origParentBgCol = parent->GetBackgroundColour();

    wxSizer * const sizer = GetContainingSizer();
    if ( !sizer )
        return;

    wxWindow *sibling = NULL;
    for ( wxSizerItemList::compatibility_iterator
            node = sizer->GetChildren().GetFirst();
            node;
            node = node->GetNext() )
    {
        if ( node->GetData()->GetWindow() == this )
        {
            // find the next window following us
            for ( node = node->GetNext();
                  node;
                  node = node->GetNext() )
            {
                wxSizerItem * const item = node->GetData();
                if ( item->IsWindow() )
                {
                    sibling = item->GetWindow();
                    break;
                }
            }

            break;
        }
    }

    if ( sibling )
        parent->SetOwnBackgroundColour(sibling->GetBackgroundColour());
}

void wxInfoBarGeneric::RestoreParentBackground()
{
    GetParent()->SetOwnBackgroundColour(m_origParentBgCol);
}

void wxInfoBarGeneric::DoHide()
{
    ChangeParentBackground();
    wxON_BLOCK_EXIT_THIS0( wxInfoBarGeneric::RestoreParentBackground );

    HideWithEffect(m_hideEffect, m_effectDuration);
    UpdateParent();
}

void wxInfoBarGeneric::DoShow()
{
    // re-layout the parent first so that the window expands into an already
    // unoccupied by the other controls area: for this we need to change our
    // internal visibility flag to force Layout() to take us into account (an
    // alternative solution to this hack would be to temporarily set
    // wxRESERVE_SPACE_EVEN_IF_HIDDEN flag but it's not really batter)

    // just change the internal flag indicating that the window is visible,
    // without really showing it
    wxWindowBase::Show();

    // an extra hack: we want the temporarily uncovered area in which we're
    // going to expand to look like part of this sibling for a better effect so
    // temporarily change the background of our parent to the same colour
    ChangeParentBackground();
    wxON_BLOCK_EXIT_THIS0( wxInfoBarGeneric::RestoreParentBackground );

    // adjust the parent layout to account for us
    UpdateParent();

    // reset the flag back before really showing the window or it wouldn't be
    // shown at all because it would believe itself already visible
    wxWindowBase::Show(false);


    // finally do really show the window.
    ShowWithEffect(m_showEffect, m_effectDuration);
}

void wxInfoBarGeneric::ShowMessage(const wxString& msg, int flags)
{
    // first update the controls
    const int icon = flags & wxICON_MASK;
    if ( !icon || (icon == wxICON_NONE) )
    {
        m_icon->Hide();
    }
    else // do show an icon
    {
        m_icon->SetBitmap(wxArtProvider::GetBitmap(
                            wxArtProvider::GetMessageBoxIconId(flags),
                          wxART_MENU));
        m_icon->Show();
    }

    // notice the use of EscapeMnemonics() to ensure that "&" come through
    // correctly
    m_text->SetLabel(wxControl::EscapeMnemonics(msg));


    // then show this entire window if not done yet
    if ( !IsShown() )
    {
        DoShow();
    }
    else // we're already shown
    {
        // just update the layout to correspond to the new message
        Layout();
    }
}

void wxInfoBarGeneric::Dismiss()
{
    DoHide();
}

void wxInfoBarGeneric::AddButton(wxWindowID btnid, const wxString& label)
{
    wxSizer * const sizer = GetSizer();
    wxCHECK_RET( sizer, "must be created first" );

    // user-added buttons replace the standard close button so remove it if we
    // hadn't done it yet
    if ( sizer->Detach(m_button) )
    {
        m_button->Hide();
    }

    sizer->Add(new wxButton(this, btnid, label),
               wxSizerFlags().Centre().DoubleBorder());
}

void wxInfoBarGeneric::RemoveButton(wxWindowID btnid)
{
    wxSizer * const sizer = GetSizer();
    wxCHECK_RET( sizer, "must be created first" );

    // iterate over the sizer items in reverse order to find the last added
    // button with this id (ids of all buttons should be unique anyhow but if
    // they are repeated removing the last added one probably makes more sense)
    const wxSizerItemList& items = sizer->GetChildren();
    for ( wxSizerItemList::compatibility_iterator node = items.GetLast();
          node != items.GetFirst();
          node = node->GetPrevious() )
    {
        const wxSizerItem * const item = node->GetData();

        // if we reached the spacer separating the buttons from the text
        // preceding them without finding our button, it must mean it's not
        // there at all
        if ( item->IsSpacer() )
        {
            wxFAIL_MSG( wxString::Format("button with id %d not found", btnid) );
            return;
        }

        // check if we found our button
        if ( item->GetWindow()->GetId() == btnid )
        {
            delete item->GetWindow();
            break;
        }
    }

    // check if there are any custom buttons left
    if ( sizer->GetChildren().GetLast()->GetData()->IsSpacer() )
    {
        // if the last item is the spacer, none are left so restore the
        // standard close button
        sizer->Add(m_button, wxSizerFlags().Centre().DoubleBorder());
        m_button->Show();
    }
}

void wxInfoBarGeneric::OnButton(wxCommandEvent& WXUNUSED(event))
{
    DoHide();
}

#endif // wxUSE_INFOBAR

/////////////////////////////////////////////////////////////////////////////
// Name:        notebook.h
// Purpose:     wxNotebook class
// Author:      Robert Roebling
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKNOTEBOOKH__
#define __GTKNOTEBOOKH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

//-----------------------------------------------------------------------------
// internal class
//-----------------------------------------------------------------------------

class wxGtkNotebookPage;

#include "wx/list.h"
WX_DECLARE_LIST(wxGtkNotebookPage, wxGtkNotebookPagesList);

//-----------------------------------------------------------------------------
// wxNotebook
//-----------------------------------------------------------------------------

class wxNotebook : public wxNotebookBase
{
public:
      // default for dynamic class
    wxNotebook();
      // the same arguments as for wxControl
    wxNotebook(wxWindow *parent,
             wxWindowID id,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = 0,
             const wxString& name = wxT("notebook"));
      // Create() function
    bool Create(wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = 0,
              const wxString& name = wxT("notebook"));
      // dtor
    virtual ~wxNotebook();

  // accessors
  // ---------

    // set the currently selected page, return the index of the previously
    // selected one (or -1 on error)
    // NB: this function will _not_ generate wxEVT_NOTEBOOK_PAGE_xxx events
  int SetSelection(size_t nPage);
    // get the currently selected page
  int GetSelection() const;

    // set/get the title of a page
  bool SetPageText(size_t nPage, const wxString& strText);
  wxString GetPageText(size_t nPage) const;

    // sets/returns item's image index in the current image list
  int  GetPageImage(size_t nPage) const;
  bool SetPageImage(size_t nPage, int nImage);

  // control the appearance of the notebook pages
    // set the size (the same for all pages)
  void SetPageSize(const wxSize& size);
    // set the padding between tabs (in pixels)
  void SetPadding(const wxSize& padding);
    // sets the size of the tabs (assumes all tabs are the same size)
  void SetTabSize(const wxSize& sz);

  // operations
  // ----------
    // remove one page from the notebook
  bool DeletePage(size_t nPage);
    // remove all pages
  bool DeleteAllPages();

    // adds a new page to the notebook (it will be deleted ny the notebook,
    // don't delete it yourself). If bSelect, this page becomes active.
    // the same as AddPage(), but adds it at the specified position
    bool InsertPage( size_t position,
                     wxNotebookPage *win,
                     const wxString& strText,
                     bool bSelect = FALSE,
                     int imageId = -1 );

    // handler for tab navigation
    // --------------------------
    void OnNavigationKey(wxNavigationKeyEvent& event);

    // implementation
    // --------------

#if wxUSE_CONSTRAINTS
    void SetConstraintSizes(bool recurse);
    bool DoPhase(int phase);
#endif

    void ApplyWidgetStyle();

    // report if window belongs to notebook
    bool IsOwnGtkWindow( GdkWindow *window );

    // common part of all ctors
    void Init();

    // helper function
    wxGtkNotebookPage* GetNotebookPage(int page) const;

    // the additional page data (the pages themselves are in m_pages array)
    wxGtkNotebookPagesList m_pagesData;

    // for reasons explained in gtk/notebook.cpp we store the current
    // selection internally instead of querying the notebook for it
    int m_selection;

    // flag set to TRUE while we're inside "switch_page" callback
    bool m_inSwitchPage;

protected:
    // remove one page from the notebook but do not destroy it
    virtual wxNotebookPage *DoRemovePage(size_t nPage);

private:
    // the padding set by SetPadding()
    int m_padding;

    DECLARE_DYNAMIC_CLASS(wxNotebook)
    DECLARE_EVENT_TABLE()
};

#endif
    // __GTKNOTEBOOKH__

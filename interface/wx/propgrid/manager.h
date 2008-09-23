/////////////////////////////////////////////////////////////////////////////
// Name:        manager.h
// Purpose:     interface of wxPropertyGridManager
// Author:      wxWidgets team
// RCS-ID:      $Id:
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/** @class wxPropertyGridPage

    Holder of property grid page information. You can subclass this and
    give instance in wxPropertyGridManager::AddPage. It inherits from
    wxEvtHandler and can be used to process events specific to this
    page (id of events will still be same as manager's). If you don't
    want to use it to process all events of the page, you need to
    return false in the derived wxPropertyGridPage::IsHandlingAllEvents.

    Please note that wxPropertyGridPage lacks many non-const property
    manipulation functions found in wxPropertyGridManager. Please use
    parent manager (m_manager member variable) when needed.

    Please note that most member functions are inherited and as such not
    documented on this page. This means you will probably also want to read
    wxPropertyGridInterface class reference.

    @section propgridpage_event_handling Event Handling

    wxPropertyGridPage receives events emitted by its wxPropertyGridManager, but
    only those events that are specific to that page. If wxPropertyGridPage::
    IsHandlingAllEvents returns false, then unhandled events are sent to the
    manager's parent, as usual.

    See @ref propgrid_event_handling "wxPropertyGrid Event Handling"
    for more information.

    @library{wxpropgrid}
    @category{propgrid}
*/
class WXDLLIMPEXP_PROPGRID wxPropertyGridPage : public wxEvtHandler,
                                                public wxPropertyGridInterface
{
    friend class wxPropertyGridManager;
public:

    wxPropertyGridPage();
    virtual ~wxPropertyGridPage();

    /**
        Deletes all properties on page.
    */
    virtual void Clear();

    /**
        Reduces column sizes to minimum possible that contents are still visibly
        (naturally some margin space will be applied as well).

        @return Returns minimum size for the page to still display everything.

        @remarks This function only works properly if size of containing grid was
                already fairly large.

                Note that you can also get calculated column widths by calling
                GetColumnWidth() immediately after this function returns.
    */
    wxSize FitColumns();

    /**
        Returns page index in manager;
    */
    inline int GetIndex() const;

    /**
        Returns "root property". It does not have name, etc. and it is not
        visible. It is only useful for accessing its children.
    */
    wxPGProperty* GetRoot() const;

    /**
        Returns x-coordinate position of splitter on a page.
    */
    int GetSplitterPosition( int col = 0 ) const;

    /**
        Returns id of the tool bar item that represents this page on
        wxPropertyGridManager's wxToolBar.
    */
    int GetToolId() const
    {
        return m_id;
    }

    /**
        Do any member initialization in this method.

        @remarks - Called every time the page is added into a manager.
                - You can add properties to the page here.
    */
    virtual void Init() {}

    /**
        Return false here to indicate unhandled events should be
        propagated to manager's parent, as normal.
    */
    virtual bool IsHandlingAllEvents() const { return true; }

    /**
        Called every time page is about to be shown.
        Useful, for instance, creating properties just-in-time.
    */
    virtual void OnShow();

    /**
        Refreshes given property on page.
    */
    virtual void RefreshProperty( wxPGProperty* p );

    /** Sets splitter position on page.
        @remarks
        Splitter position cannot exceed grid size, and therefore setting it
        during form creation may fail as initial grid size is often smaller than
        desired splitter position, especially when sizers are being used.
    */
    void SetSplitterPosition( int splitterPos, int col = 0 );
};

// -----------------------------------------------------------------------

/** @class wxPropertyGridManager

    wxPropertyGridManager is an efficient multi-page version of wxPropertyGrid,
    which can optionally have toolbar for mode and page selection, and a help text
    box.

    wxPropertyGridManager inherits from wxPropertyGridInterface, and as such
    it has most property manipulation functions. However, only some of them affect
    properties on all pages (eg. GetPropertyByName() and ExpandAll()), while some
    (eg. Append()) only apply to the currently selected page.

    To operate explicitly on properties on specific page, use
    wxPropertyGridManager::GetPage() to obtain pointer to page's
    wxPropertyGridPage object.

    Visual methods, such as SetCellBackgroundColour() are only available in
    wxPropertyGrid. Use wxPropertyGridManager::GetGrid() to obtain pointer to it.

    Non-virtual iterators will not work in wxPropertyGridManager. Instead, you must
    acquire the internal grid (GetGrid()) or wxPropertyGridPage object (GetPage()).

    wxPropertyGridManager constructor has exact same format as wxPropertyGrid
    constructor, and basicly accepts same extra window style flags (albeit also
    has some extra ones).

    Here's some example code for creating and populating a wxPropertyGridManager:

    @code

        wxPropertyGridManager* pgMan = new wxPropertyGridManager(this, PGID,
            wxDefaultPosition, wxDefaultSize,
            // These and other similar styles are automatically
            // passed to the embedded wxPropertyGrid.
            wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|
            // Include toolbar.
            wxPG_TOOLBAR |
            // Include description box.
            wxPG_DESCRIPTION |
            // Include compactor.
            wxPG_COMPACTOR |
            // Plus defaults.
            wxPGMAN_DEFAULT_STYLE
           );

        wxPropertyGridPage* page;

        pgMan->AddPage(wxT("First Page"));
        page = pgMan->GetLastPage();

        page->Append( new wxPropertyCategory(wxT("Category A1")) );

        page->Append( new wxIntProperty(wxT("Number"),wxPG_LABEL,1) );

        page->Append( new wxColourProperty(wxT("Colour"),wxPG_LABEL,*wxWHITE) );

        pgMan->AddPage(wxT("Second Page"));
        page = pgMan->GetLastPage();

        page->Append( wxT("Text"),wxPG_LABEL,wxT("(no text)") );

        page->Append( new wxFontProperty(wxT("Font"),wxPG_LABEL) );

    @endcode


    @section propgridmanager_window_styles_ Window Styles

    See @ref propgrid_window_styles.

    @section propgridmanager_event_handling Event Handling

    See @ref propgrid_event_handling "wxPropertyGrid Event Handling"
    for more information.

    @library{wxpropgrid}
    @category{propgrid}
*/
class wxPropertyGridManager : public wxPanel, public wxPropertyGridInterface
{
public:
    /**
        Creates new property page. Note that the first page is not created
        automatically.

        @param label
            A label for the page. This may be shown as a toolbar tooltip etc.

        @param bmp
            Bitmap image for toolbar. If wxNullBitmap is used, then a built-in
            default image is used.

        @param pageObj
            wxPropertyGridPage instance. Manager will take ownership of this
            object. NULL indicates that a default page instance should be created.

        @return Returns index to the page created.

        @remarks If toolbar is used, it is highly recommended that the pages are
                added when the toolbar is not turned off using window style flag
                switching. Otherwise toolbar buttons might not be added properly.
    */
    int AddPage( const wxString& label = wxEmptyString,
                 const wxBitmap& bmp = wxPG_NULL_BITMAP,
                 wxPropertyGridPage* pageObj = (wxPropertyGridPage*) NULL )
    {
        return InsertPage(-1,label,bmp,pageObj);
    }

    void ClearModifiedStatus( wxPGPropArg id );

    void ClearModifiedStatus()
    {
        m_pPropGrid->ClearModifiedStatus();
    }

    /**
        Deletes all properties and all pages.
    */
    virtual void Clear();

    /**
        Deletes all properties on given page.
    */
    void ClearPage( int page );

    /**
        Forces updating the value of property from the editor control.

        @return Returns @true if value was actually updated.
    */
    bool CommitChangesFromEditor( wxUint32 flags = 0 )
    {
        return m_pPropGrid->CommitChangesFromEditor(flags);
    }

    /**
        Two step creation. Whenever the control is created without any parameters,
        use Create to actually create it. Don't access the control's public methods
        before this is called.

        @see @ref propgrid_window_styles
    */
    bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxPGMAN_DEFAULT_STYLE,
                 const wxChar* name = wxPropertyGridManagerNameStr );

    /**
        Enables or disables (shows/hides) categories according to parameter enable.

        @remarks
            Calling his may not properly update toolbar buttons.
    */
    bool EnableCategories( bool enable )
    {
        long fl = m_windowStyle | wxPG_HIDE_CATEGORIES;
        if ( enable ) fl = m_windowStyle & ~(wxPG_HIDE_CATEGORIES);
        SetWindowStyleFlag(fl);
        return true;
    }

    /**
        Selects page, scrolls and/or expands items to ensure that the
        given item is visible.

        @return Returns @true if something was actually done.
    */
    bool EnsureVisible( wxPGPropArg id );

    /**
        Returns number of children of the root property of the selected page.
    */
    size_t GetChildrenCount()
    {
        return GetChildrenCount( m_pPropGrid->m_pState->m_properties );
    }

    /**
        Returns number of children of the root property of given page.
    */
    size_t GetChildrenCount( int pageIndex );

    /**
        Returns number of children for the property.

        NB: Cannot be in container methods class due to name hiding.
    */
    size_t GetChildrenCount( wxPGPropArg id ) const
    {
        wxPG_PROP_ARG_CALL_PROLOG_RETVAL(0)
        return p->GetChildCount();
    }

    /**
        Returns number of columns on given page. By the default,
        returns number of columns on current page.
    */
    int GetColumnCount( int page = -1 ) const;

    /**
        Returns height of the description text box.
    */
    int GetDescBoxHeight() const;

    /**
        Returns pointer to the contained wxPropertyGrid. This does not change
        after wxPropertyGridManager has been created, so you can safely obtain
        pointer once and use it for the entire lifetime of the manager
        instance.
    */
    wxPropertyGrid* GetGrid();

    /**
        Similar to GetIterator, but instead returns wxPGVIterator instance,
        which can be useful for forward-iterating through arbitrary property
        containers.
    */
    virtual wxPGVIterator GetVIterator( int flags ) const;

    /**
        Returns currently selected page.
    */
    wxPropertyGridPage* GetCurrentPage() const;

    /**
        Returns last page.
    */
    wxPropertyGridPage* GetLastPage() const
    {
        return GetPage(m_arrPages.size()-1);
    }

    /**
        Returns page object for given page index.
    */
    wxPropertyGridPage* GetPage( unsigned int ind ) const
    {
        return (wxPropertyGridPage*)m_arrPages.Item(ind);
    }

    /**
        Returns page object for given page name.
    */
    wxPropertyGridPage* GetPage( const wxString& name ) const
    {
        return GetPage(GetPageByName(name));
    }

    /**
        Returns index for a page name. If no match is found, wxNOT_FOUND is
        returned.
    */
    int GetPageByName( const wxString& name ) const;

    /**
        Returns number of managed pages.
    */
    size_t GetPageCount() const;

    /** Returns name of given page. */
    const wxString& GetPageName( int index ) const;

    /**
        Returns "root property" of the given page. It does not have name, etc.
        and it is not visible. It is only useful for accessing its children.
    */
    wxPGProperty* GetPageRoot( int index ) const;

    /** Returns index to currently selected page. */
    int GetSelectedPage() const { return m_selPage; }

    /** Shortcut for GetGrid()->GetSelection(). */
    wxPGProperty* GetSelectedProperty() const
    {
        return m_pPropGrid->GetSelection();
    }

    /** Synonyme for GetSelectedPage. */
    int GetSelection() const { return m_selPage; }

    /**
        Returns a pointer to the toolbar currently associated with the
        wxPropertyGridManager (if any).
    */
    wxToolBar* GetToolBar() const { return m_pToolbar; }

    /** Creates new property page. Note that the first page is not created
        automatically.

        @param index
            Add to this position. -1 will add as the last item.

        @param label
            A label for the page. This may be shown as a toolbar tooltip etc.

        @param bmp
            Bitmap image for toolbar. If wxNullBitmap is used, then a built-in
            default image is used.

        @param pageObj
            wxPropertyGridPage instance. Manager will take ownership of this
            object. If NULL, default page object is constructed.

        @return Returns index to the page created.
    */
    virtual int InsertPage( int index, const wxString& label,
                            const wxBitmap& bmp = wxNullBitmap,
                            wxPropertyGridPage* pageObj = NULL );

    /**
        Returns @true if any property on any page has been modified by the user.
    */
    bool IsAnyModified() const;

    /**
        Returns @true if updating is frozen (ie. Freeze() called but not yet
        Thaw() ).
    */
    bool IsFrozen() const;

    /**
        Returns @true if any property on given page has been modified by the user.
    */
    bool IsPageModified( size_t index ) const;

    /** Removes a page.

        @return Returns @false if it was not possible to remove page in question.
    */
    virtual bool RemovePage( int page );

    /** Select and displays a given page.

        @param index
            Index of page being seleced. Can be -1 to select nothing.
    */
    void SelectPage( int index );

    /**
        Select and displays a given page (by label).
    */
    void SelectPage( const wxString& label );

    /** Select and displays a given page. */
    void SelectPage( wxPropertyGridPage* page );

    /** Select a property. */
    bool SelectProperty( wxPGPropArg id, bool focus = false );

    /**
        Sets number of columns on given page (default is current page).
    */
    void SetColumnCount( int colCount, int page = -1 );

    /** Sets label and text in description box.
    */
    void SetDescription( const wxString& label, const wxString& content );

    /** Sets y coordinate of the description box splitter. */
    void SetDescBoxHeight( int ht, bool refresh = true );

    /** Moves splitter as left as possible, while still allowing all
        labels to be shown in full.

        @param subProps
            If @false, will still allow sub-properties (ie. properties which
            parent is not root or category) to be cropped.

        @param allPages
            If @true, takes labels on all pages into account.
    */
    void SetSplitterLeft( bool subProps = false, bool allPages = true );

    /** Sets splitter position on individual page. */
    void SetPageSplitterPosition( int page, int pos, int column = 0 )
    {
        GetPage(page)->DoSetSplitterPosition( pos, column );
    }

    /**
        Sets splitter position for all pages.

        @remarks Splitter position cannot exceed grid size, and therefore setting
                it during form creation may fail as initial grid size is often
                smaller than desired splitter position, especially when sizers
                are being used.
    */
    void SetSplitterPosition( int pos, int column = 0 );

    /** Synonyme for SelectPage(name). */
    void SetStringSelection( const wxChar* name )
    {
        SelectPage( GetPageByName(name) );
    }

protected:

    //
    // Subclassing helpers
    //

    /**
        Creates property grid for the manager. Override to use subclassed
        wxPropertyGrid.
    */
    virtual wxPropertyGrid* CreatePropertyGrid() const;
};

// -----------------------------------------------------------------------

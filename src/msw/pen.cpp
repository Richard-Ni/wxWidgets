/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/pen.cpp
// Purpose:     wxPen
// Author:      Julian Smart
// Modified by: Vadim Zeitlin: refactored wxPen code to wxPenRefData
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/pen.h"

#ifndef WX_PRECOMP
    #include "wx/bitmap.h"
    #include "wx/utils.h"
#endif

#include "wx/msw/private.h"

#define M_PENDATA ((wxPenRefData*)m_refData)

// Win32 has ExtCreatePen() but WinCE doesn't
#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
    #define wxHAVE_EXT_CREATE_PEN
#endif

// ----------------------------------------------------------------------------
// wxPenRefData: contains information about an HPEN and its handle
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxPenRefData : public wxGDIRefData
{
public:
    // ctors and dtor
    // --------------

    wxPenRefData();
    wxPenRefData(const wxPenRefData& data);
    wxPenRefData(const wxColour& col, int width, int style);
    wxPenRefData(const wxBitmap& stipple, int width);
    virtual ~wxPenRefData();

    bool operator==(const wxPenRefData& data) const
    {
        // we intentionally don't compare m_hPen fields here
        return m_style == data.m_style &&
               m_width == data.m_width &&
               m_join == data.m_join &&
               m_cap == data.m_cap &&
               m_colour == data.m_colour &&
               (m_style != wxSTIPPLE || m_stipple.IsSameAs(data.m_stipple)) &&
               (m_style != wxUSER_DASH ||
                (m_nbDash == data.m_nbDash &&
                    memcmp(m_dash, data.m_dash, m_nbDash*sizeof(wxDash)) == 0));
    }


    // accessors and setters
    // ---------------------

    wxColour& GetColour() const { return wx_const_cast(wxColour&, m_colour); }
    int GetWidth() const { return m_width; }
    int GetStyle() const { return m_style; }
    int GetJoin() const { return m_join; }
    int GetCap() const { return m_cap; }
    wxDash* GetDash() const { return m_dash; }
    int GetDashCount() const { return m_nbDash; }
    wxBitmap* GetStipple() const { return wx_const_cast(wxBitmap *, &m_stipple); }

    void SetColour(const wxColour& col) { Free(); m_colour = col; }
    void SetWidth(int width) { Free(); m_width = width; }
    void SetStyle(int style) { Free(); m_style = style; }
    void SetStipple(const wxBitmap& stipple)
    {
        Free();

        m_style = wxSTIPPLE;
        m_stipple = stipple;
    }

    void SetDashes(int nb_dashes, const wxDash *dash)
    {
        Free();

        m_nbDash = nb_dashes;
        m_dash = wx_const_cast(wxDash *, dash);
    }

    void SetJoin(int join) { Free(); m_join = join; }
    void SetCap(int cap) { Free(); m_cap = cap; }


    // HPEN management
    // ---------------

    // create the HPEN if we don't have it yet
    bool Alloc();

    // get the HPEN creating it on demand
    WXHPEN GetHPEN() const;

    // return true if we have a valid HPEN
    bool HasHPEN() const { return m_hPen != 0; }

    // return true if we had a valid handle before, false otherwise
    bool Free();

private:
    // initialize the fields which have reasonable default values
    //
    // doesn't initialize m_width and m_style which must be initialize in ctor
    void Init()
    {
        m_join = wxJOIN_ROUND;
        m_cap = wxCAP_ROUND;
        m_nbDash = 0;
        m_dash = NULL;
        m_hPen = 0;
    }

    int           m_width;
    int           m_style;
    int           m_join;
    int           m_cap;
    wxBitmap      m_stipple;
    int           m_nbDash;
    wxDash *      m_dash;
    wxColour      m_colour;
    HPEN          m_hPen;

    DECLARE_NO_ASSIGN_CLASS(wxPenRefData)
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxPenRefData ctors/dtor
// ----------------------------------------------------------------------------

wxPenRefData::wxPenRefData()
{
    Init();

    m_style = wxSOLID;
    m_width = 1;
}

wxPenRefData::wxPenRefData(const wxPenRefData& data)
             :wxGDIRefData()
{
    m_style = data.m_style;
    m_width = data.m_width;
    m_join = data.m_join;
    m_cap = data.m_cap;
    m_nbDash = data.m_nbDash;
    m_dash = data.m_dash;
    m_colour = data.m_colour;
    m_hPen = 0;
}

wxPenRefData::wxPenRefData(const wxColour& col, int width, int style)
{
    Init();

    m_style = style;
    m_width = width;

    m_colour = col;
}

wxPenRefData::wxPenRefData(const wxBitmap& stipple, int width)
{
    Init();

    m_style = wxSTIPPLE;
    m_width = width;

    m_stipple = stipple;
}

wxPenRefData::~wxPenRefData()
{
    if ( m_hPen )
        ::DeleteObject(m_hPen);
}

// ----------------------------------------------------------------------------
// wxPenRefData HPEN management
// ----------------------------------------------------------------------------

#ifdef wxHAVE_EXT_CREATE_PEN

static int ConvertPenStyle(int style)
{
    switch ( style )
    {
        case wxDOT:
            return PS_DOT;

        case wxDOT_DASH:
            return PS_DASHDOT;

        case wxSHORT_DASH:
        case wxLONG_DASH:
            return PS_DASH;

        case wxTRANSPARENT:
            return PS_NULL;

        case wxUSER_DASH:
            return PS_USERSTYLE;

        default:
            wxFAIL_MSG( _T("unknown pen style") );
            // fall through

        case wxSTIPPLE:
        case wxBDIAGONAL_HATCH:
        case wxCROSSDIAG_HATCH:
        case wxFDIAGONAL_HATCH:
        case wxCROSS_HATCH:
        case wxHORIZONTAL_HATCH:
        case wxVERTICAL_HATCH:
        case wxSOLID:
            return PS_SOLID;
    }
}

static int ConvertJoinStyle(int join)
{
    switch( join )
    {
        case wxJOIN_BEVEL:
            return PS_JOIN_BEVEL;

        case wxJOIN_MITER:
            return PS_JOIN_MITER;

        default:
            wxFAIL_MSG( _T("unknown pen join style") );
            // fall through

        case wxJOIN_ROUND:
            return PS_JOIN_ROUND;
    }
}

static int ConvertCapStyle(int cap)
{
    switch ( cap )
    {
        case wxCAP_PROJECTING:
            return PS_ENDCAP_SQUARE;

        case wxCAP_BUTT:
            return PS_ENDCAP_FLAT;

        default:
            wxFAIL_MSG( _T("unknown pen cap style") );
            // fall through

        case wxCAP_ROUND:
            return PS_ENDCAP_ROUND;
    }
}

#endif // wxHAVE_EXT_CREATE_PEN

bool wxPenRefData::Alloc()
{
   if ( m_hPen )
       return false;

   if ( m_style == wxTRANSPARENT )
   {
       m_hPen = (HPEN)::GetStockObject(NULL_PEN);
       return true;
   }

   const COLORREF col = m_colour.GetPixel();

#ifdef wxHAVE_EXT_CREATE_PEN
   // Only NT can display dashed or dotted lines with width > 1
   static const int os = wxGetOsVersion();
   if ( os != wxOS_WINDOWS_NT &&
           (m_style == wxDOT ||
            m_style == wxLONG_DASH ||
            m_style == wxSHORT_DASH ||
            m_style == wxDOT_DASH ||
            m_style == wxUSER_DASH) &&
            m_width > 1 )
   {
       m_width = 1;
   }

   // check if it's a standard kind of pen which can be created with just
   // CreatePen()
   if ( m_join == wxJOIN_ROUND &&
            m_cap == wxCAP_ROUND &&
                m_style != wxUSER_DASH &&
                    m_style != wxSTIPPLE &&
                        (m_width <= 1 || m_style == wxSOLID) )
#endif // !wxHAVE_EXT_CREATE_PEN
   {
       m_hPen = ::CreatePen(ConvertPenStyle(m_style), m_width, col);
   }
#ifdef wxHAVE_EXT_CREATE_PEN
   else // need to use ExtCreatePen()
   {
       DWORD styleMSW = PS_GEOMETRIC |
                        ConvertPenStyle(m_style) |
                        ConvertJoinStyle(m_join) |
                        ConvertCapStyle(m_cap);

       LOGBRUSH lb;
       switch( m_style )
       {
           case wxSTIPPLE:
               lb.lbStyle = BS_PATTERN;
               lb.lbHatch = (LONG)m_stipple.GetHBITMAP();
               break;

           case wxBDIAGONAL_HATCH:
               lb.lbStyle = BS_HATCHED;
               lb.lbHatch = HS_BDIAGONAL;
               break;

           case wxCROSSDIAG_HATCH:
               lb.lbStyle = BS_HATCHED;
               lb.lbHatch = HS_DIAGCROSS;
               break;

           case wxFDIAGONAL_HATCH:
               lb.lbStyle = BS_HATCHED;
               lb.lbHatch = HS_FDIAGONAL;
               break;

           case wxCROSS_HATCH:
               lb.lbStyle = BS_HATCHED;
               lb.lbHatch = HS_CROSS;
               break;

           case wxHORIZONTAL_HATCH:
               lb.lbStyle = BS_HATCHED;
               lb.lbHatch = HS_HORIZONTAL;
               break;

           case wxVERTICAL_HATCH:
               lb.lbStyle = BS_HATCHED;
               lb.lbHatch = HS_VERTICAL;
               break;

           default:
               lb.lbStyle = BS_SOLID;
#ifdef __WXDEBUG__
               // this should be unnecessary (it's unused) but suppresses the
               // Purify messages about uninitialized memory read
               lb.lbHatch = 0;
#endif
               break;
       }

       lb.lbColor = col;

       DWORD *dash;
       if ( m_style == wxUSER_DASH && m_nbDash && m_dash )
       {
           dash = new DWORD[m_nbDash];
           int rw = m_width > 1 ? m_width : 1;
           for ( int i = 0; i < m_nbDash; i++ )
               dash[i] = m_dash[i] * rw;
       }
       else
       {
           dash = NULL;
       }

       m_hPen = ::ExtCreatePen(styleMSW, m_width, &lb, m_nbDash, (LPDWORD)dash);

       delete [] dash;
   }
#endif // wxHAVE_EXT_CREATE_PEN

   return m_hPen != 0;
}

bool wxPenRefData::Free()
{
    if ( !m_hPen )
        return false;

    ::DeleteObject(m_hPen);
    m_hPen = 0;

    return true;
}

WXHPEN wxPenRefData::GetHPEN() const
{
    if ( !m_hPen )
        wx_const_cast(wxPenRefData *, this)->Alloc();

    return (WXHPEN)m_hPen;
}

// ----------------------------------------------------------------------------
// wxPen
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPen, wxGDIObject)

wxPen::wxPen(const wxColour& col, int width, int style)
{
    m_refData = new wxPenRefData(col, width, style);
}

wxPen::wxPen(const wxBitmap& stipple, int width)
{
    m_refData = new wxPenRefData(stipple, width);
}

bool wxPen::operator==(const wxPen& pen) const
{
    const wxPenRefData *
        penData = wx_static_cast(const wxPenRefData *, pen.m_refData);

    // an invalid pen is only equal to another invalid pen
    return m_refData ? penData && *M_PENDATA == *penData : !penData;
}

bool wxPen::RealizeResource()
{
    return M_PENDATA && M_PENDATA->Alloc();
}

WXHANDLE wxPen::GetResourceHandle() const
{
    return M_PENDATA ? M_PENDATA->GetHPEN() : 0;
}

bool wxPen::FreeResource(bool WXUNUSED(force))
{
    return M_PENDATA && M_PENDATA->Free();
}

bool wxPen::IsFree() const
{
    return M_PENDATA && !M_PENDATA->HasHPEN();
}

wxObjectRefData* wxPen::CreateRefData() const
{
    return new wxPenRefData;
}

wxObjectRefData* wxPen::CloneRefData(const wxObjectRefData* data) const
{
    return new wxPenRefData(*wx_static_cast(const wxPenRefData*, data));
}

void wxPen::SetColour(const wxColour& col)
{
    AllocExclusive();

    M_PENDATA->SetColour(col);
}

void wxPen::SetColour(unsigned char r, unsigned char g, unsigned char b)
{
    SetColour(wxColour(r, g, b));
}

void wxPen::SetWidth(int width)
{
    AllocExclusive();

    M_PENDATA->SetWidth(width);
}

void wxPen::SetStyle(int style)
{
    AllocExclusive();

    M_PENDATA->SetStyle(style);
}

void wxPen::SetStipple(const wxBitmap& stipple)
{
    AllocExclusive();

    M_PENDATA->SetStipple(stipple);
}

void wxPen::SetDashes(int nb_dashes, const wxDash *dash)
{
    AllocExclusive();

    M_PENDATA->SetDashes(nb_dashes, dash);
}

void wxPen::SetJoin(int join)
{
    AllocExclusive();

    M_PENDATA->SetJoin(join);
}

void wxPen::SetCap(int cap)
{
    AllocExclusive();

    M_PENDATA->SetCap(cap);
}

wxColour& wxPen::GetColour() const
{
    return m_refData ? M_PENDATA->GetColour() : wxNullColour;
}

int wxPen::GetWidth() const
{
    return m_refData ? M_PENDATA->GetWidth() : 0;
}

int wxPen::GetStyle() const
{
    return m_refData ? M_PENDATA->GetStyle() : 0;
}

int wxPen::GetJoin() const
{
    return m_refData ? M_PENDATA->GetJoin() : 0;
}

int wxPen::GetCap() const
{
    return m_refData ? M_PENDATA->GetCap() : 0;
}

int wxPen::GetDashes(wxDash** ptr) const
{
    if ( !m_refData )
    {
        *ptr = NULL;
        return 0;
    }

    *ptr = M_PENDATA->GetDash();
    return M_PENDATA->GetDashCount();
}

wxDash* wxPen::GetDash() const
{
    return m_refData ? M_PENDATA->GetDash() : NULL;
}

int wxPen::GetDashCount() const
{
    return m_refData ? M_PENDATA->GetDashCount() : 0;
}

wxBitmap* wxPen::GetStipple() const
{
    return m_refData ? M_PENDATA->GetStipple() : NULL;
}

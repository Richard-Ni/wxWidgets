///////////////////////////////////////////////////////////////////////////////
// Name:        common/stdpbase.cpp
// Purpose:     wxStandardPathsBase methods common to all ports
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2004-10-19
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Vadim Zeitlin <vadim@wxwindows.org>
// License:     wxWindows license
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

#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif //WX_PRECOMP

#include "wx/filename.h"
#include "wx/stdpaths.h"

// ----------------------------------------------------------------------------
// module globals
// ----------------------------------------------------------------------------

static wxStandardPaths gs_stdPaths;

// ============================================================================
// implementation
// ============================================================================

/* static */
wxStandardPaths& wxStandardPathsBase::Get()
{
    return gs_stdPaths;
}

wxStandardPathsBase::~wxStandardPathsBase()
{
    // nothing to do here
}

wxString wxStandardPathsBase::GetLocalDataDir() const
{
    return GetDataDir();
}

wxString wxStandardPathsBase::GetUserLocalDataDir() const
{
    return GetUserDataDir();
}

/* static */
wxString wxStandardPathsBase::AppendAppName(const wxString& dir)
{
    wxString subdir(dir);

    // empty string indicates that an error has occured, don't touch it then
    if ( !subdir.empty() )
    {
        const wxString appname = wxTheApp->GetAppName();
        if ( !appname.empty() )
        {
            const wxChar ch = *(subdir.end() - 1);
            if ( !wxFileName::IsPathSeparator(ch) && ch != _T('.') )
                subdir += wxFileName::GetPathSeparator();

            subdir += appname;
        }
    }

    return subdir;
}


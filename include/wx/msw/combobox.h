/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.h
// Purpose:     wxComboBox class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __COMBOBOXH__
#define __COMBOBOXH__

#ifdef __GNUG__
#pragma interface "combobox.h"
#endif

#include "wx/choice.h"

#if USE_COMBOBOX

WXDLLEXPORT_DATA(extern const char*) wxComboBoxNameStr;
WXDLLEXPORT_DATA(extern const char*) wxEmptyString;

// Combobox item
class WXDLLEXPORT wxComboBox: public wxChoice
{
  DECLARE_DYNAMIC_CLASS(wxComboBox)

 public:
  inline wxComboBox(void) {}

  inline wxComboBox(wxWindow *parent, const wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const int n = 0, const wxString choices[] = NULL,
           const long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr)
  {
    Create(parent, id, value, pos, size, n, choices, style, validator, name);
  }

  bool Create(wxWindow *parent, const wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const int n = 0, const wxString choices[] = NULL,
           const long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr);

  // List functions: see wxChoice
  
  // Text field functions
  virtual wxString GetValue(void) const ;
  virtual void SetValue(const wxString& value);

  // Clipboard operations
  virtual void Copy(void);
  virtual void Cut(void);
  virtual void Paste(void);
  virtual void SetInsertionPoint(const long pos);
  virtual void SetInsertionPointEnd(void);
  virtual long GetInsertionPoint(void) const ;
  virtual long GetLastPosition(void) const ;
  virtual void Replace(const long from, const long to, const wxString& value);
  virtual void Remove(const long from, const long to);
  virtual void SetSelection(const long from, const long to);
  virtual void SetEditable(const bool editable);

  virtual bool MSWCommand(const WXUINT param, const WXWORD id);
};

#endif // USE_COMBOBOX
#endif
    // __COMBOBOXH__

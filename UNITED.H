#ifndef UNITED_H
#define UNITED_H

// united.h : main header file for the UNITED application
//

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnitedApp:
// See united.cpp for the implementation of this class
//

class CUnitedApp : public CWinApp
{
public:
  CUnitedApp();

// Overrides
  virtual BOOL InitInstance();

// Implementation

  //{{AFX_MSG(CUnitedApp)
  afx_msg void OnAppAbout();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif

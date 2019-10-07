// newname.cpp : implementation file
//

#include "stdafx.h"
#include "united.h"
#include "newname.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NewName dialog


NewName::NewName(CWnd* pParent /*=NULL*/)
  : CDialog(NewName::IDD, pParent)
{
  //{{AFX_DATA_INIT(NewName)
  m_name = "";
  //}}AFX_DATA_INIT
}

void NewName::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(NewName)
  DDX_Text(pDX, IDC_NAME, m_name);
  DDV_MaxChars(pDX, m_name, 32);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(NewName, CDialog)
  //{{AFX_MSG_MAP(NewName)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NewName message handlers

void NewName::OnOK()
{
  UpdateData(TRUE);
  if (m_name == "")
    AfxMessageBox("Der Name muß eingegeben werden.",MB_OK,0);
  else if (check != NULL && check->FindStringExact(-1,m_name) != CB_ERR)
    AfxMessageBox("Dieser Name existiert bereits.",MB_OK,0);
  else CDialog::OnOK();
}

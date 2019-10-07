// unitedgm.cpp : implementation of the CUnitedGame class
//

#include "stdafx.h"
#include "Liga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnitedGame

IMPLEMENT_DYNCREATE(CUnitedGame, CDocument)

BEGIN_MESSAGE_MAP(CUnitedGame, CDocument)
  //{{AFX_MSG_MAP(CUnitedGame)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnitedGame construction/destruction

CUnitedGame::CUnitedGame()
{
  liga = NULL;
  // TODO: add one-time construction code here
}

CUnitedGame::~CUnitedGame()
{
  if (liga != NULL) delete liga;
  liga = NULL;
}

BOOL CUnitedGame::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  if (liga != NULL) delete liga;
  liga = Liga::LigaNew();

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CUnitedGame serialization

void CUnitedGame::Serialize(CArchive& ar)
{
  CDocument::Serialize(ar);
  if (!ar.IsStoring())
  {
    if (liga != NULL) delete liga;
    liga = Liga::LigaNew();
  }
  liga->Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CUnitedGame diagnostics

#ifdef _DEBUG
void CUnitedGame::AssertValid() const
{
  CDocument::AssertValid();
}

void CUnitedGame::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUnitedGame commands

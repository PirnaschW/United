// unitedvw.cpp : implementation of the CUnitedView class
//

#include "stdafx.h"
#include "Liga.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnitedView

IMPLEMENT_DYNCREATE(CUnitedView, CScrollView)

BEGIN_MESSAGE_MAP(CUnitedView, CScrollView)
  //{{AFX_MSG_MAP(CUnitedView)
  ON_COMMAND(ID_LIGA_EDIT, OnLigaEdit)
  ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateLigaNew)
  ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateLigaOpen)
  ON_UPDATE_COMMAND_UI(ID_LIGA_EDIT, OnUpdateLigaEdit)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateLigaSave)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateLigaSaveAs)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateLigaPrint)
  ON_UPDATE_COMMAND_UI(ID_PRINT_SETUP, OnUpdatePrintSetup)
  ON_UPDATE_COMMAND_UI(ID_FILE_PREVW, OnUpdateLigaPrevw)
  ON_COMMAND(ID_SAISON_STARTEN, OnSaisonStarten)
  ON_COMMAND(ID_SAISON_SPIELTAGBEGINNEN, OnSaisonSpieltagbeginnen)
  ON_COMMAND(ID_SAISON_AUFSTELLUNGFESTLEGEN, OnSaisonAufstellungfestlegen)
  ON_COMMAND(ID_SAISON_SPIELTAGBEENDEN, OnSaisonSpieltagbeenden)
  ON_UPDATE_COMMAND_UI(ID_SAISON_STARTEN, OnUpdateSaisonStarten)
  ON_UPDATE_COMMAND_UI(ID_SAISON_SPIELTAGBEGINNEN, OnUpdateSaisonSpieltagbeginnen)
  ON_UPDATE_COMMAND_UI(ID_SAISON_AUFSTELLUNGFESTLEGEN, OnUpdateSaisonAufstellungfestlegen)
  ON_UPDATE_COMMAND_UI(ID_SAISON_SPIELTAGBEENDEN, OnUpdateSaisonSpieltagbeenden)
  //}}AFX_MSG_MAP
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnitedView construction/destruction

CUnitedView::CUnitedView()
{
  // TODO: add construction code here
}

CUnitedView::~CUnitedView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CUnitedView drawing

void CUnitedView::OnDraw(CDC* pDC)
{
  CUnitedGame* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  // write the liga status information onto the main view
  if (!IfLiga()) pDC->TextOut(10,10,(CString) "Nächster Schritt: Liga erzeugen");
  else
  {
    pDoc->liga->ChartShow(pDC);
    ((CMainFrame *) GetParentFrame())->StatusMessage(pDoc->liga->Hint());
  }
}

void CUnitedView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();
  SetScrollSizes(MM_TEXT,CSize(535,200));
}

void CUnitedView::OnUpdate (CView* pSender, LPARAM lHint, CObject * pHint)
{
  CScrollView::OnUpdate(pSender,lHint,pHint);
  int y = 60;
  if (IfLiga()) y += (GetDocument()->liga->GetTeamsSize() + 2) * 15;
  if (y < 200) y = 200;
  SetScrollSizes(MM_TEXT,CSize(535,y));
  GetParentFrame()->RecalcLayout();
  ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CUnitedView printing

BOOL CUnitedView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CUnitedView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CUnitedView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CUnitedView diagnostics

#ifdef _DEBUG
void CUnitedView::AssertValid() const
{
  CScrollView::AssertValid();
}

void CUnitedView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}

CUnitedGame* CUnitedView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUnitedGame)));
  return (CUnitedGame*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUnitedView message handlers

// handle the liga-menu:
void CUnitedView::OnLigaEdit()    { GetDocument()->liga->EditData(); Modified(); }
// end of liga-menu

void CUnitedView::OnSaisonStarten()              { GetDocument()->liga->SeasonStart();     Modified(); }
void CUnitedView::OnSaisonSpieltagbeginnen()     { GetDocument()->liga->RoundBegin();      Modified(); }
void CUnitedView::OnSaisonAufstellungfestlegen() { GetDocument()->liga->FormationDefine(); Modified(); }
void CUnitedView::OnSaisonSpieltagbeenden()      { GetDocument()->liga->RoundEnd();        Modified(); }

void CUnitedView::ScrBarInit (CWnd * wnd, const int min, const int max, int * value)
{
  CScrollBar * bar = (CScrollBar *) wnd;
  bar->SetScrollRange(min,max,FALSE);
  ScrBarSet(bar,value);
}

void CUnitedView::ScrBarSet (CWnd * wnd, int * value)
{
  int min, max;
  CScrollBar * bar = (CScrollBar *) wnd;
  bar->GetScrollRange(&min,&max);
  bar->EnableScrollBar(ESB_ENABLE_BOTH);
  if (*value <= min)
  {
    *value = min;
    bar->EnableScrollBar(ESB_DISABLE_LTUP);
  }
  if (*value >= max)
  {
    *value = max;
    bar->EnableScrollBar(ESB_DISABLE_RTDN);
  }
  bar->SetScrollPos(*value,TRUE);
}

void CUnitedView::Modified () { GetDocument()->SetModifiedFlag(); GetDocument()->UpdateAllViews(NULL); }

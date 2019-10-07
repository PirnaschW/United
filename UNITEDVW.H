#ifndef UNITEDVW_H
#define UNITEDVW_H

// unitedvw.h : interface of the CUnitedView class
//
/////////////////////////////////////////////////////////////////////////////

class CUnitedGame;
class Liga;
class Team;
class Player;

class CUnitedView : public CScrollView
{
protected: // create from serialization only
  CUnitedView();
  DECLARE_DYNCREATE(CUnitedView)

// Attributes
public:
  CUnitedGame* GetDocument();

// Operations
public:
  static void ScrBarInit (CWnd * wnd, const int min, const int max, int * value);
  static void ScrBarSet  (CWnd * wnd, int * value);

// Implementation
public:
  virtual ~CUnitedView();
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Printing support
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

protected:
  virtual void OnInitialUpdate (); // called first time after construct
  virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject * pHint);

  void Modified     ();
  BOOL IfLiga       ();
  BOOL IfNotStarted ();
  BOOL IfStarted    ();
  BOOL IfNoRound    ();
  BOOL IfRound      ();

// Generated message map functions
protected:
  //{{AFX_MSG(CUnitedView)
  afx_msg void OnLigaNew();
  afx_msg void OnLigaEdit();
  afx_msg void OnLigaClose();
  afx_msg void OnUpdateLigaNew(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaOpen(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaEdit(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaSave(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaSaveAs(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaPrint(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaClose(CCmdUI* pCmdUI);
  afx_msg void OnUpdatePrintSetup(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLigaPrevw(CCmdUI* pCmdUI);
  afx_msg void OnSaisonStarten();
  afx_msg void OnSaisonSpieltagbeginnen();
  afx_msg void OnSaisonSpieltagbeenden();
  afx_msg void OnAufstellungFestlegen();
  afx_msg void OnTabelleAnzeigen();
  afx_msg void OnUpdateSaisonStarten(CCmdUI* pCmdUI);
  afx_msg void OnUpdateSaisonSpieltagbeginnen(CCmdUI* pCmdUI);
  afx_msg void OnUpdateSaisonSpieltagbeenden(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAufstellungFestlegen(CCmdUI* pCmdUI);
  afx_msg void OnUpdateTabelleAnzeigen(CCmdUI* pCmdUI);
	afx_msg void OnSaisonAufstellungfestlegen();
	afx_msg void OnUpdateSaisonAufstellungfestlegen(CCmdUI* pCmdUI);
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in unitedvw.cpp
inline CUnitedGame* CUnitedView::GetDocument()
   { return (CUnitedGame*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

#endif

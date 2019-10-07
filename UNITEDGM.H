#ifndef UNITEDGM_H
#define UNITEDGM_H

// unitedgm.h : interface of the CUnitedGame class
//
/////////////////////////////////////////////////////////////////////////////

class Liga;

class CUnitedGame : public CDocument
{
protected: // create from serialization only
  CUnitedGame();
  DECLARE_DYNCREATE(CUnitedGame)

// Attributes
public:
  Liga * liga;
// Operations
public:

// Implementation
public:
  virtual ~CUnitedGame();
  virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  virtual BOOL OnNewDocument();

// Generated message map functions
protected:
  //{{AFX_MSG(CUnitedGame)
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif

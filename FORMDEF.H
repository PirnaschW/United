// formdef.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FormDef dialog

#include "Row.h"
class Player;

class FormDef : public CDialog
{
// Construction
public:
  FormDef(CWnd* pParent = NULL);  // standard constructor

  void TeamDisp       (const int index = 0);
  void PlayerSelected (CListBox * m, ROW row);
  void PlayerToggle   (Player * p, int flag, Player * * arr, Player * * list, CListBox * m, ROW r);
  int  SumStrength    (CListBox * m, const ROW r) const;
  void ShowSums       ();
  void RoughChanged    (int ID, int * m);
  void HomeBonusChanged(int ID, int * m);

  TeamArray * teams;
  int n;
  Player * * arr_t;
  Player * * arr_a;
  Player * * arr_v;
  Player * * arr_m;
  Player * * arr_s;
  Player * * list_t;
  Player * * list_a;
  Player * * list_v;
  Player * * list_m;
  Player * * list_s;
  int      * sel;

// Dialog Data
  //{{AFX_DATA(FormDef)
  enum { IDD = IDD_FORMATION_DEFINE };
  CListBox  m_ftor;
  CListBox  m_fsturm;
  CListBox  m_fausputzer;
  CListBox  m_fabwehr;
  CListBox  m_fmitte;
  int   m_fteam;
  CString m_sabwehr;
  CString m_sausputzer;
  CString m_smitte;
  CString m_ssturm;
  CString m_stor;
  int   m_ha;
  int   m_ham;
  int   m_has;
  int   m_hav;
  int   m_hb;
  int   m_hbm;
  int   m_hbs;
  int   m_hbv;
  //}}AFX_DATA

// Implementation
protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

  // Generated message map functions
  //{{AFX_MSG(FormDef)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnSelchangeFteam();
  afx_msg void OnSelchangeFabwehr();
  afx_msg void OnSelchangeFausputzer();
  afx_msg void OnSelchangeFmitte();
  afx_msg void OnSelchangeFsturm();
  afx_msg void OnSelchangeFtor();
  afx_msg void OnCheck();
  afx_msg void OnNext();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnKillfocusHaS();
  afx_msg void OnKillfocusHaV();
  afx_msg void OnKillfocusHbV();
  afx_msg void OnKillfocusHbM();
  afx_msg void OnKillfocusHbS();
  afx_msg void OnKillfocusHaM();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

extern "C" {
int PlayerOrderT (const void * e1, const void * e2);
int PlayerOrderA (const void * e1, const void * e2);
int PlayerOrderV (const void * e1, const void * e2);
int PlayerOrderM (const void * e1, const void * e2);
int PlayerOrderS (const void * e1, const void * e2);
}

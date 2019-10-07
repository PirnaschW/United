// saisonst.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SaisonStart dialog

class Liga;

class SaisonStart : public CDialog
{
// Construction
public:
  SaisonStart(CWnd* pParent = NULL);  // standard constructor
  void EnableWindows (const BOOL robin, const BOOL groups, const BOOL swiss);

  Liga * liga;

// Dialog Data
  //{{AFX_DATA(SaisonStart)
  enum { IDD = IDD_SAISON_START };
  int   m_robin_times;
  int   m_groups_size;
  int   m_swiss_rounds;
  //}}AFX_DATA

// Implementation
protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

  // Generated message map functions
  //{{AFX_MSG(SaisonStart)
  virtual BOOL OnInitDialog();
  afx_msg void OnSwiss();
  afx_msg void OnRobin();
  afx_msg void OnGroups();
  afx_msg void OnKillfocusGroupsSize();
  afx_msg void OnKillfocusRobinTimes();
  afx_msg void OnKillfocusSwissRounds();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

// newname.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NewName dialog

class NewName : public CDialog
{
// Construction
public:
  NewName(CWnd* pParent = NULL);  // standard constructor
  CComboBox * check;

// Dialog Data
  //{{AFX_DATA(NewName)
  enum { IDD = IDD_NEW_NAME };
  CString m_name;
  //}}AFX_DATA

// Implementation
protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

  // Generated message map functions
  //{{AFX_MSG(NewName)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

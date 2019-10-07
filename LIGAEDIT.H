// ligaedit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LigaEdit dialog

class Liga;
class TeamArray;
class Team;
class PlayerArray;
class Player;

class LigaEdit : public CDialog
{
// Construction
public:
  LigaEdit(CWnd* pParent = NULL); // standard constructor

  void TeamDisp      (const int index = 0);
  void TeamEnableWindows (const BOOL flag);
  void PlayerDisp    (const int index = 0);
  void PlayerEnableWindows (const BOOL flag);
  void OnCBoxValue   (const int IDC, const int IDSB, int * value, LONG * var);
  void SetCBox       (const int IDC, const int value);
  void OnCBoxClicked (const int IDC, const int IDSB, int * value, LONG * var);
  void CheckLimits   (const int IDC, const int value);
  void ScrBarInit    (const int ID, const int min, const int max, int * value);
  void ScrBarSet     (CScrollBar * bar, int * value);
  Liga        * liga;
  TeamArray   * teams;
  Team        * team;
  PlayerArray * players;
  Player      * player;

// Dialog Data
  //{{AFX_DATA(LigaEdit)
  enum { IDD = IDD_LIGA_EDIT };
  CScrollBar  m_sbtor;
  CScrollBar  m_sbsturm;
  CScrollBar  m_sbmitte;
  CScrollBar  m_sbausputzer;
  CScrollBar  m_sbabwehr;
  CScrollBar  m_sbsperren;
  CScrollBar  m_sbgelb;
  CScrollBar  m_sbrot;
  CScrollBar  m_sbtrainiert;
  CScrollBar  m_sbalter;
  CScrollBar  m_sbstaerke;
  CScrollBar  m_sbtrainings;
  CScrollBar  m_sbhomebonus;
  CString m_liga;
  CString m_master;
  int   m_team;
  CString m_trainer;
  int   m_trainings;
  int   m_homebonus;
  int   m_points_plus;
  int   m_points_minus;
  int   m_goals_plus;
  int   m_goals_minus;
  int   m_player;
  int   m_staerke;
  int   m_alter;
  int   m_trainiert;
  int   m_rot;
  int   m_gelb;
  int   m_sperren;
  int   m_tor;
  int   m_sturm;
  int   m_abwehr;
  int   m_ausputzer;
  int   m_mitte;
  int   m_tactic;
  //}}AFX_DATA

// Implementation
protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

  // Generated message map functions
  //{{AFX_MSG(LigaEdit)
  virtual BOOL OnInitDialog();
  afx_msg void OnTeamNew();
  afx_msg void OnTeamDelete();
  afx_msg void OnPlayerNew();
  afx_msg void OnPlayerDelete();
  afx_msg void OnSelchangeTeam();
  afx_msg void OnSelchangePlayer();
  afx_msg void OnCabwehr();
  afx_msg void OnCausputzer();
  afx_msg void OnCmitte();
  afx_msg void OnCsturm();
  afx_msg void OnCtor();
  afx_msg void OnKillfocusMaster();
  afx_msg void OnKillfocusAbwehr();
  afx_msg void OnKillfocusAlter();
  afx_msg void OnKillfocusAusputzer();
  afx_msg void OnKillfocusGelb();
  afx_msg void OnKillfocusGoalsMinus();
  afx_msg void OnKillfocusGoalsPlus();
  afx_msg void OnKillfocusHomebonus();
  afx_msg void OnKillfocusLiga();
  afx_msg void OnKillfocusMitte();
  afx_msg void OnKillfocusPointsMinus();
  afx_msg void OnKillfocusPointsPlus();
  afx_msg void OnKillfocusRot();
  afx_msg void OnKillfocusSperren();
  afx_msg void OnKillfocusStaerke();
  afx_msg void OnKillfocusSturm();
  afx_msg void OnKillfocusTor();
  afx_msg void OnKillfocusTrainer();
  afx_msg void OnKillfocusTrainiert();
  afx_msg void OnKillfocusTrainings();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnSelchangeTactic();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

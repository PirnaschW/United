// ligaedit.cpp : implementation file
//

#include "stdafx.h"
#include "NewName.h"
#include "LigaEdit.h"
#include "Liga.h"
#include "TeamA.h"
#include "Team.h"
#include "PlayerA.h"
#include "Player.h"
#include "Row.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LigaEdit dialog

LigaEdit::LigaEdit(CWnd* pParent /*=NULL*/)
  : CDialog(LigaEdit::IDD, pParent)
{
  //{{AFX_DATA_INIT(LigaEdit)
  m_liga = "";
  m_master = "";
  m_team = -1;
  m_trainer = "";
  m_trainings = 0;
  m_homebonus = 0;
  m_points_plus = 0;
  m_points_minus = 0;
  m_goals_plus = 0;
  m_goals_minus = 0;
  m_player = -1;
  m_staerke = 0;
  m_alter = 0;
  m_trainiert = 0;
  m_rot = 0;
  m_gelb = 0;
  m_sperren = 0;
  m_tor = 0;
  m_sturm = 0;
  m_abwehr = 0;
  m_ausputzer = 0;
  m_mitte = 0;
  m_tactic = -1;
  //}}AFX_DATA_INIT
}

void LigaEdit::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(LigaEdit)
  DDX_Control(pDX, IDC_SBTOR, m_sbtor);
  DDX_Control(pDX, IDC_SBSTURM, m_sbsturm);
  DDX_Control(pDX, IDC_SBMITTE, m_sbmitte);
  DDX_Control(pDX, IDC_SBAUSPUTZER, m_sbausputzer);
  DDX_Control(pDX, IDC_SBABWEHR, m_sbabwehr);
  DDX_Control(pDX, IDC_SBSPERREN, m_sbsperren);
  DDX_Control(pDX, IDC_SBGELB, m_sbgelb);
  DDX_Control(pDX, IDC_SBROT, m_sbrot);
  DDX_Control(pDX, IDC_SBTRAINIERT, m_sbtrainiert);
  DDX_Control(pDX, IDC_SBALTER, m_sbalter);
  DDX_Control(pDX, IDC_SBSTAERKE, m_sbstaerke);
  DDX_Control(pDX, IDC_SBTRAININGS, m_sbtrainings);
  DDX_Control(pDX, IDC_SBHOMEBONUS, m_sbhomebonus);
  DDX_Text(pDX, IDC_LIGA, m_liga);
  DDV_MaxChars(pDX, m_liga, 32);
  DDX_Text(pDX, IDC_MASTER, m_master);
  DDV_MaxChars(pDX, m_master, 32);
  DDX_CBIndex(pDX, IDC_TEAM, m_team);
  DDX_Text(pDX, IDC_TRAINER, m_trainer);
  DDV_MaxChars(pDX, m_trainer, 32);
  DDX_Text(pDX, IDC_TRAININGS, m_trainings);
  DDX_Text(pDX, IDC_HOMEBONUS, m_homebonus);
  DDX_Text(pDX, IDC_POINTS_PLUS, m_points_plus);
  DDX_Text(pDX, IDC_POINTS_MINUS, m_points_minus);
  DDX_Text(pDX, IDC_GOALS_PLUS, m_goals_plus);
  DDX_Text(pDX, IDC_GOALS_MINUS, m_goals_minus);
  DDX_CBIndex(pDX, IDC_PLAYER, m_player);
  DDX_Text(pDX, IDC_STAERKE, m_staerke);
  DDX_Text(pDX, IDC_ALTER, m_alter);
  DDX_Text(pDX, IDC_TRAINIERT, m_trainiert);
  DDX_Text(pDX, IDC_ROT, m_rot);
  DDX_Text(pDX, IDC_GELB, m_gelb);
  DDX_Text(pDX, IDC_SPERREN, m_sperren);
  DDX_Text(pDX, IDC_TOR, m_tor);
  DDX_Text(pDX, IDC_STURM, m_sturm);
  DDX_Text(pDX, IDC_ABWEHR, m_abwehr);
  DDX_Text(pDX, IDC_AUSPUTZER, m_ausputzer);
  DDX_Text(pDX, IDC_MITTE, m_mitte);
  DDX_CBIndex(pDX, IDC_TACTIC, m_tactic);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(LigaEdit, CDialog)
  //{{AFX_MSG_MAP(LigaEdit)
  ON_BN_CLICKED(IDC_TEAM_NEW, OnTeamNew)
  ON_BN_CLICKED(IDC_TEAM_DELETE, OnTeamDelete)
  ON_BN_CLICKED(IDC_PLAYER_NEW, OnPlayerNew)
  ON_BN_CLICKED(IDC_PLAYER_DELETE, OnPlayerDelete)
  ON_CBN_SELCHANGE(IDC_TEAM, OnSelchangeTeam)
  ON_CBN_SELCHANGE(IDC_PLAYER, OnSelchangePlayer)
  ON_BN_CLICKED(IDC_CABWEHR, OnCabwehr)
  ON_BN_CLICKED(IDC_CAUSPUTZER, OnCausputzer)
  ON_BN_CLICKED(IDC_CMITTE, OnCmitte)
  ON_BN_CLICKED(IDC_CSTURM, OnCsturm)
  ON_BN_CLICKED(IDC_CTOR, OnCtor)
  ON_EN_KILLFOCUS(IDC_MASTER, OnKillfocusMaster)
  ON_EN_KILLFOCUS(IDC_ABWEHR, OnKillfocusAbwehr)
  ON_EN_KILLFOCUS(IDC_ALTER, OnKillfocusAlter)
  ON_EN_KILLFOCUS(IDC_AUSPUTZER, OnKillfocusAusputzer)
  ON_EN_KILLFOCUS(IDC_GELB, OnKillfocusGelb)
  ON_EN_KILLFOCUS(IDC_GOALS_MINUS, OnKillfocusGoalsMinus)
  ON_EN_KILLFOCUS(IDC_GOALS_PLUS, OnKillfocusGoalsPlus)
  ON_EN_KILLFOCUS(IDC_HOMEBONUS, OnKillfocusHomebonus)
  ON_EN_KILLFOCUS(IDC_LIGA, OnKillfocusLiga)
  ON_EN_KILLFOCUS(IDC_MITTE, OnKillfocusMitte)
  ON_EN_KILLFOCUS(IDC_POINTS_MINUS, OnKillfocusPointsMinus)
  ON_EN_KILLFOCUS(IDC_POINTS_PLUS, OnKillfocusPointsPlus)
  ON_EN_KILLFOCUS(IDC_ROT, OnKillfocusRot)
  ON_EN_KILLFOCUS(IDC_SPERREN, OnKillfocusSperren)
  ON_EN_KILLFOCUS(IDC_STAERKE, OnKillfocusStaerke)
  ON_EN_KILLFOCUS(IDC_STURM, OnKillfocusSturm)
  ON_EN_KILLFOCUS(IDC_TOR, OnKillfocusTor)
  ON_EN_KILLFOCUS(IDC_TRAINER, OnKillfocusTrainer)
  ON_EN_KILLFOCUS(IDC_TRAINIERT, OnKillfocusTrainiert)
  ON_EN_KILLFOCUS(IDC_TRAININGS, OnKillfocusTrainings)
  ON_WM_HSCROLL()
  ON_CBN_SELCHANGE(IDC_TACTIC, OnSelchangeTactic)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// LigaEdit message handlers

BOOL LigaEdit::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  m_liga   = liga->name;
  m_master = liga->master;
  teams    = liga->teams;
  TeamDisp();
  UpdateData(FALSE);
  return TRUE;  // return TRUE  unless you set the focus to a control
}

// handle changes in liga data
void LigaEdit::OnKillfocusLiga()
{
  UpdateData(TRUE);
  liga->name = m_liga;
}

void LigaEdit::OnKillfocusMaster()
{
  UpdateData(TRUE);
  liga->master = m_master;
}
// end of liga data

// handle team changes
void LigaEdit::OnSelchangeTeam()
{
  UpdateData(TRUE);
  TeamDisp(m_team);
}

void LigaEdit::OnTeamNew()
{
  NewName dlg;
  dlg.check = (CComboBox *) GetDlgItem(IDC_TEAM);
  if (dlg.DoModal() == IDOK) TeamDisp(teams->AddTeam(dlg.m_name));
}

void LigaEdit::OnTeamDelete()
{
  UpdateData(TRUE);
  if (m_team != -1) teams->DelTeam(m_team);
  if (m_team > 0) --m_team;
  TeamDisp(m_team);
}

void LigaEdit::TeamDisp (const int index)
{
  teams = liga->teams;
  GetDlgItem(IDC_TEAM)->EnableWindow(TRUE);
  teams->ListTeams((CComboBox *) GetDlgItem(IDC_TEAM));
  GetDlgItem(IDC_TEAM_NEW)->EnableWindow(TRUE);
  if (teams->TeamsExist())
  {
    TeamEnableWindows(TRUE);
    m_team = index;
    team = teams->GetTeam(m_team);
    m_trainer = team->trainer;
    m_homebonus = (int) team->heimvorteil;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHOMEBONUS),3,9,&m_homebonus);
    m_trainings = (int) team->training;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBTRAININGS),0,4,&m_trainings);
    m_tactic = team->preferred->AsInt()-2;
    m_points_plus  = (int) team->punkte[0];
    m_points_minus = (int) team->punkte[1];
    m_goals_plus   = (int) team->tore[0];
    m_goals_minus  = (int) team->tore[1];
    PlayerDisp();
  }
  else
  {
    TeamEnableWindows(FALSE);
    PlayerEnableWindows(FALSE);
  }
  UpdateData(FALSE);
}

void LigaEdit::TeamEnableWindows (const BOOL flag)
{
  GetDlgItem(IDC_TEAM_DELETE)->EnableWindow(flag);
  GetDlgItem(IDS_TRAINER)->EnableWindow(flag);
  GetDlgItem(IDC_TRAINER)->EnableWindow(flag);
  GetDlgItem(IDS_HOMEBONUS)->EnableWindow(flag);
  GetDlgItem(IDC_HOMEBONUS)->EnableWindow(flag);
  GetDlgItem(IDC_SBHOMEBONUS)->EnableWindow(flag);
  GetDlgItem(IDS_TRAININGS)->EnableWindow(flag);
  GetDlgItem(IDC_TRAININGS)->EnableWindow(flag);
  GetDlgItem(IDC_SBTRAININGS)->EnableWindow(flag);
  GetDlgItem(IDS_TACTIC)->EnableWindow(flag);
  GetDlgItem(IDC_TACTIC)->EnableWindow(flag);
  GetDlgItem(IDS_POINTS)->EnableWindow(flag);
  GetDlgItem(IDC_POINTS_PLUS)->EnableWindow(flag);
  GetDlgItem(IDS_POINTS_COLON)->EnableWindow(flag);
  GetDlgItem(IDC_POINTS_MINUS)->EnableWindow(flag);
  GetDlgItem(IDS_GOALS)->EnableWindow(flag);
  GetDlgItem(IDC_GOALS_PLUS)->EnableWindow(flag);
  GetDlgItem(IDS_GOALS_COLON)->EnableWindow(flag);
  GetDlgItem(IDC_GOALS_MINUS)->EnableWindow(flag);
  GetDlgItem(IDS_SPIELER)->EnableWindow(flag);
  GetDlgItem(IDS_PLAYER)->EnableWindow(flag);
  GetDlgItem(IDC_PLAYER)->EnableWindow(flag);
  GetDlgItem(IDC_PLAYER_NEW)->EnableWindow(flag);
}
// end of team changes

// handle changes in team data
void LigaEdit::OnKillfocusTrainer()
{
  UpdateData(TRUE);
  team->trainer = m_trainer;
}

void LigaEdit::OnKillfocusHomebonus()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBHOMEBONUS),&m_homebonus);
  team->heimvorteil = m_homebonus;
}

void LigaEdit::OnKillfocusTrainings()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBTRAININGS),&m_trainings);
  team->training = m_trainings;
}

void LigaEdit::OnSelchangeTactic()
{
  UpdateData(TRUE);
  team->preferred->Set((ROW) (m_tactic+2));
}

void LigaEdit::OnKillfocusPointsPlus()
{
  UpdateData(TRUE);
  team->punkte[0] = m_points_plus;
}

void LigaEdit::OnKillfocusPointsMinus()
{
  UpdateData(TRUE);
  team->punkte[1] = m_points_minus;
}

void LigaEdit::OnKillfocusGoalsPlus()
{
  UpdateData(TRUE);
  team->tore[0] = m_goals_plus;
}

void LigaEdit::OnKillfocusGoalsMinus()
{
  UpdateData(TRUE);
  team->tore[1] = m_goals_minus;
}
// end of team data

// handle player changes
void LigaEdit::OnSelchangePlayer()
{
  UpdateData(TRUE);
  PlayerDisp(m_player);
}

void LigaEdit::OnPlayerNew()
{
  NewName dlg;
  dlg.check = (CComboBox *) GetDlgItem(IDC_PLAYER);
  if (dlg.DoModal() == IDOK) PlayerDisp(players->AddPlayer(dlg.m_name));
}

void LigaEdit::OnPlayerDelete()
{
  UpdateData(TRUE);
  if (m_player != -1) players->DelPlayer(m_player);
  if (m_player > 0) --m_player;
  PlayerDisp(m_player);
}

void LigaEdit::PlayerDisp (const int index)
{
  players = team->players;
  players->ListPlayers((CComboBox *) GetDlgItem(IDC_PLAYER));
  if (players->PlayersExist())
  {
    PlayerEnableWindows(TRUE);
    m_player = index;
    player = players->GetPlayer(m_player);
    m_staerke = (int) player->staerke;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBSTAERKE),  0,10,&m_staerke);
    m_alter = (int) player->alter;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBALTER),    0, 6,&m_alter);
    m_trainiert = (int) player->trainiert;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBTRAINIERT),0, 8,&m_trainiert);
    m_gelb = (int) player->gelb;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBGELB),     0,99,&m_gelb);
    m_rot = (int) player->rot;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBROT),      0,99,&m_rot);
    m_sperren = (int) player->sperre;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBSPERREN),  0, 7,&m_sperren);
    m_tor = (int) player->einsatz_t;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBTOR),      0, 6,&m_tor);
    m_ausputzer = (int) player->einsatz_a;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBAUSPUTZER),0, 6,&m_ausputzer);
    m_abwehr = (int) player->einsatz_v;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBABWEHR),   0, 6,&m_abwehr);
    m_mitte = (int) player->einsatz_m;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBMITTE),    0, 6,&m_mitte);
    m_sturm = (int) player->einsatz_s;
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBSTURM),    0, 6,&m_sturm);
    SetCBox(IDC_CTOR,m_tor);
    SetCBox(IDC_CAUSPUTZER,m_ausputzer);
    SetCBox(IDC_CABWEHR,m_abwehr);
    SetCBox(IDC_CMITTE,m_mitte);
    SetCBox(IDC_CSTURM,m_sturm);
  }
  else PlayerEnableWindows(FALSE);
  UpdateData(FALSE);
}

void LigaEdit::PlayerEnableWindows (const BOOL flag)
{
  GetDlgItem(IDC_PLAYER_DELETE)->EnableWindow(flag);
  GetDlgItem(IDS_PERSON)->EnableWindow(flag);
  GetDlgItem(IDS_STAERKE)->EnableWindow(flag);
  GetDlgItem(IDC_STAERKE)->EnableWindow(flag);
  GetDlgItem(IDC_SBSTAERKE)->EnableWindow(flag);
  GetDlgItem(IDS_ALTER)->EnableWindow(flag);
  GetDlgItem(IDC_ALTER)->EnableWindow(flag);
  GetDlgItem(IDC_SBALTER)->EnableWindow(flag);
  GetDlgItem(IDS_TRAINIERT)->EnableWindow(flag);
  GetDlgItem(IDC_TRAINIERT)->EnableWindow(flag);
  GetDlgItem(IDC_SBTRAINIERT)->EnableWindow(flag);
  GetDlgItem(IDS_DISZIPLINAR)->EnableWindow(flag);
  GetDlgItem(IDS_GELB)->EnableWindow(flag);
  GetDlgItem(IDC_GELB)->EnableWindow(flag);
  GetDlgItem(IDC_SBGELB)->EnableWindow(flag);
  GetDlgItem(IDS_ROT)->EnableWindow(flag);
  GetDlgItem(IDC_ROT)->EnableWindow(flag);
  GetDlgItem(IDC_SBROT)->EnableWindow(flag);
  GetDlgItem(IDS_SPERREN)->EnableWindow(flag);
  GetDlgItem(IDC_SPERREN)->EnableWindow(flag);
  GetDlgItem(IDC_SBSPERREN)->EnableWindow(flag);
  GetDlgItem(IDS_POSITION)->EnableWindow(flag);
  GetDlgItem(IDS_EINSAETZE)->EnableWindow(flag);
  GetDlgItem(IDS_EINGESPIELT)->EnableWindow(flag);
  GetDlgItem(IDS_TOR)->EnableWindow(flag);
  GetDlgItem(IDC_TOR)->EnableWindow(flag);
  GetDlgItem(IDC_SBTOR)->EnableWindow(flag);
  GetDlgItem(IDC_CTOR)->EnableWindow(flag);
  GetDlgItem(IDS_AUSPUTZER)->EnableWindow(flag);
  GetDlgItem(IDC_AUSPUTZER)->EnableWindow(flag);
  GetDlgItem(IDC_SBAUSPUTZER)->EnableWindow(flag);
  GetDlgItem(IDC_CAUSPUTZER)->EnableWindow(flag);
  GetDlgItem(IDS_ABWEHR)->EnableWindow(flag);
  GetDlgItem(IDC_ABWEHR)->EnableWindow(flag);
  GetDlgItem(IDC_SBABWEHR)->EnableWindow(flag);
  GetDlgItem(IDC_CABWEHR)->EnableWindow(flag);
  GetDlgItem(IDS_MITTE)->EnableWindow(flag);
  GetDlgItem(IDC_MITTE)->EnableWindow(flag);
  GetDlgItem(IDC_SBMITTE)->EnableWindow(flag);
  GetDlgItem(IDC_CMITTE)->EnableWindow(flag);
  GetDlgItem(IDS_STURM)->EnableWindow(flag);
  GetDlgItem(IDC_STURM)->EnableWindow(flag);
  GetDlgItem(IDC_SBSTURM)->EnableWindow(flag);
  GetDlgItem(IDC_CSTURM)->EnableWindow(flag);
}
// end of player changes

// handle changes in player data
void LigaEdit::OnKillfocusStaerke()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet((CScrollBar *) GetDlgItem(IDC_SBSTAERKE),&m_staerke);
  player->staerke = m_staerke;
}

void LigaEdit::OnKillfocusAlter()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBALTER),&m_alter);
  player->alter = m_alter;
}

void LigaEdit::OnKillfocusTrainiert()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBTRAINIERT),&m_trainiert);
  player->trainiert = m_trainiert;
}

void LigaEdit::OnKillfocusGelb()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBGELB),&m_gelb);
  player->gelb = m_gelb;
}

void LigaEdit::OnKillfocusRot()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBROT),&m_rot);
  player->rot = m_rot;
}

void LigaEdit::OnKillfocusSperren()
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDC_SBSPERREN),&m_sperren);
  player->sperre = m_sperren;
}

void LigaEdit::OnKillfocusTor()       { OnCBoxValue(IDC_CTOR,      IDC_SBTOR,      &m_tor,      &(player->einsatz_t)); }
void LigaEdit::OnKillfocusAusputzer() { OnCBoxValue(IDC_CAUSPUTZER,IDC_SBAUSPUTZER,&m_ausputzer,&(player->einsatz_a)); }
void LigaEdit::OnKillfocusAbwehr()    { OnCBoxValue(IDC_CABWEHR,   IDC_SBABWEHR,   &m_abwehr,   &(player->einsatz_v)); }
void LigaEdit::OnKillfocusMitte()     { OnCBoxValue(IDC_CMITTE,    IDC_SBMITTE,    &m_mitte,    &(player->einsatz_m)); }
void LigaEdit::OnKillfocusSturm()     { OnCBoxValue(IDC_CSTURM,    IDC_SBSTURM,    &m_sturm,    &(player->einsatz_s)); }
void LigaEdit::OnCBoxValue (const int IDC, const int IDSB, int * value, LONG * var)
{
  UpdateData(TRUE);
  CUnitedView::ScrBarSet(GetDlgItem(IDSB),value);
  SetCBox(IDC,*value);
  *var = *value;
  UpdateData(FALSE);
}
void LigaEdit::SetCBox (const int IDC, const int value)
{
  switch (value)
  {
    case 0 :                                     ((CButton *) GetDlgItem(IDC))->SetCheck(0); break;
    case 1 : case 2 : case 3 : case 4 : case 5 : ((CButton *) GetDlgItem(IDC))->SetCheck(2); break;
    case 6 :                                     ((CButton *) GetDlgItem(IDC))->SetCheck(1); break;
  }
  // special treatment: no goalkeeper may play in the field and vice versa.
  CheckLimits(IDC,value);
}

void LigaEdit::OnCtor()       { OnCBoxClicked(IDC_CTOR,      IDC_SBTOR,      &m_tor,      &(player->einsatz_t)); }
void LigaEdit::OnCausputzer() { OnCBoxClicked(IDC_CAUSPUTZER,IDC_SBAUSPUTZER,&m_ausputzer,&(player->einsatz_a)); }
void LigaEdit::OnCabwehr()    { OnCBoxClicked(IDC_CABWEHR,   IDC_SBABWEHR,   &m_abwehr,   &(player->einsatz_v)); }
void LigaEdit::OnCmitte()     { OnCBoxClicked(IDC_CMITTE,    IDC_SBMITTE,    &m_mitte,    &(player->einsatz_m)); }
void LigaEdit::OnCsturm()     { OnCBoxClicked(IDC_CSTURM,    IDC_SBSTURM,    &m_sturm,    &(player->einsatz_s)); }
void LigaEdit::OnCBoxClicked (const int IDC, const int IDSB, int * value, LONG * var)
{
  switch (((CButton *) GetDlgItem(IDC))->GetCheck())
  {
    case 0 : *value = 0; break;
    case 1 : *value = 6; break;
    case 2 : *value = 3; break;
  }
  CUnitedView::ScrBarSet(GetDlgItem(IDSB),value);
  *var = *value;
  // special treatment: no goalkeeper may play in the field and vice versa.
  CheckLimits(IDC,*value);
  UpdateData(FALSE);
}

void LigaEdit::CheckLimits (const int IDC, const int value)
{
  // special treatment: no goalkeeper may play in the field and vice versa.
  if (IDC == IDC_CTOR && value != 0)
  {
    m_ausputzer = m_abwehr = m_mitte = m_sturm = 0;
    UpdateData(FALSE);
    OnCBoxValue(IDC_CAUSPUTZER,IDC_SBAUSPUTZER,&m_ausputzer,&(player->einsatz_a));
    OnCBoxValue(IDC_CABWEHR,   IDC_SBABWEHR,   &m_abwehr,   &(player->einsatz_v));
    OnCBoxValue(IDC_CMITTE,    IDC_SBMITTE,    &m_mitte,    &(player->einsatz_m));
    OnCBoxValue(IDC_CSTURM,    IDC_SBSTURM,    &m_sturm,    &(player->einsatz_s));
  }
  else if (IDC != IDC_CTOR && value != 0)
  {
    m_tor = 0;
    UpdateData(FALSE);
    OnCBoxValue(IDC_CTOR,      IDC_SBTOR,      &m_tor,      &(player->einsatz_t));
  }
}
// end of player data

// handle scroll bar actions
void LigaEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  int value = pScrollBar->GetScrollPos();
  
  switch (nSBCode)
  {
    case SB_LEFT          : value -= 9;   break;
    case SB_RIGHT         : value += 9;   break;
    case SB_LINELEFT      : value -= 1;   break;
    case SB_LINERIGHT     : value += 1;   break;
    case SB_PAGELEFT      : value -= 3;   break;
    case SB_PAGERIGHT     : value += 3;   break;
    case SB_THUMBPOSITION : value = nPos; break;
    case SB_THUMBTRACK    : value = nPos; break;
    case SB_ENDSCROLL     :               break;
    default               :               break;
  }
  CUnitedView::ScrBarSet(pScrollBar,&value);
  switch (pScrollBar->GetDlgCtrlID())
  {
    case IDC_SBHOMEBONUS : team->heimvorteil = m_homebonus = value; break;
    case IDC_SBTRAININGS : team->training    = m_trainings = value; break;
    case IDC_SBSTAERKE   : player->staerke   = m_staerke   = value; break;
    case IDC_SBALTER     : player->alter     = m_alter     = value; break;
    case IDC_SBTRAINIERT : player->training  = m_trainiert = value; break;
    case IDC_SBGELB      : player->gelb      = m_gelb      = value; break;
    case IDC_SBROT       : player->rot       = m_rot       = value; break;
    case IDC_SBSPERREN   : player->sperre    = m_sperren   = value; break;
    case IDC_SBTOR       : player->einsatz_t = m_tor       = value; SetCBox(IDC_CTOR,      m_tor      ); break;
    case IDC_SBAUSPUTZER : player->einsatz_a = m_ausputzer = value; SetCBox(IDC_CAUSPUTZER,m_ausputzer); break;
    case IDC_SBABWEHR    : player->einsatz_v = m_abwehr    = value; SetCBox(IDC_CABWEHR,   m_abwehr   ); break;
    case IDC_SBMITTE     : player->einsatz_m = m_mitte     = value; SetCBox(IDC_CMITTE,    m_mitte    ); break;
    case IDC_SBSTURM     : player->einsatz_s = m_sturm     = value; SetCBox(IDC_CSTURM,    m_sturm    ); break;
    default              :                                          break;
  }                                          
  UpdateData(FALSE);
}
// end of scroll bar

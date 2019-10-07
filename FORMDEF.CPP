// formdef.cpp : implementation file
//

#include "stdafx.h"
#include "united.h"
#include "TeamA.h"
#include "Team.h"
#include "PlayerA.h"
#include "Player.h"
#include "Row.h"
#include "formdef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FormDef dialog


FormDef::FormDef(CWnd* pParent /*=NULL*/)
  : CDialog(FormDef::IDD, pParent)
{
  //{{AFX_DATA_INIT(FormDef)
  m_fteam = -1;
  m_sabwehr = "";
  m_sausputzer = "";
  m_smitte = "";
  m_ssturm = "";
  m_stor = "";
  m_ha = 0;
  m_ham = 0;
  m_has = 0;
  m_hav = 0;
  m_hb = 0;
  m_hbm = 0;
  m_hbs = 0;
  m_hbv = 0;
  //}}AFX_DATA_INIT
  arr_t = NULL;
}

void FormDef::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(FormDef)
  DDX_Control(pDX, IDC_FTOR, m_ftor);
  DDX_Control(pDX, IDC_FSTURM, m_fsturm);
  DDX_Control(pDX, IDC_FAUSPUTZER, m_fausputzer);
  DDX_Control(pDX, IDC_FABWEHR, m_fabwehr);
  DDX_Control(pDX, IDC_FMITTE, m_fmitte);
  DDX_CBIndex(pDX, IDC_FTEAM, m_fteam);
  DDX_Text(pDX, IDC_SABWEHR, m_sabwehr);
  DDX_Text(pDX, IDC_SAUSPUTZER, m_sausputzer);
  DDX_Text(pDX, IDC_SMITTE, m_smitte);
  DDX_Text(pDX, IDC_SSTURM, m_ssturm);
  DDX_Text(pDX, IDC_STOR, m_stor);
  DDX_Text(pDX, IDC_HA, m_ha);
  DDX_Text(pDX, IDC_HAM, m_ham);
  DDX_Text(pDX, IDC_HAS, m_has);
  DDX_Text(pDX, IDC_HAV, m_hav);
  DDX_Text(pDX, IDC_HB, m_hb);
  DDX_Text(pDX, IDC_HBM, m_hbm);
  DDX_Text(pDX, IDC_HBS, m_hbs);
  DDX_Text(pDX, IDC_HBV, m_hbv);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(FormDef, CDialog)
  //{{AFX_MSG_MAP(FormDef)
  ON_CBN_SELCHANGE(IDC_FTEAM, OnSelchangeFteam)
  ON_LBN_SELCHANGE(IDC_FABWEHR, OnSelchangeFabwehr)
  ON_LBN_SELCHANGE(IDC_FAUSPUTZER, OnSelchangeFausputzer)
  ON_LBN_SELCHANGE(IDC_FMITTE, OnSelchangeFmitte)
  ON_LBN_SELCHANGE(IDC_FSTURM, OnSelchangeFsturm)
  ON_LBN_SELCHANGE(IDC_FTOR, OnSelchangeFtor)
  ON_BN_CLICKED(IDC_CHECK, OnCheck)
  ON_BN_CLICKED(IDC_NEXT, OnNext)
  ON_WM_HSCROLL()
  ON_EN_KILLFOCUS(IDC_HAS, OnKillfocusHaS)
  ON_EN_KILLFOCUS(IDC_HAV, OnKillfocusHaV)
  ON_EN_KILLFOCUS(IDC_HBV, OnKillfocusHbV)
  ON_EN_KILLFOCUS(IDC_HBM, OnKillfocusHbM)
  ON_EN_KILLFOCUS(IDC_HBS, OnKillfocusHbS)
  ON_EN_KILLFOCUS(IDC_HAM, OnKillfocusHaM)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// FormDef message handlers

BOOL FormDef::OnInitDialog()
{
  static int tabstops [] = { 10, 45 };
  
  CDialog::OnInitDialog();

  teams->ListTeams((CComboBox *) GetDlgItem(IDC_FTEAM));
  m_ftor      .SetTabStops(2,tabstops);
  m_fausputzer.SetTabStops(2,tabstops);
  m_fabwehr   .SetTabStops(2,tabstops);
  m_fmitte    .SetTabStops(2,tabstops);
  m_fsturm    .SetTabStops(2,tabstops);
  TeamDisp();
  UpdateData(FALSE);
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void FormDef::OnOK()
{
  if (arr_t != NULL)
  {
    free(arr_t ); free(arr_a);  free(arr_v);  free(arr_m);  free(arr_s);  
    free(list_t); free(list_a); free(list_v); free(list_m); free(list_s); 
    free(sel);
    arr_t = NULL;
  }
  CDialog::OnOK();
}

void FormDef::OnCheck() { teams->GetTeam(m_fteam)->FormationCheck(); }

void FormDef::OnNext()
{
  UpdateData(TRUE);
  if (m_fteam < teams->teams.GetSize()-1)
  {
    m_fteam++;
    UpdateData(FALSE);
    TeamDisp(m_fteam);
  }
  else OnOK();
}

void FormDef::OnSelchangeFteam()
{
  UpdateData(TRUE);
  TeamDisp(m_fteam);
}

void FormDef::TeamDisp (const int index)
{
  int i, j;
  Team * t = teams->GetTeam(m_fteam=index);
  CObArray & players = t->players->players;
  n = players.GetSize();
  if (arr_t != NULL)
  {
    free(arr_t ); free(arr_a);  free(arr_v);  free(arr_m);  free(arr_s);  
    free(list_t); free(list_a); free(list_v); free(list_m); free(list_s); 
    free(sel);
  }
  arr_t  = (Player * *) malloc(sizeof(Player *) * n);
  arr_a  = (Player * *) malloc(sizeof(Player *) * n);
  arr_v  = (Player * *) malloc(sizeof(Player *) * n);
  arr_m  = (Player * *) malloc(sizeof(Player *) * n);
  arr_s  = (Player * *) malloc(sizeof(Player *) * n);
  list_t = (Player * *) malloc(sizeof(Player *) * n);
  list_a = (Player * *) malloc(sizeof(Player *) * n);
  list_v = (Player * *) malloc(sizeof(Player *) * n);
  list_m = (Player * *) malloc(sizeof(Player *) * n);
  list_s = (Player * *) malloc(sizeof(Player *) * n);
  sel    = (int      *) malloc(sizeof(int     ) * n);
  for (i = 0; i < n; i++)
  {
    arr_t[i] = (Player *) (players[i]);
    arr_a[i] = (Player *) (players[i]);
    arr_v[i] = (Player *) (players[i]);
    arr_m[i] = (Player *) (players[i]);
    arr_s[i] = (Player *) (players[i]);
  }
  qsort(arr_t,n,sizeof(Team *),PlayerOrderT);
  qsort(arr_a,n,sizeof(Team *),PlayerOrderA);
  qsort(arr_v,n,sizeof(Team *),PlayerOrderV);
  qsort(arr_m,n,sizeof(Team *),PlayerOrderM);
  qsort(arr_s,n,sizeof(Team *),PlayerOrderS);

  for (i = 0, j = 0; i < n; i++) arr_t[i] = list_t[i] = (arr_t[i]->MayStart(TORWART     ) ? arr_t[i] : NULL);
  for (i = 0, j = 0; i < n; i++) arr_a[i] = list_a[i] = (arr_a[i]->MayStart(AUSPUTZER   ) ? arr_a[i] : NULL);
  for (i = 0, j = 0; i < n; i++) arr_v[i] = list_v[i] = (arr_v[i]->MayStart(VERTEIDIGUNG) ? arr_v[i] : NULL);
  for (i = 0, j = 0; i < n; i++) arr_m[i] = list_m[i] = (arr_m[i]->MayStart(MITTELFELD  ) ? arr_m[i] : NULL);
  for (i = 0, j = 0; i < n; i++) arr_s[i] = list_s[i] = (arr_s[i]->MayStart(STURM       ) ? arr_s[i] : NULL);

  m_ftor      .ResetContent();
  m_fausputzer.ResetContent();
  m_fabwehr   .ResetContent();
  m_fmitte    .ResetContent();
  m_fsturm    .ResetContent();
  for (i = 0; i < n; i++)
  {
    if (list_t[i] != NULL) m_ftor      .SetItemDataPtr(m_ftor      .AddString(list_t[i]->FormationLine(TORWART)     ),list_t[i]); 
    if (list_a[i] != NULL) m_fausputzer.SetItemDataPtr(m_fausputzer.AddString(list_a[i]->FormationLine(AUSPUTZER)   ),list_a[i]);
    if (list_v[i] != NULL) m_fabwehr   .SetItemDataPtr(m_fabwehr   .AddString(list_v[i]->FormationLine(VERTEIDIGUNG)),list_v[i]);
    if (list_m[i] != NULL) m_fmitte    .SetItemDataPtr(m_fmitte    .AddString(list_m[i]->FormationLine(MITTELFELD)  ),list_m[i]);
    if (list_s[i] != NULL) m_fsturm    .SetItemDataPtr(m_fsturm    .AddString(list_s[i]->FormationLine(STURM)       ),list_s[i]);
  }

  for (i = 0, j = 0; i < n; i++)
    if (list_t[i] != NULL)
    {
      if      (list_t[i]->eingesetzt == TORWART) m_ftor.SetSel(j++);
      else if (list_t[i]->eingesetzt != NONE)  { m_ftor.DeleteString(j); arr_t[i] = NULL; }
      else     j++;
    }
  for (i = 0, j = 0; i < n; i++)
    if (list_a[i] != NULL)
    {
      if      (list_a[i]->eingesetzt == AUSPUTZER) m_fausputzer.SetSel(j++);
      else if (list_a[i]->eingesetzt != NONE)    { m_fausputzer.DeleteString(j); arr_a[i] = NULL; }
      else     j++;
    }
  for (i = 0, j = 0; i < n; i++)
    if (list_v[i] != NULL)
    {
      if      (list_v[i]->eingesetzt == VERTEIDIGUNG) m_fabwehr.SetSel(j++);
      else if (list_v[i]->eingesetzt != NONE)       { m_fabwehr.DeleteString(j); arr_v[i] = NULL; }
      else     j++;
    }
  for (i = 0, j = 0; i < n; i++)
    if (list_m[i] != NULL)
    {
      if      (list_m[i]->eingesetzt == MITTELFELD) m_fmitte.SetSel(j++);
      else if (list_m[i]->eingesetzt != NONE)     { m_fmitte.DeleteString(j); arr_m[i] = NULL; }
      else     j++;
    }
  for (i = 0, j = 0; i < n; i++)
    if (list_s[i] != NULL)
    {
      if      (list_s[i]->eingesetzt == STURM) m_fsturm.SetSel(j++);
      else if (list_s[i]->eingesetzt != NONE){ m_fsturm.DeleteString(j); arr_s[i] = NULL; }
      else     j++;
    }

  m_hb  = (int) t->heimvorteil;
  m_hbv = (int) t->hb[2];
  m_hbm = (int) t->hb[3];
  m_hbs = (int) t->hb[4];
  m_hav = (int) t->ha[2];
  m_ham = (int) t->ha[3];
  m_has = (int) t->ha[4];
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHBV),0,m_hb,&m_hbv);
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHBM),0,m_hb,&m_hbm);
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHBS),0,m_hb,&m_hbs);
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHAV),0,10,&m_hav);
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHAM),0,10,&m_ham);
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHAS),0,10,&m_has);
  ShowSums();
}

void FormDef::OnSelchangeFtor()      { PlayerSelected(&m_ftor      ,TORWART);     }
void FormDef::OnSelchangeFausputzer(){ PlayerSelected(&m_fausputzer,AUSPUTZER);   }
void FormDef::OnSelchangeFabwehr()   { PlayerSelected(&m_fabwehr   ,VERTEIDIGUNG);}
void FormDef::OnSelchangeFmitte()    { PlayerSelected(&m_fmitte    ,MITTELFELD);  }
void FormDef::OnSelchangeFsturm()    { PlayerSelected(&m_fsturm    ,STURM);       }

void FormDef::PlayerSelected (CListBox * m, ROW row)
{
  int i = m->GetCaretIndex();
  Player * p = (Player *) m->GetItemDataPtr(i);
  int flag = m->GetSel(i);

  p->eingesetzt = (flag ? row : NONE);
  if (row != TORWART)      PlayerToggle(p,flag,arr_t,list_t,&m_ftor,      TORWART);
  if (row != AUSPUTZER)    PlayerToggle(p,flag,arr_a,list_a,&m_fausputzer,AUSPUTZER);
  if (row != VERTEIDIGUNG) PlayerToggle(p,flag,arr_v,list_v,&m_fabwehr,   VERTEIDIGUNG);
  if (row != MITTELFELD)   PlayerToggle(p,flag,arr_m,list_m,&m_fmitte,    MITTELFELD);
  if (row != STURM)        PlayerToggle(p,flag,arr_s,list_s,&m_fsturm,    STURM);
  ShowSums();
}

// calculate sums for all rows:
void FormDef::ShowSums ()
{
  char buffer [3];
  sprintf(buffer,"%d",            SumStrength(&m_ftor,      TORWART));      m_stor       = buffer;
  sprintf(buffer,"%d",            SumStrength(&m_fausputzer,AUSPUTZER));    m_sausputzer = buffer;
  sprintf(buffer,"%d",m_hbv+m_hav+SumStrength(&m_fabwehr,   VERTEIDIGUNG)); m_sabwehr    = buffer;
  sprintf(buffer,"%d",m_hbm+m_ham+SumStrength(&m_fmitte,    MITTELFELD));   m_smitte     = buffer;
  sprintf(buffer,"%d",m_hbs+m_has+SumStrength(&m_fsturm,    STURM));        m_ssturm     = buffer;
  m_ha = m_hav + m_ham + m_has;
  UpdateData(FALSE);
}

int FormDef::SumStrength (CListBox * m, const ROW r) const
{
  int z = m->GetSelItems(n,sel), s = 0;
  for (int i = 0; i < z; i++) s += ((Player *) m->GetItemDataPtr(sel[i]))->StrengthAt(r);
  return s;
}

void FormDef::PlayerToggle (Player * p, int flag, Player * * arr, Player * * list, CListBox * m, ROW r)
{
  int i, j;
  for (i = 0, j = 0; i < n; i++)
    if ((flag ? list[i] : arr[i]) == p)
    {
      list[i] = (flag ? NULL : p);
      if (flag) m->DeleteString(j);
      else      m->SetItemDataPtr(m->InsertString(j,p->FormationLine(r)),p);
      break;
    }
    else if (list[i] != NULL) j++;
}

extern "C" {
int PlayerOrderT (const void * e1, const void * e2) { return (*(Player * *) e2)->EstimateAt(TORWART)      - (*(Player * *) e1)->EstimateAt(TORWART);      }
int PlayerOrderA (const void * e1, const void * e2) { return (*(Player * *) e2)->EstimateAt(AUSPUTZER)    - (*(Player * *) e1)->EstimateAt(AUSPUTZER);    }
int PlayerOrderV (const void * e1, const void * e2) { return (*(Player * *) e2)->EstimateAt(VERTEIDIGUNG) - (*(Player * *) e1)->EstimateAt(VERTEIDIGUNG); }
int PlayerOrderM (const void * e1, const void * e2) { return (*(Player * *) e2)->EstimateAt(MITTELFELD)   - (*(Player * *) e1)->EstimateAt(MITTELFELD);   }
int PlayerOrderS (const void * e1, const void * e2) { return (*(Player * *) e2)->EstimateAt(STURM)        - (*(Player * *) e1)->EstimateAt(STURM);        }
}

void FormDef::OnKillfocusHaV() { RoughChanged(IDC_SBHAV,&m_hav); }
void FormDef::OnKillfocusHaM() { RoughChanged(IDC_SBHAM,&m_ham); }
void FormDef::OnKillfocusHaS() { RoughChanged(IDC_SBHAS,&m_has); }
void FormDef::RoughChanged  (int ID, int * m)
{
  UpdateData(TRUE);
  if (m_hav + m_ham + m_has > 10) *m -= m_ham + m_has + m_hav - 10;
  CUnitedView::ScrBarSet(GetDlgItem(ID),m);
  switch (ID)
  {
    case IDC_SBHAV : teams->GetTeam(m_fteam)->ha[2] = *m; break;
    case IDC_SBHAM : teams->GetTeam(m_fteam)->ha[3] = *m; break;
    case IDC_SBHAS : teams->GetTeam(m_fteam)->ha[4] = *m; break;
  }
  ShowSums();
}
void FormDef::OnKillfocusHbV() { HomeBonusChanged(IDC_SBHBV,&m_hbv); }
void FormDef::OnKillfocusHbM() { HomeBonusChanged(IDC_SBHBM,&m_hbm); }
void FormDef::OnKillfocusHbS() { HomeBonusChanged(IDC_SBHBS,&m_hbs); }
void FormDef::HomeBonusChanged (int ID, int * m)
{
  UpdateData(TRUE);
  if (m_hbv+m_hbm+m_hbs > m_hb) *m -= m_hbm + m_hbs + m_hbv - m_hb;
  CUnitedView::ScrBarSet(GetDlgItem(ID),m);
  switch (ID)
  {
    case IDC_SBHBV : teams->GetTeam(m_fteam)->hb[2] = *m; break;
    case IDC_SBHBM : teams->GetTeam(m_fteam)->hb[3] = *m; break;
    case IDC_SBHBS : teams->GetTeam(m_fteam)->hb[4] = *m; break;
  }
  ShowSums();
}

// handle scroll bar actions
void FormDef::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  int value = pScrollBar->GetScrollPos();
  int old = value, d, z;
  
  switch (nSBCode)
  {
    case SB_LEFT          : d = -9;         break;
    case SB_RIGHT         : d =  9;         break;
    case SB_LINELEFT      : d = -1;         break;
    case SB_LINERIGHT     : d =  1;         break;
    case SB_PAGELEFT      : d = -3;         break;
    case SB_PAGERIGHT     : d =  3;         break;
    case SB_THUMBPOSITION : d = nPos - old; break;
    case SB_THUMBTRACK    : d = nPos - old; break;
    case SB_ENDSCROLL     : d =  0;         break;
    default               :                 break;
  }
  
  value += d;
  //CUnitedView::ScrBarSet(pScrollBar,&value);
  d = value - old;

  switch (pScrollBar->GetDlgCtrlID())
  {
    case IDC_SBHBV : teams->GetTeam(m_fteam)->hb[2] = m_hbv = value; break;
    case IDC_SBHBM : teams->GetTeam(m_fteam)->hb[3] = m_hbm = value; break;
    case IDC_SBHBS : teams->GetTeam(m_fteam)->hb[4] = m_hbs = value; break;
    case IDC_SBHAV : teams->GetTeam(m_fteam)->ha[2] = m_hav = value; break;
    case IDC_SBHAM : teams->GetTeam(m_fteam)->ha[3] = m_ham = value; break;
    case IDC_SBHAS : teams->GetTeam(m_fteam)->ha[4] = m_has = value; break;
    default        :                                                 break;
  }                                          

  switch (pScrollBar->GetDlgCtrlID())
  {
    case IDC_SBHBV : case IDC_SBHBM : case IDC_SBHBS :
      z = m_hb - (m_hbv + m_hbm + m_hbs);
      CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHBV),0,m_hbv+z,&m_hbv);
      CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHBM),0,m_hbm+z,&m_hbm);
      CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHBS),0,m_hbs+z,&m_hbs);
      break;
    case IDC_SBHAV : case IDC_SBHAM : case IDC_SBHAS :
      z = 10 - (m_hav + m_ham + m_has);
      CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHAV),0,m_hav+z,&m_hav);
      CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHAM),0,m_ham+z,&m_ham);
      CUnitedView::ScrBarInit(GetDlgItem(IDC_SBHAS),0,m_has+z,&m_has);
      break;
  }
  ShowSums();
}
// end of scroll bar

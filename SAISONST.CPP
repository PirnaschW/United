// saisonst.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "united.h"
#include "TeamA.h"
#include "Liga.h"
#include "saisonst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SaisonStart dialog


SaisonStart::SaisonStart(CWnd* pParent /*=NULL*/)
  : CDialog(SaisonStart::IDD, pParent)
{
  //{{AFX_DATA_INIT(SaisonStart)
  m_robin_times = 1;
  m_groups_size = 2;
  m_swiss_rounds = 2;
  //}}AFX_DATA_INIT
}

void SaisonStart::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(SaisonStart)
  DDX_Text(pDX, IDC_ROBIN_TIMES, m_robin_times);
  DDX_Text(pDX, IDC_GROUPS_SIZE, m_groups_size);
  DDX_Text(pDX, IDC_SWISS_ROUNDS, m_swiss_rounds);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SaisonStart, CDialog)
  //{{AFX_MSG_MAP(SaisonStart)
  ON_BN_CLICKED(IDC_SWISS, OnSwiss)
  ON_BN_CLICKED(IDC_ROBIN, OnRobin)
  ON_BN_CLICKED(IDC_GROUPS, OnGroups)
  ON_EN_KILLFOCUS(IDC_GROUPS_SIZE, OnKillfocusGroupsSize)
  ON_EN_KILLFOCUS(IDC_ROBIN_TIMES, OnKillfocusRobinTimes)
  ON_EN_KILLFOCUS(IDC_SWISS_ROUNDS, OnKillfocusSwissRounds)
  ON_WM_HSCROLL()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SaisonStart message handlers

BOOL SaisonStart::OnInitDialog()
{
  int n;
  CDialog::OnInitDialog();

  n = liga->teams->teams.GetSize();
  m_robin_times  = 40 / (2+n);
  CUnitedView::ScrBarInit(GetDlgItem(IDC_SBROBIN), 1,20,&m_robin_times);
  ((CButton *) GetDlgItem(IDC_ROBIN))->SetCheck(1);
  EnableWindows(TRUE, FALSE,FALSE);

  if (n < 4) GetDlgItem(IDC_GROUPS)->EnableWindow(FALSE);
  else
  {
    m_groups_size  = (int) (sqrt((double) n) + 0.8);
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBGROUPS),2,(n+1)/2,&m_groups_size);
  }

  if (n < 4) GetDlgItem(IDC_SWISS)->EnableWindow(FALSE);
  else
  {
    m_swiss_rounds = (int) (log((double) n) / log (1.6));
    CUnitedView::ScrBarInit(GetDlgItem(IDC_SBSWISS), 2,n-m_swiss_rounds,&m_swiss_rounds);
  }
  UpdateData(FALSE);
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void SaisonStart::OnRobin () { EnableWindows(TRUE, FALSE,FALSE); }
void SaisonStart::OnGroups() { EnableWindows(FALSE,TRUE, FALSE); }
void SaisonStart::OnSwiss () { EnableWindows(FALSE,FALSE,TRUE ); }
void SaisonStart::EnableWindows (const BOOL robin, const BOOL groups, const BOOL swiss)
{
  GetDlgItem(IDC_ROBIN_TIMES) ->EnableWindow(robin);
  GetDlgItem(IDS_ROBIN)       ->EnableWindow(robin);
  GetDlgItem(IDC_SBROBIN)     ->EnableWindow(robin);
  GetDlgItem(IDC_GROUPS_SIZE) ->EnableWindow(groups);
  GetDlgItem(IDC_SBGROUPS)    ->EnableWindow(groups);
  GetDlgItem(IDC_SWISS_ROUNDS)->EnableWindow(swiss);
  GetDlgItem(IDS_SWISS)       ->EnableWindow(swiss);
  GetDlgItem(IDC_SBSWISS)     ->EnableWindow(swiss);
}

void SaisonStart::OnKillfocusRobinTimes () { UpdateData(TRUE); CUnitedView::ScrBarSet((CScrollBar *) GetDlgItem(IDC_SBROBIN), &m_robin_times ); }
void SaisonStart::OnKillfocusGroupsSize () { UpdateData(TRUE); CUnitedView::ScrBarSet((CScrollBar *) GetDlgItem(IDC_SBGROUPS),&m_groups_size ); }
void SaisonStart::OnKillfocusSwissRounds() { UpdateData(TRUE); CUnitedView::ScrBarSet((CScrollBar *) GetDlgItem(IDC_SBSWISS), &m_swiss_rounds); }
void SaisonStart::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
    case IDC_SBROBIN     : m_robin_times  = value; break;
    case IDC_SBGROUPS    : m_groups_size  = value; break;
    case IDC_SBSWISS     : m_swiss_rounds = value; break;
    default              :                         break;
  }                                          
  UpdateData(FALSE);
}

void SaisonStart::OnOK()
{
  if      (((CButton *) GetDlgItem(IDC_ROBIN ))->GetCheck() == 1) { liga->modus = MODUS_ROBIN; liga->modus_param = m_robin_times; }
  else if (((CButton *) GetDlgItem(IDC_GROUPS))->GetCheck() == 1) { liga->modus = MODUS_GROUP; liga->modus_param = m_groups_size; }
  else if (((CButton *) GetDlgItem(IDC_SWISS ))->GetCheck() == 1) { liga->modus = MODUS_SWISS; liga->modus_param = m_swiss_rounds; }
  else                                                            { liga->modus = MODUS_UNDEF; liga->modus_param = 0; }
  CDialog::OnOK();
}

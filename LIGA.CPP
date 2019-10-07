#include "stdafx.h"
#include "Liga.h"
#include "Pair.h"
#include "Player.h"
#include "PlayerA.h"
#include "Team.h"
#include "TeamA.h"
#include "NewName.h"
#include "LigaEdit.h"
#include "SaisonSt.h"
#include "FormDef.h"

IMPLEMENT_SERIAL(Liga,CObject,1);

extern "C" static int ChartCompare (const void * e1, const void * e2);

Liga::Liga ()
{
  teams = new TeamArray;
  modus = MODUS_UNDEF;
  modus_param = 0;
  round = 0;
  round_active = FALSE;
}

Liga::Liga (CString liganame)
{
  name = liganame;
  teams = new TeamArray;
  modus = MODUS_UNDEF;
  modus_param = 0;
  round = 0;
  round_active = FALSE;
}

Liga::~Liga ()
{
  delete teams;
}

Liga * Liga::LigaNew (const BOOL getname)
{
  if (getname)
  {
    NewName dlg;
    dlg.check = NULL;
    if (dlg.DoModal() == TRUE) return new Liga(dlg.m_name);
    else                       return NULL;
  }
  else return new Liga();
}

void Liga::Serialize (CArchive & ar)
{
  CObject::Serialize(ar);
  BYTE dummy;
  CString version = "V0.03", found;
  if (ar.IsStoring())
  {
    ar << version;
    ar << name;
    ar << master;
    ar << teams;
    ar << (BYTE) modus;
    ar << modus_param;
    ar << round;
    ar << (BYTE) round_active;
    ar << (BYTE) counter;
  }
  else
  {
    ar >> found; if (found != version) return;
    ar >> name;
    ar >> master;
    ar >> teams;
    ar >> dummy; modus = (Modus) dummy;
    ar >> modus_param;
    ar >> round;
    ar >> dummy; round_active = (dummy ? TRUE : FALSE);
    ar >> dummy; counter = dummy;
  }
}

BOOL Liga::HasTeams () const
{
  return teams->TeamsExist();
}

int  Liga::GetTeamsSize() const
{
  return teams->teams.GetSize();
}

void Liga::EditData ()
{
  LigaEdit dlg;
  dlg.liga = this;
  dlg.DoModal();
}

void Liga::SeasonStart ()
{
  // first, check all prerequisites for the season:
  int n = GetTeamsSize();
  if (n < 2)
  {
    AfxMessageBox("Mindestens zwei Teams müssen teilnehmen",MB_OK);
    return;
  }
  for (int i=0; i<n; i++)
  {
    Team * t = (Team *) teams->teams[i];
    int m = t->players->players.GetSize(), nt = 0;
    if (m < 11)
    {
      char buffer [28+32+6];
      sprintf(buffer,"Das Team %s hat nur %d Spieler",(const char *) t->name,m);
      AfxMessageBox(buffer,MB_OK);
      return;
    }
    for (int j=0; j<m; j++) if (((Player *) (t->players->players[j]))->einsatz_t > 0) nt++;
    if (nt < 1)
    {
      AfxMessageBox("Das Team " + t->name + " hat keinen Torwart",MB_OK);
      return;
    }
  }

  // now, warn the user that there is no way back:
  if (AfxMessageBox("Nach dem Saisonstart können die Mannschaften und Spieler nicht mehr geändert werden. Saison starten ?",MB_OKCANCEL) != IDOK) return;

  // finally, start the season.
  SaisonStart dlg;
  dlg.liga = this;
  if (dlg.DoModal() != IDOK) return;
  teams->ResetTabelle();
  teams->RandomSequence();
}

void Liga::RoundBegin ()
{
  int n = GetTeamsSize();

  round++;
  round_active = TRUE;
  for (int i=0; i<n; i++)
  {
    Team * t = (Team *) (teams->teams[i]);
    for (int r=0; r<5; r++) t->ha[r] = t->hb[r] = 0;
    CObArray & p = t->players->players;
    int m = p.GetSize();
    for (int j=0; j<m; j++)
    {
      Player * p0 = (Player *) (p[j]);
      p0->eingesetzt = NONE;
      if (p0->sperre > 0) p0->sperre--;
    }
  }
  pair = NULL;
  counter = (n+1)/2;
}

void Liga::RoundEnd ()
{
  // check if all teams have defined a correct formation:
  int n = GetTeamsSize();
  for (int i=0; i<n; i++) if (! ((Team *) teams->teams[i])->FormationCheck()) return;

  if (AfxMessageBox("Alle Mannschaften sind jetzt spielbereit. Spiele jetzt durchführen ?",MB_OKCANCEL) != IDOK) return;

  while ((pair = PairGet(n,counter--)) != NULL)
  {
    pair->PlayMatch();
    delete pair;
  }

  AfxMessageBox("Spieltag beendet",MB_OK);
  round_active = FALSE;
  
  if (round >= modus_param)
  {
    AfxMessageBox("Die Saison ist beendet. Alle Spieler altern jetzt.",MB_OK);
    modus = MODUS_UNDEF;
    for (int i=0; i<n; i++)
    {
      Team * t = (Team *) (teams->teams[i]);
      CObArray & p = t->players->players;
      int m = p.GetSize();
      for (int j=m-1; j>=0; j--)
      {
        Player * p0 = (Player *) (p[j]);
        p0->Aging();
        p0->sperre = 0;
        if (p0->staerke < 1) t->players->DelPlayer(j);
      }
    }
  }
}


Pair * Liga::PairGet (int count, int counter)
{
  int i1, i2, n, r;

  switch (modus)
  {
    case MODUS_ROBIN :
      if (counter > 0)
      {
        n = (count + 1) / 2 * 2;
        r = ((n / 2 * (round - 1)) % (n - 1)) + 1;
        i1 = r+counter;
        i2 = r-counter;
        if (i1 > n-1) i1 -= n-1;
        if (i2 <   1) i2 += n-1;
        if (i1 == i2)
        {
          if (n != count) return NULL;
          else {
            if (r > count/2) i1 = n;
            else             i2 = n;
          }
        }
        return new Pair((Team*) (teams->teams[i1-1]),(Team*) (teams->teams[i2-1]));
      }
      return NULL;
    case MODUS_GROUP : return new Pair((Team*) (teams->teams[0]),(Team*) (teams->teams[1]));
    case MODUS_SWISS : return new Pair((Team*) (teams->teams[0]),(Team*) (teams->teams[1]));
    default          : return NULL;
  }
}


char * Liga::Hint ()
{
  if (GetTeamsSize() < 2) return "Nächster Schritt: Mindestens 2 Mannschaften vollständig definieren";
  else if (modus==0)      return "Nächster Schritt: Saison/Starten";
  else if (!round_active) return "Nächster Schritt: Saison/Spieltag beginnen";
  else if ( round_active) return "Nächster Schritt: Aufstellung/Festlegen";
  else                    return "";
}

int Liga::ChartShow (CDC * pDC)
{
  char buffer [80];
  int z, n = GetTeamsSize(), i;
  Team * * arr = (Team * *) malloc(sizeof(Team * *) * n);

  CFont * oldfont = (CFont *) pDC->SelectStockObject(ANSI_FIXED_FONT);
//  pDC->TextOut(1,1,buffer,pDC->GetTextFace(sizeof(buffer)-1,buffer));
  pDC->TextOut(10,10,(CString) "Platz  Team                              Punkte      Tore   Dif.");

  for (i = 0; i < n; i++) arr[i] = (Team *) (teams->teams[i]);
  qsort(arr,n,sizeof(Team *),ChartCompare);
  
  for (i = 0; i < n; i++)
  {
    z = sprintf(buffer,"%3d  %-32s   %3ld :%3ld   %3ld :%3ld %+3ld",
                i+1,
                (const char *) (arr[i]->name),
                arr[i]->punkte[0],
                arr[i]->punkte[1],
                arr[i]->tore[0],
                arr[i]->tore[1],
                arr[i]->tore[0] - arr[i]->tore[1]);
    pDC->TextOut(10,30+15*i,buffer,z);
  }
  free(arr);
  pDC->SelectObject(oldfont);
  return 15*(n+1);
}

extern "C" static int ChartCompare (const void * e1, const void * e2)
{
  Team * m1 = *((Team * *) e1), * m2 = *((Team * *) e2);
  int i;
  LONG p1 [2], p2 [2], t1 [2], t2 [2], d [2];

  for (i=0; i<2; i++)
  {
    p1[i] = m1->punkte[i];
    p2[i] = m2->punkte[i];
    t1[i] = m1->tore[i];
    t2[i] = m2->tore[i];
  }
  d[0] = m1->tore[0] - m1->tore[1];
  d[1] = m2->tore[0] - m2->tore[1];

  if (p1[0] > p2[0]) return -1;
  if (p1[0] < p2[0]) return  1;
  if (p1[1] > p2[1]) return  1;
  if (p1[1] < p2[1]) return -1;

  if (d [0] > d [1]) return -1;
  if (d [0] < d [1]) return  1;
  if (t1[0] > t2[0]) return -1;
  if (t1[0] < t2[0]) return  1;

  return (m1->name < m2->name);
}

void Liga::FormationDefine ()
{
  FormDef dlg;
  dlg.teams = teams;
  dlg.DoModal();
}

#ifdef NEVER
void Liga::Altern () const
{
  for (Mannschaft * m = first; m != NULL; m = m->Next()) m->Altern();
}

void Liga::CreateTalente () const
{
  for (Mannschaft * m = first; m != NULL; m = m->Next()) m->CreateTalente();
}

void Liga::PlayRound ()
{
  int i, p;

  for (i=0,p=1; i<modus; i++,p*=2);
  if ((mannschaften+1)/2*2*p == ++(runde))
  {
    runde = -1;
    message("Die Saison ist zu Ende.\nAlle Spieler altern jetzt ein Jahr.",2);
    Altern();
  }
}
#endif

#include "stdafx.h"
#include "Team.h"
#include "PlayerA.h"
#include "Player.h"
#include "Row.h"

IMPLEMENT_SERIAL(Team,CObject,1);

static int center_line (char * line, int max, char * buffer);

Team::Team ()
{
  heimvorteil = 6;
  punkte[0] = punkte[1] = tore[0] = tore[1] = 0;
  for (int i=0; i<5; i++) ha[i] = hb[i] = 0;
  players = new PlayerArray;
  preferred = new Row;
}

Team::Team (CString team_name)
{
  name = team_name;
  heimvorteil = 6;
  punkte[0] = punkte[1] = tore[0] = tore[1] = 0;
  for (int i=0; i<5; i++) ha[i] = hb[i] = 0;
  players = new PlayerArray;
  preferred = new Row;
}

Team::~Team ()
{
  delete players;
  delete preferred;
}

void Team::Serialize (CArchive & ar)
{
  int i;
  CObject::Serialize(ar);
  players->Serialize(ar);
  if( ar.IsStoring())
  {
    ar << name;
    ar << trainer;
    ar << training;
    ar << heimvorteil;
    ar << punkte[0];
    ar << punkte[1];
    ar << tore[0];
    ar << tore[1];
    ar << (BYTE) human;
    for (i=0; i<5; i++) ar << hb[i];
    for (i=0; i<5; i++) ar << ha[i];
  }
  else
  {
    BYTE dummy;
    ar >> name;
    ar >> trainer;
    ar >> training;
    ar >> heimvorteil;
    ar >> punkte[0];
    ar >> punkte[1];
    ar >> tore[0];
    ar >> tore[1];
    ar >> dummy; human = (dummy != 0);
    for (i=0; i<5; i++) ar >> hb[i];
    for (i=0; i<5; i++) ar >> ha[i];
  }
  preferred->Serialize(ar);
}

BOOL Team::HasPlayers   () const { return players->PlayersExist(); }
void Team::ResetTabelle ()       { punkte[0] = punkte[1] = tore[0] = tore[1] = 0; }
BOOL Team::FormationCheck () const
{
  // check formation and give message about 3:1 ratio
  int nt = 0, na = 0, nv = 0, nm = 0, ns = 0;
  int st = RowSum(TORWART,     &nt) + (int) ha[0],
      sa = RowSum(AUSPUTZER,   &na) + (int) ha[1],
      sv = RowSum(VERTEIDIGUNG,&nv) + (int) ha[2],
      sm = RowSum(MITTELFELD,  &nm) + (int) ha[3],
      ss = RowSum(STURM,       &ns) + (int) ha[4];
  unsigned err = 0x0000;

  switch (nt)
  {
    case  0 : err |= 0x0001; break;
    case  1 :                break;
    default : err |= 0x0002; break;
  }
  switch (na)
  {
    case  0 :                break;
    case  1 :                break;
    default : err |= 0x0004; break;
  }
  switch (nv)
  {
    case  0 :
    case  1 : err |= 0x0008; break;
    case  2 :
    case  3 :
    case  4 :
    case  5 :
    case  6 :                break;
    default : err |= 0x0010; break;
  }
  switch (nm)
  {
    case  0 :
    case  1 : err |= 0x0020; break;
    case  2 :
    case  3 :
    case  4 :
    case  5 :
    case  6 :                break;
    default : err |= 0x0040; break;
  }
  switch (ns)
  {
    case  0 :
    case  1 : err |= 0x0080; break;
    case  2 :
    case  3 :
    case  4 :
    case  5 :
    case  6 :                break;
    default : err |= 0x0100; break;
  }
  if (nt + na + nv + nm + ns > 11) err |= 0x0200;
  if (nt + na + nv + nm + ns < 11) err |= 0x0400;
  if (sv > 3 * sm || sv > 3 * ss || sm > 3 * sv || sm > 3 * ss || ss > 3 * sv || ss > 3 * sm) err |= 0x0800;
  st += (int) hb[0];
  sa += (int) hb[1];
  sv += (int) hb[2];
  sm += (int) hb[3];
  ss += (int) hb[4];
  if (sv > 3 * sm || sv > 3 * ss || sm > 3 * sv || sm > 3 * ss || ss > 3 * sv || ss > 3 * sm) err |= 0x1000;

  static char * messages [] = {
    "Es muß ein Torwart aufgestellt werden",
    "Es darf nur ein Torwart aufgestellt werden",
    "Es darf nur ein Ausputzer aufgestellt werden",
    "Es müssen mindestens zwei Verteidiger aufgestellt werden",
    "Es dürfen höchstens sechs Verteidiger aufgestellt werden",
    "Es müssen mindestens zwei Mittelfeldspieler aufgestellt werden",
    "Es dürfen höchstens sechs Mittelfeldspieler aufgestellt werden",
    "Es müssen mindestens zwei Stürmer aufgestellt werden",
    "Es dürfen höchstens sechs Stürmer aufgestellt werden",
    "Es dürfen nur elf Spieler aufgestellt werden",
    "Es müssen elf Spieler aufgestellt werden. Die Mannschaft wird mit Amateuren aufgefüllt",
    "Das 3:1-Verhältnis ist vor Zuordnung des Heimvorteils nicht eingehalten",
    "Das 3:1-Verhältnis ist nach Zuordnung des Heimvorteils nicht eingehalten",
  };

  char buffer [600];
  int off = 0;
  off = sprintf(buffer,"Die Aufstellung von Mannschaft %s ist nicht korrekt: ",(const char *) name);
  for (int i=0; i<sizeof(messages)/sizeof(messages[0]); i++)
    if (err & (1<<i)) off += sprintf(buffer+off,"\n%s.",messages[i]);
  if (err) AfxMessageBox(buffer,MB_OK);
  return (err | 0x0400) != 0x0400 ? FALSE : TRUE;
}

int  Team::RowSum (const ROW r, int * count) const
{
  CObArray & p = players->players;
  int n = p.GetSize(), sum = 0;
  for (int i=0; i<n; i++)
    if (((Player *) p[i])->eingesetzt == r)
    {
      if (count != NULL) (*count)++;
      sum += ((Player *) p[i])->StrengthAt(r);
    }
  return sum;
}

void Team::HomeBonusAdjust (const int p)
{
  heimvorteil += p-1;
  if (heimvorteil <  6) heimvorteil =  6;
  if (heimvorteil > 20) heimvorteil = 20;
}

void Team::SkillsIncrease ()
{
  int n = players->players.GetSize();
  for (int i=0; i<n; i++) ((Player *) (players->players[i]))->SkillIncrease();
}

#ifdef NEVER
Player * Team::GetPlayer (const int n) const
{
  return (Player *) player[n];
}

Player * Team::GetPlayer (const char * name) const
{
  return (Player *) player[GetPlayerIndex(name)];
}

int Team::GetPlayerIndex (const char * name) const
{
  for (int i = 0; i < players->GetSize(); i++)
    if (! strcmp(((Player *) players[i])->GetName(),name)) return i;
  return -1;
}

void Team::AddPlayer (const char * name)
{
  players->Add(new Player(name));
}

void Team::DeletePlayer (const char * name)
{
  int i = GetPlayerIndex(name);
  delete players[i];
  players->RemoveAt(i);
}

void Team::Show () const
{
  int off, n = 0, max = 76, sums = 0, suma = 0, sumt = 0, sumd = 0, anz [5], i;
  long sumw = 0L;
  char line [80], buffer [2500], c [5];
  Player * s;

  sprintf(line,"Team  %s",name);
  off = center_line(line,max,buffer);
  off += sprintf(buffer+off,"\n\n");

  sprintf(line,"Trainer : %s",trainer);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n");

  sprintf(line,"Heimvorteil : %4.1lg",(double)heimvorteil*0.5);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n");

  off += sprintf(buffer+off,"\n%5s%-27s Stamm St„ Al- trai- DP S eingespielt  Wert","","Name");
  off += sprintf(buffer+off,"\n%5s%-27s  pos. rke ter niert       T A V M S    kDM","","");
  for (i=0; i<5; i++) anz[i] = 0;
  for (n = 0, s = first; s != NULL; s = s->Next(), n++)
  {
    off += s->Show(buffer);
    sums += s->GetStaerke() * s->Faktor();
    suma += s->GetAlter();
    sumt += s->GetTrainiert();
    sumd += s->GetDisziplinar();
    sumw += s->GetWert() * s->Faktor();
    for (i=0; i<5; i++) if (c[i] == '+') anz[i]++;
  }
  off += sprintf(buffer+off,"\n%5sGesamt: %3d Player%8s       %3d %3.1lf %3d  %3d   %2d%2d%2d%2d%2d %6d",
                 "",n,"",sums,(double)suma/(double)(n>0?n:1),sumt,sumd,
                 anz[0],anz[1],anz[2],anz[3],anz[4],sumw);

  message(buffer,n+8);

  sprintf(line,"buffer hatte %d chars",strlen(buffer));
  message(line,1);
}

void Team::Reset () const
{
  Player * s;

  for (s = first; s != NULL; s = s->Next()) s->Reset();
}


BOOL Team::IsValid () const
{
  int nt = 0, n = 0;
  char buffer [80];
  Player * s;

  for (s = first; s != NULL; s = s->Next())
  {
    n++;
    if (s->GetReihe().Get() == TORWART) nt++;
  }

  if (n < 11)
  {
    sprintf(buffer,"Team %s hat nur %d Player.",name,n);
    message(buffer,1);
    return(FALSE);
  }

  if (nt < 1)
  {
    sprintf(buffer,"Team %s hat keinen Torwart.",name);
    message(buffer,1);
    return(FALSE);
  }

  return(TRUE);
}


#ifdef NIEMALSNICH
int Team::PlayerListBuild (const Reihe & r, char * * * nlist, Player * * * slist) const
{
  int i = 0;
  Player * s;

  for (s = first; s != NULL; s = s->Next()) i++;
  if (i == 0) return(0);

  *nlist = (char * *) malloc(sizeof(char *) * (i+1));
  *slist = (Player * *) malloc(sizeof(Player *) * (i+1));

  (*nlist)[0] = (char *) malloc(5);
  strcpy((*nlist)[0],"Ende");
  (*slist)[0] = NULL;
  i = 1;
  /* 1. Uneingespielte Talente */
  for (s = first; r.Get() != ALLE && s != NULL; s = s->Next())
  {
    if (s->Einsetzbar() && s->UneingespieltesTalent(r))
      (*nlist)[i] = s->Zeile(r);
      (*slist)[i++] = s;
  }
  /* 2. Eingespielte Player */
  for (s = first; r.Get() != ALLE && s != NULL; s = s->Next())
  {
    if (s->Einsetzbar() && s->Eingespielt(r))
      (*nlist)[i] = s->Zeile(r);
      (*slist)[i++] = s;
  }
  /* 3. Uneingespielte Player */
  for (s = first; r.Get() != ALLE && s != NULL; s = s->Next())
  {
    if (s->Einsetzbar() && s->Uneingespielt(r))
      (*nlist)[i] = s->Zeile(r);
      (*slist)[i++] = s;
  }
  /* 4. Alle Player */
  for (s = first; r.Get() == ALLE && s != NULL; s = s->Next())
  {
    (*nlist)[i] = s->Zeile(r);
    (*slist)[i++] = s;
  }
  return(i);
}
#endif

void Team::Detach () const
{
  Player * s;

  for (s = first; s != NULL; s = s->Next()) s->Detach();
}

static int center_line (char * line, int max, char * buffer)
{
  int z;

  z = (max - strlen(line)) / 2;
  return(sprintf(buffer,"%*s%-*s",z,"",max-z,line));
}

Player * Team::BesterPlayer (const Reihe & r) const
{
  int       e,   e0 = 0;
  Player * s, * s0 = NULL;

  for (s = first; s != NULL; s = s->Next())
    if ((e = s->EinsatzWert(r)) > e0)
    {
      s0 = s;
      e0 = e;
    }
  if (s0 != NULL) s0->SetEingesetzt(1);
  return(s0);
}

Player * Team::BestTraining (const BOOL alle) const
{
  Player * s, * best = NULL;

  for (s = first; s != NULL; s = s->Next())
  {
    /* Bereits zum Training angemeldet ? */
    if (s->GetTraining() == 1) continue;
    /* Bereits auf 10 ? */
    if (s->GetStaerke() == 10) continue;
    /* Uneingespieltes Talent ? */
    if (s->GetAlter() == 0 && s->GetStaerke() == 0) continue;
    /* Training FALSEch erlaubt ? */
    if (s->GetTrainiert() >= 3 && s->GetAlter() != 0) continue;
    /* Zwei Punkte da ? */
    if (alle == FALSE && s->Faktor() == 2) continue;

    if ((best == NULL) ||
        (best->GetAlter()  > s->GetAlter()) ||
        (best->GetAlter() == s->GetAlter() && best->GetStaerke() < s->GetStaerke()))
      best = s;
  }
  return(best);
}

void Team::AdjustHeimvorteil (const int p)
{
  heimvorteil += p-1;
  if (heimvorteil <  6) heimvorteil =  6;
  if (heimvorteil > 20) heimvorteil = 20;
}

BOOL Team::operator > (const Team & m) const
{
  int i, p1 [2], p2 [2], t1 [2], t2 [2], d [2];

  for (i=0; i<2; i++)
  {
    p1[i] = punkte[i];
    p2[i] = m.punkte[i];
    t1[i] = tore[i];
    t2[i] = m.tore[i];
  }
  d[0] = tore[0]   - tore[1];
  d[1] = m.tore[0] - m.tore[1];

  if (p1[0] > p2[0]) return(TRUE);
  if (p1[0] < p2[0]) return(FALSE);
  if (p1[1] > p2[1]) return(FALSE);
  if (p1[1] < p2[1]) return(TRUE);

  if (d [0] > d [1]) return(TRUE);
  if (d [0] < d [1]) return(FALSE);
  if (t1[0] > t2[0]) return(TRUE);
  if (t1[0] < t2[0]) return(FALSE);

  return(TRUE);
}


char * Team::TabellenZeile () const
{
  static char buffer [80];

  sprintf(buffer,"\n%-30s  %3d:%3d  %3d:%3d %3d",
          name,
          punkte[0],
          punkte[1],
          tore[0],
          tore[1],
          tore[0] - tore[1]);
  return(buffer);
}

void Team::Altern ()
{
  Player * s, * sn;

  for (s = first; s != NULL; s = sn)
  {
    sn = s->Next();
    if (s->Altern() == FALSE) PlayerDelete(s);
  }
}

void Team::CreateTalente ()
{
  int i, n [5], t, min;
  char buffer [80];
  Player * s;
  static const int fak [5] = { 49, 59, 11, 12, 10 };

  for (i=0; i<5; i++) n[i] = 0;
  t = 0;
  for (s = first; s != NULL; s = s->Next())
  {
    if (s->GetAlter() == 0) t++;
    n[s->GetReihe().AsInt()]++;
  }

  for (i=0; i<5; i++) n[i] *= fak[i];

  while (t++ < 3)
  {
    s = PlayerAdd();
    sprintf(s->GetName(),"Neues Talent %d",t);
    for (min=0,i=1; i<5; i++) if (n[min] > n[i]) min = i;
    n[min] += fak[min];
    s->SetReihe(min);
    sprintf(buffer,"Team %s entdeckt ein neues Talent (%s).",
            name,s->GetReihe().AsChar());
    message(buffer,1);
  }
}

void Team::Entsperren () const
{
  Player * s;
  for (s = first; s != NULL; s = s->Next()) s->Entsperren();
}
#endif

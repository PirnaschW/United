#include "stdafx.h"
#include "Pair.h"
#include "Player.h"
#include "PlayerA.h"

Pair::Pair (Team * m1, Team * m2)
{
  t[0] = m1;
  t[1] = m2;
}

void Pair::PlayMatch ()
{
  int i, j, d, pt, reihe [2] [5], chancen [2], tore [2], h, m, elfmeter, off;
  char buffer [37+32+(60+32+17)*11+55+2*32+1], * b;
  BOOL mess;
  Row r;
  Player * p;

  for (i=0; i<2; i++)
  {
    reihe[i][0] = t[i]->RowSum(TORWART,     NULL);
    reihe[i][1] = t[i]->RowSum(AUSPUTZER,   NULL);
    reihe[i][2] = t[i]->RowSum(VERTEIDIGUNG,NULL);
    reihe[i][3] = t[i]->RowSum(MITTELFELD,  NULL);
    reihe[i][4] = t[i]->RowSum(STURM,       NULL);
    chancen[i] = tore[i] = 0;
  }

  /* Haerte : */
  for (i=0; i<2; i++)
  {
    for (h=0,j=0; j<5; j++) h += (int) t[i]->ha[j];
    if (h == 0) continue;
    else off = sprintf(buffer,"Mannschaft %s erhält folgende Strafen: ",
                       (const char *) t[i]->name);
    mess = FALSE;
    for (j=0; j<t[i]->players->players.GetSize(); j++)
    {
      p = (Player *) (t[i]->players->players[j]);
      if (p->eingesetzt == NONE) continue;
      /* 1. Rote Karten */
      if (rndint(400) <= 3 * h)
      {
        if (p->eingesetzt == TORWART && t[i]->ha[0] == 0) continue; /* Torwart kriegt keine Karten */
        m = rndint(90);
        reihe[i][p->eingesetzt] -= (p->StrengthAt((ROW) p->eingesetzt) * (90-m) / 90);
        mess = TRUE;
        off += sprintf(buffer+off,"Spieler %s wird vom Platz gestellt (%d. Minute), ",
                (const char *) p->name,m);
        p->eingesetzt = NONE;
        if ((b = p->Penalty(TRUE)) != NULL) 
        {
          off -= 2;
          off += sprintf(buffer+off,"und ist für %s gesperrt, ",b);
        }
      }
      /* 2. Gelbe Karten */
      else if (rndint(100) <= 3 * h)
      {
        if (p->eingesetzt == TORWART && t[i]->ha[0] == 0) continue; /* Torwart kriegt keine Karten */
        mess = TRUE;
        off += sprintf(buffer+off,"Spieler %s erhält eine gelbe Karte, ",(const char *) p->name);
        if ((b = p->Penalty(FALSE)) != NULL) 
        {
          off -= 2;
          off += sprintf(buffer+off,"und ist für %s gesperrt, ",b);
        }
      }
    }
    /* 3. Elfmeter */
    for (elfmeter=0,j=0; j<h; j++)
      if (rndint(10) == 1) {
        elfmeter++;
        if (rndint(20) > p->StrengthAt(TORWART)) tore[1-i]++;
      }

    if (elfmeter > 0)
    {
      mess = TRUE;
      for (j=0; j<t[i]->players->players.GetSize(); j++)
        if ((p = (Player *) (t[i]->players->players[j]))->eingesetzt == TORWART) break;
      off += sprintf(buffer+off,"Mannschaft %s erhält %d Elfmeter, Torwart %s hält davon %d.",
                     (const char *) t[1-i]->name,elfmeter,
                     (const char *) p->name,elfmeter-tore[1-i]);
    }
    else strcpy(buffer+off-2,".");
    if (mess) AfxMessageBox(buffer);
  }

  d = reihe[0][4] - reihe[1][2];
  if (d > reihe[1][1])       chancen[0] +=  d - reihe[1][1];
  else if (d < 0)            chancen[1] += -d/4;

  d = reihe[0][3] - reihe[1][3];
  if (d > 0)                 chancen[0] +=  d/2;
  else                       chancen[1] += -d/2;

  d = reihe[0][2] - reihe[1][4];
  if (d > 0)                 chancen[0] +=  d/4;
  else if (-d > reihe[0][1]) chancen[1] += -d - reihe[0][1];

  sprintf(buffer,"Spiel %s gegen %s: %d : %d Chancen",
          (const char *) t[0]->name,
          (const char *) t[1]->name,
          chancen[0],
          chancen[1]);
  AfxMessageBox(buffer);

  for (i=0; i<2; i++)
    for (j=0; j<chancen[i]; j++)
      if ((rndint(15) > reihe[1-i][1]) &&
          (rndint(14) > reihe[1-i][0]))
        tore[i]++;

  sprintf(buffer,"Spiel %s gegen %s: %d : %d Tore",
          (const char *) t[0]->name,
          (const char *) t[1]->name,
          tore[0],
          tore[1]);
  AfxMessageBox(buffer);

  pt = tore[0] > tore[1] ? 2 : tore[0] == tore[1] ? 1 : 0;
  for (i=0; i<2; i++)
  {
    t[i]->SkillsIncrease();
    for (j=0; j<2; j++)
    {
      t[i]->tore[j]   += tore[i==0?j :1-j];
      t[i]->punkte[j] +=      i==j?pt:2-pt;
    }
    t[i]->training += 4 + (i==0?pt:2-pt);

#ifdef TRAINING
    /* Trainieren : */
    for (j=0; a[i]->training[j] != NULL; j++)
    {
      d = faktor(a[i]->training[j]);
      if (t[i]->training < 2*d) break;
      t[i]->training -= 2*d;
      (a[i]->training[j]->staerke)++;
      (a[i]->training[j]->trainiert)++;
      sprintf(buffer,"Team %s trainiert %s auf Staerke %d.",
              t[i]->name,
              a[i]->training[j]->name,
              a[i]->training[j]->staerke);
      message(buffer,1);
    }
    if (t[i]->training > 1) t[i]->training = 1;
#endif
  }
  /* Heimvorteil : */
  t[0]->HomeBonusAdjust(pt);
}

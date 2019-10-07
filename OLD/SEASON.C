#include <math.h>
#include "united.h"

void init_season (void)
{
  int c0, n, m;

  static char * item [] = {
    "Einfachrunde     (Spieltage : 99999)",
    "Doppelrunde      (Spieltage : 99999)",
    "Vierfachrunde    (Spieltage : 99999)",
    "Achtfachrunde    (Spieltage : 99999)",
    "in Gruppen       (Spieltage : 99999)",
    "Schweizer System (Spieltage : 99999)" };

  if (check_if_game() == NO)
  {
    message("Kein Spiel aktiv !",1);
    return;
  }
  if (season_started() == YES)
  {
    message("Die Saison hat bereits begonnen.",1);
    return;
  }

  if ((n = game->mannschaften = count_mannschaften()) < 2)
  {
    message("Mindestens zwei Mannschaften muessen mitspielen.",1);
    return;
  }

  reset_mannschaften();
  create_talente();

  if (check_mannschaften() == NO) {
    message("Alle Mannschaften mssen spielbereit sein.",1);
    return;
  }

  game->round = 1;

  reset_tabelle();
  random_sequence();

  m = (int) (sqrt((double) n) + 0.5);
  if (n > 2 && m < 3) m = 3;
  sprintf(item[0]+30,"%5d)",n-1);
  sprintf(item[1]+30,"%5d)",(n-1)*2);
  sprintf(item[2]+30,"%5d)",(n-1)*4);
  sprintf(item[3]+30,"%5d)",(n-1)*8);
  sprintf(item[4]+30,"%5d)",(m-1)+2*(n-1)/m);
  sprintf(item[5]+30,"%5d)",m);

  if      (n <  4) c0 = 3;
  else if (n <  7) c0 = 2;
  else if (n < 11) c0 = 1;
  else if (n < 21) c0 = 0;
  else if (n < 50) c0 = 4;
  else             c0 = 5;

  game->modus = menu(6,c0,"Spielmodus :",item,NULL);
}


BOOLEAN season_started (void)
{
  if (game->round > 0) return(YES);
  return(NO);
}


void reset_tabelle (void)
{
  MANNSCHAFT * m;

  for (m = game->first; m != NULL; m = m->next)
    m->punkte[0] = m->punkte[1] = m->tore[0] = m->tore[1] = 0;
}


void random_sequence (void)
{
  int i, n;
  MANNSCHAFT * m, * * array;

  array = (MANNSCHAFT * * ) malloc(sizeof(MANNSCHAFT *) * game->mannschaften);
  for (i = 0, m = game->first; m != NULL; i++, m = m->next) array[i] = m;

  for (m = NULL, i = game->mannschaften; i > 0; i--) {
    n = rndint(i) - 1;
    array[n]->next = m;
    m = array[n];
    array[n] = array[i-1];
  }
  game->first = m;
  free(array);
}


void play_round (void)
{
  int i, p;
  AUFSTELLUNG * a [2];
  MANNSCHAFT  * m1, * m2;

  if (check_if_game() == NO)
  {
    message("Kein Spiel aktiv !",1);
    return;
  }
  if (season_started() == NO)
  {
    message("Die Saison hat noch nicht begonnen.",1);
    return;
  }

  init_round(game->mannschaften);

  while (get_paarung(&m1,&m2) == YES)
  {
    a[0] = get_aufstellung(m1,YES);
    a[1] = get_aufstellung(m2,NO);
    play_match(a);
    free(a[0]); free(a[1]);
  }
  message("Spieltag beendet.",1);
  show_tabelle();


  for (i=0,p=1; i<game->modus; i++,p*=2);
  if ((game->mannschaften+1)/2*2*p == ++(game->round))
  {
    game->round = -1;
    message("Die Saison ist zu Ende.\nAlle Spieler altern jetzt ein Jahr.",2);
    altern();
  }
}


static int paarung;
void init_round (int n)
{
  MANNSCHAFT * m;
  SPIELER * s;

  paarung = (n+1)/2;

  /* sperren runterzaehlen */
  for (m = game->first; m != NULL; m = m->next)
    for (s = m->first; s != NULL; s = s->next)
      if (s->sperre != 0 && s->sperre != 7) s->sperre--;
}


BOOLEAN get_paarung (MANNSCHAFT * * m1, MANNSCHAFT * * m2)
{
  int i1, i2, n, r;

  if (paarung-- > 0)
  {
    n = (game->mannschaften + 1) / 2 * 2;
    r = ((n / 2 * (game->round - 1)) % (n - 1)) + 1;
    i1 = r+paarung;
    i2 = r-paarung;
    if (i1 > n-1) i1 -= n-1;
    if (i2 <   1) i2 += n-1;

    if (i1 == i2)
    {
      if (n != game->mannschaften) return(NO);
      else {
        if (r > game->mannschaften/2) i1 = n;
        else                          i2 = n;
      }
    }
    for (*m1 = game->first; *m1 != NULL; *m1 = (*m1)->next) if (--i1 == 0) break;
    for (*m2 = game->first; *m2 != NULL; *m2 = (*m2)->next) if (--i2 == 0) break;
    return(YES);
  }
  else return(NO);
}


void altern (void)
{
  int d;
  MANNSCHAFT * m;
  SPIELER * s, * sn;

  for (m = game->first; m != NULL; m = m->next)
    for (s = m->first; s != NULL; s = sn)
    {
      switch (s->alter++)
      {
      case  0 : d = 0; break;
      case  1 : d = 2; break;
      case  2 : d = 4; break;
      default : d = 5; break;
      }
      sn = s->next;
      if (s->staerke <= d) del_spieler(m,s);
      else s->staerke -= d;
    }
}


void create_talente (void)
{
  int i, n [5], t, min;
  char buffer [80];
  MANNSCHAFT * m;
  SPIELER * s;
  static const int fak [5] = { 49, 59, 11, 12, 10 };

  for (m = game->first; m != NULL; m = m->next)
  {
    for (i=0; i<5; i++) n[i] = 0;
    t = 0;
    for (s = m->first; s != NULL; s = s->next)
    {
      if (s->alter == 0) t++;
      n[get_reihe_int(s->position)]++;
    }

    for (i=0; i<5; i++) n[i] *= fak[i];

    while (t++ < 3)
    {
      s = new_spieler(m);
      sprintf(s->name,"Neues Talent %d",t);
      for (min=0,i=1; i<5; i++) if (n[min] > n[i]) min = i;
      n[min] += fak[min];
      s->position = get_reihe(min);
      sprintf(buffer,"Mannschaft %s entdeckt ein neues Talent (%s).",
              m->name,get_position_char(s->position));
      message(buffer,1);
    }
  }
}

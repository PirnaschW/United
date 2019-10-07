#include <string.h>
#include "united.h"

static BOOLEAN check_3_1_ratio (int * reihe);

AUFSTELLUNG * get_aufstellung (MANNSCHAFT * m, BOOLEAN heim)
{
  int i, c0 = 1;
  char title [80];
  AUFSTELLUNG * a;

  static char * item [] = {
    "Beste Spieler aufstellen",
    "Verteidigung staerker",
    "Mittelfeld   staerker",
    "Sturm        staerker",
    "Einzeln aufstellen",
    "Aufstellung anzeigen",
    "Ende" };

  sprintf(title,"Aufstellung fuer Mannschaft %s :",m->name);
  a = malloc(sizeof(AUFSTELLUNG));
  a->mannschaft = m;
  a->valid = NO;
  a->training[0] = NULL;
  for (i=0; i<5; i++) a->haerte[i] = a->heimvorteil[i] = 0;
  a->heim = heim == YES ? m->heimvorteil/2 : 0;
  while (1)
  {
    if (a->valid == YES) c0 = 6;
    else c0 = (c0 + 3) % 4;
    c0 = menu(7,c0,title,item,NULL);
    switch (c0) {
    case  0 : auto_aufstellung(a,ALLE);         break;
    case  1 : auto_aufstellung(a,VERTEIDIGUNG); break;
    case  2 : auto_aufstellung(a,MITTELFELD);   break;
    case  3 : auto_aufstellung(a,STURM);        break;
    case  4 : hand_aufstellung(a);              break;
    case  5 : show_aufstellung(a);              break;
    case  6 : if (a->valid == YES) return(a);
              else message("Aufstellung ungueltig.",1);
      if (get_yes_no("Abbrechen ?",NO) == YES) return(a);
      break;
    }
  }
}


BOOLEAN check_validity (AUFSTELLUNG * a)
{
  int reihe [5];

  if ((a->torwart         == NULL) ||
      (a->verteidigung[0] == NULL) ||
      (a->verteidigung[1] == NULL) ||
      (a->mittelfeld[0]   == NULL) ||
      (a->mittelfeld[1]   == NULL) ||
      (a->sturm[0]        == NULL) ||
      (a->sturm[1]        == NULL)) return(NO);

  sum_reihe(a,reihe,NO);
  if (check_3_1_ratio(reihe) == NO) return(NO);

  sum_reihe(a,reihe,YES);
  if (check_3_1_ratio(reihe) == NO) return(NO);

  return(YES);
}


static BOOLEAN check_3_1_ratio (int * reihe)
{
  if ((reihe[2] > reihe[3] * 3) ||
      (reihe[2] > reihe[4] * 3) ||
      (reihe[3] > reihe[2] * 3) ||
      (reihe[3] > reihe[4] * 3) ||
      (reihe[4] > reihe[2] * 3) ||
      (reihe[4] > reihe[3] * 3)) return(NO);
  return(YES);
}

void sum_reihe (AUFSTELLUNG * a, int * reihe, BOOLEAN heim)
{
  int i;

  reihe[0] = einsatz_staerke(a->torwart,TORWART);
  reihe[1] = einsatz_staerke(a->ausputzer,AUSPUTZER);
  reihe[2] = i = 0;
  while (a->verteidigung[i] != NULL && i < 6) reihe[2] += einsatz_staerke(a->verteidigung[i++],VERTEIDIGUNG);
  reihe[3] = i = 0;
  while (a->mittelfeld[i] != NULL && i < 6) reihe[3] += einsatz_staerke(a->mittelfeld[i++],MITTELFELD);
  reihe[4] = i = 0;
  while (a->sturm[i] != NULL && i < 6) reihe[4] += einsatz_staerke(a->sturm[i++],STURM);
  if (heim == YES) for (i=0; i<5; i++) reihe[i] += a->haerte[i] + a->heimvorteil[i];
}


void hand_aufstellung (AUFSTELLUNG * a)
{
  int i, n = 0, h, hi;
  char prompt [40];

  detach_spieler(a->mannschaft);

  if ((a->torwart    = select_spieler(a->mannschaft,"Torwart :",  TORWART))   != NULL) n++;
  if ((a->ausputzer  = select_spieler(a->mannschaft,"Ausputzer :",AUSPUTZER)) != NULL) n++;
  i = 0;
  do if ((a->verteidigung[i] = select_spieler(a->mannschaft,"Verteidigung :",VERTEIDIGUNG)) != NULL) n++;
  while (a->verteidigung[i++] != NULL && i < 6 && n < 11);
  i = 0;
  do if ((a->mittelfeld[i] = select_spieler(a->mannschaft,"Mittelfeld :",MITTELFELD)) != NULL) n++;
  while (a->mittelfeld[i++] != NULL && i < 6 && n < 11);
  i = 0;
  do if ((a->sturm[i] = select_spieler(a->mannschaft,"Sturm :",STURM)) != NULL) n++;
  while (a->sturm[i++] != NULL && i < 6 && n < 11);
  if (i < 6) a->sturm[i] = NULL;

  for (i=0; i<5; i++) a->haerte[i] = a->heimvorteil[i] = 0;
  h = a->heim;
  for (i=2; h>0 && i<5; i++)
  {
    sprintf(prompt,"Heimvorteil %s [%d] : ",
            get_position_char(get_reihe(i)),hi=h);
    get_int(prompt,&hi);
    if (hi > h) hi = h;
    h -= hi;
    a->heimvorteil[i] = hi;
  }

  h = 10; /* haerte */
  for (i=0; h>0 && i<5; i++)
  {
    if (i == 1 && a->ausputzer == NULL) continue;
    sprintf(prompt,"Haerte %s [%d] : ",
            get_position_char(get_reihe(i)),hi=h);
    get_int(prompt,&hi);
    if (hi > h) hi = h;
    if (i < 2)
    {
      hi = hi / 2 * 2;
      if (i == 0) if (hi/2 + a->torwart->staerke   > 10) hi = 2*(10-a->torwart->staerke);
      if (i == 1) if (hi/2 + a->ausputzer->staerke > 10) hi = 2*(10-a->ausputzer->staerke);
    }
    h -= hi;
    a->haerte[i] = hi / (i < 2 ? 2 : 1);
  }

  i = 0;
  do if ((a->training[i] = select_spieler(a->mannschaft,"Training :",ALLE)) != NULL);
  while (a->training[i++] != NULL && i < 3);
  if (i < 3) a->training[i] = NULL;

  if ((a->valid = check_validity(a)) == NO) message("Aufstellung ungueltig.",1);
}


void show_aufstellung (AUFSTELLUNG * a)
{
  int off, i, z, max = 70, maxp = max / 6 - 1;
  char buffer [1200], line [80];


  if (a->valid == NO)
  {
    message("Keine Aufstellung gewaehlt.",1);
    return;
  }

  sprintf(line,"Aufstellung fuer %s",a->mannschaft->name);
  off  = center_line(line,max,buffer);
  off += sprintf(buffer+off,"\n%*sHeim H„r\n%*svrt. te\n",
                 max-2,"",max-2,"");

  if (a->torwart == NULL) off += sprintf(buffer+off,"\n\n\n");
  else
  {
    off += center_line(a->torwart->name,max,buffer+off);
    off += sprintf(buffer+off,"   %+3d\n",a->haerte[0]/2);
    off += center_line(data_line(a->torwart,TORWART),max,buffer+off);
    off += sprintf(buffer+off,"\n\n");
  }

  if (a->ausputzer == NULL) off += sprintf(buffer+off,"\n\n\n");
  else
  {
    off += center_line(a->ausputzer->name,max,buffer+off);
    off += sprintf(buffer+off,"   %+3d\n",a->haerte[1]/2);
    off += center_line(data_line(a->ausputzer,AUSPUTZER),max,buffer+off);
    off += sprintf(buffer+off,"\n\n");
  }

  i = z = 0;
  while (a->verteidigung[i] != NULL && i < 6)
    z += sprintf(line+z,"%-*.*s ",maxp,maxp,a->verteidigung[i++]->name);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"%+3d%+3d\n",a->heimvorteil[2],a->haerte[2]);
  i = z = 0;
  while (a->verteidigung[i] != NULL && i < 6)
    z += sprintf(line+z,"%-*.*s ",maxp,maxp,data_line(a->verteidigung[i++],VERTEIDIGUNG));
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n\n");

  i = z = 0;
  while (a->mittelfeld[i] != NULL && i < 6)
    z += sprintf(line+z,"%-*.*s ",maxp,maxp,a->mittelfeld[i++]->name);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"%+3d%+3d\n",a->heimvorteil[3],a->haerte[3]);
  i = z = 0;
  while (a->mittelfeld[i] != NULL && i < 6)
    z += sprintf(line+z,"%-*.*s ",maxp,maxp,data_line(a->mittelfeld[i++],MITTELFELD));
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n\n");

  i = z = 0;
  while (a->sturm[i] != NULL && i < 6)
    z += sprintf(line+z,"%-*.*s ",maxp,maxp,a->sturm[i++]->name);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"%+3d%+3d\n",a->heimvorteil[4],a->haerte[4]);
  i = z = 0;
  while (a->sturm[i] != NULL && i < 6)
    z += sprintf(line+z,"%-*.*s ",maxp,maxp,data_line(a->sturm[i++],STURM));
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n");

  message(buffer,19);

  sprintf(line,"buffer hatte %d chars",strlen(buffer));
  message(line,1);
}


void play_match (AUFSTELLUNG * * a)
{
  int i, j, d, p, reihe [2] [5], chancen [2], tore [2], h, m;
  char buffer [200];
  REIHE r;
  SPIELER * s;

  for (i=0; i<2; i++)
  {
    sum_reihe(a[i],reihe[i],YES);
    chancen[i] = tore[i] = 0;
  }

  /* Haerte : */
  for (i=0; i<2; i++)
  {
    for (h=0,j=0; j<5; j++) h += a[i]->haerte[j];
  /* 1. Rote Karten */
    for (j=0; j<11; j++)
    {
      if (rndint(400) <= 3 * h)
      {
        if (j == 0 && a[i]->haerte[0] == 0) continue; /* Torwart kriegt keine Karten */
        m = rndint(90);
        if ((s = get_aufgestellt(a[i],j,&r)) == NULL) continue;
        s->eingesetzt = 0;
        reihe[i][get_reihe_int(r)] -= (einsatz_staerke(s,r) * (90-m) / 90);
        sprintf(buffer,"Spieler %s von %s\nwird vom Platz gestellt (%d. Minute)",
                s->name,a[i]->mannschaft->name,m);
        message(buffer,2);
        strafe(s,10);
      }
    }
  /* 2. Gelbe Karten */
    for (j=0; j<11; j++)
    {
      if (rndint(100) <= 3 * h)
      {
        if (j == 0 && a[i]->haerte[0] == 0) continue; /* Torwart kriegt keine Karten */
        if ((s = get_aufgestellt(a[i],j,&r)) == NULL) continue;
        if (s->eingesetzt == 0) continue;
        sprintf(buffer,"Spieler %s von %s\nerhaelt eine gelbe Karte",
                s->name,a[i]->mannschaft->name);
        message(buffer,2);
        strafe(s,4);
      }
    }
  /* 3. Elfmeter */
    for (j=0; j<h; j++)
    {
      if (rndint(10) == 1)
      {
        if (rndint(20) > a[i]->torwart->staerke)
        {
          sprintf(buffer,"%s erhaelt einen Elfmeter und verwandelt ihn.",
                  a[1-i]->mannschaft->name);
          tore[1-i]++;
        }
        else
          sprintf(buffer,"Torwart %s haelt einen Elfmeter.",
                  a[i]->torwart->name);

        message(buffer,1);
      }
    }
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

  sprintf(buffer,"Chancen : %d : %d",chancen[0],chancen[1]);
  message(buffer,1);

  for (i=0; i<2; i++)
    for (j=0; j<chancen[i]; j++)
      if ((rndint(15) > reihe[1-i][1]) &&
          (rndint(14) > reihe[1-i][0]))
        tore[i]++;

  sprintf(buffer,"Ergebnis : %d : %d",tore[0],tore[1]);
  message(buffer,1);

  p = tore[0] > tore[1] ? 2 : tore[0] == tore[1] ? 1 : 0;
  for (i=0; i<2; i++)
  {
    note_einsaetze(a[i]);
    for (j=0; j<2; j++)
    {
      a[i]->mannschaft->tore[j]   += tore[i==0?j:1-j];
      a[i]->mannschaft->punkte[j] +=      i==j?p:2-p;
    }
    a[i]->mannschaft->training += 4 + (i==0?p:2-p);

    /* Trainieren : */
    for (j=0; a[i]->training[j] != NULL; j++)
    {
      d = faktor(a[i]->training[j]);
      if (a[i]->mannschaft->training < 2*d) break;
      a[i]->mannschaft->training -= 2*d;
      (a[i]->training[j]->staerke)++;
      (a[i]->training[j]->trainiert)++;
      sprintf(buffer,"Mannschaft %s trainiert %s auf Staerke %d.",
              a[i]->mannschaft->name,
              a[i]->training[j]->name,
              a[i]->training[j]->staerke);
      message(buffer,1);
    }
    if (a[i]->mannschaft->training > 1) a[i]->mannschaft->training = 1;
  }
  /* Heimvorteil : */
  adjust_heimvorteil(a[0]->mannschaft,p);
}


SPIELER * get_aufgestellt (AUFSTELLUNG * a, int n, REIHE * r)
{
  int i, z = 0;

  if (++z > n) { *r = TORWART; return(a->torwart); }
  if (a->ausputzer != NULL)
    if (++z > n) { *r = AUSPUTZER; return(a->ausputzer); }
  for (i = 0; a->verteidigung[i] != NULL && i < 6; i++)
    if (++z > n) { *r = VERTEIDIGUNG; return(a->verteidigung[i]); }
  for (i = 0; a->mittelfeld[i]   != NULL && i < 6; i++)
    if (++z > n) { *r = MITTELFELD; return(a->mittelfeld[i]); }
  for (i = 0; a->sturm[i]        != NULL && i < 6; i++)
    if (++z > n) { *r = STURM; return(a->sturm[i]); }
  return(NULL);
}


int einsatz_staerke (SPIELER * s, REIHE r)
{
  int effektiv;

  if (s == NULL) return(0);

  effektiv = s->staerke;
  if (s->position == r) return(s->staerke); /* auf Stammposition */
  if (eingespielt(s,r) == NO) switch (r)
  {
  case TORWART      : effektiv = 0; break;
  case AUSPUTZER    : effektiv = (s->staerke + (eingespielt(s,VERTEIDIGUNG) == YES ? 1 : 0)) / 3; break;
  case VERTEIDIGUNG : if (eingespielt(s,AUSPUTZER) == YES) effektiv++;
  case MITTELFELD   :
  case STURM        : effektiv--; break;
  }
  else if ((r == AUSPUTZER) && (s->position != r))
    {
      if (eingespielt(s,VERTEIDIGUNG) == YES)
        effektiv =  s->staerke    / 2;
      else
        effektiv = (s->staerke-1) / 2;
    }

  return(effektiv);
}


void note_einsaetze (AUFSTELLUNG * a)
{
  int i;

  if (a->torwart   != NULL) increase_einsaetze(a->torwart,  TORWART);
  if (a->ausputzer != NULL) increase_einsaetze(a->ausputzer,AUSPUTZER);
  for (i = 0; a->verteidigung[i] != NULL && i < 6; i++ )
    increase_einsaetze(a->verteidigung[i],VERTEIDIGUNG);
  for (i = 0; a->mittelfeld  [i] != NULL && i < 6; i++ )
    increase_einsaetze(a->mittelfeld  [i],MITTELFELD);
  for (i = 0; a->sturm       [i] != NULL && i < 6; i++ )
    increase_einsaetze(a->sturm       [i],STURM);
}


void increase_einsaetze (SPIELER * s, REIHE r)
{
  char buffer [80];

  switch (r)
  {
  case TORWART      : s->einsatz_t++; break;
  case AUSPUTZER    : s->einsatz_a++; break;
  case VERTEIDIGUNG : s->einsatz_v++; break;
  case MITTELFELD   : s->einsatz_m++; break;
  case STURM        : s->einsatz_s++; break;
  }
  if (s->alter == 0 && s->staerke == 0 && eingespielt(s,r) == YES)
  {
    s->staerke = 2;
    sprintf(buffer,"Talent %s ist jetzt eingespielt\nund hat Staerke 2.",
            s->name);
    message(buffer,2);
  }
}


void strafe (SPIELER * s, int punkte)
{
  int n1, n2;
  char buffer [80];

  n1 = s->disziplinar / 10;
  s->disziplinar += punkte;
  n2 = s->disziplinar / 10;
  if (n2 != n1)
  {
    switch(n2)
    {
    case 1 :
      sprintf(buffer,"Spieler %s ist fuer 1 Spiel gesperrt",s->name);
      s->sperre = 2;
      break;
    case 2 :
      sprintf(buffer,"Spieler %s ist fuer 3 Spiele gesperrt",s->name);
      s->sperre = 4;
      break;
    default:
      sprintf(buffer,"Spieler %s ist fuer die ganze Saison gesperrt",s->name);
      s->sperre = 7;
      break;
    }
    message(buffer,1);
  }
}


void adjust_heimvorteil (MANNSCHAFT * m, int p)
{
  m->heimvorteil += p-1;
  if (m->heimvorteil <  6) m->heimvorteil =  6;
  if (m->heimvorteil > 20) m->heimvorteil = 20;
}

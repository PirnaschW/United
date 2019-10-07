#include <string.h>
#include "united.h"

void kill_mannschaft (MANNSCHAFT * m)
{
  SPIELER * s, * sn;

  s = m->first;
  while (s != NULL)
  {
    sn = s->next;
    kill_spieler(s);
    s = sn;
  }
  free(m);
}


MANNSCHAFT * new_mannschaft (void)
{
  MANNSCHAFT * * m = &(game->first);

  while (*m != NULL) m = &((*m)->next);
  *m = malloc(sizeof(MANNSCHAFT));
  (*m)->next = NULL;
  (*m)->name[0] = 0;
  (*m)->owner[0] = 0;
  (*m)->heimvorteil = 0;
  (*m)->first = NULL;
  return(*m);
}


void menu_mannschaften (void)
{
  static int c0 = 5;

  static const char * item [] = {
    "Mannschaft addieren",
    "Mannschaft aendern",
    "Mannschaft anzeigen",
    "Mannschaft loeschen",
    "Spieler bearbeiten",
    "Ende" };

  if (check_if_game() == NO)
  {
    message("Kein Spiel aktiv !",1);
    return;
  }

  while (1)
  {
    c0 = menu(6,c0,"MANNSCHAFTSMENU",item,NULL);
    switch (c0) {
    case 0 : query_add_mannschaft();  break;
    case 1 : query_edit_mannschaft(); break;
    case 2 : query_show_mannschaft(); break;
    case 3 : query_del_mannschaft();  break;
    case 4 : query_menu_spieler();    break;
    default: return;
    }
  }
}


void query_add_mannschaft (void)
{
  double h = 6.0;
  MANNSCHAFT * m;

  if (season_started() == YES)
  {
    message("Die Saison hat bereits begonnen.",1);
    return;
  }

  m = new_mannschaft();
  get_char("Name der Mannschaft : ",LNNAM-1,m->name);
  get_char("Name des Trainers : ",LNNAM-1,m->owner);
  get_double("Heimvorteil : ",&h);
  m->heimvorteil = (int) (2.0 * h);
}


void query_edit_mannschaft (void)
{
  MANNSCHAFT * m;

  if ((m = select_mannschaft()) == NULL) return;
  edit_mannschaft(m);
}


void query_show_mannschaft (void)
{
  MANNSCHAFT * m;

  if ((m = select_mannschaft()) == NULL) return;
  show_mannschaft(m);
}


void query_del_mannschaft (void)
{
  MANNSCHAFT * m;

  if (season_started() == YES)
  {
    message("Die Saison hat bereits begonnen.",1);
    return;
  }

  if ((m = select_mannschaft()) == NULL) return;
  del_mannschaft(m);
}


void query_menu_spieler (void)
{
  MANNSCHAFT * m;

  if ((m = select_mannschaft()) == NULL) return;
  menu_spieler(m);
}


void edit_mannschaft (MANNSCHAFT * m)
{
  double h;
  char buffer [LNNAM];
  static int c0 = 3;

  static char * item [] = {
    "Mannschaft  :                                   ",
    "Trainer     :                                   ",
    "Heimvorteil : 10.0",
    "Ende" };

  static const char * prompt [] = {
    "Neuer Name : ",
    "Neuer Trainer : ",
    "Heimvorteil : " };

  while (1)
  {
    sprintf(item[0]+15,"%s",m->name);
    sprintf(item[1]+15,"%s",m->owner);
    sprintf(item[2]+14,"%4.1lf",(double)(m->heimvorteil)*0.5);
    if ((c0 = menu(4,c0,"Mannschaft aendern",item,NULL)) == 3) return;
    if (c0 == 2)
    {
      get_double(prompt[c0],&h);
      if (h > 10.0) h = 10.0;
      if (h <  3.0) h =  3.0;
      m->heimvorteil = (int) (2.0 * h);
    }
    else
    {
      get_char(prompt[c0],LNNAM-1,buffer);
      if (buffer[0] != '\0') strcpy(c0==0?m->name:m->owner,buffer);
    }
  }
}


void del_mannschaft (MANNSCHAFT * m)
{
  MANNSCHAFT * mp;

  if (m == game->first) game->first = m->next;
  else
  {
    for (mp = game->first; mp != NULL; mp = mp->next)
      if (mp->next == m) break;
    mp->next = m->next;
  }
  kill_mannschaft(m);
}


MANNSCHAFT * select_mannschaft (void)
{
  int n;
  char * * item, * name;

  n = build_list_mannschaft(&item);
  if (n == 0)
  {
    message("Keine Mannschaften vorhanden !",1);
    return(NULL);
  }
  name = item[menu(n,0,"MANNSCHAFT WAEHLEN:",item,NULL)];
  free(item);
  return(get_mannschaft(name));
}


int build_list_mannschaft (char * * * list)
{
  int i = 0;
  MANNSCHAFT * m;

  if ((i = count_mannschaften())== 0) return(0);
  *list = (char * *) malloc(sizeof(char *) * i);
  for (i = 0, m = game->first; m != NULL; m = m->next) (*list)[i++] = m->name;
  return(i);
}


MANNSCHAFT * get_mannschaft (char * name)
{
  MANNSCHAFT * m;

  for (m = game->first; m != NULL; m = m->next)
    if (m->name == name) break;
  return(m);
}


void show_mannschaft (MANNSCHAFT * m)
{
  int off, n = 0, max = 76, sums = 0, suma = 0, sumt = 0, sumd = 0, anz [5], i;
  long sumw = 0L, w;
  char line [80], buffer [2500], c [5];
  SPIELER * s;

  sprintf(line,"Mannschaft  %s",m->name);
  off = center_line(line,max,buffer);
  off += sprintf(buffer+off,"\n\n");

  sprintf(line,"Trainer : %s",m->owner);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n");

  sprintf(line,"Heimvorteil : %4.1lg",(double)m->heimvorteil*0.5);
  off += center_line(line,max,buffer+off);
  off += sprintf(buffer+off,"\n");

  off += sprintf(buffer+off,"\n%5s%-27s Stamm St„ Al- trai- DP S eingespielt  Wert","","Name");
  off += sprintf(buffer+off,"\n%5s%-27s  pos. rke ter niert       T A V M S    kDM","","");
  for (i=0; i<5; i++) anz[i] = 0;
  for (n = 0, s = m->first; s != NULL; s = s->next, n++)
  {
    c[0] = s->einsatz_t >= EINSPIELEN ? '+' : ('0' + s->einsatz_t);
    c[1] = s->einsatz_a >= EINSPIELEN ? '+' : ('0' + s->einsatz_a);
    c[2] = s->einsatz_v >= EINSPIELEN ? '+' : ('0' + s->einsatz_v);
    c[3] = s->einsatz_m >= EINSPIELEN ? '+' : ('0' + s->einsatz_m);
    c[4] = s->einsatz_s >= EINSPIELEN ? '+' : ('0' + s->einsatz_s);
    sumw += (w = get_wert(s));

    off += sprintf(buffer+off,"\n%5s%-30s %.1s   %2d  %-3s  %1d   %2d %1d  %c %c %c %c %c %6ld",
      "",
      s->name,
      get_position_char(s->position),
      s->staerke,
      s->alter == 0 ? "nT" : roman(s->alter),
      s->trainiert,
      s->disziplinar,
      s->sperre>1?s->sperre-1:0,
      c[0],c[1],c[2],c[3],c[4],w);
    sums += s->staerke * faktor(s);
    suma += s->alter;
    sumt += s->trainiert;
    sumd += s->disziplinar;
    for (i=0; i<5; i++) if (c[i] == '+') anz[i]++;
  }
  off += sprintf(buffer+off,"\n%5sGesamt: %3d Spieler%8s       %3d %3.1lf %3d  %3d   %2d%2d%2d%2d%2d %6d",
                 "",n,"",sums,(double)suma/(double)(n>0?n:1),sumt,sumd,
                 anz[0],anz[1],anz[2],anz[3],anz[4],sumw);

  message(buffer,n+8);

  sprintf(line,"buffer hatte %d chars",strlen(buffer));
  message(line,1);
}


int count_mannschaften (void)
{
  int i = 0;
  MANNSCHAFT * m;

  for (m = game->first; m != NULL; m = m->next) i++;
  return(i);
}


void reset_mannschaften (void)
{
  MANNSCHAFT * m;
  SPIELER * s;

  for (m = game->first; m != NULL; m = m->next)
    for (s = m->first; s != NULL; s = s->next)
      s->trainiert = s->disziplinar = s->sperre = 0;
}


BOOLEAN check_mannschaften (void)
{
  MANNSCHAFT * m;

  for (m = game->first; m != NULL; m = m->next)
    if (mannschaft_valid(m) == NO) return(NO);
  return(YES);
}


BOOLEAN mannschaft_valid (MANNSCHAFT * m)
{
  int nt = 0, n = 0;
  char buffer [80];
  SPIELER * s;

  for (s = m->first; s != NULL; s = s->next)
  {
    n++;
    if (s->position == TORWART) nt++;
  }

  if (n < 11)
  {
    sprintf(buffer,"Mannschaft %s hat nur %d Spieler.",m->name,n);
    message(buffer,1);
    return(NO);
  }

  if (nt < 1)
  {
    sprintf(buffer,"Mannschaft %s hat keinen Torwart.",m->name);
    message(buffer,1);
    return(NO);
  }

  return(YES);
}


char * data_line (SPIELER * s, REIHE r)
{
  static char buffer [20];

  sprintf(buffer,"%s %d (%d)",
	  s->alter == 0 ? "nT" : roman(s->alter),
	  einsatz_staerke(s,r),einsaetze(s,r));
  return(buffer);
}


int center_line (char * line, int max, char * buffer)
{
  int z;

  z = (max - strlen(line)) / 2;
  return(sprintf(buffer,"%*s%-*s",z,"",max-z,line));
}

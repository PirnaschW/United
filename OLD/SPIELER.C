#include <string.h>
#include "united.h"

static char * spieler_zeile (SPIELER * s, REIHE r);

void kill_spieler (SPIELER * s)
{
  free(s);
}


SPIELER * new_spieler (MANNSCHAFT * m)
{
  SPIELER * * s = &(m->first);

  while (*s != NULL) s = &((*s)->next);
  *s = malloc(sizeof(SPIELER));
  (*s)->next       = NULL;
  (*s)->name[0]    = 0;
  (*s)->position   = ALLE;
  (*s)->staerke    = 0;
  (*s)->alter      = 0;
  (*s)->eingesetzt = 0;
  (*s)->trainiert  = 0;
  (*s)->disziplinar= 0;
  (*s)->sperre     = 0;
  (*s)->einsatz_t  = 0;
  (*s)->einsatz_a  = 0;
  (*s)->einsatz_v  = 0;
  (*s)->einsatz_m  = 0;
  (*s)->einsatz_s  = 0;
  return(*s);
}


void menu_spieler (MANNSCHAFT * m)
{
  static int c0 = 3;

  static const char * item [] = {
    "Spieler addieren",
    "Spieler aendern",
    "Spieler loeschen",
    "Ende" };

  while (1)
  {
    c0 = menu(4,c0,"SPIELERMENU",item,NULL);
    switch (c0) {
    case 0 : query_add_spieler(m);    break;
    case 1 : query_edit_spieler(m);   break;
    case 2 : query_del_spieler(m);    break;
    default: return;
    }
  }
}


void query_add_spieler (MANNSCHAFT * m)
{
  edit_spieler(new_spieler(m));
}


void query_edit_spieler (MANNSCHAFT * m)
{
  SPIELER * s;

  if ((s = select_spieler(m,"Spieler aendern :",ALLE)) == NULL) return;
  edit_spieler(s);
}


void query_del_spieler (MANNSCHAFT * m)
{
  SPIELER * s;

  if ((s = select_spieler(m,"Spieler loeschen :",ALLE)) == NULL) return;
  del_spieler(m,s);
}

void del_spieler (MANNSCHAFT * m, SPIELER * s)
{
  SPIELER * sp;

  if (s == m->first) m->first = s->next;
  else
  {
    for (sp = m->first; sp != NULL; sp = sp->next) if (sp->next == s) break;
    sp->next = s->next;
  }
  kill_spieler(s);
}


SPIELER * select_spieler (MANNSCHAFT * m, char * title, REIHE r)
{
  int n, z = 0;
  static char * * item = NULL;

  n = build_list_spieler(m,r,&item);
  if (n <= 1) message("Keine Spieler vorhanden !",1);
  else z = menu(n,0,title,item,NULL);
  release_list_spieler(&item,n);

  return(get_spieler(m,r,z));
}


int build_list_spieler (MANNSCHAFT * m, REIHE r, char * * * list)
{
  int i = 0;
  SPIELER * s;

  for (s = m->first; s != NULL; s = s->next) i++;
  if (i == 0) return(0);

  *list = (char * *) malloc(sizeof(char *) * (i+1));

  (*list)[0] = malloc(5);
  strcpy((*list)[0],"Ende");
  i = 1;
  /* 1. Uneingespielte Talente */
  for (s = m->first; r != ALLE && s != NULL; s = s->next)
  {
    if ((s->sperre != 0) || (s->eingesetzt != 0)) continue;
    if (s->alter == 0 && s->staerke == 0 && s->position == r)
      (*list)[i++] = spieler_zeile(s,r);
  }
  /* 2. Eingespielte Spieler */
  for (s = m->first; r != ALLE && s != NULL; s = s->next)
  {
    if ((s->sperre != 0) || (s->eingesetzt != 0)) continue;
    if (eingespielt(s,r) == YES)
      (*list)[i++] = spieler_zeile(s,r);
  }
  /* 3. Uneingespielte Spieler */
  for (s = m->first; r != ALLE && s != NULL; s = s->next)
  {
    if ((s->sperre != 0) || (s->eingesetzt != 0)) continue;
    if ((eingespielt(s,r) == NO) &&
        (r != TORWART) &&
        (s->position != TORWART) &&
        (s->staerke > 0)) /* keine uneingespielten Talente */
      (*list)[i++] = spieler_zeile(s,r);
  }
  /* 4. Alle Spieler */
  for (s = m->first; r == ALLE && s != NULL; s = s->next)
  {
    (*list)[i++] = spieler_zeile(s,r);
  }
  return(i);
}


static char * spieler_zeile (SPIELER * s, REIHE r)
{
  char * ptr;
  static char * format [2] = { "%-*.*s %c%2d (%1d)", "%-*.*s %c%2d" };

  sprintf(ptr=(char *) malloc(LNNAM+11),
          format[r==ALLE||r==s->position ? 1 : 0],
          LNNAM,LNNAM,s->name,
          get_position_char(s->position)[0],
          s->staerke,
          einsaetze(s,r));
  return(ptr);
}


void release_list_spieler (char * * * list, int n)
{
  int i = 0;

  if (list == NULL) return;

  for (i=0; i<n; i++) free((*list)[i]);
  free(*list);
  *list = NULL;
}


SPIELER * get_spieler (MANNSCHAFT * m, REIHE r, int n)
{
  int z;
  SPIELER * s;

  if ((z = n) == 0) return(NULL);
  /* 1. Uneingespielte Talente */
  for (s = m->first; r != ALLE && s != NULL; s = s->next)
  {
    if ((s->sperre != 0) || (s->eingesetzt != 0)) continue;
    if ((s->alter == 0 && s->staerke == 0 && s->position == r) &&
        (--z < 1)) return(s->eingesetzt = 1,s);
  }
  /* 2. Eingespielte Spieler */
  for (s = m->first; r != ALLE && s != NULL; s = s->next)
  {
    if ((s->sperre != 0) || (s->eingesetzt != 0)) continue;
    if ((eingespielt(s,r) == YES) &&
        (--z < 1)) return(s->eingesetzt = 1,s);
  }
  /* 3. Uneingespielte Spieler */
  for (s = m->first; r != ALLE && s != NULL; s = s->next)
  {
    if ((s->sperre != 0) || (s->eingesetzt != 0)) continue;
    if ((eingespielt(s,r) == NO) &&
        (r != TORWART) &&
        (s->position != TORWART) &&
        (s->staerke > 0) && /* keine uneingespielten Talente */
        (--z < 1)) return(s->eingesetzt = 1,s);
  }
  /* 4. Alle Spieler */
  for (s = m->first; r == ALLE && s != NULL; s = s->next)
  {
    if (--z < 1) return(s->eingesetzt = 1,s);
  }

  message("Spieler nicht gefunden...",1);
  return(NULL);
}


void edit_spieler (SPIELER * s)
{
  int z;
  char buffer [LNNAM];
  static int c0 = 13;

  static char * item [] = {
    "Name :                                  ",
    "Stammposition : VERTEIDIGUNG",
    "Staerke                :  00",
    "Alter                  :  00",
    "Bereits trainiert      :  00",
    "Disziplinarpunkte      :  00",
    "Sperren                :  00",
    "Einsaetze                   ",
    "  im Tor               : 000",
    "  als Ausputzer        : 000",
    "  in der Verteidigung  : 000",
    "  im Mittelfeld        : 000",
    "  im Sturm             : 000",
    "Ende" };

  static const char * prompt [] = {
    "Neuer Name : ",
    "Stammposition : ",
    "Staerke : ",
    "Alter : ",
    "Bereits trainiert : ",
    "Disziplinarpunkte : ",
    "Sperren : ",
    NULL,
    "Einsaetze im Tor : ",
    "Einsaetze als Ausputzer : ",
    "Einsaetze in der Verteidigung : ",
    "Einsaetze im Mittelfeld : ",
    "Einsaetze im Sturm : " };

  s->eingesetzt = 0;
  while (1)
  {
    sprintf(item[ 0]+ 7,"%s",s->name);
    sprintf(item[ 1]+16,"%s",get_position_char(s->position));
    sprintf(item[ 2]+26,"%2d",s->staerke);
    sprintf(item[ 3]+26,"%2d",s->alter);
    sprintf(item[ 4]+26,"%2d",s->trainiert);
    sprintf(item[ 5]+26,"%2d",s->disziplinar);
    sprintf(item[ 6]+26,"%2d",s->sperre>1?s->sperre-1:0);
    sprintf(item[ 8]+25,"%3d",s->einsatz_t);
    sprintf(item[ 9]+25,"%3d",s->einsatz_a);
    sprintf(item[10]+25,"%3d",s->einsatz_v);
    sprintf(item[11]+25,"%3d",s->einsatz_m);
    sprintf(item[12]+25,"%3d",s->einsatz_s);
    if ((c0 = menu(14,c0,"Spieler „ndern",item,NULL)) == 13) return;
    if (c0 == 0) get_char(prompt[0],LNNAM-1,buffer);
    else if (c0 == 1) get_position(prompt[1],&(s->position));
    else if (c0 != 7) get_int(prompt[c0],&z);
    switch (c0) {
    case  2 : s->staerke     = z; break;
    case  3 : s->alter       = z; break;
    case  4 : s->trainiert   = z; break;
    case  5 : s->disziplinar = z; break;
    case  6 : s->sperre      = z>0?z+1:0; break;
    case  8 : s->einsatz_t   = z; break;
    case  9 : s->einsatz_a   = z; break;
    case 10 : s->einsatz_v   = z; break;
    case 11 : s->einsatz_m   = z; break;
    case 12 : s->einsatz_s   = z; break;
    }
  }
}


BOOLEAN eingespielt (SPIELER * s, REIHE r)
{
  if (einsaetze(s,r) < EINSPIELEN) return(NO);
  return(YES);
}


int einsaetze (SPIELER * s, REIHE r)
{
  switch (r) {
  case TORWART      : return(s->einsatz_t);
  case AUSPUTZER    : return(s->einsatz_a);
  case VERTEIDIGUNG : return(s->einsatz_v);
  case MITTELFELD   : return(s->einsatz_m);
  case STURM        : return(s->einsatz_s);
  default           : return(0);
  }
}


void get_position (const char * prompt, REIHE * r)
{
  static char * item [5];
  static BOOLEAN first = YES;

  if (first == YES)
  {
    int i;
    first = NO;
    for (i=0; i<5; i++) item[i] = get_position_char(get_reihe(i));
  }
  *r = get_reihe(menu(5,get_reihe_int(*r),prompt,item,NULL));
}


char * get_position_char (REIHE r)
{
  static char * name [] = {
  "Torwart",
  "Ausputzer",
  "Verteidigung",
  "Mittelfeld",
  "Sturm",
  NULL };

  return(name[get_reihe_int(r)]);
}


REIHE get_reihe (int i)
{
  const static REIHE reihe [5] =
    { TORWART, AUSPUTZER, VERTEIDIGUNG, MITTELFELD, STURM };
  return(reihe[i]);
}


int get_reihe_int (REIHE r)
{
  int i;

  for (i=0; i<5; i++) if (r == get_reihe(i)) break;
  return(i);
}


void detach_spieler (MANNSCHAFT * m)
{
  SPIELER * s;

  for (s = m->first; s != NULL; s = s->next)
  {
    s->eingesetzt = 0;
    s->training = 0;
  }
}


long get_wert (SPIELER * s)
{
  int n = 0, i;
  long w = s->staerke * (20 * (5 - s->alter) - s->disziplinar);

  if (w < 0L) w = 0L;
  w *= faktor(s);
  for (i=2; i<5; i++) if (eingespielt(s,get_reihe(i)) == YES) n++;
  switch (n)
  {
  default:
  case 1 : return(w);
  case 2 : return(w*5/4);
  case 3 : return(w*6/4);
  }
}


int faktor (SPIELER * s)
{
  return(((s->position == TORWART) ||
          (s->position == AUSPUTZER)) ? 2 : 1);
}

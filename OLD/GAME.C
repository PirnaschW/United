#include <errno.h>
#include <string.h>
#include "united.h"

void query_new_game (void)
{
  if (check_if_game())
  {
    if (query_kill_game() == E_ABORT) return;
    else kill_game();
  }
  new_game();
}


BOOLEAN check_if_game (void)
{
  if (game == NULL) return(NO);
  return(YES);
}


STATUS query_kill_game (void)
{
  int c0 = 0;
  static const char * item [] = {
    "Spiel speichern",
    "Spiel wegwerfen",
    "Befehl abbrechen" };

  c0 = menu(3,c0,"Ein Spiel ist gerade aktiv.",item,NULL);
  switch (c0) {
  case 0 : save_game(); return(E_SAVED);
  case 1 :              return(E_NOTSAVED);
  default:              return(E_ABORT);
  }
}


void kill_game (void)
{
  MANNSCHAFT * m, *mn;

  m = game->first;
  while (m != NULL)
  {
    mn = m->next;
    kill_mannschaft(m);
    m = mn;
  }
  free(game);
}


void new_game (void)
{
  if (alloc_new_game() == NULL) return;
  get_char("Name des Spiels : ",8,game->name);
  get_char("Name des Spielleiters : ",LNNAM-1,game->master);
}


GAME * alloc_new_game (void)
{
  if ((game = malloc(sizeof(GAME))) == NULL)
    message("Kein Speicherplatz frei !",1);
  else
  {
    game->first = NULL;
    game->name[0] = 0;
    game->master[0] = 0;
    game->mannschaften = 0;
    game->round = 0;
  }
  return(game);
}


void query_load_game (void)
{
  char newgame [9];

  if (check_if_game())
  {
    if (query_kill_game() == E_ABORT) return;
    else kill_game();
  }
  newgame[0] = 0;
  get_char("Name des Spiels : ",8,newgame);
  load_game(newgame);
}


void query_save_game (void)
{
  if (check_if_game() == NO) message("Kein Spiel aktiv !",1);
  else save_game();
}


void save_game (void)
{
  char gname [13], buffer[120];
  FILE * f;
  MANNSCHAFT * m;
  SPIELER * s;

  sprintf(gname,"%.8s.UTD",game->name);
  if ((f = fopen(gname,"w")) == NULL)
  {
    sprintf(buffer,
            "Kann Spiel %s nicht schreiben\n(Fehlercode %d)",
            game->name,
            errno);
    message(buffer,2);
  }

  fprintf(f,"%s\n",game->master);
  for (m = game->first; m != NULL; m = m->next)
  {
    fprintf(f,"%s\n%lg %s\n",m->name,(double)(m->heimvorteil)*0.5,m->owner);
    for (s = m->first; s != NULL; s = s->next)
    {
      fprintf(f,"%c %2d %1d %2d %3d %1d %4d %4d %4d %4d %4d %s\n",
              get_position_char(s->position)[0],
              s->staerke,
              s->alter,
              s->trainiert,
              s->disziplinar,
              s->sperre,
              s->einsatz_t,
              s->einsatz_a,
              s->einsatz_v,
              s->einsatz_m,
              s->einsatz_s,
              s->name);
    }
    fprintf(f,ENDSTRING);
  }

  fclose(f);
}


void load_game (char * name)
{
  int st, al, tr, dz, sp, et, ea, ev, em, es, i;
  double h;
  char gname [13], buffer [120], c;
  FILE * f;
  MANNSCHAFT * m;
  SPIELER * s;

  sprintf(gname,"%.8s.UTD",name);
  if ((f = fopen(gname,"r")) == NULL)
  {
    sprintf(buffer,
            "Spiel %s nicht gefunden oder nicht lesbar\n(Fehlercode %d)",
            name,
            errno);
    message(buffer,2);
  }
  if (alloc_new_game() == NULL) return;

  strcpy(game->name,name);
  fgets(buffer,sizeof(buffer),f);
  sscanf(buffer,"%[^\n]",game->master);
  while(fgets(buffer,sizeof(buffer),f) != NULL)
  {
    m = new_mannschaft();
    sscanf(buffer,"%[^\n]",m->name);
    fgets(buffer,sizeof(buffer),f);
    sscanf(buffer,"%lg %[^\n]",&h,m->owner);
    m->heimvorteil = (int) (2.0 * h);
    while(fgets(buffer,sizeof(buffer),f) != NULL)
    {
      if (strcmp(buffer,ENDSTRING) == 0) break;
      s = new_spieler(m);
      if (sscanf(buffer,
             "%c %d %d %d %d %d %d %d %d %d %d %[^\n]",
             &c,
             &st,
             &al,
             &tr,
             &dz,
             &sp,
             &et,
             &ea,
             &ev,
             &em,
             &es,
             s->name) != 12) message("Fehler beim Lesen des Spiels.",1);
      for (i=0; i<5; i++) if (get_position_char(get_reihe(i))[0] == c) break;
      s->position    = get_reihe(i);
      s->staerke     = st;
      s->alter       = al;
      s->trainiert   = tr;
      s->disziplinar = dz;
      s->sperre      = sp;
      s->einsatz_t   = et;
      s->einsatz_a   = ea;
      s->einsatz_v   = ev;
      s->einsatz_m   = em;
      s->einsatz_s   = es;
    }
  }

  fclose(f);
}

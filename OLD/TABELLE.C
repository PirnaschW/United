#include "united.h"

void show_tabelle (void)
{
  int i, j, max, off;
  char buffer [2000];
  MANNSCHAFT * m, * * array;

  if (check_if_game() == NO)
  {
    message("Kein Spiel aktiv !",1);
    return;
  }
  if (game->round == 0)
  {
    message("Die Saison hat noch nicht begonnen.",1);
    return;
  }

  array = (MANNSCHAFT * *) malloc(sizeof(MANNSCHAFT *) * game->mannschaften);
  for (i = 0, m = game->first; m != NULL; m = m->next) array[i++] = m;

  off = sprintf(buffer,"%-30s   Punkte    Tore  Dif","Mannschaft");
  for (i=0; i<game->mannschaften; i++)
  {
    max = 0;
    for (j=1; j<game->mannschaften; j++)
      if (array[j] == NULL) continue;
      else if (array[max] == NULL) max = j;
      else if (besser(array[j],array[max]) == YES) max = j;

    off += sprintf(buffer+off,"%s",show_tabellen_eintrag(array[max]));
    array[max] = NULL;
  }
  free(array);
  message(buffer,game->mannschaften+1);
}


BOOLEAN besser (MANNSCHAFT * m1, MANNSCHAFT * m2)
{
  int i, p1 [2], p2 [2], t1 [2], t2 [2], d [2];

  for (i=0; i<2; i++)
  {
    p1[i] = m1->punkte[i];
    p2[i] = m2->punkte[i];
    t1[i] = m1->tore[i];
    t2[i] = m2->tore[i];
  }
  d[0] = m1->tore[0] - m1->tore[1];
  d[1] = m2->tore[0] - m2->tore[1];

  if (p1[0] > p2[0]) return(YES);
  if (p1[0] < p2[0]) return(NO);
  if (p1[1] > p2[1]) return(NO);
  if (p1[1] < p2[1]) return(YES);

  if (d[0] > d[1]) return(YES);
  if (d[0] < d[1]) return(NO);
  if (t1[0] > t2[0]) return(YES);
  if (t1[0] < t2[0]) return(NO);

  return(YES);
}


char * show_tabellen_eintrag (MANNSCHAFT * m)
{
  static char buffer [80];

  sprintf(buffer,"\n%-30s  %3d:%3d  %3d:%3d %3d",
          m->name,
          m->punkte[0],
          m->punkte[1],
          m->tore[0],
          m->tore[1],
          m->tore[0] - m->tore[1]);
  return(buffer);
}

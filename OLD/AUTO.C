#include "united.h"

void auto_aufstellung (AUFSTELLUNG * a, REIHE r)
{
  int i, n;
  BOOLEAN alle = YES;

  detach_spieler(a->mannschaft);

  a->torwart    = bester_spieler(a->mannschaft,TORWART);
  a->ausputzer  = bester_spieler(a->mannschaft,AUSPUTZER);

  n = auto_anzahl(VERTEIDIGUNG,r);
  for (i=0; i<n; i++) a->verteidigung[i] = bester_spieler(a->mannschaft,VERTEIDIGUNG);
  a->verteidigung[n] = NULL;

  n = auto_anzahl(MITTELFELD,r);
  for (i=0; i<n; i++) a->mittelfeld[i] = bester_spieler(a->mannschaft,MITTELFELD);
  a->mittelfeld[n] = NULL;

  n = auto_anzahl(STURM,r);
  for (i=0; i<n; i++) a->sturm[i] = bester_spieler(a->mannschaft,STURM);
  a->sturm[n] = NULL;

  for (i=0; i<5; i++) a->haerte[i] = a->heimvorteil[i] = 0;
  if (r == ALLE)
  {
    for (i=2; i<4; i++) a->heimvorteil[i] += a->heim / 3;
    a->heimvorteil[4] += a->heim - a->heim / 3 * 2;
  }
  else a->heimvorteil[get_reihe_int(r)] = a->heim;

  /* Training: */
  for (n=0; n<3; n++)
  {
    if ((a->training[n] = best_training(a->mannschaft,alle)) == NULL) break;
    a->training[n]->training = 1;
    alle = NO;
  }

  if ((a->valid = check_validity(a)) == NO) message("Aufstellung ungueltig.",1);
}


SPIELER * bester_spieler (MANNSCHAFT * m, REIHE r)
{
  SPIELER * s, * s0;

  s0 = NULL;
  for (s = m->first; s != NULL; s = s->next)
  {
    /* Kein Spieler darf zweimal eingesetzt werden */
    if (s->eingesetzt == 1) continue;
    if (s0 == NULL) s0 = s;
    else
    {
      /* Neue Talente immer bevorzugen : */
      if (s->alter == 0 && s->staerke == 0 && s->position == r)
      {
        s0 = s;
        break;
      }
      else
      {
        if (einsatz_staerke(s,r) < einsatz_staerke(s0,r)) continue;
        if (einsatz_staerke(s,r) > einsatz_staerke(s0,r)) s0 = s;
        else
        {
          if (einsaetze(s,r) < einsaetze(s0,r)) continue;
          if (einsaetze(s,r) > einsaetze(s0,r)) s0 = s;
          else
          {
            if (s->alter > s0->alter) continue;
            if (s->alter < s0->alter) s0 = s;
          }
        }
      }
    }
  }
  if (s0 != NULL) s0->eingesetzt = 1;
  return(s0);
}


int auto_anzahl (REIHE aktuell, REIHE betont)
{
  return(betont == ALLE ? 3 : betont == aktuell ? 5 : 2);
}


SPIELER * best_training (MANNSCHAFT * m, BOOLEAN alle)
{
  SPIELER * s, * best = NULL;

  for (s = m->first; s != NULL; s = s->next)
  {
    /* Bereits zum Training angemeldet ? */
    if (s->training == 1) continue;
    /* Bereits auf 10 ? */
    if (s->staerke == 10) continue;
    /* Uneingespieltes Talent ? */
    if (s->alter == 0 && s->staerke == 0) continue;
    /* Training noch erlaubt ? */
    if (s->trainiert >= 3 && s->alter != 0) continue;
    /* Zwei Punkte da ? */
    if (alle == NO && faktor(s) == 2) continue;

    if ((best == NULL) ||
        (best->alter > s->alter) ||
        (best->alter == s->alter && best->staerke < s->staerke))
      best = s;
  }
  return(best);
}

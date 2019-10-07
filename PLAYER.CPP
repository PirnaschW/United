#include "stdafx.h"
#include <malloc.h>
#include "Player.h"
#include "Row.h"

IMPLEMENT_SERIAL(Player,CObject,1);

// Konstruktor :
Player::Player ()
{
  row = new Row;
}

Player::Player (CString player_name)
{
  name = player_name;
  row = new Row;
  staerke    = 0;
  alter      = 0;
  eingesetzt = 0;
  trainiert  = 0;
  rot        = 0;
  gelb       = 0;
  sperre     = 0;
  einsatz_t  = 0;
  einsatz_a  = 0;
  einsatz_v  = 0;
  einsatz_m  = 0;
  einsatz_s  = 0;
}

void Player::Serialize (CArchive & ar)
{
  CObject::Serialize(ar);
  if( ar.IsStoring())
  {
    ar << name;
    ar << staerke;
    ar << alter;
    ar << trainiert;
    ar << gelb;
    ar << rot;
    ar << sperre;
    ar << eingesetzt;
    ar << training;
    ar << einsatz_t;
    ar << einsatz_a;
    ar << einsatz_v;
    ar << einsatz_m;
    ar << einsatz_s;
    if (row->Get() == NONE)
    {
      if      (UsedTo(TORWART     )) row->Set(TORWART);
      else if (UsedTo(AUSPUTZER   )) row->Set(AUSPUTZER);
      else if (UsedTo(VERTEIDIGUNG)) row->Set(VERTEIDIGUNG);
      else if (UsedTo(MITTELFELD  )) row->Set(MITTELFELD);
      else if (UsedTo(STURM       )) row->Set(STURM);
    }
  }
  else
  {
    ar >> name;
    ar >> staerke;
    ar >> alter;
    ar >> trainiert;
    ar >> gelb;
    ar >> rot;
    ar >> sperre;
    ar >> eingesetzt;
    ar >> training;
    ar >> einsatz_t;
    ar >> einsatz_a;
    ar >> einsatz_v;
    ar >> einsatz_m;
    ar >> einsatz_s;
  }
  row->Serialize(ar);
}

const char * Player::FormationLine (const ROW r) const
{
  static char buffer [45];
  sprintf(buffer,"%d\t%-.10s\t%c%d",StrengthAt(r),(const char *) name,row->FirstChar(),(int) staerke);
  return buffer;
}

BOOL Player::MayStart (const ROW r) const
{
  if (((r == TORWART) ^ (row->Get() == TORWART)) ||
      (StrengthAt(r) < 0)) return FALSE;
  else return TRUE;
}

int  Player::StrengthAt (const ROW r) const
{
  if (row->Get() == r) return (int) staerke; /* auf Stammposition */

  int effektiv = (int) staerke;
  if (UsedTo(r) == FALSE) switch (r)
  {
  case TORWART      : effektiv = 0; break;
  case AUSPUTZER    : effektiv = ((int) staerke + (UsedTo(VERTEIDIGUNG) == TRUE ? 1 : 0)) / 3; break;
  case VERTEIDIGUNG : if (UsedTo(AUSPUTZER) == TRUE) effektiv++;
  case MITTELFELD   :
  case STURM        : effektiv--; break;
  }
  else
    if (r == AUSPUTZER)
    {
      if (UsedTo(VERTEIDIGUNG) == TRUE)
        effektiv =  (int) staerke    / 2;
      else
        effektiv = ((int) staerke-1) / 2;
    }

  return(effektiv);
}

int Player::EstimateAt (const ROW r) const
{
  /* Neue Talente immer bevorzugen : */
  if (IsLearner(r)) return 3000 + PlayedGames(r);

  return 200 * StrengthAt(r) +
          20 * (UsedTo(r) ? 7 : PlayedGames(r)) +
          10 - (int) alter;
}

BOOL Player::IsLearner (const ROW r) const
{
  return alter == 0 && staerke == 0 && row->Get() == r ? TRUE : FALSE;
}

BOOL Player::UsedTo (const ROW r) const
{
  if (PlayedGames(r) < 6) return(FALSE);
  else return(TRUE);
}

int Player::PlayedGames (const ROW r) const
{
  switch (r)
  {
    case TORWART      : return (int) einsatz_t;
    case AUSPUTZER    : return (int) einsatz_a;
    case VERTEIDIGUNG : return (int) einsatz_v;
    case MITTELFELD   : return (int) einsatz_m;
    case STURM        : return (int) einsatz_s;
    default           : return       0;
  }
}

void Player::SkillIncrease ()
{
  switch (eingesetzt)
  {
    case TORWART      : einsatz_t++; break;
    case AUSPUTZER    : einsatz_a++; break;
    case VERTEIDIGUNG : einsatz_v++; break;
    case MITTELFELD   : einsatz_m++; break;
    case STURM        : einsatz_s++; break;
    default           :              break;
  }
}

char * Player::Penalty (const BOOL red)
{
  int n1 = (10 * (int) rot + 4 * (int) gelb) / 10, n2;

  if (red) rot++; else gelb++;
  n2 = (10 * (int) rot + 4 * (int) gelb) / 10;
  if (n2 != n1)
  {
    switch(n2)
    {
      case 1 : sperre =  2; return  "1 Spiel";
      case 2 : sperre =  4; return  "3 Spiele";
      default: sperre = -1; return  "die ganze Saison";
    }
  }
  return NULL;
}

void Player::Aging ()
{
  switch (alter++)
  {
    default : staerke -= 1;
    case  2 : staerke -= 2;
    case  1 : staerke -= 2;
    case  0 : return;
  }
}

#ifdef NEVER
char * Player::DataLine (const Row & r) const
{
  static char buffer [20];

  sprintf(buffer,"%s %d (%d)",
    alter == 0 ? "nT" : roman(alter),
    EinsatzStaerke(r),Einsaetze(r));
  return(buffer);
}

long Player::GetWert () const
{
  int n = 0, i;
  long w = staerke * (20 * (5 - alter) - disziplinar);
  Row r;

  if (w < 0L) w = 0L;
  w *= Faktor();
  for (i=2; i<5; i++)
  {
    r.Set(i);
    if (Eingespielt(r) == TRUE) n++;
  }
  switch (n)
  {
  default:
  case 1 : return(w);
  case 2 : return(w*5/4);
  case 3 : return(w*6/4);
  }
}

int Player::Faktor () const
{
  return(((row.Get() == TORWART) ||
          (row.Get() == AUSPUTZER)) ? 2 : 1);
}

void Player::Entsperren ()
{
  if (sperre != 0 && sperre != 7) sperre--;
}

BOOL Player::Altern ()
{
  unsigned int d;

  switch (alter++)
  {
  case  0 : d = 0; break;
  case  1 : d = 2; break;
  case  2 : d = 4; break;
  default : d = 5; break;
  }

  if (staerke <= d) return(FALSE);
  staerke -= d;
  return(TRUE);
}
#endif

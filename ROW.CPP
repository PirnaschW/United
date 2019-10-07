#include "stdafx.h"
#include "Row.h"

static char * name [] = {
  "Torwart",
  "Ausputzer",
  "Verteidigung",
  "Mittelfeld",
  "Sturm",
  "Undefiniert"
};

IMPLEMENT_SERIAL(Row,CObject,1);

Row::Row ()
{
  row = ALLE;
}

Row::Row (const int i)
{
  row = (ROW) i;
}

Row::Row (const ROW r)
{
  row = r;
}

void Row::Set (const ROW r)
{
  row = r;
}

ROW Row::Get () const
{
  return row;
}

int Row::AsInt () const
{
  return (int) this->row;
}

char * Row::AsChar () const
{
  return(name[AsInt()]);
}

char Row::FirstChar () const
{
  return AsChar()[0];
}

void Row::Serialize (CArchive & ar)
{
  BYTE dummy;

  CObject::Serialize(ar);
  if( ar.IsStoring()) ar << (BYTE) row;
  else              { ar >> dummy; row = (ROW) dummy; }
}

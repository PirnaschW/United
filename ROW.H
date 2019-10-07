#ifndef _ROW_H_
#define _ROW_H_

#include "stdafx.h"

typedef enum {
  TORWART      = 0,
  AUSPUTZER    = 1,
  VERTEIDIGUNG = 2,
  MITTELFELD   = 3,
  STURM        = 4,
  ALLE         = 5,
  NONE         =-1,
} ROW;

class Row : public CObject
{
  DECLARE_SERIAL(Row);
  protected:
    ROW row;
  public:
    Row ();
    Row (const int i);
    Row (const ROW r);
    void Set (const ROW r);
    ROW Get () const;
    int AsInt () const;
    char * AsChar () const;
    char FirstChar () const;
    virtual void Serialize (CArchive & ar);
};

#endif

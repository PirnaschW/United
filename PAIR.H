#ifndef _PAIR_H_
#define _PAIR_H_

#include "Team.h"

class Pair
{
  protected:
    Team * t [2];
  protected:
    Pair ();
  public:
    Pair (Team * t1, Team * t2);
    void PlayMatch ();
};

#endif

#ifndef _TEAMA_H_
#define _TEAMA_H_

#include "stdafx.h"
class Team;

class TeamArray : public CObject
{
  DECLARE_SERIAL(TeamArray);
  public:
    CObArray teams;

  public:
    TeamArray ();
    ~TeamArray ();
    int  AddTeam    (const char * name);
    void DelTeam    (const int index);
    void ListTeams  (CComboBox * c) const;
    BOOL TeamExists (const char * name) const;
    BOOL TeamsExist () const;
    Team * GetTeam  (const int i) const;
    virtual void Serialize (CArchive & ar);
    void ResetTabelle   ();
    void RandomSequence ();
};

#endif

#ifndef _TEAM_H_
#define _TEAM_H_

#include "stdafx.h"
#include "Row.h"
class PlayerArray;
class Player;

class Team : public CObject
{
  DECLARE_SERIAL(Team);
  public:
    PlayerArray * players;
    CString name;
    CString trainer;
    LONG training;
    LONG heimvorteil;
    LONG punkte[2];
    LONG tore[2];
    BOOL human;
    Row * preferred;
    LONG hb[5];
    LONG ha[5];
  public:
    Team ();
    Team (CString team_name);
    ~Team ();
    virtual void Serialize (CArchive & ar);

    BOOL HasPlayers    () const;
    int GetHeimvorteil () const;
    void ResetTabelle  ();
    BOOL FormationCheck() const;
    int  RowSum        (const ROW r, int * count) const;
    void HomeBonusAdjust(const int p);
    void SkillsIncrease();
#ifdef NEVER
     Player * GetPlayer (const int n) const;
     Player * GetPlayer (const char * name) const;
     int  GetPlayerIndex (const char * name) const;
     void AddPlayer (const char * name);
     void DeletePlayer (const char * name);
    int GetHeimvorteil () const;
    void Show () const;
    void Reset () const;
    void Detach () const;
    BOOL IsValid () const;
    Player * PlayerAdd ();
    void PlayerEdit ();
    void PlayerDelete (Player * s);
    Player * PlayerSelect (const char * title, const Reihe & r) const;
    Player * BesterPlayer (const Reihe & reihe) const;
    Player * BestTraining  (const BOOL alle) const;
    BOOL operator > (const Team & m) const;
    void AdjustHeimvorteil (const int p);
    char * TabellenZeile () const;
    void Altern ();
    void CreateTalente ();
    void ResetTabelle ();
    void Entsperren () const;
#endif
};

#endif

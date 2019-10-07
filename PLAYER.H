#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "stdafx.h"
#include "Row.h"

class Player : public CObject
{
  DECLARE_SERIAL(Player);
  public:
    CString name;
    Row * row;
    LONG staerke;
    LONG alter;
    LONG trainiert;
    LONG gelb;
    LONG rot;
    LONG sperre;
    LONG eingesetzt;
    LONG training;
    LONG einsatz_t;
    LONG einsatz_a;
    LONG einsatz_v;
    LONG einsatz_m;
    LONG einsatz_s;

  public:
    Player ();
    Player (CString player_name);
    virtual void Serialize (CArchive & ar);
    const char * FormationLine (const ROW r) const;
    BOOL MayStart   (const ROW r) const;
    int  StrengthAt (const ROW r) const;
    int  EstimateAt (const ROW r) const;
    BOOL IsLearner  (const ROW r) const;
    BOOL UsedTo     (const ROW r) const;
    int  PlayedGames(const ROW r) const;
    void SkillIncrease ();
    char * Penalty  (const BOOL red);
    void Aging      ();
#ifdef NEVER
    void Edit ();
    BOOL Eingespielt (const Row & r) const;
    int Einsaetze (const Row & r) const;
    void Detach ();
    long GetWert () const;
    int Faktor () const;
    Player * Next() const;
    void SetNext (Player *);
    void SetTraining (const int i);
    void SetEingesetzt (const int i);
    void SetRow (const Row & r);
    int Show (char * buffer) const;
    void Reset ();
    BOOL Einsetzbar () const;
    BOOL UneingespieltesTalent (const Row & r) const;
    BOOL Uneingespielt (const Row & r) const;
    char * Zeile (const Row & r) const;
    char * DataLine (const Row & r) const;
    int EinsatzStaerke (const Row & r) const;
    int EinsatzWert (const Row & r) const;
    void NotiereEinsatz (const Row & r);
    void Entsperren ();
    void Strafe (const int s);
    BOOL Altern ();
#endif
};

#endif

#ifndef _LIGA_H_
#define _LIGA_H_

#include "stdafx.h"

typedef enum { MODUS_UNDEF, MODUS_ROBIN, MODUS_GROUP, MODUS_SWISS } Modus;

class TeamArray;
class Pair;

class Liga : public CObject
{
  DECLARE_SERIAL(Liga);
  public:
    TeamArray * teams;
    CString name;
    CString master;
    Modus modus;
    BYTE modus_param;
    BYTE round;
    BOOL round_active;
    Pair * pair;
    int counter;

  public:
    static Liga * LigaNew (const BOOL getname = FALSE);

  protected:
    Liga ();
  public:
    Liga (CString liganame);
    ~Liga ();
    virtual void Serialize (CArchive & ar);
    BOOL HasTeams () const;
    int  GetTeamsSize() const;

    char * Hint     ();
    void EditData   ();
    void SeasonStart();
    void RoundBegin ();
    void RoundEnd   ();
    int  ChartShow  (CDC * pDC);
    void FormationDefine ();
    Pair * PairGet (int count, int counter);

#ifdef NEVER
    void Reset () const;
    BOOL SaisonStarted () const;
    void InitSaison ();
    void PlayRound ();
    void ShowTabelle () const;
    void Altern () const;
    void CreateTalente () const;
    void ResetTabelle () const;
    void RandomSequence ();
    BOOL IsValid () const;
    void SpieltagStart ();
    void SpieltagEnde ();
    BOOL GetPaarung ();
#endif
};

#endif

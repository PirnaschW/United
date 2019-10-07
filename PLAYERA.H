#ifndef _PLAYERA_H_
#define _PLAYERA_H_

#include "stdafx.h"
class Player;

class PlayerArray : public CObject
{
  DECLARE_SERIAL(PlayerArray);
  public:
    CObArray players;

  public:
    PlayerArray ();
    ~PlayerArray ();
    int  AddPlayer    (CString name);
    void DelPlayer    (const int index);
    void ListPlayers  (CComboBox * c) const;
    BOOL PlayerExists (CString name) const;
    BOOL PlayersExist () const;
    Player * GetPlayer (const int i) const;
    virtual void Serialize (CArchive & ar);
    void ResetSperren ();
};

#endif

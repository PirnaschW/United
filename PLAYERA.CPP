#include "stdafx.h"
#include "PlayerA.h"
#include "Player.h"

IMPLEMENT_SERIAL(PlayerArray,CObject,1);

         PlayerArray::PlayerArray  ()  {}
         PlayerArray::~PlayerArray ()  { for (int i= 0; i < players.GetSize(); i++) delete players[i]; }
int      PlayerArray::AddPlayer    (CString name) { players.Add(new Player(name)); return players.GetSize() - 1; }
void     PlayerArray::DelPlayer    (const int index)   { delete (Player *) players[index];  players.RemoveAt(index); }
BOOL     PlayerArray::PlayersExist () const { return players.GetSize() > 0 ? TRUE : FALSE; }
Player * PlayerArray::GetPlayer    (const int i) const { return (Player *) players[i]; }
void     PlayerArray::Serialize    (CArchive & ar)
{
  CObject::Serialize(ar);
  if( ar.IsStoring())
  {
    ar << (WORD) players.GetSize();
    for (int i=0; i < players.GetSize(); i++) players[i]->Serialize(ar);
  }
  else
  {
    WORD n;
    ar >> n;
    for (WORD i=0; i < n; i++)
    {
      players.Add(new Player());
      players[i]->Serialize(ar);
    }
  }
}

void     PlayerArray::ListPlayers (CComboBox * c) const
{
  c->ResetContent();
  for (int i = 0; i < players.GetSize(); i++)
    c->AddString(((Player *)players[i])->name);
  c->SetCurSel(players.GetSize()-1);
}

BOOL     PlayerArray::PlayerExists (CString name) const
{
  for (int i = 0; i < players.GetSize(); i++)
    if (((Player *) players[i])->name == name) return TRUE;
  return FALSE;
}

void     PlayerArray::ResetSperren ()
{
  for (int i = 0; i < players.GetSize(); i++)
    ((Player *) players[i])->rot = ((Player *) players[i])->gelb = ((Player *) players[i])->sperre = 0;
}

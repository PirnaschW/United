#include "stdafx.h"
#include "TeamA.h"
#include "Team.h"
#include "PlayerA.h"

IMPLEMENT_SERIAL(TeamArray,CObject,1);

       TeamArray::TeamArray  () {}
       TeamArray::~TeamArray () { for (int i=0; i < teams.GetSize(); i++) delete teams[i]; }
int    TeamArray::AddTeam    (const char * name) { teams.Add(new Team(name)); return teams.GetSize() - 1; }
void   TeamArray::DelTeam    (const int index)   { delete (Team *) teams[index];  teams.RemoveAt(index); }
BOOL   TeamArray::TeamsExist () const { return teams.GetSize() > 0 ? TRUE : FALSE; }
Team * TeamArray::GetTeam    (const int i) const { return (Team *) teams[i]; }
void   TeamArray::Serialize  (CArchive & ar)
{
  CObject::Serialize(ar);
  if( ar.IsStoring())
  {
    ar << (WORD) teams.GetSize();
    for (int i=0; i < teams.GetSize(); i++) teams[i]->Serialize(ar);
  }
  else
  {
    WORD n;
    ar >> n;
    for (WORD i=0; i < n; i++)
    {
      teams.Add(new Team());
      teams[i]->Serialize(ar);
    }
  }
}

void TeamArray::ListTeams (CComboBox * c) const
{
  c->ResetContent();
  for (int i = 0; i < teams.GetSize(); i++)
    c->AddString(((Team *)teams[i])->name);
  c->SetCurSel(teams.GetSize()-1);
}

BOOL TeamArray::TeamExists (const char * name) const
{
  for (int i = 0; i < teams.GetSize(); i++)
    if (((Team *)teams[i])->name == name) return TRUE;
  return FALSE;
}

void TeamArray::ResetTabelle   ()
{
  for (int i = 0; i < teams.GetSize(); i++)
  {
    ((Team *) teams[i])->ResetTabelle();
    ((Team *) teams[i])->players->ResetSperren();
  }
}
void TeamArray::RandomSequence ()
{
  int n = teams.GetSize();
  CObject * t;
  for (int i = 0; i < n-1; i++)
  {
    int j = i+rndint(n-i)-1;
    t = teams[i];
    teams[i] = teams[j];
    teams[j] = t;
  }
}

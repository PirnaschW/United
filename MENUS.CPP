#include "stdafx.h"
#include "Liga.h"

BOOL CUnitedView::IfLiga       () { return (GetDocument()->liga == NULL) ? FALSE : TRUE; }
BOOL CUnitedView::IfNotStarted () { return (GetDocument()->liga == NULL || GetDocument()->liga->modus != 0) ? FALSE : TRUE ; }
BOOL CUnitedView::IfStarted    () { return (GetDocument()->liga != NULL && GetDocument()->liga->modus != 0) ? TRUE  : FALSE; }
BOOL CUnitedView::IfNoRound    () { return (IfStarted() && !GetDocument()->liga->round_active) ? TRUE  : FALSE; }
BOOL CUnitedView::IfRound      () { return (IfStarted() &&  GetDocument()->liga->round_active) ? TRUE  : FALSE; }

void CUnitedView::OnUpdateLigaNew                   (CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
void CUnitedView::OnUpdateLigaOpen                  (CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
void CUnitedView::OnUpdateLigaEdit                  (CCmdUI* pCmdUI) { pCmdUI->Enable(IfNotStarted()); }
void CUnitedView::OnUpdateLigaPrint                 (CCmdUI* pCmdUI) { pCmdUI->Enable(IfLiga()); }
void CUnitedView::OnUpdateLigaSave                  (CCmdUI* pCmdUI) { pCmdUI->Enable(IfLiga()); }
void CUnitedView::OnUpdateLigaSaveAs                (CCmdUI* pCmdUI) { pCmdUI->Enable(IfLiga()); }
void CUnitedView::OnUpdateLigaPrevw                 (CCmdUI* pCmdUI) { pCmdUI->Enable(IfLiga()); }
void CUnitedView::OnUpdatePrintSetup                (CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
void CUnitedView::OnUpdateSaisonStarten             (CCmdUI* pCmdUI) { pCmdUI->Enable(IfNotStarted()); }
void CUnitedView::OnUpdateSaisonSpieltagbeginnen    (CCmdUI* pCmdUI) { pCmdUI->Enable(IfNoRound()); }
void CUnitedView::OnUpdateSaisonAufstellungfestlegen(CCmdUI* pCmdUI) { pCmdUI->Enable(IfRound()); }
void CUnitedView::OnUpdateSaisonSpieltagbeenden     (CCmdUI* pCmdUI) { pCmdUI->Enable(IfRound()); }

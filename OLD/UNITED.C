#include <string.h>
#include "united.h"

GAME * game = NULL;

void hexdump (void);
void hexdump (void) {}


int main (void)
{
  int i, c0 = 0;
  char logo [500];

  static const char * item [] = {
    "Neues Spiel",
    "Spiel laden",
    "Spiel speichern",
    "Daten aendern",
    "Saison starten",
    "Naechster Spieltag",
    "Tabelle anzeigen",
    "Ende",
    "Copyright" } ;

  static const char * _logo [] = {
  "\n",
  "   л   л лл  л ллллл ллллл ллллл лллл\n",
  "   л   л л л л   л     л   л     л   л   \n",
  "   л   л л л л   л     л   ллл   л   л\n",
  "   л   л л л л   л     л   л     л   л\n",
  "    ллл  л  лл ллллл   л   ллллл лллл\n\n\n",
  "        Fuсball-Simulations-Spiel\n\n",
  "           Programmed by W.P.\n",
  "   Version 1.00   10.06.94 - 20.07.94" };

  for (logo[0]=i=0; i<9; i++) strcat(logo,_logo[i]);
  running_message(logo,13,41);

/* Hauptmenu-Schleife : */
  while (1) {
    secure_check();
    message("done.",1);
    c0 = menu(9,c0,"UNITED MAIN MENU",item,NULL);
    switch (c0) {
    case 0 : query_new_game();    break; /* new */
    case 1 : query_load_game();   break; /* load */
    case 2 : query_save_game();   break; /* save */
    case 3 : menu_mannschaften(); break; /* edit */
    case 4 : init_season();       break; /* init */
    case 5 : play_round();        break; /* round */
    case 6 : show_tabelle();      break; /* chart */
    case 7 :
      if (check_if_game())
      {
        if (query_kill_game() == E_ABORT) break;
        else kill_game();
      }
      return(0);
    case 8 : message(logo,13);           break; /* show logo */
    }
  }
}


#ifdef NEVER
void save (MANNSCHAFT * pr)
{
  static MANNSCHAFT * ptr;

  #include "io.h"



}
#endif

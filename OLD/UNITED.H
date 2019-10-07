#include <stdio.h>
#include "util.h"

#define  LNNAM                       30
#define  EINSPIELEN                   6
#define  UNEINGESPIELT                1
#define  ENDSTRING                   "ENDE MANNSCHAFT\n"

typedef enum {
  E_SAVED,
  E_NOTSAVED,
  E_ABORT,
} STATUS;


typedef enum {
  ALLE,
  TORWART,
  AUSPUTZER,
  VERTEIDIGUNG,
  MITTELFELD,
  STURM
} REIHE;


typedef struct Spieler {
  struct Spieler * next;
  char name [LNNAM];
  REIHE position;
  unsigned staerke    : 4;
  unsigned alter      : 3;
  unsigned trainiert  : 4;
  unsigned disziplinar: 6;
  unsigned sperre     : 3;
  unsigned eingesetzt : 1;
  unsigned training   : 1;
  unsigned char einsatz_t;
  unsigned char einsatz_a;
  unsigned char einsatz_v;
  unsigned char einsatz_m;
  unsigned char einsatz_s;
} SPIELER;


typedef struct Mannschaft{
  struct Mannschaft * next;
  char name [LNNAM];
  char owner [LNNAM];
  SPIELER * first;
  int training;
  int heimvorteil;
  int punkte[2];
  int tore[2];
  BOOLEAN human;
  REIHE bevorzugt;
} MANNSCHAFT;


typedef struct Aufstellung {
  MANNSCHAFT * mannschaft;
  SPIELER * torwart;
  SPIELER * ausputzer;
  SPIELER * verteidigung [6];
  SPIELER * mittelfeld   [6];
  SPIELER * sturm        [6];
  SPIELER * training     [3];
  int haerte [5];
  int heim;
  int heimvorteil [5];
  BOOLEAN valid;
} AUFSTELLUNG;


typedef struct Game {
  char name [LNNAM];
  char master [LNNAM];
  MANNSCHAFT * first;
  int mannschaften;
  int modus;
  int round;
} GAME;

extern GAME * game;

void query_new_game (void);
BOOLEAN check_if_game (void);
STATUS query_kill_game (void);
void kill_game (void);
void kill_mannschaft (MANNSCHAFT * m);
void kill_spieler (SPIELER * s);
void new_game (void);
GAME * alloc_new_game (void);
void query_load_game (void);
void query_save_game (void);
void save_game (void);
void load_game (char * name);
MANNSCHAFT * new_mannschaft (void);
SPIELER * new_spieler (MANNSCHAFT * m);
void menu_mannschaften (void);
void query_add_mannschaft (void);
void query_edit_mannschaft (void);
void query_show_mannschaft (void);
void query_del_mannschaft (void);
void query_menu_spieler (void);
void edit_mannschaft (MANNSCHAFT * m);
void del_mannschaft (MANNSCHAFT * m);
MANNSCHAFT * select_mannschaft (void);
int build_list_mannschaft (char * * * list);
MANNSCHAFT * get_mannschaft (char * name);
void show_mannschaft (MANNSCHAFT * m);
void menu_spieler (MANNSCHAFT * m);
void query_add_spieler (MANNSCHAFT * m);
void query_ren_spieler (MANNSCHAFT * m);
void query_edit_spieler (MANNSCHAFT * m);
void query_del_spieler (MANNSCHAFT * m);
void del_spieler (MANNSCHAFT * m, SPIELER * s);
SPIELER * select_spieler (MANNSCHAFT * m, char * title, REIHE r);
int build_list_spieler (MANNSCHAFT * m, REIHE r, char * * * list);
SPIELER * get_spieler (MANNSCHAFT * m, REIHE r, int z);
BOOLEAN eingespielt (SPIELER * s, REIHE r);
int einsaetze (SPIELER * s, REIHE r);
void release_list_spieler (char * * * list, int n);
void edit_spieler (SPIELER * m);
void get_position (const char * prompt, REIHE * r);
char * get_position_char (REIHE r);
REIHE get_reihe (int i);
int get_reihe_int (REIHE r);
void init_season (void);
int count_mannschaften (void);
BOOLEAN season_started (void);
void reset_tabelle (void);
void reset_mannschaften (void);
void create_talente (void);
BOOLEAN check_mannschaften (void);
BOOLEAN mannschaft_valid (MANNSCHAFT * m);
void random_sequence (void);
void play_round (void);
void init_round (int n);
BOOLEAN get_paarung (MANNSCHAFT * * m1, MANNSCHAFT * * m2);
AUFSTELLUNG * get_aufstellung (MANNSCHAFT * m, BOOLEAN heim);
void auto_aufstellung (AUFSTELLUNG * a, REIHE r);
SPIELER * bester_spieler (MANNSCHAFT * m, REIHE r);
SPIELER * best_training (MANNSCHAFT * m, BOOLEAN alle);
int auto_anzahl (REIHE aktuell, REIHE betont);
BOOLEAN check_validity (AUFSTELLUNG * a);
void sum_reihe (AUFSTELLUNG * a, int * reihe, BOOLEAN heim);
void hand_aufstellung (AUFSTELLUNG * a);
void show_aufstellung (AUFSTELLUNG * a);
char * data_line (SPIELER * s, REIHE r);
int center_line (char * line, int max, char * buffer);
void detach_spieler (MANNSCHAFT * m);
void play_match (AUFSTELLUNG * * a);
SPIELER * get_aufgestellt (AUFSTELLUNG * a, int n, REIHE * r);
int einsatz_staerke (SPIELER * s, REIHE r);
void note_einsaetze (AUFSTELLUNG * a);
void increase_einsaetze (SPIELER * s, REIHE r);
void altern (void);
void show_tabelle (void);
BOOLEAN besser (MANNSCHAFT * m1, MANNSCHAFT * m2);
char * show_tabellen_eintrag (MANNSCHAFT * m);
long get_wert (SPIELER * s);
void strafe (SPIELER * s, int punkte);
void adjust_heimvorteil (MANNSCHAFT * m, int p);
int faktor (SPIELER * s);

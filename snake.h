#ifndef _SNAKE_H
#define _SNAKE_H

/* Globale Variabeln */

extern int hindernis_check;
extern int check;  /* bool 0: kein * getroffen, 1: * getroffen -> führe nicht \
	     * del(first) aus. */
extern char eingabe; // richtungseingabe
extern int geschwindigkeit; 
extern int len_x;
extern int len_y;
extern int score;
extern int level;
extern int sync_del_special_point; /* bool 0: '$' wurde nich erwischt
			       1: '$' wurde erwischt, lösche pos($) nicht */
extern int special_point_active;

/* int sync_blinken; /\* synchronisiere special point blinken. *\/  */

struct snake {
  int xachse;
  int yachse;
  struct snake *next; 
  struct snake *previous;
  };

struct koordinaten {
  int x;
  int y;
};

extern struct koordinaten arg;

extern struct snake *anfang;
extern struct snake *next;
extern struct snake *ende;

void anhaengen(int xachse, int yachse);
void x_move(int richtung,char **playfield);
void y_move(int richtung,char **playfield);
void snake_create_playground(char **playfield);
void snake_print_out(char **playfield);
void create_snake(char **playfield);
void del_first(char **playfield);
void move_snake(char **playfield);


int new_tty (int fd);
int restore_tty (int fd);
void *read_stdin();
void game_over();
int zufallsauswahl(int minimum, int maximum); 
void create_hindernis(char **playfield);
int get_cols();
int get_lines();
void set_geschwindigkeit(int eingabe);
void special_point( pthread_t th1, pthread_t th2, char **playfield);
void *del_special_point(void *param);
void special_point_blink(void *param);

#endif

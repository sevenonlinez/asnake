/* Globale Variabeln */

extern char playfield[1000][1000];
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

/* int sync_blinken; /\* synchronisiere special point blinken. *\/  */

// int special_point_active;  /* bool: 0: nicht active
//			      1: active  */

extern struct koordinaten arg;



void anhaengen(int xachse, int yachse);
void snake_create_playground();
void snake_print_out();
void create_snake();
void del_first();
int new_tty (int fd);
int restore_tty (int fd);
void x_move(int richtung);
void y_move(int richtung);
void move_snake();
void *read_stdin();
void game_over();
int zufallsauswahl(int minimum, int maximum); 
void create_hindernis();
int get_cols();
int get_lines();
void set_geschwindigkeit(int eingabe);
void special_point( pthread_t th1, pthread_t th2);
void *del_special_point(/* struct koordinaten *point */);

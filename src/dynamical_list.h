#ifndef _DYNAMICAL_LIST_H
#define _DYNAMICAL_LIST_H

int special_point_active;
static int counter_special_point=0;
int hindernis_check;
int sync_del_special_point;


struct point {
	int x;
	int y;
};

struct snake_link {
	struct point pos;
    struct snake_link *next;
    struct snake_link *previous;
};

struct snake {
	struct point movement;
    struct snake_link *head;
	struct snake_link *tail;
};

void snake_create_snake (char **playfield, struct point start_point, struct snake *snake1, int lenght);
void snake_del_tail_link (char **playfield, struct snake *snake1);
void snake_append_head_link (char **playfield, struct point pos, struct snake *snake1);
void snake_move_snake(char **playfield, struct snake *snake1);



#endif // _DYNAMICAL_LIST_H

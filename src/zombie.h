/*
==============================================
zombie.h
==============================================
*/

#include "map.h"
#include "macros.h"

/* in zombie.c */
void intro_splash(void);
void intro_story(void);
void street_normal(char *);
void street_w_zombie (char *location);
void menu_main (void);
int encounter (enum direction);
void melee(int);
void enter_building(void);
void draw_entrance(void);
void drawZombie(int y, int x);
void hitZombie(int y, int x);
void death_check(void);
void write_score (void);
void confirm_exit(int *);
void show_inventory(void);
void flashZombie(int y, int x);
void people(void);
void dead_body(void);
void add_item(int);
void show_todo_list(void);
void high_scores(void);
void check_experience(void);

void check_camping(void);
boolean special_item(int, int *);

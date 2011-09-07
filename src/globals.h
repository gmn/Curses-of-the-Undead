/*
============================================
globals.h
============================================
*/

//#include "defs.h"
//#include "environ.h"

// all global vars should go here
int city_map[CITY_SIZE];
int city_seen[CITY_SIZE];
int row, col;
int place = 		0;
int dead = 		0;
int turn_num = 		0;
int start_hour = 	15;
int inventory[20];
int inv_size = 		15;
int menuborder = 	35;
int health = 		100;
int money = 		25;
int experience = 	0;
int party_size = 	1;
int max_party = 	12;
int MAP_MODE = MODE_CITY;
int MAP_SIZE = CITY_SIZE;
int SIZE_W = CITY_W;
int SIZE_H = CITY_H;
int MAX_PLACES = 0;
int *MAP_P = city_map;
int *SEEN_P = city_seen;
int water = 0;
int cellphone = 0;
int turnstileticket = 0;
int boatkeys = 0;
int working_cellphone = 0;

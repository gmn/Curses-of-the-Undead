/*
===========================================
map.c
===========================================
*/

#include <string.h>
#include <stdlib.h>

#include "defs.h"
#include "map.h"

#define SIZEOF_ARRAY(x)  (sizeof(x)/sizeof(x[0])) 

extern int city_map[];
extern char * place_major[];
extern int special_place[];
extern int MAP_SIZE;
extern int SIZE_W;
extern int SIZE_H;
extern int MAP_MODE;
extern int MAX_PLACES;
extern int place;
//extern int city_seen[];
extern int *SEEN_P;
extern int *MAP_P;
extern char *major_letter[];


/*
** map array coordinate orientation:
** top, left to right, to bottom
*/

/*
** general map info:  
** the map is bordered to the west and north
** by a lake.  The sw corner there is an entrance to the freeway
** which is where the player starts.  the east side and bottom is 
** bordered by a mountain and raised freeway.  
 */

/* outlay:
** - a park of 9 squares goes anywhere.   
** - The pier goes on the northside, picked at random.
** - the freeway entrance goes in sw corner, or nearest sw sq
**   on southern border
** then,
** - lay down all of the other special tiles, one at a time on
**   empty squares
** - fill in the remaining empty squares with buildings and
**   apartments and neighborhoods.
 */

// finds instance of string s in t[] (array of strings).  
// t is of size sz.  returns index of place or -1 if not found
int strstrp (char **t, char *s, int sz)
{
	int i, index = -1;
	char *x;
	boolean found = false;

	for (i = 0; i < sz && !found; i++) {
		x = strstr(t[i], s);
		if ((x != NULL) && !(strcmp(s, x))) {
			found = true;
			index = i;
		}
	}
	return index;
}


boolean on_west_side(int x)
{
	if ( (!(x >= 0)) || (!(x < MAP_SIZE)) )
		return false;
	
	if ((x%SIZE_W) == 0)
		return true;
	else
		return false;
}

boolean on_east_side(int x)
{
	if ( (!(x >= 0)) || (!(x < MAP_SIZE)) )
		return false;

	if ((x%SIZE_W) == (SIZE_W-1)) 
		return true;
	else
		return false;
}

boolean on_north_side(int x)
{
	if (x < SIZE_W)
		return true;
	else
		return false;
}

boolean on_south_side(int x)
{
	if ((x >= (MAP_SIZE-SIZE_W)) && (x < MAP_SIZE))
		return true;
	else
		return false;
}

// in top of generate_map, set global modes,
// instead of using CITY_SIZE, CITY_W, CITY_H in the funcs
// and subs, use map_h, map_w, map_size, and get these 
// values depending on what map_mode is.
// map_mode can be something from location[], city, countr,
// military base, subway, etc..., each haveing predefined
// dimensions and information arrays.

void generate_map(void) 
{
	char **PLACE_P = place_major;
	int *MAP_P = city_map;
	int *SPECIAL_P = special_place;
	time_t time_struct;
	int sqfull[MAP_SIZE];
	int i, j, p, sq, sz, sz2 = 0;
	boolean found, empty, special;

	// for now this is going here, 
	// but it should go in zombie.c in main()
	// set SIZE_W, etc., depending on MAP_MODE
	switch (MAP_MODE) {
	case MODE_CITY:
		SIZE_W = CITY_W;
		SIZE_H = CITY_H;
		MAP_SIZE = CITY_SIZE;
		PLACE_P = place_major;
		MAX_PLACES = MAX_PLACE_MAJOR;
		MAP_P = city_map;
		SPECIAL_P = special_place;
		sz2 = MAX_PLACE_SPECIAL;
		break;
	}
	

	
	// set to zeros, all empty
	memset(sqfull, 0, sizeof(sqfull));

	srand(time(&time_struct));

	// find a place to put the statue, 
	// cant be on the edge of the map
	found = false;
	while(! found) {
		// get any square
		sq = rand()%MAP_SIZE;
		if ( (!on_west_side(sq)) &&
			(!on_east_side(sq)) &&
			(!on_north_side(sq)) &&
			(!on_south_side(sq)) ) 
			found = true;
	}

	sqfull[sq] = 1;

	// 1st square is statue
	MAP_P[sq] = strstrp(PLACE_P, "Statue", MAX_PLACES);

	//place park squares in surrounding positions
//	p = strstrp(place_major, "Park", SIZEOF_ARRAY(place_major));
	p = strstrp(PLACE_P, "Park", MAX_PLACES);
	MAP_P[sq-SIZE_W-1] = p;
	MAP_P[sq-SIZE_W] = p;
	MAP_P[sq-SIZE_W+1] = p;
	MAP_P[sq-1] = p;
	MAP_P[sq+1] = p;
	MAP_P[sq+SIZE_W-1] = p;
	MAP_P[sq+SIZE_W] = p;
	MAP_P[sq+SIZE_W+1] = p;

	// mark as filled in sqfull[]
	sqfull[sq-SIZE_W-1] = 1;
	sqfull[sq-SIZE_W] = 1;
	sqfull[sq-SIZE_W+1] = 1;
	sqfull[sq-1] = 1;
	sqfull[sq+1] = 1;
	sqfull[sq+SIZE_W-1] = 1;
	sqfull[sq+SIZE_W] = 1;
	sqfull[sq+SIZE_W+1] = 1;

	
	// place pier on the top border somewhere
	//
	// look for an empty square
	while (sqfull[(sq = (rand()%SIZE_W))] == 1)
		;
	sqfull[sq] = 1;
	MAP_P[sq] = strstrp(PLACE_P, "Pier", MAX_PLACES);


	// place freeway entrance,  south
	while (sqfull[(sq = (rand()%SIZE_W)+MAP_SIZE-SIZE_W)] == 1)
		;
	sqfull[sq] = 1;
	MAP_P[sq] = strstrp(PLACE_P, "Freeway Entrance", MAX_PLACES);
	
	// for every unique special location (place_major)
	// get rand sq, check to see if its empty, also
	// check to make sure its not a reserved loc, 
	// if it is either of those, get another
	// when you get one that is empty and not reserved, 
	// place the new loc on the sq, mark filled
 
	sz = MAX_PLACES;
	for (i = 0; i < sz; i++) {
		empty = false;
		while (!empty) {
			//find not placed sq at pseudo-random
			empty = false;
			sq = rand()%MAP_SIZE;
			if (sqfull[sq] == 0)
				empty = true;
		}

		special = false;
		for (j = 0; j < sz2; j++)
			if (SPECIAL_P[j] == i)
				special = true;

		if (!special) {
			MAP_P[sq] = i;
			sqfull[sq] = 1;
		}
	}

	for (i = 0; i < MAP_SIZE; i++) 
		if (sqfull[i] == 0) 
			MAP_P[i] = rand()%2;	
	
}

void simple_map_display(void)
{
	int i;
	FILE *fp;

	fp = fopen("map", "w");
	
	for (i = 0; i < MAP_SIZE; i++) {
		fprintf(fp, "%2d ", city_map[i]);
		if ((i%SIZE_W) == SIZE_W-1) 
			fprintf(fp, "\n");
	}

	fclose(fp);
}

// return false if place is on the same edge that dir specifies
// other wise return true
int check_dir (enum direction dir)
{
// remember map orientation
	int i;
	if (dir == M_NONE)
		return true;
	if (dir == M_WEST) 
		for (i = 0; i < MAP_SIZE; i+=SIZE_W) 
			if (place == i)
				return false;
	if (dir == M_EAST)
		for (i = SIZE_W-1; i < MAP_SIZE; i+=SIZE_W) 
			if (place == i)
				return false;
	if (dir == M_SOUTH) 
		for (i = (MAP_SIZE-SIZE_W); i < MAP_SIZE; i++) 
			if (place == i)
				return false;
	if (dir == M_NORTH) 
		for (i = 0; i < SIZE_W; i++) 
			if (place == i)
				return false;
	return true;
}

// int get_square( "Freeway Entrance")
// returns the square number, or -1 if not found
int get_square_by_name(char *where)
{
	int i;
	int place_index, map_index = -1;
	int *MAP_P = city_map;
	char **PLACE_P = place_major;
	int maxp = MAX_PLACE_MAJOR;
	
	/* GMN: todo: once again, move this to zombie.c */
	switch(MAP_MODE) {
	case MODE_CITY:
		MAP_P = city_map;
		MAP_SIZE = CITY_SIZE;
		SIZE_W = CITY_W;
		PLACE_P = place_major;
		break;
	}

	// get the number associated with the place we are
	// asking for
	place_index = strstrp(PLACE_P, where, maxp);

	// crawl map & find the place (or first instance of)
	for (i = 0; i < MAP_SIZE; i++) 
		if (MAP_P[i] == place_index)
			map_index = i;

	return map_index;

/* NOTE: notice this only works for unique places. searching for 
non-unique locations will merely return the first instance found */
}

int go_north(void)
{
	if (check_dir (M_NORTH))
		place -= SIZE_W;
	else 
		return 0;
	return 1;
}

int go_south(void)
{
	if (check_dir (M_SOUTH))
		place += SIZE_W;
	else 
		return 0;
	return 1;
}

int go_east(void)
{
	if (check_dir (M_EAST))
		++place;
	else 
		return 0;
	return 1;
}

int go_west(void)
{
	if (check_dir (M_WEST))
		--place;
	else 
		return 0;
	return 1;
}

void border_message(int y, int x)
{
	move(y, x);
	if (!check_dir(M_NORTH))
		printw("There is a lake to the north.  ");
	if (!check_dir(M_WEST))
		printw("There is a lake to the west.  ");
	if (!check_dir(M_EAST))
		printw("There is a mountain to the east.  ");
	if (!check_dir(M_SOUTH))
		printw("You see a freeway overpass to the south.");
}

void display_map (void)
{
	int x, y, s, m;
	x = 45; y = 14;
	unsigned int sides = 0x0;
	
/*
__| |__| |__| |__
__   __ H __   __
__| |__| |__| |__
__   __   __   __
__| |__| |__| |__
__   __   __   __
  | |  | |  | |
*/

/* 9 map types, need 9 funcs 
1 - |_   // bot, left
2 -  _   // bot, mid
3 -  _|  // bot, right
4 - |    // mid, left
5 -      // mid, mid
6 -   |  // mid, right
     _   
7 - |    // top, left
     _
8 -      // top, mid
     _
9 -   |  // top, right

+ 8 close up types for the 8 edges

*/


/* 1st step, look for edges right next to you */
/* ( order )  N S E W  : ie, 0bWESNWESN */
	if (on_north_side(place))
		sides |= 0x1;
	if (on_south_side(place))
		sides |= 0x2;
	if (on_east_side(place))
		sides |= 0x4;
	if (on_west_side(place))
		sides |= 0x8;
	// look for edges one over from you
	if (((place-SIZE_W) >= 0) && (on_north_side(place-SIZE_W)))
		sides |= 0x10;
	if (((place+SIZE_W) < MAP_SIZE) && (on_south_side(place+SIZE_W)))
		sides |= 0x20;
	if (((place+1) < MAP_SIZE) && (on_east_side(place+1)))
		sides |= 0x40;
	if (((place-1) >= 0) && (on_west_side(place-1)))
		sides |= 0x80;

	// select appropriate map
	switch (sides) {
	case 0x01:
		top_mid_edge(y,x);
		break;
	case 0x02:
		bot_mid_edge(y,x);
		break;
	case 0x04:
		mid_right_edge(y,x);
		break;
	case 0x05:
		top_right_edge(y,x);
		break;
	case 0x06:
		bot_right_edge(y,x);
		break;
	case 0x08:
		mid_left_edge(y,x);
		break;
	case 0x09:
		top_left_edge(y,x);
		break;
	case 0x0a:
		bot_left_edge(y,x);
		break;
	case 0x00:  
		mid_mid(y, x);
		break; 
	case 0x10:
		top_mid(y,x);
		break;
	case 0x20:
		bot_mid(y,x);
		break;
	case 0x40:
		mid_right(y,x);
		break;
	case 0x50:
		top_right(y,x);
		break;
	case 0x60:
		bot_right(y,x);
		break;
	case 0x80:
		mid_left(y,x);
		break;
	case 0x90:
		top_left(y,x);
		break;
	case 0xa0:
		bot_left(y,x);
		break;
	case 0x28:
		bot_left_up(y,x);
		break;
	case 0x82:
		bot_left_right(y,x);
		break;
	case 0x24:
		bot_right_up(y,x);
		break;
	case 0x42:
		bot_right_left(y,x);
		break;
	case 0x81:
		top_left_right(y,x);
		break;
	case 0x18:
		top_left_down(y,x);
		break;
	case 0x41:
		top_right_left(y,x);
		break;
	case 0x14:
		top_right_down(y,x);
		break;
	default:
		break;
	}

	/* display landmarks */


	// bot left
	if (((s = (place + SIZE_W - 1)) < MAP_SIZE) && (!on_west_side(place))) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			if ((m != 0) && (m != 1))
				mvprintw(y+5, x+3, "%1s", major_letter[m]);
		} else {
			mvprintw(y+5, x+3, "?");
		}
	}
	// bot
	if ((s = (place + SIZE_W)) < MAP_SIZE) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			if ((m != 0) && (m != 1))
				mvprintw(y+5, x+8, "%1s", major_letter[m]);
		} else {
			mvprintw(y+5, x+8, "?");
		}
	}
	// bot right
	if (((s = (place + SIZE_W + 1)) < MAP_SIZE) && (!on_east_side(place))) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			if ((m != 0) && (m != 1))
				mvprintw(y+5, x+13, "%1s", major_letter[m]);
		} else {
			mvprintw(y+5, x+13, "?");
		}
	}
	// left
	if (((s = (place - 1)) >= 0 ) && (!on_west_side(place))) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			mvprintw(y+3, x+3, "%1s", major_letter[m]);
		} else {
			mvprintw(y+3, x+3, "?");
		}
	}
	// right
	if (((s = (place + 1)) < MAP_SIZE) && (!on_east_side(place))) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			mvprintw(y+3, x+13, "%1s", major_letter[m]);
		} else {
			mvprintw(y+3, x+13, "?");
		}
	}
	// top left
	if (((s = (place - SIZE_W - 1)) >= 0) && (!on_west_side(place))) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			mvprintw(y+1, x+3, "%1s", major_letter[m]);
		} else {
			mvprintw(y+1, x+3, "?");
		}
	}
	// top
	if ((s = (place - SIZE_W)) >= 0) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			mvprintw(y+1, x+8, "%1s", major_letter[m]);
		} else {
			mvprintw(y+1, x+8, "?");
		}
	}
	// top right
	if (((s = (place - SIZE_W + 1)) >= 0) && (!on_east_side(place))) {
		if (SEEN_P[s] == seen) {
			m = MAP_P[s];
			mvprintw(y+1, x+13, "%1s", major_letter[m]);
		} else {
			mvprintw(y+1, x+13, "?");
		}
	}
	
		

		

	//print protagonist character
	mvprintw(y+3,x+8,"o");
}

void bot_left (int y, int x)
{
mvprintw(y  ,x,"~ | |__| |__| |__");
mvprintw(y+1,x," ~|  __   __   __");
mvprintw(y+2,x,"~ | |__| |__| |__");
mvprintw(y+3,x," ~|  __   __   __");
mvprintw(y+4,x,"~ | |__| |__| |__");
mvprintw(y+5,x," ~|______________");
mvprintw(y+6,x,"=================");
}
void bot_mid (int y, int x)
{
mvprintw(y  ,x,"__| |__| |__| |__");
mvprintw(y+1,x,"__   __   __   __");
mvprintw(y+2,x,"__| |__| |__| |__");
mvprintw(y+3,x,"__   __   __   __");
mvprintw(y+4,x,"__| |__| |__| |__");
mvprintw(y+5,x,"_________________");
mvprintw(y+6,x,"=================");
}
void bot_right (int y, int x)
{
mvprintw(y  ,x,"__| |__| |__| | ^");
mvprintw(y+1,x,"__   __   __  |^ ");
mvprintw(y+2,x,"__| |__| |__| | ^");
mvprintw(y+3,x,"__   __   __  |^ ");
mvprintw(y+4,x,"__| |__| |__| | ^");
mvprintw(y+5,x,"______________|^ ");
mvprintw(y+6,x,"=================");
}
void mid_left (int y, int x)
{
mvprintw(y  ,x,"~ | |__| |__| |__");
mvprintw(y+1,x," `|  __   __   __");
mvprintw(y+2,x,"~ | |__| |__| |__");
mvprintw(y+3,x," ~|  __   __   __");
mvprintw(y+4,x,"~ | |__| |__| |__");
mvprintw(y+5,x," ~|  __   __   __");
mvprintw(y+6,x,"~ | |  | |  | |  ");
}
void mid_mid(int y, int x)
{
mvprintw(y  ,x,"__| |__| |__| |__");
mvprintw(y+1,x,"__   __   __   __");
mvprintw(y+2,x,"__| |__| |__| |__");
mvprintw(y+3,x,"__   __   __   __");
mvprintw(y+4,x,"__| |__| |__| |__");
mvprintw(y+5,x,"__   __   __   __");
mvprintw(y+6,x,"  | |  | |  | |  ");
}
void mid_right(int y, int x)
{
mvprintw(y  ,x,"__| |__| |__| |^ ");
mvprintw(y+1,x,"__   __   __  | ^");
mvprintw(y+2,x,"__| |__| |__| |^ ");
mvprintw(y+3,x,"__   __   __  | ^");
mvprintw(y+4,x,"__| |__| |__| |^ ");
mvprintw(y+5,x,"__   __   __  | ^");
mvprintw(y+6,x,"  | |  | |  | |^ ");
}
void top_left(int y, int x)
{
mvprintw(y  ,x,"   ______________");
mvprintw(y+1,x," ~|  __   __   __");
mvprintw(y+2,x,"~ | |__| |__| |__");
mvprintw(y+3,x," ~|  __   __   __");
mvprintw(y+4,x,"~ | |__| |__| |__");
mvprintw(y+5,x," ~|  __   __   __");
mvprintw(y+6,x,"~ | |  | |  | |  ");
}
void top_mid(int y, int x)
{
mvprintw(y  ,x,"_________________");
mvprintw(y+1,x,"__   __   __   __");
mvprintw(y+2,x,"__| |__| |__| |__");
mvprintw(y+3,x,"__   __   __   __");
mvprintw(y+4,x,"__| |__| |__| |__");
mvprintw(y+5,x,"__   __   __   __");
mvprintw(y+6,x,"  | |  | |  | |  ");
}
void top_right(int y, int x)
{
mvprintw(y  ,x,"______________  ~");
mvprintw(y+1,x,"__   __   __  |^ ");
mvprintw(y+2,x,"__| |__| |__| | ^");
mvprintw(y+3,x,"__   __   __  |^ ");
mvprintw(y+4,x,"__| |__| |__| | ^");
mvprintw(y+5,x,"__   __   __  |^ ");
mvprintw(y+6,x,"  | |  | |  | | ^");
}
void bot_left_edge (int y, int x)
{
mvprintw(y+0,x," ~ ` ~ | |__| |__");
mvprintw(y+1,x,"~ ~ ~ ~|  __   __");
mvprintw(y+2,x," ~ ~ ~ | |__| |__");
mvprintw(y+3,x,"~ ~ ~ ~|_________");
mvprintw(y+4,x,"=================");
mvprintw(y+5,x,"                 ");
mvprintw(y+6,x,"                 ");
}
void bot_mid_edge (int y, int x)
{
mvprintw(y+0,x,"__| |__| |__| |__");
mvprintw(y+1,x,"__   __   __   __");
mvprintw(y+2,x,"__| |__| |__| |__");
mvprintw(y+3,x,"_________________");
mvprintw(y+4,x,"=================");
mvprintw(y+5,x,"                 ");
mvprintw(y+6,x,"                 ");
}
void bot_right_edge (int y, int x)
{
mvprintw(y+0,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+1,x,"__   __  |^ ^ ^ ^");
mvprintw(y+2,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+3,x,"_________|^ ^ ^ ^");
mvprintw(y+4,x,"============= ^ ^");
mvprintw(y+5,x,"            || ^ ");
mvprintw(y+6,x,"             || ^");
}
void mid_left_edge (int y, int x)
{
mvprintw(y  ,x,"~ ` ~ ~| |__| |__");
mvprintw(y+1,x," ~ ~ ~ |  __   __");
mvprintw(y+2,x,"~ ~ ~ ~| |__| |__");
mvprintw(y+3,x," ~ ~ ~ |  __   __");
mvprintw(y+4,x,"~ ~ ~ `| |__| |__");
mvprintw(y+5,x," ~ ~ ~ |  __   __");
mvprintw(y+6,x,"~ ~ ~ ~| |  | |  ");
}
/* no such thing as mid-mid edge
void mid_mid_edge(int y, int x)
{
mvprintw(y  ,x,"__| |__| |__| |__");
mvprintw(y+1,x,"__   __   __   __");
mvprintw(y+2,x,"__| |__| |__| |__");
mvprintw(y+3,x,"__   __   __   __");
mvprintw(y+4,x,"__| |__| |__| |__");
mvprintw(y+5,x,"__   __   __   __");
mvprintw(y+6,x,"  | |  | |  | |  ");
}
*/
void mid_right_edge(int y, int x)
{
mvprintw(y  ,x,"__| |__| |^ ^ ^ ^");
mvprintw(y+1,x,"__   __  | ^ ^ ^ ");
mvprintw(y+2,x,"__| |__| |^ ^ ^ ^");
mvprintw(y+3,x,"__   __  | ^ ^ ^ ");
mvprintw(y+4,x,"__| |__| |^ ^ ^ ^");
mvprintw(y+5,x,"__   __  | ^ ^ ^ ");
mvprintw(y+6,x,"  | |  | |^ ^ ^ ^");
}
void top_left_edge(int y, int x)
{
mvprintw(y  ,x,"~ ~ ~ ~ ~ ~ ~ ~ ~");
mvprintw(y+1,x," ~ ~ ~ ~ ~ ~ ~ ~ ");
mvprintw(y+2,x,"~ ~ ~ ~ _________");
mvprintw(y+3,x," ~ ~ ~ |  __   __");
mvprintw(y+4,x,"~ ~ ~ ~| |__| |__");
mvprintw(y+5,x," ~ ~ ~ |  __   __");
mvprintw(y+6,x,"~ ~ ~ ~| |__| |__");
}
void top_mid_edge(int y, int x)
{
mvprintw(y  ,x,"~ ~ ~ ~ ~ ~ ~ ~ ~");
mvprintw(y+1,x," ~ ~ ~ ~ ~ ~ ~ ~ ");
mvprintw(y+2,x,"_________________");
mvprintw(y+3,x,"__   __   __   __");
mvprintw(y+4,x,"__| |__| |__| |__");
mvprintw(y+5,x,"__   __   __   __");
mvprintw(y+6,x,"__| |__| |__| |__");
}
void top_right_edge(int y, int x)
{
mvprintw(y  ,x,"~ ~ ~ ~ ~ ~ ~ ~ ~");
mvprintw(y+1,x," ~ ~ ` ~ ~ ` ~ ~ ");
mvprintw(y+2,x,"_________  ~ ~ ~ ");
mvprintw(y+3,x,"__   __  |^ ^ ^ ^");
mvprintw(y+4,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+5,x,"__   __  |^ ^ ^ ^");
mvprintw(y+6,x,"__| |__| | ^ ^ ^ ");
}
void bot_left_up (int y, int x)
{
mvprintw(y  ,x," ~ ~ ~ | |__| |__");
mvprintw(y+1,x,"~ ~ ~ ~|  __   __");
mvprintw(y+2,x," ~ ~ ~ | |__| |__");
mvprintw(y+3,x,"~ ~ ~ ~|  __   __");
mvprintw(y+4,x," ~ ~ ~ | |__| |__");
mvprintw(y+5,x,"~ ~ ~ ~|_________");
mvprintw(y+6,x,"=================");
}
void bot_left_right (int y, int x)
{
mvprintw(y+0,x,"~ | |__| |__| |__");
mvprintw(y+1,x," ~|  __   __   __");
mvprintw(y+2,x,"~ | |__| |__| |__");
mvprintw(y+3,x," ~|______________");
mvprintw(y+4,x,"=================");
mvprintw(y+5,x,"                 ");
mvprintw(y+6,x,"                 ");
}
void bot_right_up(int y, int x)
{
mvprintw(y  ,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+1,x,"__   __  |^ ^ ^ ^");
mvprintw(y+2,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+3,x,"__   __  |^ ^ ^ ^");
mvprintw(y+4,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+5,x,"_________|^ ^ ^ ^");
mvprintw(y+6,x,"=============  ^ ");
}
void bot_right_left (int y, int x)
{
mvprintw(y+0,x,"__| |__| |__| | ^");
mvprintw(y+1,x,"__   __   __  |^ ");
mvprintw(y+2,x,"__| |__| |__| | ^");
mvprintw(y+3,x,"______________|^ ");
mvprintw(y+4,x,"=================");
mvprintw(y+5,x,"                 ");
mvprintw(y+6,x,"                 ");
}
void top_left_down (int y, int x)
{
mvprintw(y  ,x," ~ ~ ~ ~_________");
mvprintw(y+1,x,"~ ~ ~ ~|  __   __");
mvprintw(y+2,x," ~ ~ ~ | |__| |__");
mvprintw(y+3,x,"~ ~ ~ ~|  __   __");
mvprintw(y+4,x," ~ ~ ~ | |__| |__");
mvprintw(y+5,x,"~ ~ ~ ~|  __   __");
mvprintw(y+6,x," ~ ~ ~ | |  | |  ");
}
void top_left_right(int y, int x)
{
mvprintw(y  ,x,"~ ~ ~ ~ ~ ~ ~ ~ ~");
mvprintw(y+1,x," ~ ~ ~ ~ ~ ~ ~ ~ ");
mvprintw(y+2,x,"~ ~______________");
mvprintw(y+3,x," ~|  __   __   __");
mvprintw(y+4,x,"~ | |__| |__| |__");
mvprintw(y+5,x," ~|  __   __   __");
mvprintw(y+6,x,"~ | |__| |__| |__");
}
void top_right_left (int y, int x)
{
mvprintw(y  ,x,"~ ~ ~ ~ ~ ~ ~ ~ ~");
mvprintw(y+1,x," ~ ~ ~ ~ ~ ~ ~ ~ ");
mvprintw(y+2,x,"______________~ ~");
mvprintw(y+3,x,"__   __   __  |^ ");
mvprintw(y+4,x,"__| |__| |__| | ^");
mvprintw(y+5,x,"__   __   __  |^ ");
mvprintw(y+6,x,"__| |__| |__| |^ ");
}
void top_right_down(int y, int x)
{
mvprintw(y  ,x,"_________~ ~ ~ ~ ");
mvprintw(y+1,x,"__   __  |^ ^ ^ ^");
mvprintw(y+2,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+3,x,"__   __  |^ ^ ^ ^");
mvprintw(y+4,x,"__| |__| | ^ ^ ^ ");
mvprintw(y+5,x,"__   __  |^ ^ ^ ^");
mvprintw(y+6,x,"  | |  | | ^ ^ ^ ");
}



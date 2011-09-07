/*
 *
 * defs.h
 *
 */

// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <sys/time.h>

typedef int boolean;
#define true 	1
#define false 	0

#define MODE_CITY 		0
#define MODE_COUNTRY		1
#define MODE_SPACE		2
#define MODE_MILITARY_BASE	3
#define MODE_GOVT_LAB		4
#define	MODE_SEWER		5
#define MODE_SUBWAY		6

#define I_NULL	0
#define I_WEAP	1
#define I_FOOD	2
#define I_TOOL	3

// GMN: fix this: used in melee...stupidly, separate into smaller funcs
// and get rid of these
#define MELEE_ZOMBIE	0
#define MELEE_RUFFIAN	1

#define CITY_H 	20
#define CITY_W 	10
#define CITY_SIZE (CITY_H*CITY_W)

#define MAX_PLACE_MAJOR 33
#define MAX_PLACE_SPECIAL 6

enum direction { M_NONE = 01, 
		M_NORTH = 02, 
		M_SOUTH = 04, 
		M_EAST = 010, 
		M_WEST = 020 };

#define seen	1
#define unseen	0

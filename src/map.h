/*
==============================================
map.h
==============================================
*/

int strstrp (char **, char *, int );
boolean on_west_side(int);
boolean on_east_side(int);
boolean on_north_side(int);
boolean on_south_side(int);
void generate_map(void);
void simple_map_display(void);

/* is direction to move in legal (on boundary) */
int check_dir (enum direction);

/* passed string "name", if found return the map square num that
it is on */
int get_square_by_name(char *);

/* funcs that move the character */
int go_east(void);
int go_west(void);
int go_north(void);
int go_south(void);

/* if on a border print a message */
void border_message(int y, int x);

/* map display for ncurses */
void display_map (void);

/* map drawing sub-functions */
void bot_left	(int, int);
void bot_mid	(int, int);
void bot_right	(int, int);
void mid_left	(int, int);
void mid_mid	(int, int);
void mid_right 	(int, int);
void top_left	(int, int);
void top_mid	(int, int);
void top_right	(int, int);

void bot_left_edge	(int, int);
void bot_mid_edge	(int, int);
void bot_right_edge	(int, int);
void mid_left_edge	(int, int);
void mid_right_edge	(int, int);
void top_left_edge	(int, int);
void top_mid_edge	(int, int);
void top_right_edge	(int, int);

void bot_left_up	(int, int);
void bot_left_right	(int, int);
void bot_right_left	(int, int);
void bot_right_up	(int, int);
void top_left_down	(int, int);
void top_left_right	(int, int);
void top_right_down	(int, int);
void top_right_left	(int, int);

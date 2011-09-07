/*
 * zombie.c - "Curses of the Undead", 
 * 	a game of post-apocalyptic zombie hunting
 */

#include "defs.h"
#include "globals.h"	// global variables and defs
#include "environ.h"	// all the environmental vars
#include "zombie.h"	// function headers


int main(int argc, char *argv[])
{
	int place_index;
	int choice;
	int i;
	static int uexit = 0;
	time_t time_struct;

	// over-ellaborate random seed
	srand(time(&time_struct)%getpid()+time(&time_struct));

	initscr();
	getmaxyx(stdscr,row,col);
	if ((row < 24) || (col < 80)) {
		endwin();
		fprintf(stderr, "your terminal is %dx%d.  %s requires at least 80x25\n", col, row, argv[0]);
		exit(-1);
	}


	// ??
	raw();
	// ??
	keypad(stdscr, TRUE);

	cbreak();
	noecho();

	// initialize inventory to all empty
	for (i = 0; i < (sizeof(inventory)/4); i++)
		inventory[i] = 0;
	
	// set seen array to all zeros
	memset(SEEN_P, unseen, MAP_SIZE);

	intro_splash();
	intro_story();

	// initialize map
	generate_map();
	simple_map_display();

	// get starting place on the map
	place = get_square_by_name("Freeway Entrance");

//	curs_set(1);

	for (;;) {
		clear();
		place_index = MAP_P[place];
		street_normal(place_major[place_index]);
		menu_main();
		refresh();
		timeout(-1);

		// only valid keys get through
		choice = getch();
		while ((choice != 'E') &&
			(choice != 'e') &&
			(choice != 'n') &&
			(choice != 'N') &&
			(choice != 'S') &&
			(choice != 's') &&
			(choice != 'W') &&
			(choice != 'w') &&
			(choice != 'B') &&
			(choice != 'b') &&
			(choice != 'I') &&
			(choice != 'i') &&
			(choice != 'R') &&
			(choice != 'r') &&
			(choice != 'Q') &&
			(choice != 'q') &&
			(choice != KEY_LEFT) &&
			(choice != KEY_RIGHT) &&
			(choice != KEY_UP) &&
			(choice != KEY_DOWN) &&
			(choice != 27 ))
			choice = getch();

		switch(choice) {
		case 'N': case 'n': case KEY_UP:
			++turn_num;
			if (encounter(M_NORTH)) 
				go_north();
			break;
		case 'S': case 's': case KEY_DOWN:
			++turn_num;
			if (encounter(M_SOUTH))
				go_south();
			break;
		case 'E': case 'e': case KEY_RIGHT:
			if (encounter(M_EAST))
				go_east();
			++turn_num;
			break;
		case 'W': case 'w': case KEY_LEFT:
			++turn_num;
			if (encounter(M_WEST))
				go_west();
			break;
		case 'B': case 'b':
			++turn_num;
			enter_building();
			break;
		case 'R': case 'r':
			show_todo_list();
			break;
		case 'I': case 'i':
			show_inventory();
			break;
		case 'Q': case 'q': case 27:
			uexit = 1;
			break;
		default:
			break;
		}
		if (uexit) confirm_exit(&uexit);
		if (uexit||dead) break;
		check_camping();
		check_experience();
	}

	flushinp();

	clear();
	refresh();
	nocbreak();
	endwin();

	return EXIT_SUCCESS;
}

/* small main for testing
int main() {
//	generate_map();
//	simple_map_display();
	
	place = get_square_by_name("Freeway Entrance");
	return 0;
}
*/

void street_normal (char *location)
{
	clear();

	int len = strlen(location);
	len = 34 - len;
	if (len < 0)
		len = 0;
	else
		len /= 2;

	printw("\n");
	attrset(A_REVERSE);
	mvprintw(1, len, "%s\n", location);
	attrset(A_NORMAL);
	printw("                                       \n");
	printw("                                       \n");
	printw("                                       \n");
	printw("        ___/         \\_____             \n");
	printw("           |         |                  \n");
	printw("    ___    |         |                  \n");
	printw("      /|   |         |   |\\           \n");
	printw("     / |   |         |   | \\           \n");
	printw("    /  |   |         |   |  \\          \n");
	printw("   //| |   |         |   | |\\\\       \n");
	printw("  / |/ |   |         |   | || \\       \n");
	printw(" /     |   |         |   | \\|  \\     \n");
	printw("|/|    |  /  /     \\     |      |      \n");
	printw("||/    | /  /       \\    |      |      \n");
	printw("|      |/  /         \\   |      |      \n");
	printw("|   .  |__             __|      |      \n");
	printw("|  /| /  /             \\  \\ |\\  |\n");
	printw("|  ||/  /               \\  \\||  |\n");
	printw("|  |/  /                 \\  \\|  |\n");
	printw("|  /  /                   \\  \\  |\n");
	printw("| /  /                     \\  \\ |\n");
	printw("|/  /                       \\  \\|\n");

	curs_set(0);
	return;
}

void intro_splash(void)
{
	flushinp();
	clear();
//	printw("                          L          J          ##       \n");
//	printw("                           L        J          ###        \n");
//	printw("                       #    `-.__.-'          ####         \n");
//	printw("              _____   ##                 .---#####-...__   \n");
//	printw("          .--'     `-###          .--..-'    ######     ""`---....\n");
//	printw(" _____.----.        ###`.._____ .'          #######  \n");
//	printw("                  ~~~~~~~~~~~~~                        \n");
	printw("                                        \n");
//	printw("                /               \\                      \n");
	printw("   ____ _    _  ____ _  _ ___    __  ____   ____ _  _ ___  \n");
	printw("  |  _ | |  | ||  __| || | __|  /  \\|  __| |_  _| || | __|\n");
	printw("  |  __| | | o | | _| || | |_  | /\\ |  __|   || |    | |_ \n");
	printw("  | |  | |_|   | |_ | || | |_  | \\/ | |      || |    | |_ \n");
	printw("  |_|  |___|_|_|____|____|___|  \\__/|_|      || |_||_|___|\n");
	printw("         /                             \\                 \n");
	printw("        / _   _   _   _   ___    _____  \\ _    ___       \n");
	printw("       | | | | | | \\ | | |   \\  |   __|  / \\  |   \\   \n");
	printw("       | | | | | |  \\| | |  _ \\ |  |__  /   \\ |  _ \\  \n");
	printw("       | | | | | |     | | | | ||   __||  O  || | | |    \n");
	printw("       | | | | | |     | | `-' ||  |__ |  _  || `-' |    \n");
	printw("       | | `-' | | |\\  | |    / |     || | | ||    /     \n");
	printw("       | `-----' `-' `-' `---'  `-----'`-' `-'`---'    \n");
	printw("        \\                               /              \n");
	printw("         \\                     A game of Zombie Curses \n");
	printw(".--.--..--\\ --.----'~~--.-~--..-.--..----~-,.........-..---.-..---\n");
	printw("           \\                            \\ \n");
	printw("             \\   Press ");
	attrset(A_REVERSE);
	printw("ANY");
	attrset(A_NORMAL);
 	printw(" Key to Begin   | \n");
	printw("                \\_                       _:  \n");
	printw("                   ~~~~--.--......--.-~~~~  \n");

   curs_set(0);
   refresh();

   getch();
   curs_set(1);
   return;
}

void street_w_zombie (char *location)
{
//	flushinp();
	clear();
	int len = strlen(location);
	len = 34 - len;
	if (len < 0)
		len = 0;
	else
		len /= 2;
	
	printw("\n");
	attrset(A_REVERSE);
	mvprintw(1, len, "%s\n", location);
	attrset(A_NORMAL);
	printw("                                      \n");
	printw("                                      \n");
	printw("                                      \n");
	printw("        ___//|       \\_____             \n");
	printw("           ||/       |                  \n");
	printw("    ___    |         |                  \n");
	printw("      /|   | /|      |   |\\           \n");
	printw("     / |   | ||      |   | \\           \n");
	printw("    /  |   | |       |   |  \\          \n");
	printw("   //| |   | /       |   | |\\\\       \n");
	printw("  / |/ |   |/        |   | || \\       \n");
	printw(" /     |   |  /   \\  |   | \\|  \\     \n");
	printw("|/|    |  /  /     \\     |      |      \n");
	printw("||/    | /  /   o   \\    |      |      \n");
	printw("|      |/  /   /|\\   \\   |      |      \n");
	printw("|   .  |__     / \\     __|      |      \n");
	printw("|  /| /  /             \\  \\ |\\  |\n");
	printw("|  ||/  /   o           \\  \\||  |\n");
	printw("|  |/  /   /|\\           \\  \\|  |\n");
	printw("|  /  /    / \\            \\  \\  |\n");
	printw("| /  /                     \\  \\ |\n");
	printw("|/  /                       \\  \\|\n");

   curs_set(0);
   refresh();
//   getch();
   curs_set(1);
   return;
}

void intro_story(void)
{
	flushinp();
	clear();
	printw("\n");
	printw("\n");
//	printw("\n");
	printw("\n");
	printw("\n");
	printw("\n");
	printw("\n");
	printw("        You were away in the mountains\n");
	printw("        when a virus developed in underground\n"); 
	printw("        government laboratories escapes into the\n");
	printw("        cities.  It spreads across the country in\n");
	printw("        a matter of days turning all caught in its\n"); 
	printw("        path into zombies... into the undead.\n");
	printw("\n");
	printw("        You are driving into the city.  Everything is quiet\n");
	printw("\n");
	printw("        Press ");
	attrset(A_REVERSE);
	printw("ANY");
	attrset(A_NORMAL);
 	printw(" Key to continue   \n");
	curs_set(0);
	refresh();
	getch();
	curs_set(1);
	return;
}

void menu_main (void)
{
	int place_index;
	int menux = menuborder+5;
	int y = 4;
	int vowel = 0;
	int message_index;

	// special messages go at (Y,X) = 1,2

	// 75% that message is blank
	message_index = (rand()%4) ? 0 : 1+rand()%(MAX_MESSAGE-1);

	mvprintw(1, menux, "It is %s", day_description[((turn_num+start_hour)%24)/3]);
	border_message(2,2);

	mvprintw(3, 2, "%s", message[message_index]);
	++y;
	mvprintw(y, menux, "You are by ");

	// determine if article is 'an' or 'a'
	place_index = MAP_P[place];
	switch (place_major[place_index][0]) {
	case 'A': case 'a':
	case 'E': case 'e':
	case 'I': case 'i':
	case 'O': case 'o':
	case 'U': case 'u':
		vowel = 1;
		break;
	default:
		break;
	}
	if (vowel) printw("an "); else printw("a ");

	attrset(A_REVERSE); 
	  printw("%s", place_major[place_index]);
	attrset(A_NORMAL);
	mvprintw(y+1, menux, "arrow keys: |  <--  ^ ");
	mvprintw(y+2, menux, "            V  -->  | ");
	mvprintw(y+3, menux, "N)orth?  S)outh?");
	mvprintw(y+4, menux, "E)ast?   W)est?");
	mvprintw(y+5, menux, "B)reak into a building?");
	mvprintw(y+6, menux, "I)nventory?");
	mvprintw(y+7, menux, "R)ead todo list?");

	
	mvprintw(22, menux-3, "exp: ");
	attrset(A_REVERSE); printw("%d", experience); attrset(A_NORMAL);

	mvprintw(22, menux+5, "health: ");
	attrset(A_REVERSE); printw("%d", health); attrset(A_NORMAL);

	mvprintw(22, menux+17, "party: ");
	attrset(A_REVERSE); printw("%d", party_size); attrset(A_NORMAL);

	display_map();

	// mark the place you are standing on as seen
	SEEN_P[place] = seen;
}

void melee (int type)
{
	int r, x, y, i;
	int col, row, itmp;
	int nzom;
	int choice;
	int exit = 0;
	int adv;
	int *zom_array;
	int random_zom;
	int meleenum = 0;
	int rounds = 0;
	int sizeof_array;
	int place_index;
	int damage;

	clear();
	curs_set(0);

	if (type == MELEE_ZOMBIE) {

		// how many zombies?
		r = rand() % 10;
		if (r < 3) { 
			nzom = 1;
		} else if (r < 7) {
			nzom = 2;
		} else {
			nzom = 1 + (rand() % (party_size + skill.level * skill.level));
		}

		// setup array of zombies
		sizeof_array = nzom * sizeof(int);
		zom_array = (int *) malloc (sizeof_array);
		for (i = 0; i < nzom; i++) {
			zom_array[i] = 1;
		}

		place_index = MAP_P[place];
		street_w_zombie(place_major[place_index]);
		mvprintw(4, menuborder, "There are %d zombies here!!!", nzom);
		mvprintw(6, menuborder, "Do you want to:");
		mvprintw(7, menuborder+5, "R)un?");
		mvprintw(8, menuborder+5, "F)ight?");
		if (skill.acting > 0) 
			mvprintw(9, menuborder+5, "Try to P)retend to be a zombie?");
		

		timeout(-1);
		choice = getch();

		// get a choice
		if (skill.acting > 0) {
			while (!exit) {
				switch(choice) {
				case 'r':
				case 'R':
				case 'f':
				case 'F':
				case 'p':
				case 'P':
					exit = 1;
					break;
				default:
					choice = getch();
					break;
				}
			}
		} else {
			while (!exit) {
				switch(choice) {
				case 'r':
				case 'R':
				case 'f':
				case 'F':
					exit = 1;
					break;
				default:
					choice = getch();
					break;
				}
			}
		}

		while (nzom > 0) {
			
			if ((choice == 'p') || (choice == 'P')) {
				clear();
				mvprintw(2, 2, "You try to pretend to be a zombie");
				refresh();
				timeout(2000);
				getch();

				mvprintw(4, 2, "You grope closer and closer the group of insane zombies");
				refresh();
				timeout(2000);
				getch();
				mvprintw(6,2, "for a moment your eyes meet the yellow eyes of a decrepit man..."); 
				mvprintw(7,2, "parts of his fallow white skin falling off of his bones.");
				mvprintw(8,2, "you gaze at him for a reaction...");
				refresh();
				blocking_timeout(3000);
	
				r = rand() % 10;
				// 70% chance to pretend
				if (r > 6) {
					choice = 'f';
					clear();
					mvprintw(6, 2, "His mouth opens and he lets out a wicked shriek!\n");
					mvprintw(8, 2, "The other zombies turn to look and spot you by the surprised look on your face");
					attrset(A_REVERSE);
					  mvprintw(10,2, "Busted!!");
					attrset(A_NORMAL);
					printw("  Guess you'll just have to fight em.");
			
					refresh();
					timeout(3000);
					getch();
				} else {
					clear();
					mvprintw(4,5,"phew.  you made it.");
					mvprintw(5,5,"You've got the dragging your leg thing down.");
					nzom = 0;
					printw("\n");
					refresh();
					timeout(3000);
					getch();
				}
			} 

			if ((choice == 'r') || (choice == 'R')) {
				clear();
				mvprintw(5, 5, "running...");
				refresh();
				timeout(3000);
				getch();
	
				r = rand() % 10;
				// 50/50 chance to outrun
				if (r > 4) {
					choice = 'f';
					clear();
					mvprintw(4, 5, "You couldn't out run them.\n");
					mvprintw(5, 5, "you're gonna have to fight.\n");
					printw("\n");
					refresh();
					timeout(3000);
					getch();
				} else {
					clear();
					mvprintw(4,5,"phew.  you got away.");
					nzom = 0;
					printw("\n");
					refresh();
					timeout(3000);
					getch();
				}
			} 
				
			if ((choice == 'f') || (choice == 'F')) {
				clear();
				mvprintw(1, 2, "There are %d zombies", nzom);
				mvprintw(1, 37, "health: %d", health);
				mvprintw(1, 50, "party strength: %d", party_size);
	 			flushinp();
				col = 10;
				row = nzom / col + 1;
	 			for (i = 0; i < nzom; i++) {
					x = i % col;
					y = 3;
					itmp = i;
					while (itmp >= col) {
						y += 4;
						itmp -= col;
					}
	
					drawZombie(y, 3+(x*5));
	 			}
				refresh();
				timeout(3000);
				getch();
				timeout(-1);
	
				// adv is advantage calculation
				// if r = 7,8,9 then they surprise you
				// if r = 3,4,5,6 = regular battle
				// else r = 0,1,2 = you have advantage
				//
	
				r = rand() % 10;
				if ((r > 6) && (! rounds)) 
					adv = -1;
				else if ((r > 3) && (! rounds)) 
					adv = 1;
				else 
					adv = 0;
				
				if (adv == -1)
					mvprintw(2, 2, "They surprised you");
				else if (adv == 0)
					//they attack first anyway
					mvprintw(2, 2, "The zombies get ahold of you");
				else
					mvprintw(2, 2, "You snuck up on em");
	
				refresh();
				blocking_timeout(1500);
				flushinp();
				timeout(0);

				// zombie attack
				while ((adv < 1) && (! dead)) {
					clrtoeol();
			        	for (i = 0; i < 3; i++) {
						for (y = 0; y < 24; y++) {
					                for (x = 0; x < 79; x++) { 
								move(y, x); 
								printw("*"); 
							} 
						} 
						refresh(); 
						usleep(200000); 
						clear(); 
						refresh(); 
						usleep(200000); 
					} 
					++adv;

					// Calculate Damage
					// 1 in 8 someone gets killed
					if ((!(rand()%8)) && (party_size > 1)) { 
						--party_size;
						move(1,2);
						clrtoeol();
						mvprintw(1,2,"The zombies got one of your buddies.  You didn't even catch their name.");
						refresh();
						blocking_timeout(1500);
					// otherwise, lose health
					} else {
						// zombies hit 2-10
						damage = (rand() % 9) + 2;
						health -= damage;
						death_check();
						if (damage > 7) {
							move(3,2);
							printw("They thrashed you a good one!");
							refresh();
						}
					}

					if ((adv == 0) && (!dead)) {
						clear();
						mvprintw(2,2,"you are disoriented");
						mvprintw(3,2,"they attack again");
						refresh();
						blocking_timeout(1800);
						flushinp();
						timeout(0);
						clrtoeol();
					}
				}

				if (dead) {
					free(zom_array);
					return;
				}
				
				blocking_timeout(1000);
				clrtoeol();
				if ((r < 7) && (r > 3) && (! rounds))
					mvprintw(2,2, "You ready for the attack");
				else
					mvprintw(2,2, "You regain your bearings and attack the walking corpse(s)");
				refresh();

				// Our Attack
				blocking_timeout(2000);
				flushinp();
				timeout(0);

				move(3,2);
				clrtoeol();
				// draw the zombie army again
				col = 10;
				row = nzom / col + 1;
	 			for (i = 0; i < nzom; i++) {
					x = i % col;
					y = 3;
					itmp = i;
					while (itmp >= col) {
						y += 4;
						itmp -= col;
					}
	
					drawZombie(y, 3+(x*5));
				}

				// calculate attacks
				meleenum = 0;
				for (i = 0; (i < party_size) && (nzom > meleenum); i++) {

					// find a standing zombie randomly
					while((zom_array[(random_zom = rand()%nzom)]) != 1) 
						;
					// Hit!
					// 75% chance of hitting
					if ((rand()%12) < 9) {
						// got a hit ***
	 					flushinp();
						col = 10;
						row = nzom / col + 1;
						x = random_zom % col;
						y = 3;
						itmp = random_zom;
						while (itmp >= col) {
							y += 4;
							itmp -= col;
						}
						hitZombie(y, 3+(5*x));
						++meleenum;

						// ice this guy
						zom_array[random_zom] = 0;
					} else {
						// draw zombie miss
						flushinp();
						col = 10;
						row = nzom / col + 1;
						x = random_zom % col;
						y = 3;
						itmp = random_zom;
						while(itmp >= col) {
							y+=4;
							itmp -= col;
						}
						flashZombie(y, 3+(5*x));
					}
				} // end for total zoms
				nzom -= meleenum;
				experience += meleenum;
				//re-initialize zom_array for nextround
				for(itmp = 0; itmp < (sizeof_array/sizeof(int)); itmp++) {
					zom_array[itmp] = (itmp < nzom) ? 1 : 0;
				}
			} // end (choice == f) (fight)

			++rounds;
		} // end while (nzom > 0)
	
		if ((choice == 'f') || (choice == 'F')) {
			clear();
			mvprintw(1,2,"Killed em all!");
			refresh();
			blocking_timeout(2000);

			move(3,2);
			// see if you find anything. 1 in 10 chance per zombie
			for (i = 0; i < (sizeof_array/sizeof(int)); i++) {
				if ((rand()%10) == 9) r = 100;  //r couldn't be 100 otherwise
			}
			if (r == 100) {
				r = 1+rand()%(MAX_ITEM-1);
				add_item(r);
				mvprintw(5,2,"You find a ");
				attrset(A_REVERSE);
				printw("%s", item[r].inam);
				attrset(A_NORMAL);
				printw(" on one of the dismembered corpses");
				refresh();
				blocking_timeout(2000);
			}
		}

		free(zom_array);

	} else if (type == MELEE_RUFFIAN) {
		place_index = MAP_P[place];
		street_w_zombie(place_major[place_index]);
		mvprintw(3, menuborder , "You come across a gang of ruffians");
		mvprintw(5, menuborder, "Do you:");
		mvprintw(6, menuborder+5, "R)un away before they see you");
		mvprintw(7, menuborder+5, "A)ttack them before they see you");
		if (skill.acting > 0) 
			mvprintw(8, menuborder+5, "T)ry to reason with them");

		timeout(-1);
		choice = getch();
		if (choice == 'a') {
			mvprintw(10, menuborder, "They take off in a pickup truck.");
			mvprintw(11, menuborder, "you didn't want them around anyway");
			mvprintw(12, menuborder, "They looked dangerous");
			refresh();
			blocking_timeout(2000);
		}
		if (choice == 'r') {
			mvprintw(10, menuborder+5, "run away... chicken");
			refresh();
			blocking_timeout(2000);
		}
	}
}

int encounter (enum direction dir)
{
	int r, m;

	move(1,2);
	clrtoeol();
	move(2,2);
	clrtoeol();
	if (check_dir(dir)) {
		mvprintw(2,12, "walking ");
		r = 1;
		switch(dir) {
		case M_NONE:
			break;
		case M_NORTH:
			printw("north");
			break;
		case M_SOUTH:
			printw("south");
			break;
		case M_WEST:
			printw("west");
			break;
		case M_EAST:
			printw("east");
			break;
		}
		printw("...");
	} else {
		r = 0;
		switch(dir) {
		case M_NONE:
			break;
		case M_NORTH:
			switch(rand()%4) {
			case 0:
				mvprintw(2,5, "You cannot cross the lake!");
				break;
			case 1:
				mvprintw(2,5, "You can't walk on water!");
				break;
			case 2:
				mvprintw(2,5, "There is a lake barring your way!");
				break;
			case 3:
				mvprintw(2,5, "Whaddya trying to do, drown!?");
				break;
			}
			break;
		case M_SOUTH:
			mvprintw(2,4, "There is a large freeway overpass blocking your way!");
			break;
		case M_EAST:
			mvprintw(2,4, "There are mountains in your way!");
			break;
		case M_WEST:
			switch(rand()%4) {
			case 0:
				mvprintw(2,5, "You cannot cross the lake!");
				break;
			case 1:
				mvprintw(2,5, "You can't walk on water!");
				break;
			case 2:
				mvprintw(2,5, "There is a lake barring your way!");
				break;
			case 3:
				mvprintw(2,5, "Whaddya trying to do, drown!?");
				break;
			}
			break;
		}
		refresh();
		blocking_timeout(2000);
		flushinp();
		timeout(0);
	} // if checkdir

	// pseudo animate the o guy
	if (r) {
		m = MAP_P[place];
		/* dont print anything if only a building */
		if (!(m == 0) && !(m == 1))
			mvprintw(17,53,"%1s", major_letter[m]);
		else {
			if (on_south_side(place))
				mvprintw(17,53,"_");
			else {
				mvprintw(17,53," ");
			}
		}
		switch (dir) {
		case M_NONE:
			mvprintw(17,53, "o");
			break;
		case M_NORTH:
			mvprintw(16,53, "o");
			refresh();
			blocking_timeout(530);
			mvprintw(16,53, " ");
			mvprintw(15,53, "o");
			refresh();
			blocking_timeout(530);
			break;
		case M_SOUTH:
			mvprintw(18,53, "o");
			refresh();
			blocking_timeout(600);
			mvprintw(18,53, " ");
			mvprintw(19,53, "o");
			refresh();
			blocking_timeout(600);
			break;
		case M_WEST:
			mvprintw(17,52, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,52, " ");
			mvprintw(17,51, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,51, "_");
			mvprintw(17,50, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,50, "_");
			mvprintw(17,49, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,49, " ");
			mvprintw(17,48, "o");
			refresh();
			blocking_timeout(330);
			break;
		case M_EAST:
			mvprintw(17,54, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,54, " ");
			mvprintw(17,55, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,55, "_");
			mvprintw(17,56, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,56, "_");
			mvprintw(17,57, "o");
			refresh();
			blocking_timeout(330);
			mvprintw(17,57, " ");
			mvprintw(17,58, "o");
			refresh();
			blocking_timeout(330);
			break;
		}
	}
	refresh();
	blocking_timeout(300);
	timeout(0);

	timeout(0);
	flushinp();

	r = rand() % 65;
	if (r < 15) {
		melee(MELEE_ZOMBIE);
		r = 1;
	} else if (r < 25) {
		dead_body();
		r = 1;
	} else if (r < 30) {
		melee(MELEE_RUFFIAN);
		r = 1;
	} else if (r < 40) {
		people();
		r = 1;
	} else {
		// nothing.  arrive uneventful
		timeout(1000);
		getch();
	}

	return r;
}

void enter_building(void)
{
	int r = rand() % 10;
	int choice;

	clear();

	draw_entrance();
	refresh();
	blocking_timeout(1000);
	flushinp();
	timeout(0);
	choice = getch();

	move(6,menuborder+12);
	if (r > 4) {
		printw("the door is jammed");
		refresh();
		blocking_timeout(2000);
		flushinp();
		timeout(0);
		choice = getch();
	} else {
		printw("You got in.");
		refresh();
		blocking_timeout(1500);
		flushinp();
		timeout(0);
		choice = getch();
	}
	return;
}

void draw_entrance(void)
{
	clear();
	flushinp();

//printw("___|___|___|___|||      ||      ||___|___|___|\n");
printw("__|___|___|___|_||      ||      ||__|___|___|\n");
printw("___|___|___|___|||______||______||___|___|___|\n");
printw("__|___|___|___|_|________________|__|___|___|\n");
printw("___|___|___|___|||              ||___|___|__\n");
printw("__|___|___|___|.||              ||__|___|___|\n");
printw("___|___|___|___|||              ||___|___|__\n");
printw("__|___|___|___|_||              ||__|___|___|\n");
printw("_|___|___|___|__||              ||___|___|___|\n");
printw("__|___|___|___|_||              ||__|___|___|\n");
printw("___|___|___|___|||              ||___|___|___|\n");
printw("__|___|___|___|_||____________/|||__|___|___|\n");
printw("_|___|___|___|__||____________[|||___|___|___|\n");
printw("__|___|___|___|_||            \\|||__|___|___|\n");
printw("_|___|___|___|__||              ||___|___|___|\n");
printw("__|___|___|___|_||              ||__|___|___|\n");
printw("___|___|___|___|||              ||___|___|___|\n");
printw("__|___|___|___|_||              ||__|___|___|\n");
printw("___|___|___|___|||              ||___|___|___|\n");
printw("__|___|___|___|_||              ||__|___|___|\n");
printw("___|___|___|___|||______________||___|___|___|\n");
printw("___________________________________________/ | \n");
printw("                                          |  |\n");
printw("__________________________________________| /\n");
printw("                                         / /\n");
//printw("________________________________________/ /\n");
//printw("________________________________________|/\n");

	curs_set(0);
	curs_set(1);
}

void drawZombie(int y, int x)
{
	mvprintw(y, x, "_O_");
	mvprintw(y+1, x, "\\| \\");
	mvprintw(y+2, x, "/ \\");
}

void hitZombie(int y, int x)
{
	mvprintw(y, x, "***");
	mvprintw(y+1, x, "***");
	mvprintw(y+2, x, "***");
	refresh();
	usleep(250000);

	mvprintw(y, x, "_O_");
	mvprintw(y+1, x, "\\| \\");
	mvprintw(y+2, x, "/ \\");
	refresh();
	usleep(200000);

	mvprintw(y, x, "  O");
	mvprintw(y+1, x, " / \\");
	mvprintw(y+2, x, "/ \\");
	refresh();
	usleep(200000);

	mvprintw(y, x, "_O_");
	mvprintw(y+1, x, "\\| \\");
	mvprintw(y+2, x, "/ \\");
	refresh();
	usleep(200000);

	mvprintw(y, x, "  O");
	mvprintw(y+1, x, " / \\");
	mvprintw(y+2, x, "/ \\");
	refresh();
	usleep(100000);

	mvprintw(y, x, "  O");
	mvprintw(y+1, x, " /\\ ");
	mvprintw(y+2, x, "//  ");
	refresh();
	usleep(100000);

	mvprintw(y, x, "    ");
	mvprintw(y+1, x, " .-0");
	mvprintw(y+2, x, "/  \\");
	refresh();
	usleep(100000);

	mvprintw(y, x, "    ");
	mvprintw(y+1, x, "    ");
	mvprintw(y+2, x, ">->o");
	move(1,0);
	clrtoeol();
	mvprintw(1, 2, "you got one!");
	refresh();
	blocking_timeout(1000);
	return;
}

void flashZombie(int y, int x)
{
	mvprintw(y, x, "***");
	mvprintw(y+1, x, "***");
	mvprintw(y+2, x, "***");
	refresh();
	usleep(350000);

	mvprintw(y, x, "_O_");
	mvprintw(y+1, x, "\\| \\");
	mvprintw(y+2, x, "/ \\");
	refresh();
	usleep(200000);

	move(1,0);
	clrtoeol();
	mvprintw(1, 2, "you missed");
	refresh();
	timeout(1000);
	getch();
	return;
}

void death_check(void)
{
	if (health > 0)
		return;
	else {
		flushinp();
		dead = 1;
		clear();
		move(8,10);
		printw("You are dead.");
		mvprintw(10,10, "You killed %d zombies and stayed alive for %d days", experience, turn_num/24);
		move(12,10);
		if (skill.level > 1) 
			printw("You knew how to ");
		else
			printw("You didn't know how to do anything");
		if (skill.camping) 
			printw("camp");
		if (skill.acting)  
			printw(", act");
		if (skill.medical) 
			printw(", heal people");
		if (skill.science) 
			printw(", heal the undead!");
		mvprintw(14,10, "Your corpse lies rotting until eventually...");
		mvprintw(16,10, "you rise again.");
		write_score (); 

		refresh();
		timeout(10000);
		getch();
	}
}

void write_score (void) 
{
	FILE *fp;
	if ((fp = fopen (".scores", "a")) == NULL) {
		fprintf(stderr, "couldn't open score file\n");
		return;
	}
	fprintf(fp, "%3d : %3d\n", experience, skill.level);
	fclose(fp);
}

void confirm_exit(int *uexit)
{
	int x, y;
	int choice = -1;
	while ((choice != 'n') && (choice != 'N') && 
		(choice != 'Y') && (choice != 'y')) { 
		move(2,2);
		clrtoeol();
		move(2,2);
		printw("Are you sure you want to quit? (y/n)");
		refresh();
		timeout(-2000);
		choice = getch();
	}
	if ((choice == 'Y') || (choice == 'y')) {
		y = 4;
		x = 10;
		write_score();
		clear();
		mvprintw(y,x, "You killed %d zombies and stayed alive for %d days", experience, turn_num/24);
		move(y+2,x);
		if (skill.level > 1)
			printw("You knew how to ");
		else
			printw("You didn't know how to do anything");
		if (skill.camping) 
			printw("camp");
		if (skill.acting) 
			printw(", act");
		if (skill.medical)
			printw(", heal people");
		if (skill.science)
			printw(", heal zombies!");
		move(y+3,x);
	//	printw("Eventually you become worn out.");
		move(y+4,x);
		printw("no time to sleep, no place to hide");
		move(y+5,x);
		printw("you give up and are subsumed by a large");
		move(y+6,x);
		printw("group of zombies, coming ever closer");
		move(y+7,x);
		printw("you can't escape");
		move(y+9,x);
		printw("press ");
		attrset(A_REVERSE);
		printw("ANY");
		attrset(A_NORMAL);
		printw(" key to quit");
		refresh();
		timeout(10000);
		getch();
		return;
	}
	*uexit = 0;
	move(2,2);
	clrtoeol();
	printw("Excellent!  Back to zombie huntin!");
	refresh();
	timeout(2000);
	getch();
	return;
}

void show_inventory(void) 
{
	int choice, thing, points, confirm;
	int x,y,i;

	x = 1;

	clear();
	move(0,x);
	printw("Inventory");

	move(2,x);
	attrset(A_REVERSE);
	printw("D");
	attrset(A_NORMAL);
	printw(")rop an Item.  ");
	attrset(A_REVERSE);
	printw("U");
	attrset(A_NORMAL);
	printw(")se an Item.  ");
	attrset(A_REVERSE);
	printw("ANY");
	attrset(A_NORMAL);
	printw(" other key returns");
	
	for(i = 0; i < inv_size; i++) {
		y = i + 4;
		move (y, x);
		printw("%x - ", i+1);
		
		// turn off high-lighting for empty
		if (inventory[i])
			attrset(A_REVERSE);
		printw("%s", item[inventory[i]].inam);
		if (inventory[i])
			attrset(A_NORMAL);
		move(y, x+38); 
		switch (item[inventory[i]].ityp) {
		case I_NULL:
			printw(" ");
			break;
		case I_WEAP:
			printw("weapon");
			break;
		case I_FOOD:
			printw("food/consumable");
			break;
		case I_TOOL:
			printw("tool/useful item");
			break;
		}
	}
	curs_set(1);
	refresh();
	timeout(-1);
	choice = getch();
	if ((choice == 'd') || (choice == 'D')) {
		move(2,x);
		clrtoeol();
		printw("which item to drop: ");

		refresh();
		timeout(-1);
		choice = getch();
		if (choice <= '9') 
			choice -= '0';
		else
			choice = choice - 'a' + 10;

		thing = inventory[choice-1];

		if (thing != 0) {
			move(2,x);
			clrtoeol();
			printw("are you sure you want to drop ");
			attrset(A_REVERSE);
			printw("%s", item[thing].inam);
			attrset(A_NORMAL);
			refresh();
			timeout(3000);
			confirm = getch();
		
			if ((confirm == 'y') || (confirm == 'Y')) {
				move(2,x);
				clrtoeol();
				printw("You dropped a %s", item[thing].inam);
				inventory[choice-1] = 0;
			} else {
				move(2,x);
				clrtoeol();
				printw("You didn't drop anything");
			}
			refresh();
			timeout(3000);
			getch();
			
		} else {
			move (2,x);
			clrtoeol();
			printw("you cant drop that");
			refresh();
			timeout(3000);
			getch();
		}
	}
	if ((choice == 'u') || (choice == 'U')) {
		move(2,x);
		clrtoeol();
		printw("which item to use: ");

		refresh();
		timeout(-1);
		choice = getch();
		if (choice <= '9') 
			choice -= '0';
		else
			choice = choice - 'a' + 10;
		
		clrtoeol();
		move(2,x);
		clrtoeol();
		thing = inventory[choice-1];
		if ((thing <= MAX_ITEM) && (thing != 0)) {
			mvprintw(1, x, "%s", item_use[thing].iattr);
			if (!special_item(thing, &points)) {
				if (item[thing].ityp == I_FOOD) {
					points = food_val[item[thing].ifnum];
	
					if ((health + points) <= 100) {
						mvprintw(2,x, "You gain %d health", points);
						health += points;
						inventory[choice-1]=0;
					} else {
						move(1,x);
						clrtoeol();
						mvprintw(2,x,"You better save that for later.  You're gonna need it");
					}
				}
			}
		} else {
			mvprintw(2, x, "You dont have that");
		}
		refresh();
		timeout(3000);
		getch();
	}
	return;
}

void people(void)
{
// sick 		25%
// afraid		25%
// elderly & kids	25%
// fight		25%

	int i, r;
	int choice = -1;
	// 2 - 6 people
	int np = rand() % 5 + 2;
	int status = 0;
	int ratio;
	int sick = 0, 
	    afraid = 0, 
	    elderly = 0, 
	    willing = 0;
	int gift = 0;

	move(2,2);
	clrtoeol();
	mvprintw(2, 9, "You see someone");
	refresh();
	timeout(1000);
	getch();
	for(i = 0; i < np; i++) {
		status = rand() % 20;
		if (status < 5) ++sick;
		else if (status < 10) ++afraid;
		else if (status < 15) ++elderly;
		else ++willing;
	}
	clear();
	mvprintw(2, 2, "You come across a group of people");
	mvprintw(4, 10, "There are %d people", np);
	if (sick) mvprintw(6, 10, "%d are sick", sick);
	if (afraid) mvprintw(7, 10, "%d are afraid", afraid);
	if (elderly) mvprintw(8, 10, "%d are elderly", elderly);
	if (willing) mvprintw(9, 10, "%d are able-bodied", willing);

	refresh();
	blocking_timeout(2500);

	// get ratio
	// basically the better the ratio of able-bodied to
	// sick, the better chances of them wanting to join up
	// with you.
	if (willing == np)
		ratio = 99;
	else {
		ratio = (int) ((float)willing/(float)np*99.0);
	}

	if (willing > 0) 
		status = rand()%100;
	// 50% chance they wont want to come
	if (status > (100-ratio)) {
		mvprintw(11,10, "The able-bodied want to join your party");
		mvprintw(12,10, "Do you take them? ");
		move(12, 28);
		curs_set(1);
		refresh();
		flushinp();
		choice = get_one();

		while  ((choice != 'n') &&
			(choice != 'N') &&
			(choice != 'Y') &&
			(choice != 'y'))
			choice = get_one();

		if ((choice == 'y') || (choice == 'Y')) {
			mvprintw(14,10, "%d joined your party", willing);
			party_size += willing;
			refresh();
			timeout(3000);
			getch();
		} else {
			mvprintw(14,10, "You decided you'd better go it alone");
			refresh();
			timeout(3000);
			getch();
		}

	} else {
		if (willing == 0) 
			mvprintw(11,10, "none of them can join you");
		else
			mvprintw(11,10, "none of them want to join you");
		mvprintw(13,10, "You tell them that you're going for help and keep moving...");
		refresh();
		blocking_timeout(1500);
		timeout(1500);
		getch();
	}
	// 1/10th chance per person
	for (i = 0; i < np; i++) {
		if (!(rand()%10)) gift = 1;
	}
	if (gift) {
		r = 1+rand()%(MAX_ITEM-1);
		mvprintw(15,10,"one of the people gives you a ");
		attrset(A_REVERSE);
		printw("%s", item[r].inam);
		attrset(A_NORMAL);
		mvprintw(16,10,"You accept their gift and press on");
		add_item(r);
		refresh();
		blocking_timeout(1500);
	}

	/* if you have medical skill, you can also take
	 * 1 or more of the sick with you */
	if (skill.medical && sick) 
	{
		sick = (sick > 1) ? (1 + rand() % (sick-1)) : 1;
		mvprintw(18,10, "You are able to heal %d of the sick in the party.", sick);
		mvprintw(19,10, "They swear allegiance to you and join your posse.");
		refresh();
		timeout(1000);
		getch();
		attrset(A_REVERSE);
		mvprintw(20,10, "%d", sick);
		attrset(A_NORMAL);
		printw(" more joined your party");
		party_size += sick;
		refresh();
		timeout(2000);
		getch();
	}
}

void dead_body(void)
{
	int r;
	int item_num;
	clear();
	mvprintw(3, 2, "you come across a group of dead bodies");
	refresh();
	blocking_timeout(3000);

	r = rand()%3;
	if (!r) {

		item_num = r = 1+rand()%(MAX_ITEM-1);

		mvprintw(5, 2, "one of them is clutching a ");
		attrset(A_REVERSE);
		printw("%s", item[r].inam);
		attrset(A_NORMAL);
		mvprintw(6, 2, "you pick it up");
		refresh();
		blocking_timeout(1600);
		r = 0;
		add_item(item_num);
	} else {
		mvprintw(5,2, "there is nothing");
		refresh();
		timeout(2000);
		getch();
	}
}

void add_item(int item_num)
{
	if (!item_num) 
		return;
	int r = 0;
	while (inventory[r] != 0)
		++r;
	if (r < inv_size) {
		inventory[r] = item_num;
		/* check special items
		 * - water jug
		 * - cell phone
		 */
		if (strcmp(item[item_num].inam, "5 gallon water jug") == 0)
			water += 5; 
		if (strcmp(item[item_num].inam, "cell-phone") == 0)
			cellphone += 1;
		if (strcmp(item[item_num].inam, "set of boat keys") == 0)
			++boatkeys;
		if (strcmp(item[item_num].inam, "turnstile ticket") == 0)
			turnstileticket += 1;
	} else {
		clear();
		mvprintw(7,10,"You cant hold the %s", item[item_num].inam);
		mvprintw(8,10,"You are carrying too much stuff");
		refresh();
		blocking_timeout(2000);
		getch();
	}
}

void show_todo_list(void)
{
	clear();
	move(2,2);
	printw("Todo list, August 5th, 2005");
	mvprintw(3,2,"-Call Sandy: 324-7706");
	mvprintw(4,2,"-do laundry");
	mvprintw(5,2,"-go job hunting");
	mvprintw(6,2,"-start running again");
	mvprintw(7,2,"-Call Russ' Amoco and see if your car is ready 325-3203"); 
	mvprintw(8,2,"-call aunt Sharon and uncle Ned: 424-2100");

	refresh();
	timeout(6000);
	getch();
}

void high_scores(void)
{
}

void check_experience(void)
{
	int levels, i;
	levels = 0;

	if (experience > 9) 
		if (skill.level < 2) 
			++levels;
	if (experience > 24) 
		if (skill.level < 3) 
			++levels;
	if (experience > 49) 
		if (skill.level < 4) 
			++levels;
	if (experience > 99) 
		if (skill.level < 5) 
			++levels;
	if (levels > 0) {
		clear();
		mvprintw(2,2,"You feel experienced");
		for (i = 0; i < levels; i++) 
			mvprintw(3+i,2,"You have gained an ability!");
	}

	i = levels;
	while (levels > 0) {
		switch(skill.level) {
		case 1:
			skill.camping = 1;
			skill.level += 1;
			break;
		case 2:
			skill.acting = 1;
			skill.level += 1;
			break;
		case 3:
			skill.medical = 1;	
			skill.level += 1;
			break;
		case 4:
			skill.science = 1;
			skill.level += 1;
			break;
		default:
			break;
		}
		--levels;
	}
	
	if (i > 0) {
		refresh();
		blocking_timeout(3000);
	}
}

void check_camping(void)
{
	int i, c, hp;
	boolean camp = false;
	// camping happens at 2 am
	if (((turn_num+start_hour)%24) != 2)
		return;
	// you have to have the camping skill
	if (!skill.camping)
		return;
	// you need a camping item to camp
	for (i = 0; i < inv_size; i++) {
		c = inventory[i];
		if (c != 0) {
			if (strcmp(item[c].inam, "collection of camping gear") == 0)
				camp = true;
			if (strcmp (item[c].inam, "goosedown winter coat") == 0)
				camp = true;
			if (strcmp (item[c].inam, "sleeping bag") == 0)
				camp = true;
			if (strcmp (item[c].inam, "camping stove") == 0)
				camp = true;
		}
	}
	if (camp == false)
		return;

	// We're camping baby!!
	
	// advance time significantly
	turn_num += 4;

	move(2,2);
	clrtoeol();
	mvprintw(2,2, "you found a place to camp and managed to get some shuteye");
	move (3,2);
	clrtoeol();
	// 2-10 points of healin
	hp = 2+rand()%9;
	printw("you healed up ");
	attrset(A_REVERSE);
	  printw("%d", ((100-health) < hp) ? (100-health) : hp);
	attrset(A_NORMAL);
	printw(" points of health");

	health = ((100-health) < hp) ? (100-health) : (health+hp);
	refresh();
	blocking_timeout(2400);
}

/* take the item_num directly and a pointer to points
 * return true if the item is special or false otherwise */ 
boolean special_item(int item_num, int *points)
{
	/* check for special items */
	/* 5 gallon water jug
	 * cell-phone
	 * turnstile ticket
	 * set of boat keys
	 * wristwatch
	 */
	if (! strcmp(item[item_num].inam, "5 gallon water jug")) {
		if (water > 0) {
			--water;
			*points = 1;
			mvprintw(2,1, "You gain %d health", (health < 100) ? *points : 0);
			health += *points;
		} else {
			move(1,1); clrtoeol();
			move(2,1); clrtoeol();
			mvprintw(2,1,"The water jug seems to be empty");
		}
		return true;
	}
	if (! strcmp(item[item_num].inam, "cell-phone")) {
		/* you already have a working cell phone */
		if (working_cellphone) {
		}
		if (cellphone > 0) {
			/* 15% chance of working cell-phone */
			if ((rand()%20) > 16) {
				++working_cellphone;
			}
		}	
	}
	return false;
}

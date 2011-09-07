/*
============================================
macros.c - macros for ncurses
============================================
*/


#include "defs.h"

/*
 * dont do anything until the alotted time has expired
 */
void blocking_timeout(int msec)
{
	//
	// this func only has resolution of 1/10th sec.  fuckit
	//
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	int timeup;
	register int i;
	timeup = msec / 100;
	for (i = 0; i < timeup; i++) {
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		select(0, NULL, NULL, NULL, &tv);
	}
	flushinp(); // flush all keyboard activity
	timeout(0); // set read mode back to non-blocking
	return;
}

/*
 * block forever until they press one of the keys 
 * in key[].    keylen is sizeof(key)/4
 */
void blocking_timeout_w_key(int *key, int keylen)
{
	int exit = 0;
	register int i;
	int kp; 
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));

	while (!exit) {
		// sleep for 1/10th sec
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		select(0, NULL, NULL, NULL, &tv);

		kp = getch();
		for (i = 0; i < keylen; i++) {
			if (kp == key[i])
				exit = 1;
		}
	}
	return;
}

/*
 * any keypress falls through, otherwise waits till msec
 *
void nonblocking_timeout(int msec)

 *
 * blocks to msec, unless specific key is pressed
 *
void nonblocking_timeout_w_key(int msec, int *key)
*/

// from taipan.c
long get_num(int maxlen)
{
   char number[maxlen + 1];

   int  input,
        character = 0;

   long amount;

   while ((input = getch()) != '\n')
   {
      if (((input == 8) || (input == 127)) && (character == 0))
      {
         refresh();
      } else if ((input == 8) || (input == 127)) {
         printw("%c", 8);
         printw(" ");
         printw("%c", 8);
         number[character] = '\0';
         character--;
         refresh();
      } else if (character >= maxlen) {
         refresh();
      } else if (input == '\33') {
         flushinp();
         refresh();
      } else if (((input == 'A') || (input == 'a')) &&
                 (character == 0) && (maxlen > 1)) {
         printw("%c", input);
         number[character] = input;
         character++;
         refresh();
      } else if ((input < 48) || (input > 57)) {
         refresh();
      } else {
         printw("%c", input);
         number[character] = input;
         character++;
         refresh();
      }
   }

   number[character] = '\0';
   if ((strcmp(number, "A") == 0) || (strcmp(number, "a") == 0))
   {
      amount = -1;
   } else {
      amount = strtol(number, (char **)NULL, 10);
   }

   return amount;
}

// from taipan.c
int get_one(void)
{
	timeout(-1);

   int input,
       choice = 0,
       character = 0;

   while ((input = getch()) != '\n')
   {
      if (((input == 8) || (input == 127)) && (character == 0))
      {
         refresh();
      } else if ((input == 8) || (input == 127)) {
         printw("%c", 8);
         printw(" ");
         printw("%c", 8);
         character--;
         refresh();
      } else if (character >= 1) {
         refresh();
      } else if (input == '\33') {
         flushinp();
         refresh();
      } else {
         printw("%c", input);
         choice = input;
         character++;
         refresh();
      }
   }
   return choice;
}

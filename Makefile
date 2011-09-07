#========================================#
#                                        #
# $ Makefile for Curses of the Undead  $ #
#                                        #
#========================================#

SRCDIR=src
CC=gcc
CFLAGS=-g -O -Wall -Wno-missing-braces
EXENAME=zombie
LIBS=-lcurses
VERS=0.03
TARBZIP=tar cvjf
OBJ=	$(SRCDIR)/zombie.o \
	$(SRCDIR)/macros.o \
	$(SRCDIR)/map.o

SCOREFILE=.scores
MAPDEBUG=map

all: $(EXENAME)

# see $^ is right of : $@ is left of colon
# not sure of $< yet....

#$(EXENAME): $(OBJ)
#	$(CC) $(CFLAGS) $(LIBS) $(OBJ) -o $@ 

$(EXENAME): $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ 

$(SRCDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(EXENAME) $(OBJ) 

cleaner:
	rm -f $(EXENAME) $(OBJ) $(MAPDEBUG) $(SCOREFILE)

release:
	rm -f $(EXENAME) $(OBJ) $(SCOREFILE) $(MAPDEBUG); \
	DATE=$(shell date +%Y%m%d) && cd ../ && \
	$(TARBZIP) zombie-$(VERS)-$(shell date +%Y%m%d).tar.bz2 zombie

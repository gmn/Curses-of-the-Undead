/* 
 * environ.h - various data structures and lists of stuff
 * for use with zombie.c - a cool-ass zombie hunting game
 */

char    *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

char 	*day_description[] = { 
			"midnight",	// 00-02
			"late night", 		// 03-05, etc..
			"early morning", 	// 06
			"morning", 		// 09
			"noon", 		// 12
			"midday", 		// 15
			"evening",		// 18
			"nighttime" 		// 21-23
};		


char    *health_description[] = { "Critical", "  Poor", "  Fair",
                  "  Good", " Prime", "Perfect" };
#define MAX_HEALTH (sizeof(health_description)/sizeof(char *))


char	*location[] = { "City", 
			"Country", 
			"Military Base",
			"Government Lab",
			"Sewer", 
			"Subway",
			"(ISS) International Space Station" 
};
#define MAX_LOCATIONS (sizeof(location)/sizeof(location[]));

char 	*place_major[] = {
			"Building", 		// 0
			"Apartment Building", 	// 1
			"Hospital",		// 2
			"Department Store", 	// 3
			"Church", 		// 4
			"Library", 		// 5
			"University",		// 6
			"Museum", 		// 7
			"Gas Station", 		// 8
			"Super Market", 	// 9
			"Police Station", 	// 10
			"Sporting Goods Store", // 11
			"Walmart",		// 12
			"Shopping Mall", 	// 13
			"Strip Mall", 		// 14
			"Circle K", 		// 15
			"Fountain",		// 16
			"Park", 		// 17
			"Concert Hall", 	// 18
			"Pier", 		// 19
			"Office Building",	// 20
			"Restaurant",		// 21
			"Pub",   		// 22
			"Starbucks", 		// 23
			"K-Mart", 		// 24
			"House",		// 25
			"Statue",		// 26
			"Record Shop",		// 27
			"Skyscraper",		// 28
			"Luxury Hotel",		// 29
			"Subway entrance", 	// 30
			"Channel 7 News building",  // 31
			"Freeway Entrance"	// 32
			};
//#define MAX_PLACE_MAJOR (sizeof(place_major)/sizeof(char *))
//#define MAX_PLACE_MAJOR 33

char *major_letter[] = { "", "", "H", "D", "C", "L", "U", "M", "G", "K", "P", "S", "W", "M", "M", "C", 
			 "F", "P", "C", "P", "O", "R", "P", "S", "K", "H", "S", "R", "S", "L", "S", "7", "F" };


int special_place[] = { 26, // statue
			17, // park
			19, // pier
			0,  // building
			1,  // apartment building
			32  // freeway entrance
};

//#define MAX_SPECIAL_PLACE (sizeof(special_place)/sizeof(special_place[0]))


// perhaps camping skill???
struct skill_t {
	int camping;	// l2
	int acting; 	// l3
	int medical; 	// l4
	int science; 	// l5
	int level;
} skill = { 0, 0, 0, 0, 1 };


struct items {
	int ityp;
	int ifnum;
	char *inam;
} item[] = { 
	I_NULL, 0, "<empty>",  			

	// start weapons
	I_WEAP, 1, "pistol", 			
	I_WEAP, 2, "Antique M1 Carbine Rifle",
	I_WEAP, 3, "shovel", 		
	I_WEAP, 4, "baseball bat",		
	I_WEAP, 5, "large knife",  	
	I_WEAP, 6, "chain",  			
	I_WEAP, 7, "shotgun", 			
	I_WEAP, 8, "flare gun", 			
	I_WEAP, 9, "axe", 				
	I_WEAP, 10, "machine gun", 			
	I_WEAP, 11, "large rock",		
	I_WEAP, 12, "ammo cartrige",		
	I_WEAP, 13, "sword",			
	I_WEAP, 14, "pick axe",			
	I_WEAP, 15, "Ted Nugent compound bow",	
	I_WEAP, 16, "crow bar",			
	I_WEAP, 17, "tire iron",			
	I_WEAP, 18, "chain saw",			
		
		// end weapons
		// begin food/consumables

	I_FOOD,	0, "12-pack of Mountain Dew", 
	I_FOOD,	1, "bag of chips",		
	I_FOOD,	2, "can of Albacore",		
	I_FOOD,	3, "joint",		
	I_FOOD,	4, "pack of cigarettes",		
	I_FOOD,	5, "candybar", 		
	I_FOOD,	6, "bottle of Kentucky whiskey",	
	I_FOOD,	7, "can of peaches",	
	I_FOOD,	8, "case of beer",		
	I_FOOD, 9, "bottle of Glenmorangie whiskey", 	
	I_FOOD, 10, "case of imported beer",	
	I_FOOD, 11, "package of beef jerky",		

		// end food
		// begin useful items
		
	I_TOOL, 0, "collection of camping gear",  
	I_TOOL,	1, "can of gasoline",		
	I_TOOL,	2, "radio",		
	I_TOOL,	3, "wristwatch",		
	I_TOOL,	4, "set of boat keys",	
	I_TOOL,	5, "cell-phone",		
	I_TOOL,	6, "can of spray paint",	
	I_TOOL,	7, "turnstile ticket",	
	I_TOOL,	8, "backpack", 		
	I_TOOL, 9, "goosedown winter coat",
	I_TOOL, 10, "raincoat",
	I_TOOL, 11, "sleeping bag",
	I_TOOL, 12, "camping stove",  	
	I_FOOD, 13, "5 gallon water jug",
	I_TOOL, 14, "binoculars",

	I_WEAP, 19, "old WWII flame thrower",
	I_WEAP, 20, "bullwhip",

	I_FOOD, 12, "can of Spam",
	I_FOOD, 13, "sealed package of smoked salmon",
	I_FOOD, 14, "can of creamed corn",
	I_FOOD, 15, "can of chili",
	I_FOOD, 16, "can of beef stew",
	I_FOOD, 17, "can of beans",
	I_FOOD, 18, "can of ravioli",
	I_FOOD, 19, "box of crackers",
	I_FOOD, 20, "italian salami",

	I_TOOL, 15, "pair of old rayban sunglasses", 
	I_TOOL, 16, "flashlight" };

#define MAX_ITEM (sizeof(item)/sizeof(struct items))

// these are things that are esplicitly called from
// the items dialog.  for instance, if you try to use,
// a pistol, it will report, "whaddya ya going to do 
// with that, shoot something?"
struct item_use_t {
	int inum;
//	int ityp;
	char *iattr;
} item_use[] = { 
	0, NULL,

	1, "Whaddya going to do with that, shoot something?",
	2, "You'll shoot your eye out kid.",
	2, "you dig a hole",
	3, "Here's the pitch..  Its going way back on the warning track... its.. its.. Outta here!",
	4, "That's not a knife.  Now that's a knife",
	5, "You swing a chain around wildly",
	6, "This is my boomstick! It's a twelve gauge double barreled Remington, S-Mart's top-of-the-line. You can find this in the sporting goods department. That's right this sweet baby was made in Grand Rapids Michigan. Retails for about $109.95. It's got a walnut stock, cobalt blue steel and a hair trigger.", 
	7, "You shoot a flare up over the tree tops",
	8, "You chop through someone's front door.  \"Here's Johnny\"",
	9, "You fire off a few shots at the side of a building.  It looked fairly cool but accomplished very little", 
	10, "You practice hurling a baseball sized rock at a mailbox 60 ft. away.",
	11, "an Ammo clip will make your machine gun last longer",
	12, "You slice through the air with great ease",
	13, "There's gold in them thar hills.  You pick at some bricks senselessly",
	14, "Fuck Yeah, Ted Nugent.  Now if i just had a loin cloth",
	15, "This might help you pry a door open",
	16, "changing tires seems futile here",
	17, "Nnnnnn, Nnnnnn, you wish there were some ice around here",

	18, "You pound some Mountain Dew's.  Caffeine and Sugar, that'll keep ya goin", 
	19, "Crunch crunch crunch, You eat a bag of chips",
	20, "You open up a can of Tuna and eat it straight",
	21, "You smoke a joint to calm your nerves.  Nothin but existin...",
	22, "You smoke some cigarettes to calm down",
	23, "You eat a candy bar.  It tasted soo good",
	24, "You drink the firey kentucky whiskey.  It takes the edge off",
	0, "You eat some Peaches and think of Duane Allman", 
	0, "You slam some beer.  Burrrrrp ... better",
	0, "You reverantly down a bottle of Glenmorangie scotch.  Beautiful",
	0, "You drink the yummy imported beer.  Hey, someone had to drink it!" ,
	0, "You eat some beef jerky",

// tools
	0, "You should find a better place to camp",//30
	0, "You think of Mad Max and decide that its probably a good idea to save the gas for later",
	0, "You check all the bands and only hear static.",
	0, NULL , //wristwatch is special
	0, "Hmm, a set of boat keys.  You wonder what they go to?",  
	0, NULL, //35  cell-phone is special too
	0, "You practice your graffiti on a nearby wall.  - Ramo",
	0, "You're not sure what this is for, but you think it might be important",
	0, "A backpack will allow you to carry more stuff",
	0, "You put on the down coat.  Its good and toasty now.",
	0, "You put on a rain coat.  You feel very dry." ,//40
	0, "You should look for a better place to camp than here",
	0, "You turn on the camping stove and make a nice cup of tea",
	0, "You drink a long, cool draut of fresh water" ,
	0, "You carefully search the perimeter but find no zombies",
	0, "You practice setting some shrubbery on fire",//45
	0, "You practice whipping things",
	0, "You open a can of Spam. Given the circumstances, you rather enjoy it",
	0, "OMFG.  You open a package of smoked salmon and eat with great relish",
	0, "You open a can of creamed corn and throw it out because its rotten",
	0, "You open a can of chili and eat ravenously",//50
	0, "You eat a can of Beef Stew heartily",
	0, "You eat some of them thar beans ... the musical fruit",
	0, "Oh boy, chef boy-ar-dee.  You eat a can of ravioli",
	0, "You munch on a box of crackers with gusto",
	0, "You carve up a hard salami and savor every piece",
	0, NULL,
	0, NULL,
	0, NULL,
	0, NULL,
	0, NULL
};

int food_val[] = { 
	3, //mountaindew
	2, // chips
	4, // albacore
	5, // joint
	1, // smokes
	1, // candybar
	4, // whiskey
	1, // peaches
	7, // beer
	14, // glenmorangie
	9, // imports 
	5, // jerky
	4, // spam
	8,  // smoked salmon
	0, // creamed corn
	3, // chili
	4, // beef
	3, // beans
	4, // raviolis
	4, // crackers
	6  // salami
}; 
	

// messages are things that start with "You see "
// or "there is ... here"
// or "there are "
char *message[] = { " ",
		"You see a cloud of green gas",
		"The pages of a thousand newspapers whirl around the street in a dance",
		"You see a cloud of green gas hovering around a pile of body parts",
		"There is garbage strewn everywhere",
		"There is a Volkswagen bus completely covered in cockroaches on the curb",
		"You see two skeletons sitting inside a Volvo",
		"There are some tipped over mailboxes",
		"There was a terrific traffic accident here",
		"There is a headless zombie walking in circles here",
		"There is a jack-knifed semi blocking the road",
		"There is a building on fire across the street",
		"You're up to your waist in murky water",
		"There are dark purple clouds blanketing the sky", 
		"A truck had run over a fire-hydrant. Water is shooting 50 feet into the air!",
		"A zombie poodle is barking at you",
		"You see a chopper on the pad on top of the Channel 7 News building",
		"There is a dead jackass laying on the sidewalk wearing a hawkeye t-shirt",
		"Pieces of a crashed airliner are scattered everywhere",
		"You see some Starwars advertising. You think how silly it looks now",
		"From somewhere you here Bob Dylan singin, \"The times, they are a changin\"",
		"You see a giant billboard with a dumbass on it",
		"Down the street you see a group of zombies dancing on graves",
		"You see a dead Hells Angel",
		"Across the lake there is a lantern blinking S.O.S.",
		"A manhole cover has been removed.  You decide not to go down there.",
		"You see some looted stores",
		"Someone spray painted \"Remember Citadel\" on the side of a brick building",
		"You wonder where Steven King is...",
		"You barely makeout an airplane in the sky.",
		"Live powerlines are dangling from a nearby tree",
		"Someone pasted a dozen Nick Cave posters on the fence",
		"There is a loud explosion in the distance",
		"You see \"They Live\" scrawled in blood on a shop window",
		"You yell and wave your arms at a passing helicopter.  They didn't see you.",
		"You see a deadhead sticker on a Cadillac",
		"You hear a zombie say, \"brains\".  How cliche.",
		"A bluish haze decends upon the city", 
		"You curse at George Romero for being right",
		"You see mushroom clouds on the television in a department store display",
		"You feel relief at the sight of some dead hippies",
		"A brisk wind howls down the side-street",
		"There is a dead rich lady on a park bench who was probably afraid of death",
		"You see zombie George Peppard ducking into a fashionable menswear store",
		"You catch a whiff of rotted flesh",
		"You see a stencil of J.R. Bob Dobbs on the side of a house",
		"You wonder where Charlton Heston is...",
		"You realize George Bush is out of office and let out a huge sigh of relief",
		"it is raining",
		"it is overcast",
		"A recording on the loudspeaker says the city is under marshall law",
		"There is a full moon" };

#define MAX_MESSAGE (sizeof(message)/sizeof(char *))

char *good_bands[] = {
	"Tortoise", "The Sea & Cake", "Sam Prekop", 
	"The Chicago Underground Duo", "Interpol",
	"The Strokes", "The Kings of Convenience", 
	"Calexico", "The Decemberists", "Deathcab for Cutie",
	"Slint", "Mirah", "Radiohead", "Karate", 
	"Wilco", "Coldplay", "Avril Lavigne", 
	"Iron and Wine" 
};


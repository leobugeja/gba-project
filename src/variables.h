// Misc variables:	
	int posx=120;						// x-Position of the cannon
	u16 Int_Flag;						// Data storage for Flags while processing Interrupt HBlank
	int global_time = 0;				// global time reset for each level.
	int playerHealth=3; 				// variable for player health displayed on the bottom left corner
	int level=0;						// declaration of level

// Level Design ------------------------------------------------------------------------------------
	int level_1_setup = 0; // level 1 has not been set up yet
	int level_1_timing[5]; 
	int j ;						

// Background for Menu

	struct background { // structure with four attributes for the backgound enemies in the menu screen 
			int x_pos;
			int y_pos;
			int x_speed;
			int y_speed;
	};

	int backgroundSpriteOffset=50;		// Offset for organisation of the sprites
	struct background Background[5];		// Array of enemies moving as background in the menu

// Calculation of Score-----------------------------------------------------------------------------

	int totalNumLasers=0;
	int laserScore=0;
	int healthScore=0;
	int lasersNeeded=50;				// min number of lasers needed to win the game

// LASERS ------------------------------------------------------------------------------------------

	#define INT_BUTTON					0x1000

	int i=0; 							// counting variable	
	int laser_allowed=1; 			// shot function is allowed to run in the beginning
	int counter_lasers=10; 			// number of lasers player can use in one level
	int display_lasers=10;			// max number of lasers displayed at the same time	
	int laserSpriteOffset = 40; 	// arbitraily set that the projects will start from sprites 40 to 40+max laser number.




// Initialization of arrays: 
struct Projectile Lasers [10];  // creates an array of projectile objects that is the length of the max number of shots.


// Definition of Structures: 
struct Projectile { // structure for the project with three attributes
		int inUse;
		int x_pos;
		int y_pos;
	};


// ENEMIES -----------------------------------------------------------------------------------------

	int display_enemies = 20;			// max number of enemys displayed at the same time	
	int enemySpriteOffset = 10; 	// arbitraily set that the projects will start from sprites 10 to 10+max enemy number.
	int enemyHitbox=8;				// pixel size of the enemy hitbox

// Initialization of arrays: 
struct enemy Enemies [20];  // creates an array of projectile objects that is the length of the max number of shots.


// Definition of Structures: 
struct enemy { // structure for the project with attributes
		int inUse;			// Variable to help us differentiate between sprites that are used and ones that are free
		int mode;
		int health;			// health of the enemy counting down
		int x_pos;
		int y_pos;
		int x_speed;
		int y_speed;
};


// Text Variables
	int title_x_pos = 1000;
	int title_y_pos = 700;
	int title_x_speed = 1;
	int title_y_speed = 1;
	int title_color = 10;

// BOSSES ------------------------------------------------------------------------------------------

	int display_bosses = 5;			// max number of enemys displayed at the same time	
	int bossSpriteOffset = 60; 	// arbitraily set that the projects will start from sprites 60 to 60+display_bosses.
	int bossWidth=12;				// pixel size of the enemy hitbox
	int bossHeight=8;					// how high is the sprite, that displayes the boss

// Initialization of arrays for bosses: 
struct boss Bosses [5];  // creates an array of projectile objects that is the length of the max number of shots.


// Definition of Structures for bosses: 
struct boss { // structure for the project with three attributes
		int inUse;
		int health;
		int x_pos;
		int y_pos;
		int x_speed;
		int y_speed;
		int shot_delay;			// how long bewtween the attack of a boss
	   int animation_timer;
		int animation;				// State of animaion there was a plan for a blinking trump with two different sprites
		//int direction;
};

// TRUMP -------------------------------------------------------------------------------------------
int TrumpOffset = 71;			// Offset for sprite organisation
struct boss Trump;

// Menu

int MenueOffset=80;			// Offset for sprite organisation


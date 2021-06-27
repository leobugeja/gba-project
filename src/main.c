/* Corona Videogame for ME3241
TEAM 30: Unmesh Vanka, Leo Bugeja, Franz Lange
*/

#include "graphics.h" // inclusion of all header files
#include "gba.h" 
#include "variables.h"

// Include Assembler function: 

extern int calculateScore(int lasersNeeded, int laserScore, int healthScore);

#include "menu.h"
#include "functions.h"
#include "enemyFunctions.h" 
#include "bossFunctions.h" 
#include "lazerFunctions.h" 

// Definition of global constants:

#define INPUT  			(KEY_MASK & (~REG_KEYS))					// Defines Button Input

#define INT_BUTTON					0x1000								// Defines Button Interrupt Position in REG_IE & REG_IF
#define INT_HBLANK					0x0002								// Defines HBlank Interrupt Position in REG_IE & REG_IF

#define KEYCNT *(volatile u16*) 	0x4000132							// Defines Adresse of Key Interrupt Controller
#define KEYINPUT *(volatile u16*) 0x4000130							// Defines Adresse of Key Input Register (=1 for pressed; =0 for not pressed)


#define INT_Timer_0 					0x0008								// Defines Adresse of Timer_0 Interrupt in REG_IE & REG_IF
#define INT_Timer_3					0x0040								// Defines Adresse of Timer_3 Interrupt in REG_IE & REG_IF

// -----------------------------------------------------------------------------
// Handler function 
// -----------------------------------------------------------------------------

void Handler(void)
{

	REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one

	Int_Flag = REG_IF;	// Saving the flag registers

	if ((REG_IF & INT_BUTTON) == INT_BUTTON)		// Checks if button interrupt flag is elevated in REG_IF
	{
		checkbutton();			// checks button register for elevated flags
	}


	if ((REG_IF & INT_Timer_0) == INT_Timer_0) // Timer with high frequency
	{

		if (level == 1 | level == 2 | level == 3)			// only starts these functions if we are in a level
		{
			Collisions(); 										// tests every cycle if a collision between a laser and a enemy/boss/trump is happening

			moveLasers(); 										// decreases posy of active Lasers and sets them .inUse=0 when they reach posy==0
			moveEnemy();            						// changes the x (and/or y) values of the enemies
			moveBoss();

			healthcheckEnemy();								// cycles trough all enemies and changes the Enemies[i].inUse in regards to their Enemies[i].health 
			healthcheckBoss();								// same for bosses

			bossShoot();  										// creates enemy structs if bosses or trump is active and the time is right
		}

		if (level == 0) {
			moveTitle();										// moves the titel by changing x/y variables of the title sprites
			moveBackground();									// moves the background enemies by changing their x/y variables
		}

	}

	if ((REG_IF & INT_Timer_3) == INT_Timer_3) // Timer with slow frequency
	{
		global_time += 1;								// Global time for timing the enemy/boss/trump spawn
		// 1 unit of time is equal to 0.1 seconds
	}

	if ((REG_IF & INT_HBLANK) == INT_HBLANK) // INT_HBLANK Interrupt draws the picture, it contains all the draw sprite functions
	{
		// Draw the sprites for the menu
		drawMenu();
		drawTitle();
		drawAllBackground();
		// Draw the sprites for the levels
		drawGun();																// with sprites 0
		drawPlayerHealth(); 													// with sprites 1, 2, 3
		drawShotCounter(); 													// with sprites 4, 5
		drawSprite(level, 6, 230, 5);										// Draw level counter with sprite 6

		drawAllLasers();						// draws all Lasers that are Lasers[].inUse==1
		drawAllBosses();  					// draws the bosses that are Bosses[i].inUse==1
		drawAllEnemies();						// same for the enemies

	}

	REG_IF = Int_Flag;						// backup flag registers

	KEYINPUT = KEYINPUT; 				// Update interrupt table buttons, to confirm we have handled this interrupt
	REG_IF = REG_IF; 						// Update interrupt table, to confirm we have handled this interrupt

	REG_IME = 0x01;  						// Re-enable interrupt handling
}												// handler 

// -----------------------------------------------------------------------------
// MAIN Code 
// -----------------------------------------------------------------------------

int main(void)
{

	int i;

	// Set Mode 2
	*(unsigned short*)0x4000000 = 0x40 | 0x2 | 0x1000;

	// Fill SpritePal
	// Define the colors

	*(unsigned short*)0x5000200 = 0; // Background Black
	*(unsigned short*)0x5000202 = RGB(25, 25, 25); // White   
	*(unsigned short*)0x5000204 = RGB(31, 0, 0); // Red      
	*(unsigned short*)0x5000206 = RGB(20, 20, 20); // Grey 
	*(unsigned short*)0x5000208 = RGB(31, 31, 5); // Yellow 
	*(unsigned short*)0x500020A = RGB(31, 22, 5); // Skin  
	*(unsigned short*)0x500020C = RGB(31, 31, 25); // Cream 
	*(unsigned short*)0x500020E = RGB(10, 10, 31); // Navy  
	*(unsigned short*)0x5000210 = RGB(31, 25, 23); // Pink  
	*(unsigned short*)0x5000212 = RGB(0, 31, 0); // Green 
	*(unsigned short*)0x5000214 = RGB(10, 0, 20); // Purple
	*(unsigned short*)0x5000216 = RGB(10, 31, 20); // Special Color (can change)
	*(unsigned short*)0x5000218 = RGB(0, 0, 31); // Light Blue Bl

   // Fill SpriteData   
	for (i = 0; i < 62 * 8 * 8 / 2; i++)
		spriteData[i] = (graphics[i * 2 + 1] << 8) + graphics[i * 2];

	// Set Handler Function for interrupts and enable selected interrupts

	REG_IME = 0x0;		// Disable interrupt handling  

	REG_INT = (int)&Handler;

	REG_IE |= INT_BUTTON;			// enable button interruts
	REG_IE |= INT_HBLANK;			// enable horizontal blank interrupt

	REG_IE |= INT_Timer_0;			// enable Timer_0
	REG_IE |= INT_Timer_3;			// enable Timer_3 

	KEYCNT |= 0x7FFF;   				// Enable all buttons
	REG_DISPSTAT |= 0x10;				// Enable H_Blank signal from display

	REG_TM0D = 64000;									 								// initialize start point of Timer_0
	REG_TM0CNT |= TIMER_FREQUENCY_64 | TIMER_ENABLE | TIMER_INTERRUPTS;		// initialize high frequency timer: Timer_0 

	 // overloads every 0.1 milisecond to increament timer
	REG_TM3D = 39316;																	// initialize start point of Timer_0
	REG_TM3CNT |= TIMER_FREQUENCY_64 | TIMER_ENABLE | TIMER_INTERRUPTS;		// initialize high frequency timer: Timer_0

	REG_IME = 0x1;		// Enable interrupt handling


	// LEVEL Characters Setup -----------------------------------------
	// Draw the level sprites in the top right corner
	drawLetter(0, 100, 194, 4); // L
	drawLetter(1, 101, 200, 4); // E
	drawLetter(2, 102, 206, 4); // V
	drawLetter(1, 103, 212, 4); // E
	drawLetter(0, 104, 218, 4); // L
	drawLetter(3, 105, 223, 5); // :

	// -----------------------------------------------------------------------------
	// MAIN GAME LOOP     
	// -----------------------------------------------------------------------------
start:
	laserScore = totalNumLasers;			// save the lasers used in the last round as results for score
	healthScore = playerHealth;			// save the pkayer health of the last round for score
	totalNumLasers = 0;						// reset the number of lasers shot for the next round
	resetGame(); 							// reset all the key parameters of the game
	createBackgound();					// the background in the menu are created

	while (1) {								// while loop to wait until the game starts and level value changes from 0 to 1
		if (level == 1) {
			createEnemy(1, randInt(8, 232), 0, 1); // Create random enemies that drop from the ceiling in the first Level
			break;
		}
	}

	// LEVEL 1 -----------------------------------------
	int level_1_timing[13];
	int j = 1;
	level_1_timing[0] = global_time;

	for (j; j <= 12;j++)
	{
		level_1_timing[j] = level_1_timing[j - 1] + randInt(20, 30);   // create an array of consecutive timing for the enemies to come in. randInt means that the time interval will be between 2-3 seconds
	}
	int enemy_count = 1;	  // denotes the current enemy that will come in

	while (1)
	{
		if (level != 1) // if the level no longer equals 1 then reset the game
		{
			goto start;
		}

		if (global_time >= level_1_timing[enemy_count]) // if the enemy incoming time is equal to the current time, then create the boss or enemy.
		{
			if (enemy_count == 3) // if enemy_count == 3 then creat a boss, otherwise create and enemy
			{
				createBoss(randInt(30, 200), 1, 0);
			}
			else
			{
				createEnemy(1, randInt(8, 210), 0, 1);
			}

			enemy_count += 1;	// increment to be the next enemy

			if (enemy_count == 12) // if all 12 enemies for level 1 have been created then exit out o fwhile loop
			{
				break;
			}
		}

		if (playerHealth == 0) // if at any point the player health is 0, then reset the game back to the menu screen
		{
			goto start;
		}

	}

	while (1) { // wait till bosses are all dead from level 1 then break to go to level 2
		if (allBossesDead()) {
			break;
		}

		if (playerHealth == 0)
		{
			goto start;
		}
	}

	// LEVEL 2 --------------------------------------------------
	level = 2;
	createBoss(randInt(8, 100), 1, 1);
	createBoss(randInt(100, 210), -1, -1);

	while (1) { // wait till bosses are all dead from level 2 then break to level 3
		if (allBossesDead())
		{
			break;    
		}
		if (playerHealth == 0)
		{
			goto start;
		}
	}

	// LEVEL 3 --------------------------------------------------
	level = 3;
	int level_3_start = global_time;   // set the initial start time of level 3 to the global time
	enemy_count = 1;

	while (1)
	{
		if (level != 3 | (enemiesDead() & TrumpDead() & enemy_count == 0))
		{
			goto start;
		}

		if ((global_time >= level_3_start + 10) & (enemy_count == 1))  // create trump boss after 1 second of level 3 start time
		{
			createTrump(100);
			enemy_count = 0;
		}

		if (playerHealth == 0)
		{
			goto start;
		}
	}

	goto start;  // once reach bottom of game loop, return back to the start.
}


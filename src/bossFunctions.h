int TrumpDead(void) {      // returns true if trump is dead
	return 1 - Trump.inUse;
}

int allBossesDead(void) {    // returns true if all the bosses are dead
	int alldead = 1;
	int i = 0;
	for (i;i < display_bosses;i++) {      // Scans through boss array
		if (Bosses[i].inUse == 1) {
			alldead = 0;                  // Bosses are alive if alldead = 0
		}
	}
	return alldead;
}


void createBoss(int x_start, int x_speed, int y_speed)	// Creates a struct/object in the Lasers Array with the posx of the gun
{
	int i = 0;
	for (i; i < display_bosses; i++) {
		if (Bosses[i].inUse == 0) {                         // Finds first free boss sprite position in boss array
			Bosses[i].x_pos = x_start * 10;                 // y_pos and x_pos multiplied by 10 for better speed control
			Bosses[i].y_pos = 6 * 10;                       // Spawns at top of screen
			Bosses[i].inUse = 1;
			Bosses[i].health = 3;                          
			Bosses[i].x_speed = x_speed;
			Bosses[i].y_speed = y_speed;
			Bosses[i].shot_delay = global_time + 30;        // Can only fire enemy sprites after every 30 time increments
			Bosses[i].animation_timer = global_time + 3;    // Bat sprite switches between sprite 1 and 2 every 3 time increments
			Bosses[i].animation = 1;
			break; // break for loop once found an available enemy space.
		}
	}
}

void createTrump(int x_start) 			// Creates a struct/object in the Lasers Array with the posx of the gun
{
	if (Trump.inUse == 0) {             // Only 1 trump sprite so no trump array
		Trump.x_pos = x_start * 10;
		Trump.y_pos = 10;
		Trump.inUse = 1;
		Trump.health = 4;
		Trump.x_speed = 1;
		Trump.y_speed = 0;              // Only moves horizontally
		Trump.shot_delay = global_time + 10;         // Shoots enemy sprites every 10 time increments
	}
}

void drawTrump(int image, int N, int x, int y)								// Draws laser, as they have their sprites are between bossSpriteOffset -> (bossSpriteOffset+display_bosses)
{
	N = N + TrumpOffset;            
	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	// trying to get trump to verticaly flip depending on his direction facing but doesnt work
	if (Trump.x_speed == 1) {                                     // Draws trump normally if moving to left
		*(unsigned short*)(0x7000002 + 8 * N) = x | 0x8000;
	}
	else {                                                        // Flips trump if moving to right
		*(unsigned short*)(0x7000002 + 8 * N) = x | 0x8000; // adjust 0x8000 to flip
	}
	*(unsigned short*)(0x7000004 + 8 * N) = (image + 18) * 2;
}

void drawBoss(int boss, int N, int x, int y)								// Draws laser, as they have their sprites are between bossSpriteOffset -> (bossSpriteOffset+display_bosses)
{
	N = N + bossSpriteOffset;                                       
	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	*(unsigned short*)(0x7000002 + 8 * N) = x | 0x4000;
	if (Bosses[boss].animation == 1)                                // Draws different sprite for bat to animate it
	{
		*(unsigned short*)(0x7000004 + 8 * N) = (boss + 39) * 2;    // Wings up vs wings down
	}
	else {
		*(unsigned short*)(0x7000004 + 8 * N) = (boss + 35) * 2;    
	}
}

void drawAllBosses(void)		// draws a boss if it is in use
{
	int i = 0;
	for (i; i < display_bosses; i++)          // Scans through boss array
	{
		if (Bosses[i].inUse == 1)             // Only draws bossses in use
		{
			drawBoss(0, i, Bosses[i].x_pos / 10, Bosses[i].y_pos / 10);  // x and y_pos are divided by 10 because all their postions are 10x their pixel value so they can move slower
		}
		else if (Bosses[i].inUse == 0)        // Bosses not in use are drawn off screen 
		{
			drawBoss(0, i, 270, 0);
		}
	}

	if (Trump.inUse == 1) {                  // Checks if the trump boss is in use
		drawTrump(0, 0, Trump.x_pos / 10, Trump.y_pos / 10);
	}
	else {
		drawTrump(0, 0, 270, 0);
	}
}


void moveBoss() {
	// increases y_pos of all active enemies objects by its speed
	int i = 0;
	for (i; i < display_bosses; i++)         // Scans through boss array
	{
		if (Bosses[i].inUse == 1)            // Only increment active bosses
		{

			if (Bosses[i].x_pos == 0 | Bosses[i].x_pos == 224 * 10)         // Inverses x speed if boss hits x boundary
			{
				Bosses[i].x_speed = Bosses[i].x_speed * -1;
			}
			Bosses[i].x_pos = Bosses[i].x_pos + Bosses[i].x_speed;          // Increments x position by adding speed in x direction

			if (Bosses[i].y_pos <= 5 | Bosses[i].y_pos >= 50 * 10)          // Inverses y speed if boss hits y bounrdaru
			{
				Bosses[i].y_speed = Bosses[i].y_speed * -1;                 
			}
			Bosses[i].y_pos = Bosses[i].y_pos + Bosses[i].y_speed;          // Increments y position by adding y speed
		}
		// animate the bat sprite to switch every 0.3 seconds
		if (global_time + 3 > Bosses[i].animation_timer)
		{
			Bosses[i].animation_timer = Bosses[i].animation_timer + 3;     // Adds another 3 time increments once condition is met
			Bosses[i].animation = Bosses[i].animation * -1;                // Changes boss animation that is drawn
		}

	}

	if (Trump.inUse == 1) {
		if (Trump.x_pos == 0 | Trump.x_pos == 224 * 10)                // Inverses x speed once trump reaches x boundary
		{
			Trump.x_speed = Trump.x_speed * -1;   
		}
		Trump.x_pos = Trump.x_pos + Trump.x_speed;                     // The x position is the current x position plus the x speed

		if (Trump.y_pos <= 5 | Trump.y_pos >= 50 * 10)                 // Inverses y speed once trump reaches y boundary
		{
			Trump.y_speed = Trump.y_speed * -1;
		}
		Trump.y_pos = Trump.y_pos + Trump.y_speed;                     // The y position is the current y position plus the y speed
	}
}

void healthcheckBoss(void)   // loops though all the bosses and if there health is 0 set their use to 0
{
	int i = 0;
	for (i; i < display_bosses; i++)        // Scans through all bosses
	{

		if (Bosses[i].health == 0)          // Despawns dead bosses
		{
			Bosses[i].inUse = 0;
		}
	}
	if (Trump.health == 0) {                // Despawns trump if dead
		Trump.inUse = 0;
	}
}

void bossShoot(void)  // sets up the delay timing between shooting shots from the boss
{
	int i = 0;
	for (i; i < display_bosses; i++)        // Scans through bosses
	{
		if (Bosses[i].inUse == 1)           // For active bosses
		{
			if (Bosses[i].shot_delay <= global_time)            // Only shoots if shot timer is equal to global time
			{
				createEnemy(1, Bosses[i].x_pos / 10, Bosses[i].y_pos / 10 + 16, 1);    // Shoots by creating enemy at same x position and y position as boss
				Bosses[i].shot_delay += 30;                     // Adds another 30 time increments to shot timer (another enemy will spawn in 30 time increments)
			}
		}
	}
	if (Trump.inUse == 1)
	{
		if (Trump.shot_delay <= global_time)           // Shoots every 60 time increments
		{
			createEnemy(2, Trump.x_pos / 10, Trump.y_pos / 10 + 16, 1);                                   // Enemy goes to left
			createEnemy(3, Trump.x_pos / 10 + 8 * Trump.x_speed, Trump.y_pos / 10 + 16, Trump.x_speed);   // enemy goes same direction as trump
			createEnemy(2, Trump.x_pos / 10, Trump.y_pos / 10 + 16, -1);                                  // Enemy goes right
			Trump.shot_delay += 60;                    // Adds another 60 time increments to shot timer
		}
	}
}


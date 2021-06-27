
// Definition of Functions: 

void createEnemy(int mode, int x_start, int y_start, int direction) 	// Creates a struct/object in the Lasers Array with the posx of the gun
{
	int i = 0;
	for (i; i < display_enemies; i++) {
		if (Enemies[i].inUse == 0) {                 // Finds first not in use enemy sprite in enemy array
			Enemies[i].mode = mode;                  // Determines enemy sprites moving behaviour (vertical vs diagonal)   
			Enemies[i].x_pos = x_start * 10;         // X and y position multipied by 10 for better speed control
			Enemies[i].y_pos = y_start * 10;
			Enemies[i].inUse = 1;                  
			Enemies[i].health = 1;
			Enemies[i].y_speed = 1;
			if (mode == 2) {                            
				Enemies[i].x_speed = 3 * direction;  // Fast diagonal movement
			}
			else if (mode == 3) {
				Enemies[i].x_speed = direction;      // Slow diagonal movement
			}
			else {
				Enemies[i].x_speed = 0;              // Straight down movement for other modes
			}
			break; // break for loop once found an available enemy space.
		}
	}
}

void drawEnemy(int virus, int N, int x, int y)								// Draws laser, as they have their sprites are between laserSpriteOffset -> (laserSpriteOffset+display_lasers)
{
	N = N + enemySpriteOffset;
	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	*(unsigned short*)(0x7000002 + 8 * N) = x | 0x4000;
	*(unsigned short*)(0x7000004 + 8 * N) = (virus + 10) * 2;
}

void drawAllEnemies(void)   // loops through all the enemy sprites and draws them if they are in use
{
	int i = 0;
	for (i; i < display_enemies; i++)				// Scans through enemy array
	{
		if (Enemies[i].inUse == 1)                  // Only draws in use enemy sprites
		{
			drawEnemy(0, i, Enemies[i].x_pos / 10, Enemies[i].y_pos / 10);      // x.pos and y.pos are defined as 10 times actual x and y
		}
		else if (Enemies[i].inUse == 0)
		{
			drawEnemy(0, i, 270, 0);                // Draws inactive sprites off screen
		}
	}
}

void moveEnemy() {
	// increases y_pos of all active enemies objects by its speed
	int i = 0;
	for (i; i < display_enemies; i++) {
		if (Enemies[i].inUse == 1) {
			if (Enemies[i].x_pos > 230 * 10 | Enemies[i].x_pos < 10 * 10) {       // Inverts enemy direction when they hit the side boundaries
				Enemies[i].x_speed = Enemies[i].x_speed * -1;
			}
			Enemies[i].x_pos = Enemies[i].x_pos + Enemies[i].x_speed;             // If enemy sprite is in mode 2 or 3

			Enemies[i].y_pos = Enemies[i].y_pos + Enemies[i].y_speed;             // Always increments y position

			if (Enemies[i].y_pos >= 150 * 10)         // Despawns enemies if they reach the bottom of the screen
			{
				Enemies[i].inUse = 0;

				playerHealth--;						  // decreases playerHealth when enemy hits bottom
			}
		}
	}
}


void healthcheckEnemy(void)	// funciton to check health of enemies and if their health is 0 then set their use to 0
{
	int i = 0;
	for (i; i < display_enemies; i++)         // Scans through enemy array
	{

		if (Enemies[i].health == 0)
		{
			Enemies[i].inUse = 0;            // Despawns all dead enemies
		}
	}
}

int enemiesDead(void)   //  returns true if all the enemies are dead
{
	int x = 1;
	int i = 0;
	for (i; i < display_enemies; i++)     // Scans through enemy array
	{

		if (Enemies[i].inUse == 1)      
		{
			x = 0;                       // If enemies are still alive x = 0
		}
	}

	return x;
}


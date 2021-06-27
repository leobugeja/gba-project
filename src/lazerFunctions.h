
// Definition of Functions: 

void createLaser(int x_start) 													// Creates a struct/object in the Lasers Array with the posx of the gun
{
	int i = 0;
	for (i; i < display_lasers; i++) {
		if (Lasers[i].inUse == 0) {                 // Finds first free laser sprite                      
			Lasers[i].x_pos = x_start;              // X position defined by player's x position
			Lasers[i].y_pos = 150;                  // Bottom of screen
			Lasers[i].inUse = 1;                  
			break; // break for loop once found an available laser space.
		}
	}
}

void Collisions() 								// Detects collisions between lasers and other objects
{
	int i = 0;
	for (i; i < display_lasers; i++)            // scans through all laser sprites
	{

		if (Lasers[i].inUse == 1)               // Only checks laser sprites that are in use
		{

            // Scans through every enemy sprite
			int j = 0;
			for (j; j < display_enemies; j++) // if laser is within the collision box of an active enemy, then remove health from that enemy and remove the laser
			{
				if ((Lasers[i].y_pos >= (Enemies[j].y_pos / 10)) & (Lasers[i].y_pos <= (Enemies[j].y_pos / 10 + enemyHitbox)) & (Enemies[j].inUse))
				{
					if (Lasers[i].x_pos >= (Enemies[j].x_pos / 10) & (Lasers[i].x_pos <= (Enemies[j].x_pos / 10) + ((enemyHitbox) * 2) - 2))
					{
						Lasers[i].inUse = 0;    // laser disappears after collision
						Enemies[j].health--;
					}
				}
			}
             
			// This forloop scans through every boss sprite
			int z = 0;
			for (z; z < display_bosses; z++)  // if laser is within the collision box of an active boss, then remove health from that enemy and remove the laser
			{
				if ((Lasers[i].y_pos >= (Bosses[z].y_pos / 10)) & (Lasers[i].y_pos <= (Bosses[z].y_pos / 10 + bossHeight)) & (Bosses[z].inUse))
				{
					if ((Lasers[i].x_pos >= (Bosses[z].x_pos / 10)) & (Lasers[i].x_pos <= (Bosses[z].x_pos / 10 + bossWidth)))
					{
						Lasers[i].inUse = 0;   // laser disappears after collision
						Bosses[z].health--;
					}
				}
			}
			// if laser is within the collision box of an active trump, then remove health from that enemy and remove the laser
			if ((Lasers[i].y_pos >= (Trump.y_pos / 10)) & (Lasers[i].y_pos <= (Trump.y_pos / 10 + 16)) & (Trump.inUse))
			{

				if ((Lasers[i].x_pos >= (Trump.x_pos / 10 + 8)) & (Lasers[i].x_pos <= (Trump.x_pos / 10 + 24)))
				{
					Lasers[i].inUse = 0;
					Trump.health--;
				}
			}
		}
	}
}

void drawLaser(int numb, int N, int x, int y)								// Draws laser, as they have their sprites  between laserSpriteOffset -> (laserSpriteOffset+display_lasers)
{
	N = N + laserSpriteOffset;
	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	*(unsigned short*)(0x7000002 + 8 * N) = x;
	*(unsigned short*)(0x7000004 + 8 * N) = (numb + 34) * 2;
}

void drawAllLasers(void) // loops through all the lasers sprites and draws them if they are in use
{
	int i = 0;
	for (i; i < display_lasers; i++)                               // Scans through laserm array
	{
		if (Lasers[i].inUse == 1)
		{
			drawLaser(0, i, Lasers[i].x_pos, Lasers[i].y_pos);     // Only draws active lasers on screen
		}
		else if (Lasers[i].inUse == 0)
		{
			drawLaser(0, i, 270, 0);                               // All inactive lasers are drawn off screen
		}
	}

	// Added to check if the button key is up so that holding down the up key does not continously shoot
	u16 buttons = INPUT;
	if ((buttons & KEY_UP) != KEY_UP) {
		laser_allowed = 1;
	}
}



void moveLasers() {															// reduces y_pos of all active laser objects by one and sets inUse=0 when they reach the top border of the screen
	i = 0;
	for (i; i < display_lasers; i++)                    
	{
		if (Lasers[i].inUse == 1)                         // Only increments active lasers in laser array
		{
			Lasers[i].y_pos = Lasers[i].y_pos - 1;

			if (Lasers[i].y_pos == 0)
			{
				Lasers[i].inUse = 0;                      // Lasers despawn once they reach the top of the screen
			}
		}
	}
}


void reload()      // reloads the laser counter
{
	counter_lasers = 10;
}

void shoot(int posx)
{
	if (laser_allowed == 1 && counter_lasers > 0)					// To stop "overshooting": shot function must be allowed to run by the timer & To control amount of shots fired
	{
		createLaser(posx);
		counter_lasers--;
		totalNumLasers++;
	}
	laser_allowed = 0;					// To stop "overshooting": shot function now not allowed to run
}

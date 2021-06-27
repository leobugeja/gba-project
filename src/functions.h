#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

int randInt(int lower, int upper) { 					// returns a random number between the upper and lower limit
	int num = (rand() % (upper - lower + 1)) + lower;

	return num;
}


void resetGame() {						// reset all the key values (after an automatic reset when going back to the menu or a user forced reset)
	level = 0;
	wipeSprites();
	playerHealth = 3;
	counter_lasers = 10;
}

void drawSprite(int numb, int N, int x, int y) // default function for drawing a sprite
{
	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	*(unsigned short*)(0x7000002 + 8 * N) = x;
	*(unsigned short*)(0x7000004 + 8 * N) = numb * 2;
}

void drawLetter(int letter, int N, int x, int y) // draws a character sprite
{

	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	*(unsigned short*)(0x7000002 + 8 * N) = x;
	*(unsigned short*)(0x7000004 + 8 * N) = (letter + 43) * 2;
}

void drawLungs(int numb, int N, int x, int y)  // draws the lung sprite
{

	*(unsigned short*)(0x7000000 + 8 * N) = y | 0x2000;
	*(unsigned short*)(0x7000002 + 8 * N) = x | 0x4000;;
	*(unsigned short*)(0x7000004 + 8 * N) = (numb + 14) * 2;
}

void drawGun() // if level is 0, then draw off screen otherwise draw a 4*4 hand gel sprite
{
	if (level == 0)
	{
		drawSprite(0, 0, 270, 0);									   	// Draw shooter/gun with sprite 0 outside of screen
	}
	else
	{
		*(unsigned short*)(0x7000000) = 146 | 0x2000;				// Draw shooter/gun with sprite 0 at its x postion
		*(unsigned short*)(0x7000002) = (posx - 4) | 0x4000;;
		*(unsigned short*)(0x7000004) = (58) * 2;
	}
}
void drawShotCounter()  // draw shot counter in bottom right of the screen
{
	if (level == 0)
	{
		drawSprite(0, 4, 270, 0);			// Draw left shot counter number with sprite 4
		drawSprite(0, 5, 270, 0);			// Draw right shot counter number with sprite 5
	}
	else
	{
		drawSprite((counter_lasers / 10) % 10, 4, 220, 150);			// Draw left shot counter number with sprite 4
		drawSprite(counter_lasers % 10, 5, 230, 150);					// Draw right shot counter number with sprite 5
	}
}


void drawPlayerHealth()  // Draw different amounts of lung sprites in the bottom left corner of the screen corresponding to the amount of health left
{
	if (level == 0)  // if level == 0 draw lungs off screen
	{
		drawLungs(0, 1, 270, 0);
		drawLungs(0, 2, 270, 0);
		drawLungs(0, 3, 270, 0);
	}
	else
	{

		if (playerHealth == 3)
		{
			drawLungs(0, 1, 0, 146);
			drawLungs(0, 2, 18, 146);
			drawLungs(0, 3, 36, 146);
		}

		if (playerHealth == 2)
		{
			drawLungs(0, 1, 0, 146);
			drawLungs(0, 2, 18, 146);
			drawLungs(0, 3, 0, 270);
		}

		if (playerHealth == 1)
		{
			drawLungs(0, 1, 0, 146);
			drawLungs(0, 2, 0, 270);
			drawLungs(0, 3, 0, 270);
		}

		if (playerHealth == 0)
		{
			drawLungs(0, 1, 0, 270);
			drawLungs(0, 2, 0, 270);
			drawLungs(0, 3, 0, 270);

		}
	}
}

void wipeSprites()		// Set every .inUse to zero so that the structs can be reused and the sprites get drawn out of sight at position (270, 0)
{

	i = 0;
	for (i; i < display_lasers; i++) {
		Lasers[i].inUse = 0;
	}

	i = 0;
	for (i; i < display_enemies; i++) {
		Enemies[i].inUse = 0;
	}
	i = 0;
	for (i; i < display_bosses; i++) {
		Bosses[i].inUse = 0;
	}
	Trump.inUse = 0;

}


void checkbutton(void)		// function that checks the flag in the button register for inputs
{
	u16 buttons = INPUT;

	if ((buttons & KEY_A) == KEY_A)					// Franz: for me this key is Y on my keyboard, for you guys probably Z
	{
		if (level == 0) {								// start level one by setting variable "level" and "playerHealth"
			level = 1;
			playerHealth = 3;
		}
	}
	if ((buttons & KEY_B) == KEY_B)
	{
		if (level != 0) { // resets game when pressing key
			resetGame();
		}
	}
	if ((buttons & KEY_RIGHT) == KEY_RIGHT)
	{
		if (posx < 232)				// change the x postition of the gun, but only until it hits the rigth screen border
		{
			posx = posx + 2;
		}
	}
	if ((buttons & KEY_LEFT) == KEY_LEFT)
	{
		if (posx > 0)				// change the y postition of the gun, but only until it hits the left screen border
		{
			posx = posx - 2;
		}
	}
	if ((buttons & KEY_UP) == KEY_UP)
	{
		if (level != 0) {		// if we are in a level shoot a laser
			shoot(posx);
		}
	}
	if ((buttons & KEY_DOWN) == KEY_DOWN)
	{
		reload();					// change the shot counter at the right hand corner back to its start value, so that we can shoot again

	}
}


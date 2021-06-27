
int randSpeed() {  // funciton to return -1 or 1 randomly
	 int direction = (rand() % (2)) + 1; 
	 int x=0;	
	
	if (direction==1)
		{x=1;}
	else if  (direction=2)
		{x=-1;}

	 return x;
} 

void drawMenu()   // funciton to draw all the menue items
{	int x=10;
	int y=100;
	int score=100;

	if (level==0)
	{
		// Laser/Score Counter

		score = calculateScore(lasersNeeded, laserScore, healthScore);
		drawSprite((score/10)%10, 80, 135-x, y);			// Draw left totalNumLasers with sprite 80
		drawSprite(score%10, 81, 142-x, y);				// Draw right totalNumLasers with sprite 81

		// Score
		drawLetter(4, 106, 100-x, y); // S
		drawLetter(5, 107, 106-x, y); // C 
		drawLetter(6, 108, 112-x, y); // O
		drawLetter(7, 109, 118-x, y); // R
		drawLetter(1, 110, 124-x, y); // E 
		drawLetter(3, 111, 129-x, y); // :
	}
	else
	{
		drawSprite(0, 80, 270, 0);			// Draw left totalNumLasers with sprite 80
		drawSprite(0, 81, 270, 0);				// Draw right totalNumLasers with sprite 81

		drawLetter(4, 106, 270, 0); // S
		drawLetter(5, 107, 270, 0); // C 
		drawLetter(6, 108, 270, 0); // O
		drawLetter(7, 109, 270, 0); // R
		drawLetter(1, 110, 270, 0); // E 
		drawLetter(3, 111, 270, 1); // :
	}
}

void moveBackground()						// changes the x and y variables of the backround enemies in the menu 
{
															
	int i = 0;
	for (i; i < 5; i++)			// cycles trough all the background enemies
	{
			
			if (Background[i].x_pos==0 | Background[i].x_pos==224*10)		// if the x postion is equivalent to the x screen border values (0/224) then negate the x_speed to move in the other direction
			{
					Background[i].x_speed = Background[i].x_speed*-1;
			}
			Background[i].x_pos = Background[i].x_pos + Background[i].x_speed;	// add
 
			if (Background[i].y_pos<=5 | Background[i].y_pos>=145*10)
			{
					Background[i].y_speed = Background[i].y_speed*-1;
			}
			Background[i].y_pos = Background[i].y_pos + Background[i].y_speed;
		
	}

}

void createBackgound(void) 													// Creates a struct/object in the Backgound Array with random posx
{
	int i = 0;
	for (i; i < 5; i++) 
		{
			Background[i].x_pos = randInt(8,2100);
			Background[i].y_pos = randInt(8,1300);
			Background[i].y_speed = randSpeed();
			Background[i].x_speed = randSpeed();;
		} 
	
}

void drawAllBackground(void) // draw all background items
{
	int i = 0;
	for (i; i < 5; i++) 
		{
		if (level==0) 
			{
			drawBackground(0, i, Background[i].x_pos/10, Background[i].y_pos/10); 
			}
		else 
			{
			drawBackground(0, i, 270, 0); 
			}
		}
}

void drawBackground(int virus, int N, int x, int y)			// Draws laser, as they have their sprites are between laserSpriteOffset -> (laserSpriteOffset+display_lasers)
{
	 N = N + backgroundSpriteOffset ;
	 *(unsigned short *)(0x7000000 + 8*N) = y | 0x2000;
    *(unsigned short *)(0x7000002 + 8*N) = x | 0x4000;
    *(unsigned short *)(0x7000004 + 8*N) = (virus+10)*2;
}


void drawTitle(void) {     // draws the game title
	if (level == 0) {
	// Corona Killer (game title)
	int x_pos = title_x_pos/10;
	int y_pos = title_y_pos/10;
	drawLetter(5, 112, x_pos, y_pos); // C 
	drawLetter(6, 113, x_pos + 6, y_pos); // O
	drawLetter(7, 114, x_pos + 12, y_pos); // R
	drawLetter(6, 115, x_pos + 18, y_pos); // O
	drawLetter(9, 116, x_pos + 24, y_pos); // N
	drawLetter(8, 117, x_pos + 30, y_pos); // A

	drawLetter(10, 118, x_pos + 36 + 6, y_pos); // K
	drawLetter(11, 119, x_pos + 42 + 6, y_pos); // I
	drawLetter(0, 120, x_pos + 48 + 6, y_pos); // L
	drawLetter(0, 121, x_pos + 54 + 6, y_pos); // L
	drawLetter(1, 122, x_pos + 60 + 6, y_pos); // E
	drawLetter(7, 123, x_pos + 66 + 6, y_pos); // R

	} else {
		// else wipe all sprites
		int i = 112;
		for(i;i<=123;i++) {
			drawLetter(0, i, 300, 0); // R
		}
	}
}

void moveTitle(void) { 			// function to draw the game title
	if (title_x_pos < 500|title_x_pos > 1200) {
		title_x_speed = title_x_speed*-1;
		*(unsigned short *) 0x5000216 = RGB(10,0,20); // Special 
	}
	if (title_y_pos < 400|title_y_pos > 800) {
		title_y_speed = title_y_speed*-1;
		*(unsigned short *) 0x5000216 = RGB(10,31,20); // Special 
	}
	title_x_pos = title_x_pos + title_x_speed;
	title_y_pos = title_y_pos + title_y_speed;
}


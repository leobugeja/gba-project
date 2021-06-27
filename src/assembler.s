.GLOBL calculateScore
calculateScore:
    swp     r4, r4, [sp]			 @ pop last input argument from stack and put it in r4
    stmfd   sp!, {r4-r11, lr}     @ save content of r4-r11 and link register into the sp register
    
    @ function body:
	// r0: lasersNeeded (Score); r1: laserScore (lasersExceeded) , r2: int healthScore (livesLost), r3: varialbe for calculations
		rsb r2, r2, #3				// livesLost=3-healthScore
		subs r1, r1, r0				// lasersExeeded=laserScore-lasersNeeded
		
		mov r0, #100				// score=100
		bmi break					//if (lasersExeeded<0) {goto break}

		cmp r1, #15					// compare lasersExceeded to 15
		suble r0, r0, r1, LSL #1 // if (lasersExceeded<=15) {score=score-lasersExceeded*2}

		subgt r1, r1, #15		//if (lasersExceeded>15) {lasersExceeded-15)
		subgt r0, r0, r1		//if (lasersExceeded>15) {score=score-(lasersExceeded-15)}
		subgt r0, r0, #30		//if (lasersExceeded>15) {score=score-30}

		
		mov r3, r2, LSL #5			// x=32*livesLost
		sub r3, r3, r2, LSL #1	// x=x-2*livesLost
		subs r0, r0, r3			// score=score-x (x=30*livesLost)
		movmi r0, #0				// if (score<0) {score=0;}

		cmp r2, #3		//if(livesLost==3)
		moveq r0, #0	//score=0;
		break:
/*
// Above function in C

int calculateScore(int lasersNeeded, int laserScore, int healthScore)
{

	int livesLost=3-healthScore;
	int lasersExceeded=laserScore-lasersNeeded;
	
	
	if (lasersExceeded<=15)
	{
		score=score-lasersExceeded*2;
	}
	else
	{
		score=score-(lasersExceeded-15);				
		score=score-30;
	}
		
	score=score-(livesLost*30);
	if(score<0)
	{
		score=0;
	}

	if ((livesLost==3))
	{
		score=0;
	}



	return score;
}
*/

    @ Exit from function:
    ldmfd   sp!, {r4-r11, lr}     @ Recover past state of r4-r11 and link register from sp register
    swp     r4, r4, [sp]			 @ Restore state of r4
    mov     pc, lr					 @ Branch back to lr (go back to C code that called this function)


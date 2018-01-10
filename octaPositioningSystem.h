#ifndef OCTA_POSITIONING_SYSTEM_H
#define OCTA_POSITIONING_SYSTEM_H

/*
 *	x position (square wise)
 */
byte xSquare;

/*
 *	y position (square wise)
 */
byte ySquare;

/*
 *	x position inside square (0 - 255)
 */
byte x;

/*
 *	y position inside square (0 - 255)
 */
byte y;

/*
 *	the direction waiter is facing in degrees (0-359)
 *  0 = North
 */
int direction;

/*
 *
 */
task calculatePosition() {
	static float dpmA = rpmToDistance(OUT_A); 
	static float dpmB = rpmToDistance(OUT_B);

	while(1) {
		
	}
}

/*
 *	Converts rpm into distance/second
 */
float rpmToDistance(byte motor) {
	switch(motor) {
		case OUT_A:
			return rpmA;
		case OUT_B:
			return rpmB;
		case OUT_C:
			return rpmC;
	}
}

#endif // OCTA_POSITIONING_SYSTEM_H


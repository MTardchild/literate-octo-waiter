#ifndef OCTA_POSITIONING_SYSTEM_H
#define OCTA_POSITIONING_SYSTEM_H

#define OPS_CALC_FREQUENCY 100

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
 *	The direction waiter is facing in degrees (0-359)
 *  0 = North
 */
int direction;

/*
 *	Cm equivalent of a square point.
 */
const float squarePointDistance = 0.09803922;

/*
 *	Cm equivalent of a wheel rotation.
 */
const float rotationDistance = 13.67;

/*
 *	Calculates the current position and writes result into global variables. 
 *	This method assumes that it gets called every OPS_CALC_FREQUENCY ms.
 */
task calculatePosition() {
	while(1) {
		Wait(OPS_CALC_FREQUENCY);
		static float dpmA = rpmToDpm(OUT_A); 
		static float dpmB = rpmToDpm(OUT_B);
	}
}

/*
 *	Converts rpm into distance/minute
 */
float rpmToDpm(byte motor) {
	float distance;

	switch(motor) {
		case OUT_A:
			distance = rpmA;
		case OUT_B:
			distance = rpmB;
		case OUT_C:
			distance = rpmC;
	}

	return distance * rotationDistance * squarePointDistance; 
}

#endif // OCTA_POSITIONING_SYSTEM_H


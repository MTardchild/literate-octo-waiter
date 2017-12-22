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

task calculatePosition() {

}

#endif // OCTA_POSITIONING_SYSTEM_H

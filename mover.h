#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H

#include "octaPositioningSystem.h"
#include "pathfinder.h"
#include "adjustmentController.h"
#include "mapUtil.h"

#define TURN_EPSILON_ROUGH 40
#define TURN_EPSILON_FINE 2
#define DRIVE_EPSILON_ROUGH 40
#define DRIVE_EPSILON_FINE 4

#define GO_QUICK 35
#define GO_SLOW 10
#define TURN_QUICK 20
#define TURN_SLOW 2

#define SENSOR_DIST (9.5 / squarePointDistance)


/*
*  Converts ops style degrees (0 - 359) to path style directions (1 - 4) 
*
*  direction: direction to be converted
*/
int convertFromDeg(int direction){
	return ((direction + 45) / 90) + 1;
}

/*
*  Converts path style directions (1 - 4) to ops style degrees (0 - 359)
*
*  direction: direction to be converted
*/
int convertToDeg(byte direction){
	direction -= 1;
	return direction * 90;
}

#define min(x, y) (((x)<(y)) ? (x) : (y))
#define max(x, y) (((x)>(y)) ? (x) : (y))

bool isFacingDirection(int direction, float epsilon) {
	int x = min(convertToDeg(direction), ops_dir);
	int y = max(convertToDeg(direction), ops_dir);
	x += ((y - x) > 180) ? 360 : 0;
#ifdef DEBUG
			ClearLine(LCD_LINE7);
			TextOut(0, LCD_LINE7, "epsilon");
			NumOut(50, LCD_LINE7, abs(y-x) < epsilon);
#endif
	return abs(y-x) < epsilon;
}

bool turnLeft(short direction){
	direction += abs(ops_dir - direction) > 180 ? 360 : 0;
	direction = ops_dir - direction;
	return direction < 0;
}


/*
 *  Returns: If the path has been finished
 */
#define isOver(currentStep) ((currentStep == MAX_PATH_SIZE) || (pf_path[currentStep] == 0))

/* 
 *  Moves the path calculated by pathfinder.
 *  smooth: smooth or not.
 */
void movePath(bool smooth);

/*calculats the target square in the current direction
*
*/
byte getTargetSquare(byte distance){
	byte direction = convertFromDeg(ops_dir);
	switch(direction){
	case n:
		return ops_ySquare - distance;
	case e:
		return ops_xSquare + distance;
	case s:
		return ops_ySquare + distance;
	case w:	
		return ops_xSquare - distance;
	default:
		//dunno
		return 0;
  }
}

/*
*   chechs if target square in the current driving direction has been reached
*/
bool reachedTarget(byte target, float epsilon){
	byte direction = convertFromDeg(ops_dir);
	byte current_in_dir_value;
    byte current_square;
	if (direction == n || direction == s){
        current_in_dir_value = ops_y;
		current_square = ops_ySquare;
		
}	else if (direction == e || direction == w){
		current_in_dir_value = ops_x;
        current_square = ops_xSquare;
  }
    if (abs(current_in_dir_value - 128) < epsilon && current_square == target)
		return true;
	return false;	
}

#define adjustRight ((ops_dir % 90) > 45)

float calcAdjustFactor(){
	int adjustFactor = adjustRight ? 90 - (ops_dir % 90) : (ops_dir % 90);
	const float constAdjustFactor = 0.05;
	return 1 + adjustFactor * constAdjustFactor;
}


/*
 *  Drives in currently facing direction. Default power value.
 *
 *  distance: Determines how far the robot should be driving (1 - 12)
 *  smooth: Determines if the motion should be smooth (true/false).
 */
void goStraight(byte distance, byte smooth){
	float adjFacRight;
	float adjFacLeft;

	byte targetSquare = getTargetSquare(distance); 

	if(smooth){
		while(!reachedTarget(targetSquare, DRIVE_EPSILON_ROUGH)){
			if (adjustRight){
				adjFacRight = 1;
				adjFacLeft = calcAdjustFactor();
			}else{
				adjFacRight = calcAdjustFactor();
				adjFacLeft = 1;
			}
			OnFwd(OUT_A, pidController(OUT_A, GO_QUICK * adjFacLeft, rpmA));
			OnFwd(OUT_B, pidController(OUT_B, GO_QUICK * adjFacRight, rpmB));
			Wait(100);
		}
		while(!reachedTarget(targetSquare, DRIVE_EPSILON_FINE)){
			float adjFac = calcAdjustFactor();
			OnFwd(OUT_A, pidController(OUT_A, GO_SLOW, rpmA));
			OnFwd(OUT_B, pidController(OUT_B, GO_SLOW, rpmB));
			Wait(100);
		}
	}else{
		while (!reachedTarget(targetSquare, DRIVE_EPSILON_FINE)){
			float adjFac = calcAdjustFactor();
			OnFwd(OUT_A, pidController(OUT_A, GO_QUICK, rpmA));
			OnFwd(OUT_B, pidController(OUT_B, GO_QUICK, rpmB));
			Wait(100);
		}
	}
	Off(OUT_AB);
	ac_resetAll();
}

#define DIR_LEFT -1
#define DIR_RIGHT 1

/*
 *  Turns the robot so that it faces the given direction. Default power value.
 *
 *
 *  direction: Determines the direction the robot should turn (1 - 4)
 *  smooth: Determines if the motion should be smooth (true/false)
 *  turnDir: turnLeft / turnRight
 */
 
 void turn(byte direction, byte smooth){
  int turnDir;
	if(turnLeft(convertToDeg(direction)))
		turnDir = DIR_LEFT;
	else
		turnDir = DIR_RIGHT;
	
	if(smooth){
		while(!isFacingDirection(direction, TURN_EPSILON_ROUGH)){
			OnFwd(OUT_A, pidController(OUT_A, TURN_QUICK * turnDir * -1, rpmA));
			OnFwd(OUT_B, pidController(OUT_B, TURN_QUICK * turnDir, rpmB));
			Wait(100);
		}
		while(!isFacingDirection(direction, TURN_EPSILON_FINE)){
			OnFwd(OUT_A, pidController(OUT_A, TURN_SLOW * turnDir * -1, rpmA));
			OnFwd(OUT_B, pidController(OUT_B, TURN_SLOW * turnDir, rpmB));
			Wait(100);
		}
	}else{
		while (!isFacingDirection(direction, TURN_EPSILON_FINE)){
			OnFwd(OUT_A, pidController(OUT_A, TURN_QUICK * turnDir * -1, rpmA));
			OnFwd(OUT_B, pidController(OUT_B, TURN_QUICK * turnDir, rpmB));
			Wait(100);
		}
	}

	Off(OUT_AB);
	ac_resetAll();
}

void offTheWall(){
	byte xSquareBefore = ops_xSquare;
	byte ySquareBefore = ops_ySquare;
	byte targetSquare;
	
	/*
	HIT THE WALL
	*/
	SetSensor(IN_1, SENSOR_TOUCH);
	SetSensor(IN_4, SENSOR_TOUCH);
	bool leftHit = false;
	bool rightHit = false;
	while(true){	
		if(!rightHit && SENSOR_1)
			rightHit = true;
		if(!leftHit && SENSOR_4)
			leftHit = true;

		if(leftHit)
			Off(OUT_A);
		else
			OnFwd(OUT_A, pidController(OUT_A, GO_QUICK, rpmA));
		if(rightHit)
			Off(OUT_B);
		else
			OnFwd(OUT_B, pidController(OUT_B, GO_QUICK, rpmB));
		
		if(leftHit && rightHit)
			break;
		
		Wait(50);
	}
	ac_resetAll();
	
	/*
	BE SORRY
	*/	
	ops_xSquare = xSquareBefore;
	ops_ySquare = ySquareBefore;

	byte currentDir = convertFromDeg(ops_dir);

	switch(currentDir){
		case s:
			ops_y = 255 - SENSOR_DIST;
			targetSquare = ops_ySquare;
			ops_dir = 180;
			break;
		case n:
			ops_y = 0 + SENSOR_DIST;
			targetSquare = ops_ySquare;
			ops_dir = 0;
			break;
		case e:
			ops_x = 255 - SENSOR_DIST;
			targetSquare = ops_xSquare;
			ops_dir = 90;
			break;
		case w:
			ops_x = 0 + SENSOR_DIST;
			targetSquare = ops_xSquare;
			ops_dir = 270;
			break;
	}
	
	/*
	APOLOGISE
	*/	
	while(true){
		bool test = reachedTarget(targetSquare, DRIVE_EPSILON_FINE);
		if(test){
			Off(OUT_AB);
			break;
		}else{
			OnFwd(OUT_A, pidController(OUT_A, GO_SLOW * (-1), rpmA));
			OnFwd(OUT_B, pidController(OUT_B, GO_SLOW * (-1), rpmB));
		}
		
		Wait(50);
	}
	ac_resetAll();	
}

int initCurrentStep(){
	bool test = ((pf_path[0] == convertFromDeg(ops_dir)) && (pf_path[0] == pf_path[1]));
    if (test)
        return 1;
    return 0;
}    

void movePath(bool smooth) {
    byte consecutiveSameDirections;
    int currentStep = initCurrentStep();
    bool isOver9000 = true;
	if (isTable(ops_ySquare, ops_xSquare, convertFromDeg(ops_dir)))
		offTheWall();
    while (isOver9000) {
        consecutiveSameDirections = 1;
		bool test = isFacingDirection(pf_path[currentStep], TURN_EPSILON_ROUGH);
        if (test) {
            ++currentStep;
											ClearLine(LCD_LINE8);
											TextOut(0, LCD_LINE8, "currentStep");
											NumOut(50, LCD_LINE8, currentStep);
			test = isFacingDirection(pf_path[currentStep], TURN_EPSILON_ROUGH) && !isOver(currentStep);
            while(test){
                ++consecutiveSameDirections;
                ++currentStep;
				test = isFacingDirection(pf_path[currentStep], TURN_EPSILON_ROUGH) && !isOver(currentStep);
            }
											ClearLine(LCD_LINE5);
											TextOut(0, LCD_LINE5, "Steps");
											NumOut(50, LCD_LINE5, consecutiveSameDirections);
            goStraight(consecutiveSameDirections, smooth);
			test = isWall(ops_ySquare, ops_xSquare, convertFromDeg(ops_dir));
			if (test)
				offTheWall();
        } else {
            turn(pf_path[currentStep], smooth);
			++currentStep;
        }
	isOver9000 = !isOver(currentStep);
    }
}

#endif // OCTA_MOVER_H


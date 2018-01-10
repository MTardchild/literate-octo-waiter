#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H


#define TURN_EPSILON_ROUGH 5
#define TURN_EPSILON_FINE 0.4
#define DRIVE_EPSILON_ROUGH ??
#define DRIVE_EPSILON_FINE ??

#define GO_QUICK 35
#define GO_SLOW 15
#define TURN_QUICK 15
#define TURN_SLOW 5


/*
*  Converts ops style degrees (0 - 359) to path style directions (1 - 4) 
*
*  direction: direction to be converted
*/
int convertFromDeg(byte direction){
	return (direction + 45) / 90;
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

#define min(x, y) ((x)<(y) ? (x) : (y))
#define max(x, y) ((x)>(y) ? (x) : (y))

bool isFacingDirection(byte direction, float epsilon) {
	x = min(direction, ops_dir);
	y = max(direction, ops_dir)
	x += (y - x > 180) ? 360 : 0;
	return abs(y-x) < epsilon;
}

bool turnLeft(short direction){
	direction += (abs(y - x) > 180) ? 360 : 0;
	return (ops_dir - direction) > 0;
}


/*
 *  Returns: If the path has been finished
 */
#define isOver(currentStep) (currentStep != MAX_PATH_SIZE && pf_path[currentStep] != 0)

/* 
 *  Moves the path calculated by pathfinder.
 *  Default smoothness and power values.
 */
void movePath();

/*calculats the target square in the current direction
*
*/
byte getTargetSquare(byte distance){
	byte direction = convertFromDeg(ops_dir);
	switch(direction)
	case north:
		return ops_y - distance;
	case east:
		return ops_x + distance;
	case south:
		return ops_y + distance;
	case west:	
		return ops_x - distance;
	default:
		//dunno
		return 0;
}

/*
*   chechs if target square in the current driving direction has been reached
*/
bool reachedTarget(byte target, float epsilon){
	byte direction = convertFromDeg(ops_dir);
	byte current_in_dir_value;
	if (direction == north || direction == south)
		current_in_dir_value = ops_y;
	else if (direction == north || direction == south)
		current_in_dir_value = ops_x;
	if (abs(current_in_dir_value - epsilon)<epsilon)
		return true;
	return false;	
}

/*
 *  Drives in currently facing direction. Default power value.
 *
 *  distance: Determines how far the robot should be driving (1 - 12)
 *  smooth: Determines if the motion should be smooth (true/false).
 */
void goStraight(byte distance, byte smooth){
	byte targetSquare = getTargetSquare(distance); 
	if(smooth){
		while(!reachedTarget(targetSquare, DRIVE_EPSILON_ROUGH)){
			OnFwd(OUT_A, pidController(OUT_A, GO_QUICK, rpmA);
			OnFwd(OUT_B, pidController(OUT_B, GO_QUICK, rpmB);
		}
		while(!reachedTarget(targetSquare, DRIVE_EPSILON_FINE)){
			OnFwd(OUT_A, pidController(OUT_A, GO_SLOW, rpmA);
			OnFwd(OUT_B, pidController(OUT_B, GO_SLOW, rpmB);
		}
	}else{
		while (!reachedTarget(targetSquare, DRIVE_EPSILON_FINE)){
			OnFwd(OUT_A, pidController(OUT_A, GO_QUICK, rpmA);
			OnFwd(OUT_B, pidController(OUT_B, GO_QUICK, rpmB);
		}
	}
	Off(OUT_AB);
}

#define turnLeft  -1
#define turnRight  1

/*
 *  Turns the robot so that it faces the given direction. Default power value.
 *
 *
 *  direction: Determines the direction the robot should turn (1 - 4)
 *  smooth: Determines if the motion should be smooth (true/false)
 *  turnDir: turnLeft / turnRight
 */
 
 void turn(byte direction, byte smooth){
	direction = convertToDeg(direction)
	if(turnLeft(direction))
		goTurn(direction, smooth, turnLeft);
	else
		goTurn(direction, smooth, turnRight);
	
	if(smooth){
		while(!isFacingDirection(direction, TURN_EPSILON_ROUGH)){
			OnFwd(OUT_A, pidController(OUT_A, TURN_QUICK, rpmA * turnDir);
			OnFwd(OUT_B, pidController(OUT_B, TURN_QUICK, rpmB * turnDir * -1);
		}
		while(!isFacingDirection(direction, TURN_EPSILON_FINE)){
			OnFwd(OUT_A, pidController(OUT_A, TURN_SLOW, rpmA * turnDir);
			OnFwd(OUT_B, pidController(OUT_B, TURN_SLOW, rpmB * turnDir * -1);
		}
	}else{
		while (!isFacingDirection(direction, TURN_EPSILON_FINE)){
			OnFwd(OUT_A, pidController(OUT_A, TURN_QUICK, rpmA * turnDir);
			OnFwd(OUT_B, pidController(OUT_B, TURN_QUICK, rpmB * turnDir * -1);
		}
	}
	Off(OUT_AB);
}

 
void movePath(bool smooth) {
	int currentStep = 0;
    while (!isOver(currentStep) {
        consecutiveSameDirections = 1;
        if (isFacingDirection(pf_path[currentStep], TURN_EPSILON_ROUGH) {
            ++currentStep;
            while(isFacingDirection(pf_path[currentStep], TURN_EPSILON_ROUGH) && !isOver(currentStep){
                ++consecutiveSameDirections;
                ++currentStep;
            }
            goStraight(consecutiveSameDirections, smooth)
        } else {
            turn(pf_path[currentStep], smooth);
			++currentStep;
        }
    }
}

#endif // OCTA_MOVER_H

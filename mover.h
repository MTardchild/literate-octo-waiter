#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H

/* 
 *  Moves the path calculated by pathfinder.
 *  Default smoothness and power values.
 */
void movePath();

/*
 *  Drives in currently facing direction. Default power value.
 *
 *  distance: Determines how far the robot should be driving (1 - 12)
 *  smooth: Determines if the motion should be smooth (true/false).
 */
void goStraight(byte distance, byte smooth){


}

/*
 *  Turns the robot so that it faces the given direction. Default power value.
 *
 *
 *  direction: Determines the direction the robot should face afterwards. (1 - 4)
 *  smooth: Determines if the motion should be smooth (true/false).
 */
void turn(byte direction, byte smooth){
	

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

// THIS DOES NOT WORK
bool isFacingDirection(short direction) {
	direction = convertToDeg(direction)
    return ops_direction >= ((direction - EPSILON_DIRECTION) % 360)
        && ops_direction <= ((direction + EPSILON_DIRECTION) % 360) ;
}

/*
 *  Returns: If the path has been finished
 */
#define isOver(currentStep) (currentStep != MAX_PATH_SIZE && pf_path[currentStep] != 0)

 
void movePath(bool smooth) {
	int currentStep = 0;
    while (!isOver(currentStep) {
        consecutiveSameDirections = 1;
        if (isFacingDirection(pf_path[currentStep]) {
            ++currentStep;
            while(isFacingDirection(pf_path[currentStep]) && !isOver(currentStep){
                ++consecutiveSameDirections;
                ++currentStep;
            }
            go_straight(consecutiveSameDirections, smooth)
        } else {
            turn(pf_path[currentStep], smooth)
        }
    }
}

#endif // OCTA_MOVER_H

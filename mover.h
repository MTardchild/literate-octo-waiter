#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H

/*
 *  Stores the current step the robot is in (0 - MAX_PATH_SIZE).
 *  This essentially keeps track of pf_path[].
 */
byte currentStep = -1;

/* 
 *  Moves the path calculated by pathfinder.
 *  Default smoothness and power values.
 */
void movePath();


/*
 *  Stores the target direction of the turning motion (0 - 359).
 *
 *  0: North // 45: North-East
 *  90: East // 135: South-East
 *  180: South // 225: South-West
 *  270: West // 315: North-West
 */
short targetDirection = -1;

/*
 *  Drives in currently facing direction. Default power value.
 *
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 *  distance: Determines the distance of the drive (in mm).
 */
void move(byte smoothness, unsigned int distance);

/*
 *  Drives in currently facing direction.
 *
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 *  power: Determines the power (in %) used to control the engines (0 - 100).
 *  distance: Determines the distance of the drive (in mm).
 */
void movePower(byte smoothness, byte power, unsigned int distance); 

/*
 *  Turns the robot so that it faces the given direction. Default power value.
 *
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 *  direction: Determines the direction the robot should face afterwards. (0 - 359)
 */
void turn(byte smoothness, unsigned short direction);

/*
 *  Turns the robot so that it faces the given direction.
 *
 *  direction: Determines the direction the robot should face afterwards. (0 - 359)
 *  power: Determines the power (in %) used to control the engines (0 - 100).
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 */
void turnPower(byte smoothness, byte power, unsigned short direction);

void start_drive(){


}

void stop_drive(){


}

void drive(int dist){
	

}

void start_turn(int dir){


}

void stop_turn(int dir){


}

void turn(int dir){
	

}

bool isFacingDirection(short direction) {
    return actualDirection >= direction - EPSILON_DIRECTION
        && actualDirection <= direction + EPSILON_DIRECTION;
}

/*
 *  Returns: Distance (in mm) driven since the engine power on.
 */


void movePath() {
    while (currentStep != MAX_PATH_SIZE) {
        consecutiveSameDirections = 1;
        if (isFacingDirection(pf_path[currentStep]) {
            ++currentStep;
            while(isFacingDirection(pf_path[currentStep]) {
                ++consecutiveSameDirections;
                ++currentStep;
            }

            // move consecutiveSameDirections squares forward
        } else {
            // turn to target direction
        }
    }
}

#endif // OCTA_MOVER_H

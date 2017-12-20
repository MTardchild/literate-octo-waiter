#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H

/*
 *  Stores the actual step the robot is in (0 - MAX_PATH_SIZE).
 *  This essentially keeps track of pf_path[].
 */
byte currentStep = -1;

/* 
 *  Moves the path calculated by pathfinder.
 *  Default smoothness and power values.
 */
void movePath();

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

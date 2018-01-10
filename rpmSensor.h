#ifndef OCTA_RPM_SENSOR
#define OCTA_RPM_SENSOR

#define T 0.1

/*
 *	RPM of OUT_A
 */
float rpmA = 0;

/*
 *	RPM of OUT_B
 */
float rpmB = 0;

/*
 *	RPM of OUT_C
 */
float rpmC = 0;

/*
 *	Reads rotation count of given motor and calculates
 *	the corresponding rpm.
 *
 *	motor: motor in hex
 *	returns: rpm of motor
 */
float readRpm(byte motor) {
	long static oldRotationCount = MotorRotationCount(motor);
	long rotationCount = MotorRotationCount(motor);
	float rpm = (rotationCount - oldRotationCount) / T * 60 / 360;
	oldRotationCount = rotationCount;
	
	return rpm;
}

/*
 *	Stores given rpm into corresponding global.
 *
 *	motor: motor in hex
 *	rpm: rpm of motor
 */
void storeRpm(byte motor, float rpm) {
	switch(motor) {
		case OUT_A:
			rpmA = rpm;
			break;
		case OUT_B:
			rpmB = rpm;
			break;
		case OUT_C:
			rpmC = rpm;
			break;
	}
}

/*
 *	Parallel Task to check rpm of all motors.
 */
task checkRpm() {
	while(1) {
		Wait(1000 * T);
		storeRpm(OUT_A, readRpm(OUT_A));
		storeRpm(OUT_B, readRpm(OUT_B));
		storeRpm(OUT_C, readRpm(OUT_C));
	}
}

#endif // OCTA_RPM_SENSOR

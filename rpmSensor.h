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
 *	Reads rotation count of given motor and calculates
 *	the corresponding rpm.
 *
 *	motor: motor in hex
 *	returns: rpm of motor
 */
float readRpmA() {
	static long oldRotationCount = 0;
	long rotationCount = MotorRotationCount(OUT_A);
	float rpm = (rotationCount - oldRotationCount) / T * 60 / 360.0;
	oldRotationCount = rotationCount;
	return rpm;
}

float readRpmB() {
	static long oldRotationCount = 0;
	long rotationCount = MotorRotationCount(OUT_B);
	float rpm = (rotationCount - oldRotationCount) / T * 60 / 360.0;
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
#ifdef DEBUG
			ClearLine(LCD_LINE4);
			TextOut(0, LCD_LINE4, "rpmA");
			NumOut(50, LCD_LINE4, rpm);			
#endif
			break;
		case OUT_B:
			rpmB = rpm;
#ifdef DEBUG
			ClearLine(LCD_LINE5);
			TextOut(0, LCD_LINE5, "rpmB");
			NumOut(50, LCD_LINE5, rpm);			
#endif
			break;

	}
}

/*
 *	Parallel Task to check rpm of all motors.
 */
task checkRpm() {
	while(1) {
		Wait(1000 * T);
		storeRpm(OUT_A, readRpmA());
		storeRpm(OUT_B, readRpmB());
	}
}

#endif // OCTA_RPM_SENSOR

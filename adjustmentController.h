#ifndef OCTA_ADJUSTMENT_CONTROLLER_H
#define OCTA_ADJUSTMENT_CONTROLLER_H

#define kp 0.6
#define ki 0.1
#define kd 0.0

#define T 0.3

#define SOLL 35
#define SOCKELWERT 20
#define p_regler(ist) ((SOLL-ist) * kp)


int i_regler(int ist)
{
  static int s = 0;
  s += (SOLL - ist);
  return ki * T * s;
}

int d_regler(int ist)
{
  static int delta_alt = 0;
  int a = kd * (SOLL-ist - delta_alt) / T;
  delta_alt = SOLL - ist;

  return a;
}

int pi_regler(int ist)
{
  return p_regler(ist) + i_regler(ist);
}

int pd_regler(int ist)
{
  return p_regler(ist) + d_regler(ist);
}

int pid_regler(int ist)
{
  return p_regler(ist) + d_regler(ist) + i_regler(ist);
}



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


	
task main() {
  long rotCount, oldRotCount;
  int power = 0;

  oldRotCount = MotorRotationCount(OUT_A);

  while (1) {
     Wait(T*1000);

     rotCount = MotorRotationCount(OUT_A);
     TextOut(0, LCD_LINE2, "    ");
     int rpm = (rotCount - oldRotCount) / T * 60 / 360;
     NumOut(0, LCD_LINE2, rpm);
     power += pi_regler(rpm);
     //OnFwd(OUT_A, power);
     OnFwd(OUT_A, SOCKELWERT + pid_regler(rpm));

     oldRotCount = rotCount;
  }
}

#endif // OCTA_ADJUSTMENT_CONTROLLER_H

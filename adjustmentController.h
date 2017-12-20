#ifndef OCTA_ADJUSTMENT_CONTROLLER_H
#define OCTA_ADJUSTMENT_CONTROLLER_H

#define kp 0.6
#define ki 0.1
#define kd 0.0

#define T 0.3

#define SOLL 35
#define SOCKELWERT 20
#define pController(ist) ((SOLL-ist) * kp)
#define EPSILON_DIRECTION 0.5



int iController(int ist)
{
  static int s = 0;
  s += (SOLL - ist);
  return ki * T * s;
}

int dController(int ist)
{
  static int delta_alt = 0;
  int a = kd * (SOLL-ist - delta_alt) / T;
  delta_alt = SOLL - ist;

  return a;
}

int piController(int ist)
{
  return pController(ist) + i_regler(ist);
}

int pdController(int ist)
{
  return pController(ist) + d_regler(ist);
}

int pidController(int ist)
{
  return pController(ist) + d_regler(ist) + i_regler(ist);
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
     power += piController(rpm);
     //OnFwd(OUT_A, power);
     OnFwd(OUT_A, SOCKELWERT + pidController(rpm));

     oldRotCount = rotCount;
  }
}

#endif // OCTA_ADJUSTMENT_CONTROLLER_H

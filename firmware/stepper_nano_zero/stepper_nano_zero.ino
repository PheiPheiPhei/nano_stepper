#include "nzs.h"
#include "stepper_controller.h"
#include <Wire.h>

NZS nzs;
  

extern StepperCtrl stepperCtrl;
void setup() {
  nzs.begin();
  Wire.begin(0x08);
  Wire.onRequest(wireRequestHandler);
}


void loop() {
	nzs.loop(); 
}

void wireRequestHandler() {
  union {
    byte b[4];
    float fval;
  } u;
  u.fval = ANGLE_T0_DEGREES(ptrsCtrl.getCurrentAngle());
  Wire.write(u.b, 4);
}


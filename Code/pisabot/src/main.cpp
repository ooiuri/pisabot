#include <AccelStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPinA 26
#define stepPinA 27
#define dirPinB 12
#define stepPinB 14
#define motorInterfaceType 1
#define MAX_SPEED 400
// Create a new instance of the AccelStepper class:
AccelStepper stepperA = AccelStepper(motorInterfaceType, stepPinA, dirPinA);
AccelStepper stepperB = AccelStepper(motorInterfaceType, stepPinB, dirPinB);

void setup()
{  
  stepperA.setMaxSpeed(50.0);
  stepperA.setAcceleration(50.0);
  stepperA.moveTo(1000);
  
  stepperB.setMaxSpeed(50.0);
  stepperB.setAcceleration(50.0);
  stepperB.moveTo(-1000);
}
void loop()
{
  // Change direction at the limits
  if (stepperA.distanceToGo() == 0)
    stepperA.moveTo(-stepperA.currentPosition());
  if (stepperB.distanceToGo() == 0)
    stepperB.moveTo(-stepperB.currentPosition());
  stepperA.run();
  stepperB.run();
}
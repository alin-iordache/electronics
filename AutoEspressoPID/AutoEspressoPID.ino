
#include <PID_v1.h>
#define HEATER_PIN 4

double Setpoint, Input, Output;

boolean calibration = 0;
float Res;                  // thermistor resistance value for calibration 

// initializing PID
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

void PidSetup(){
  windowStartTime = millis();
  
  //initialize the variables we're linked to
  Setpoint = getTargetedTemp();

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void setup(void)
{
  PidSetup();
  
  LcdInitialise();
  LcdClear();
  
  Serial.begin(9600);
  initialiseMenu();
  enter();
  
}

void loop(void)
{
  stabilizeTempFromSensor();
  Input = getFreshTemp();
  myPID.Compute();
  
  readButtons();  
  //display the current sensor temperature in status bar
  updateCurrentTemp();

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if(millis() - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  
  if(Output < millis() - windowStartTime) 
    digitalWrite(HEATER_PIN, HIGH);
  else 
    digitalWrite(HEATER_PIN, LOW);
}

// Original code changed for read thermistor

// [[[[[[With the AD 595, this process is just a matter of doing some math on an 
// analog input
//
// Thanks to Karl Gruenewald for the conversion formula 
// All code released under
// Creative Commons Attribution-Noncommercial-Share Alike 3.0 

// This current version is based on sensing temperature with 
// an AD595 and thermocouple through an A/D pin.  Any other 
// sensor could be used by replacing this one function.
// feel free to use degrees C as well, it will just give a different 
// PID tuning than those from F.
// ]]]]]]

#include <math.h>
#define TEMP_SENSOR_PIN 2

//float pad=10000;              // pad resistor value 10K to GND, use 0.1% resistors
float pad=5580; //5490;
float Vin = 3.48; //5.01;            // if you can, measure the output voltage on the 7805 for more precision  

// calibration coefficients obtained from here http://www.capgo.com/Resources/Temperature/Thermistor/ThermistorCalc.html

//float a = 0.0009161928;      // 9.161928E-4   -- values for EPCOS - B57551G1103F005 - THERMISTOR 10K 1%
//float b = 0.00025593705;     // 2.5593705E-4  -- resolution at 95C = 0.2 ~ 0.3C 
//float c = 0.00000010284751;   // 1.0284751E-7 --

float a = 0.0007152497;      //  values for EPCOS - B57560G104F - THERMISTOR 100K 1%
float b = 0.00021739925;     //  resolution at 95C = 0.1C  
float c = 0.00000008988123;   // 

//float a = 0.001129148;          // my first cheap 10% thermistor 
//float b = 0.000234125;       
//float c = 0.0000000876741;   


float tcSum = 0.0;
float latestReading = 0.0;
int readCount = 0;
//float multiplier;
//void setupTempSensor() {
//  multiplier = 1.0/(1023.0) * 500.0 * 9.0 / 5.0;
//}  

void stabilizeTempFromSensor() {
    tcSum += Thermister(analogRead(TEMP_SENSOR_PIN));
  //tcSum += analogRead(TEMP_SENSOR_PIN); //output from AD595 to analog pin 1
    readCount +=1;
}

float getFreshTemp() { 
     latestReading = tcSum/readCount;
   //  Tempvalue = latestReading;
  // latestReading = tcSum* multiplier/readCount+32.0;
      readCount = 0;
      tcSum = 0.0;
  return latestReading;

}

float getLastTemp() {
  return latestReading;

}



float Thermister(int RawADC) {
long Resistance;  
float Temp;
  // See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
 // Temp = log(((10240000/RawADC) - 10000));


///////////  Used Only for thermistor calibration >>>>>
if (calibration) { 
                   float Vout = RawADC/1024.0*Vin;    // convert the ADC to voltage  
                   Res = Vin*(pad/Vout)-pad;          // Calculate the resistance of the thermistor
                 }
//////////


Resistance=((1024 * pad / RawADC) - pad);
  Temp = log(Resistance);
  Temp = 1 / (a + (b * Temp) + (c * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  return Temp;
}
// END Temperature Sensor

float Pressure(int presADC){ 
                            float press;   
                            press = ((presADC*Vin/1024.0)-0.48)*13.79/3.98;    //  0.48V (0.02V loss in cable) = offset --- pressure sensor max 13.79 bar (200psi) at 4V  (output from 0.5 to 4.5V)  
                            if (press < 0) press = 0;
                            return press;
}

// End Pressure Sensor

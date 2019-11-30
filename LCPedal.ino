#include "Gamepad.h"
#include "HX711-multi.h"
#include "SimpleKalmanFilter.h"
#include <math.h>

Gamepad pedal;
#define BAUD_RATE 9600

// Pins to the load cell amp
#define CLK 2      // clock pin to the load cell amps
#define DOUT1 3    // data pin to the first lca
#define DOUT2 4    // data pin to the second lca
#define DOUT3 5    // data pin to the third lca

#define TARE_TIMEOUT_SECONDS 4

byte DOUTS[3] = {DOUT1, DOUT2, DOUT3};

#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)

long int results[CHANNEL_COUNT];

HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);
int16_t gas = 0 ;
int16_t brake = 0;
int16_t clutch = 0;

SimpleKalmanFilter gasFilter(2, 2, 0.01);
SimpleKalmanFilter brakeFilter(2, 2, 0.01);
SimpleKalmanFilter clutchFilter(2, 2, 0.01);

const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

void setup() {

  Serial.begin(BAUD_RATE);
  pedal.begin();
  tare();
}

void loop() {
  scales.read(results);
  results[0] = constrain(results[0] * (-1), 0, 1000000);
  results[1] = constrain(results[1], 0, 1000000);
  results[2] = constrain(results[2] * (-1), 0, 1000000);
//    int16_t gas= convert(results[0],0,1000000,-32768,32767);
// Serial.println(results[0]);
  if (results[0] < 280000) {    
    gas = convert(results[0], 0, 280000, -32768, 15000); 
  } else if (results[0] >= 280000 && (results[0] < 700000)){

    gas = convert(results[0], 280000, 700000, 15000, 28000);

  } else {
    gas = convert(results[0], 700000, 1000000, 28000, 32767);
  }    

  brake= convert(results[1],0,1000000,-32768,32767);
  clutch= convert(results[2],0,1000000,-32768,32767);
  
//  pedal.rxAxis(gas);
  pedal.rxAxis(gasFilter.updateEstimate(gas));
  pedal.ryAxis(gasFilter.updateEstimate(brake));
  pedal.rzAxis(clutchFilter.updateEstimate(clutch));
//  pedal.rzAxis(clutch);
  pedal.write();
  delay(10);

  //on serial data (any data) re-tare
  if (Serial.available() > 0) {
    while (Serial.available()) {
      Serial.read();
    }
    tare();
  }

}

void tare() {
  bool tareSuccessful = false;
  unsigned long tareStartTime = millis();
  while (!tareSuccessful && millis() < (tareStartTime + TARE_TIMEOUT_SECONDS * 1000)) {
    tareSuccessful = scales.tare(20, 10000); //reject 'tare' if still ringing
  }
}

int16_t convert(long input, long minInput, long maxInput, long minOutput, long maxOutput) {
  float ratio = ((float)input - (float)minInput)/ ((float)maxInput - (float)minInput);
  float result =  minOutput + (maxOutput - minOutput) * ratio;
  return (int16_t) result;
}

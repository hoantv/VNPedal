#include "Gamepad.h"
#include "HX711-multi.h"
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

void setup() {

  Serial.begin(BAUD_RATE);
  pedal.begin();
  tare();
}

void loop() {  
  scales.read(results);
  results[0] = constrain(results[0]*(-1), 0, 1000000);
  results[1] = constrain(results[1], 0, 1000000);
  results[2] = constrain(results[2]*(-1), 0, 1000000);

  int16_t gas = convert(results[0],0,1000000,-32768,32767);
  int16_t brake = convert(results[1],0,1000000,-32768,32767);
  int16_t clutch = convert(results[2],0,1000000,-32768,32767);
  
  pedal.rxAxis(gas);
  pedal.ryAxis(brake);
  pedal.rzAxis(clutch);
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
  float ratio = (float)input/(maxInput-minInput); 
  float result =  minOutput + (maxOutput-minOutput)* ratio;
  return (int16_t) result;
}

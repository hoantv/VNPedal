#include "Gamepad.h"
#include "HX711-multi.h"

Gamepad gamepad;
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
int32_t gas = 0 ;
int32_t brake = 0;
int32_t clutch = 0;

const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

//XY shifter use potentiometer
uint16_t x = A0;
uint16_t y = A1;
uint8_t shifterValue = 0;


void setup() {
  Serial.begin(BAUD_RATE);
  gamepad.begin();
  tare();
}

void loop() {
//  release all previos value of gamepad
  gamepad.releaseAll();
  shifterValue = calculatedShifter(mappedX(analogRead(x)), mappedY(analogRead(y)), shifterValue); 
  gamepad.press(shifterValue+1);  
  scales.read(results);
  results[0] = constrain(results[0] * (-1), 0, 300000); // *(-1) and 300000 are based on the loadcell value.
  results[1] = constrain(results[1], 0, 1048576);
  results[2] = constrain(results[2] * (-1), 0, 500000);
  gas = convert(results[0], 0, 300000, 0, 1048576);
  brake = results[1];
  clutch = convert(results[2], 0, 500000, 0, 1048576);
  gamepad.rxAxis((gas));
  gamepad.ryAxis((brake));
  gamepad.rzAxis((clutch));
 
  gamepad.write();
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

long convert(long input, long minInput, long maxInput, long minOutput, long maxOutput) {
  float ratio = ((float)input - (float)minInput) / ((float)maxInput - (float)minInput);
  float result =  minOutput + (maxOutput - minOutput) * ratio;
  return (long) result;
}

uint8_t mappedX(uint16_t x) {
  if ( x > 540) {
    return 0;
  } else if (x < 530 && x > 510) {
    return 1;
  }
  else if (x < 490 && x > 470) {
    return 2;
  } else if (x < 460) {
    return 3;
  }
  else {
    return 4;
  }
}

uint8_t mappedY(uint16_t y) {
  if ( y < 470) {
    return 0;
  } else if (y > 550) {
    return 2;
  }
  else {
    return 1;
  }
}
uint8_t calculatedShifter(uint8_t x, uint8_t y, uint8_t shifter) {
  if (x == 0 && y == 0) {
    return 1;
  } else if (x == 0 && y == 2) {
    return 2;
  } else if (x == 1 && y == 0) {
    return 3;
  }
  else if (x == 1 && y == 2) {
    return 4;
  }
  else if (x == 2 && y == 0) {
    return 5;
  }
  else if (x == 2 && y == 2) {
    return 6;
  }
  else if (x == 3 && y == 2) {
    return 7;
  } else if (x == 1 && y == 1) {
    return 0;
  }
  else {
    return shifter;
  }
}

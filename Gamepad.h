/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2019 HoanTV
See the readme for credit to other people.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef GAMEPAD_h
#define GAMEPAD_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else


typedef struct
{ //WheelReport
  uint32_t buttons = 0x00;
  int16_t xAxis;
  int16_t yAxis;
  int16_t zAxis;
  int16_t rxAxis;
  int16_t ryAxis;
  int16_t rzAxis;
} GamepadReport;

class Gamepad
{
  private:
    GamepadReport gamepadReport;
    void SendReport(void* data, int len);

  public:
    Gamepad(void);

    void begin(void);
    void end(void);
    void write(void);
    void press(uint8_t b);
    void release(uint8_t b);
    void releaseAll(void);
    void buttons(uint32_t b);
    void xAxis(int16_t a);
    void yAxis(int16_t a);
    void zAxis(int16_t a);
    void rxAxis(int16_t a); 
    void ryAxis(int16_t a); 
    void rzAxis(int16_t a); 
};

#endif
#endif

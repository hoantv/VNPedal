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
#include "Gamepad.h"
#define HID_REPORTID_GAMEPAD 0x01
static const uint8_t _hidReportDescriptor[] PROGMEM = {
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  0x09, 0x04, // USAGE (Joystick)
  0xA1, 0x01, // COLLECTION (Application)
  //================================Input Report======================================//
  0x09, 0x01, // USAGE (Pointer)
  // WheelReport
  0x85, HID_REPORTID_GAMEPAD, // REPORT_ID (1)
  0xA1, 0x00, // COLLECTION (Physical)
  // Button for Shifter
  0x05, 0x09, // USAGE_PAGE  (Button)
  0x19, 0x01, // USAGE_MINIMUM (Button 1)
  0x29, 0x20, // USAGE_MAXIMUM (Button 8)
  0x15, 0x00, // LOGICAL_MINIMUM (0)
  0x25, 0x01, // LOGICAL_MAXIMUM (1)
  0x75, 0x01, // REPORT_SIZE (1)
  0x95, 0x20, // REPORT_COUNT (8)
  0x81, 0x02, //INPUT (Data,Var,Abs)

  //6 Axis for steering wheel, accelerator, brake, clutch, handbrake and spare
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  0xa1, 0x00, // COLLECTION (Physical)
  0x09, 0x30, // USAGE (X)
  0x09, 0x31, // USAGE (Y)
  0x09, 0x32, // USAGE (Z)
  0x09, 0x33, // USAGE (Rx)
  0x09, 0x34, // USAGE (Ry)
  0x09, 0x35, // USAGE (Rz) */
  0x16, 0x00, 0x80, //LOGICAL_MINIMUM (-32768)
  0x26, 0xFF, 0x7F, //LOGICAL_MAXIMUM (32767)
  0x75, 0x10, // REPORT_SIZE (16)
  0x95, 0x06, // REPORT_COUNT (6)
  0x81, 0x02, // INPUT (Data,Var,Abs)
  0xc0, // END_COLLECTION */
  0xc0, // END_COLLECTION */

  0xC0, // END COLLECTION ()
};



//static const uint8_t _hidReportDescriptor[] PROGMEM = {
//  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
//  0x09, 0x04, // USAGE (Joystick)
//  0xA1, 0x01, // COLLECTION (Application)
//  //================================Input Report======================================//
//  0x09, 0x01, // USAGE (Pointer)
//  // WheelReport
//  0x85, HID_REPORTID_GAMEPAD, // REPORT_ID (1)
//  0xA1, 0x00, // COLLECTION (Physical)
//  // Button for Shifter
//  0x05, 0x09, // USAGE_PAGE  (Button)
//  0x19, 0x01, // USAGE_MINIMUM (Button 1)
//  0x29, 0x20, // USAGE_MAXIMUM (Button 8)
//  0x15, 0x00, // LOGICAL_MINIMUM (0)
//  0x25, 0x01, // LOGICAL_MAXIMUM (1)
//  0x75, 0x01, // REPORT_SIZE (1)
//  0x95, 0x20, // REPORT_COUNT (8)
//  0x81, 0x02, //INPUT (Data,Var,Abs)
//
//
//
//  //6 Axis for steering wheel, accelerator, brake, clutch, handbrake and spare
//  0x05, 0x02, // USAGE_PAGE (Simulation Controls)
//  0xa1, 0x00, // COLLECTION (Physical)
//  0x09, 0xC4, // USAGE (Accelerator)
//  0x09, 0xC5, // USAGE (Brake)
//  0x09, 0xBB, // USAGE (Throttle)
//
//  0x16, 0x00, 0x80, //LOGICAL_MINIMUM (-32768)
//  0x26, 0xFF, 0x7F, //LOGICAL_MAXIMUM (32767)
//  0x75, 0x10, // REPORT_SIZE (16)
//  0x95, 0x04, // REPORT_COUNT (4)
//  0x81, 0x02, // INPUT (Data,Var,Abs)
//  0xc0, // END_COLLECTION */
//
//
//    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
//  0xa1, 0x00, // COLLECTION (Physical) 
//  0x09, 0x32, // USAGE (Z)  
//  0x09, 0x35, // USAGE (Rz) */
//  0x16, 0x00, 0x80, //LOGICAL_MINIMUM (-32768)
//  0x26, 0xFF, 0x7F, //LOGICAL_MAXIMUM (32767)
//  0x75, 0x10, // REPORT_SIZE (16)
//  0x95, 0x02, // REPORT_COUNT (2)
//  0x81, 0x02, // INPUT (Data,Var,Abs)
//  0xc0, // END_COLLECTION */
//  
//  0xc0, // END_COLLECTION */
//  
//
//  0xC0, // END COLLECTION ()
//};




Gamepad::Gamepad(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

void Gamepad::SendReport(void* data, int length) {
  HID().SendReport(HID_REPORTID_GAMEPAD, data, length);
}

void Gamepad::begin(void) {
  // release all buttons
  end();
}

void Gamepad::end(void) {
  memset(&gamepadReport, 0x00, sizeof(gamepadReport));
  SendReport(&gamepadReport, sizeof(gamepadReport));
}


void Gamepad::write(void) {
  SendReport(&gamepadReport, sizeof(gamepadReport));
}


void Gamepad::press(uint8_t b) {
  gamepadReport.buttons |= (uint32_t)1 << (b - 1);
}


void Gamepad::release(uint8_t b) {
  gamepadReport.buttons &= ~((uint32_t)1 << (b - 1));
}


void Gamepad::releaseAll(void) {
  memset(&gamepadReport, 0x00, sizeof(gamepadReport));
}

void Gamepad::buttons(uint32_t  b) {
  gamepadReport.buttons = b;
}


void Gamepad::xAxis(int16_t a) {
  gamepadReport.xAxis = a;
}


void Gamepad::yAxis(int16_t a) {
  gamepadReport.yAxis = a;
}

void Gamepad::zAxis(int16_t a) {
  gamepadReport.zAxis = a;
}

void Gamepad::rxAxis(int16_t a) {
  gamepadReport.rxAxis = a;
}
void Gamepad::ryAxis(int16_t a) {
  gamepadReport.ryAxis = a;
}
void Gamepad::rzAxis(int16_t a) {
  gamepadReport.rzAxis = a;
}

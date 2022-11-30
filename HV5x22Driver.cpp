/*
 * Description: support library to handle latched and unlatched seriell-to-parallel shift registers HV5x22
 * Author     : Michael Scholz
 * eMail      : Michael_Scholz@hotmail.com 
 * Creation   : 2022.11.29 
 * Modified   : ---
*/

#include <Arduino.h>
#include "HV5x22Driver.h"


HV5x22Driver::HV5x22Driver(int dataIn, int clkIn, int oe, int str, bool lsbFirst) {
  _dataIn = dataIn;
  _clkIn = clkIn;
  _oe = oe;
  _str = str;
  _lsbFirst = lsbFirst;
  _latched = false;

  // mode
  pinMode(_dataIn, OUTPUT);
  pinMode(_clkIn, OUTPUT);
  pinMode(_oe, OUTPUT);
  pinMode(_str,  OUTPUT);

  // initital config
  digitalWrite(_dataIn, LOW);
  digitalWrite(_clkIn, LOW);
  digitalWrite(_oe,  LOW);
  digitalWrite(_str, HIGH);
}

HV5x22Driver::HV5x22Driver(int dataIn, int clkIn, int le, int bl, int pol, bool lsbFirst) {
  _dataIn = dataIn;
  _clkIn = clkIn;
  _le = le;
  _bl = bl;
  _pol = pol;
  _lsbFirst = lsbFirst;
  _latched = true;

  // mode
  pinMode(_dataIn, OUTPUT);
  pinMode(_clkIn, OUTPUT);
  pinMode(_le, OUTPUT);
  pinMode(_bl,  OUTPUT);
  pinMode(_pol,  OUTPUT);
  
  // initital config
  digitalWrite(_dataIn, LOW);
  digitalWrite(_clkIn, LOW);
  digitalWrite(_le,  LOW);
  digitalWrite(_bl, HIGH);
  digitalWrite(_pol, HIGH);
}

HV5x22Driver::~HV5x22Driver() {
}

void HV5x22Driver::send(char* data) {
  for(int i=0; i<sizeof(data); i++) {
    transmit(data[i], _lsbFirst);
  }
}

void HV5x22Driver::clearShr(int numBytes) {
  char clear[] = {0};
  for (int i=0; i<numBytes; i++) {
    send(clear);  // sending numBytes x 8 bits worth of 0s down the shift register
  }
}

void HV5x22Driver::blank(bool isBlank) {
  digitalWrite(_oe, isBlank?LOW:HIGH);
}

void HV5x22Driver::latchData() {
  digitalWrite(_bl, HIGH);
  digitalWrite(_pol, HIGH);
  digitalWrite(_le, LOW);
  vTaskDelay(1);
  digitalWrite(_le, HIGH);
  vTaskDelay(1);
}

void HV5x22Driver::transmit(char byte, bool lsbFirst) {
  const int width = 8;  // the min data cell to send is 8 bits wide
  const int slack = 1;  // wait 1ms between toggling the signals

  if (_latched) {
    // nothing to do here
  }
  else {
    blank(false);
  }

  // shift the data out bitwise
  for (int i=0; i<width; i++) {
    bool bitToSend = byte & (1<<(lsbFirst?i:width-1-i));
    digitalWrite(_clkIn, HIGH);
    vTaskDelay(slack);
    digitalWrite(_dataIn, bitToSend);
    vTaskDelay(slack);
    digitalWrite(_clkIn, LOW);
    vTaskDelay(slack);
  }

  // make result visible
  if (_latched) {
    latchData();
  }
  else {
    blank(true);
  }
}

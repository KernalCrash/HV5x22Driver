/*
 * Description: support library to handle latched and unlatched seriell-to-parallel shift registers HV5x22
 * Author     : Michael Scholz
 * eMail      : Michael_Scholz@hotmail.com 
 * Creation   : 2022.11.29 
 * Modified   : ---
*/

#include <Arduino.h>
#include "HV5x22Driver.h"


HV5x22Driver::HV5x22Driver(int dataIn, int clkIn, int oe, int str) {
  _dataIn = dataIn;
  _clkIn = clkIn;
  _oe = oe;
  _str = str;
  _latched = false;
  _slack = 1;  // 1µs

  // mode
  pinMode(_dataIn, OUTPUT);
  pinMode(_clkIn, OUTPUT);
  pinMode(_oe, OUTPUT);
  pinMode(_str,  OUTPUT);

  // initital config
  digitalWriteSlacked(_dataIn, LOW, _slack);
  digitalWriteSlacked(_clkIn, LOW, _slack);
  digitalWriteSlacked(_oe,  LOW, _slack);
  digitalWriteSlacked(_str, HIGH, _slack);
}

HV5x22Driver::HV5x22Driver(int dataIn, int clkIn, int le, int bl, int pol) {
  _dataIn = dataIn;
  _clkIn = clkIn;
  _le = le;
  _bl = bl;
  _pol = pol;
  _latched = true;
  _slack = 1;  // 1µs

  // mode
  pinMode(_dataIn, OUTPUT);
  pinMode(_clkIn, OUTPUT);
  pinMode(_le, OUTPUT);
  pinMode(_bl,  OUTPUT);
  pinMode(_pol,  OUTPUT);
  
  // initital config
  digitalWriteSlacked(_dataIn, LOW, _slack);
  digitalWriteSlacked(_clkIn, LOW, _slack);
  digitalWriteSlacked(_le,  LOW, _slack);
  digitalWriteSlacked(_bl, HIGH, _slack);
  digitalWriteSlacked(_pol, HIGH, _slack);
}

HV5x22Driver::~HV5x22Driver() {
}

void HV5x22Driver::send(uint32_t data, BitOrder bitOrder, bool transactionFinished) {
  _lsbFirst = (bitOrder==LSBITFIRST)?true:false;
  if (_latched) {
    // nothing to do here
  }
  else {
    blank(true);
  }
  transmit(data);
  // if there is more to come, e. g. loading a cascaded shift register, do not finish just yet
  if (transactionFinished) {
    if (_latched) {
      latchData();
    }
    else {
      blank(false);  // make result visible
    }
  }
}

void HV5x22Driver::clearShr(uint numOfCascadedChips) {
  const uint32_t clearPat = (uint32_t)(0);

  for (int i=0; i<numOfCascadedChips; i++) {
    send(clearPat, LSBITFIRST, (i==(numOfCascadedChips-1)?true:false));  // sending 32 bits worth of 0
  }
}

void HV5x22Driver::blank(bool isBlank) {
  digitalWriteSlacked(_oe, isBlank?LOW:HIGH, _slack);
}

void HV5x22Driver::latchData() {
  digitalWriteSlacked(_bl, HIGH, _slack);
  digitalWriteSlacked(_pol, HIGH, _slack);
  digitalWriteSlacked(_le, LOW, _slack);
  digitalWriteSlacked(_le, HIGH, _slack);
}

void HV5x22Driver::transmit(uint32_t data) {
  const int width = 32;  // the min data cell to send is 32 bits wide

  // shift the data out bitwise
  for (int i=0; i<width; i++) {
    //bool bitToSend = data & (1<<(lsbFirst?i:width-1-i));
    digitalWriteSlacked(_clkIn, HIGH, _slack);
    digitalWriteSlacked(_dataIn, (data >> (_lsbFirst?i:width-1-i)) & 1, _slack);
    digitalWriteSlacked(_clkIn, LOW, _slack);
  }
}

void HV5x22Driver::digitalWriteSlacked(int pin, int data, int postWriteSlack) {
  digitalWrite(pin, data);
  delayMicroseconds(postWriteSlack);
}
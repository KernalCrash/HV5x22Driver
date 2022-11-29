/*
 * Description: support library to handle latched and unlatched seriell-to-parallel shift registers HV5x22
 * Author     : Michael Scholz
 * eMail      : Michael_Scholz@hotmail.com 
 * Creation   : 2022.11.29 
 * Modified   : ---
*/

#ifndef HV5X22DRIVER
#define HV5X22DRIVER

class HV5x22Driver {

 public:
  HV5x22Driver(int dataIn, int clkIn, int oe, int str, bool lsbFirst);  // HV5122 / HV5222
  HV5x22Driver(int dataIn, int clkIn, int le, int bl, int pol, bool lsbFirst);  // HV5522
  ~HV5x22Driver();


  void send(char* data);
  void clearShr(int numBytes);

  
 private:
  // Pins
  int _dataIn;
  int _clkIn;
  int _oe;
  int _str;
  int _le;
  int _bl;
  int _pol;

  // flow control
  bool _lsbFirst;
  bool _latched;

  // Helpers
  void blank(bool isBlank);
  void latchData();
  void transmit(char byte, bool lsbFirst);

};

#endif  // HV5X22DRIVER